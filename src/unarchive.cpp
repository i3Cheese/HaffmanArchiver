#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <stdexcept>
#include "bits_stream.h"
#include "nine_bits.h"
#include "trie.h"
#include "bits.h"
#include "constants.h"

using HaffmanTrieNode = TrieNode<NineBits, 2>;

template <typename IntT, typename IStreamT>
static IntT ReadNineBitsAs(BitsIStream<IStreamT> &stream) {
    NineBits nine_bits{};
    stream >> nine_bits;
    return static_cast<IntT>(nine_bits);
}

template <typename StreamT>
static HaffmanTrieNode ReadCode(BitsIStream<StreamT> &archive_stream) {
    size_t symbols_count = ReadNineBitsAs<size_t>(archive_stream);
    std::vector<NineBits> symbols(symbols_count);
    for (NineBits &symbol : symbols) {
        archive_stream >> symbol;
    }

    std::vector<std::pair<NineBits, Bits>> haffman_codes;
    haffman_codes.reserve(symbols.size());
    Bits current_code;
    for (size_t code_size = 1; haffman_codes.size() != symbols_count; ++code_size) {
        size_t symbols_with_size = ReadNineBitsAs<size_t>(archive_stream);
        current_code <<= 1;
        if (haffman_codes.size() + symbols_with_size > symbols_count) {
            throw std::runtime_error("Bad archive");
        }
        for (size_t i = 0; i < symbols_with_size; ++i) {
            haffman_codes.emplace_back(symbols[haffman_codes.size()], current_code);
            ++current_code;
        }
    }
    HaffmanTrieNode codes_trie(false);
    for (const auto &[symbol, code] : haffman_codes) {
        std::vector<size_t> way;
        way.reserve(code.Size());
        for (Bit b : code) {
            way.push_back(static_cast<size_t>(b));
        }
        codes_trie.AddWay(way.begin(), way.end(), symbol);
    }
    return codes_trie;
}

template <typename StreamT>
static NineBits ReadEncodedSymbol(BitsIStream<StreamT> &archive_stream, const HaffmanTrieNode &root) {
    const HaffmanTrieNode *now_node = &root;  // never owns object
    while (now_node != nullptr && !now_node->IsTerminal()) {
        Bit b;
        archive_stream >> b;
        now_node = now_node->GetChild(static_cast<size_t>(b));
    }
    if (now_node == nullptr) {
        throw std::runtime_error("Not find symbol");
    }
    return now_node->Value();
}

template <typename StreamT>
static std::string ReadFileName(BitsIStream<StreamT> &archive_stream, const HaffmanTrieNode &codes_trie) {
    std::string filename;
    while (true) {
        NineBits symbol = ReadEncodedSymbol(archive_stream, codes_trie);
        if (symbol == FILENAME_END) {
            return filename;
        } else if (static_cast<uint16_t>(symbol) >= 256) {
            throw std::runtime_error("Enexpected control symbol");
        } else {
            filename.push_back(static_cast<char>(symbol));
        }
    }
}

/**
 * @return false if it is last file, true otherwise
 */
template <typename StreamT>
static bool DecodeContent(BitsIStream<StreamT> &archive_stream, const HaffmanTrieNode &codes_trie,
                          std::ostream &content_stream) {
    while (true) {
        NineBits symbol = ReadEncodedSymbol(archive_stream, codes_trie);
        if (symbol == ONE_MORE_FILE) {
            return true;
        } else if (symbol == ARCHIVE_END) {
            return false;
        } else if (static_cast<uint16_t>(symbol) >= 256) {
            throw std::runtime_error("Enexpected control symbol");
        } else {
            content_stream << static_cast<char>(symbol);
        }
    }
}

/**
 * @return true if it is last file, false otherwise
 */
template <typename StreamT>
static bool UnarchiveFile(BitsIStream<StreamT> &archive_stream) {
    auto codes_trie = ReadCode(archive_stream);
    std::string filename = ReadFileName(archive_stream, codes_trie);
    std::ofstream file_stream(filename);
    file_stream.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);
    try {
        return DecodeContent(archive_stream, codes_trie, file_stream);
    } catch (...) {
        std::filesystem::remove(filename);
        std::rethrow_exception(std::current_exception());
    }
}

void Unarchive(std::filesystem::path archive_name) {
    std::ifstream file_archive_stream(archive_name);
    file_archive_stream.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);
    BitsIStream archive_stream(file_archive_stream);
    while (UnarchiveFile(archive_stream)) {
    }
}
