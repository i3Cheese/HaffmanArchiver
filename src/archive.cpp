#include <compare>
#include <cstddef>
#include <ios>
#include <iostream>
#include <istream>
#include <map>
#include <string>
#include <sstream>
#include <iterator>
#include <array>
#include <limits>
#include <filesystem>
#include <fstream>
#include <vector>
#include <memory>
#include <utility>

#include "bits_stream.h"
#include "priority_queue.h"
#include "trie.h"
#include "nine_bits.h"
#include "symbols_counter.h"
#include "bits.h"
#include "constants.h"

using SortedHaffmanCodes = const std::vector<std::pair<NineBits, Bits>>;
using HaffmanCodes = std::map<NineBits, Bits>;

template <typename It>
static void CountSymbols(It first, It last, SymbolsCounter &counter) {
    for (; first != last; ++first) {
        ++counter[CharToNineBits(*first)];
    }
}

/**
 * @brief build canonical Haffman codes
 */
static SortedHaffmanCodes BuildCodes(const SymbolsCounter &counter) {
    using HaffmanTrieNode = TrieNode<NineBits, 2>;

    struct PQValue {
        std::unique_ptr<HaffmanTrieNode> node;
        size_t count;
        auto operator<=>(const PQValue &other) const {
            if (count == other.count) {
                return node->Value() <=> other.node->Value();
            } else {
                return count <=> other.count;
            }
        }
    };

    PriorityQueue<PQValue, std::greater<PQValue>> chars;
    for (size_t i = 0; i < counter.size(); ++i) {
        auto c = static_cast<NineBits>(i);
        if (counter[i] == 0) {
            continue;
        }
        auto node = std::make_unique<HaffmanTrieNode>(true, c);
        chars.Push({std::move(node), counter[i]});
    }
    if (chars.Size() == 0) {
        return {};
    }
    while (chars.Size() >= 2) {
        auto pq_v0 = chars.TopPop();
        auto pq_v1 = chars.TopPop();
        NineBits min_c = std::min(pq_v0.node->Value(), pq_v1.node->Value());
        auto new_node = std::make_unique<HaffmanTrieNode>(false, min_c);
        new_node->SetChildren(0, std::move(pq_v0.node));
        new_node->SetChildren(1, std::move(pq_v1.node));
        chars.Push({std::move(new_node), pq_v0.count + pq_v1.count});
    }

    std::vector<std::pair<size_t, NineBits>> codes_sizes;
    chars.TopPop().node->WalkTrie([&codes_sizes](auto way, NineBits symbol) {
        codes_sizes.push_back({way.size(), symbol});
    });
    std::sort(codes_sizes.begin(), codes_sizes.end());

    std::vector<std::pair<NineBits, Bits>> codes;
    codes.reserve(codes_sizes.size());
    auto code = Bits() << codes_sizes[0].first;
    codes.emplace_back(codes_sizes[0].second, code);
    for (size_t i = 1; i < codes_sizes.size(); ++i) {
        code = (++code) << static_cast<size_t>(codes_sizes[i].first - codes_sizes[i - 1].first);
        codes.emplace_back(codes_sizes[i].second, code);
    }
    return codes;
}

template <typename It, typename StreamT>
static void ArchiveIt(It first, It last, const HaffmanCodes &codes, BitsOStream<StreamT> &archive_stream) {
    for (; first != last; ++first) {
        archive_stream << codes.at(CharToNineBits(*first));
    }
}

template <typename StreamT>
static void ArchiveCodes(const SortedHaffmanCodes &sorted_codes, BitsOStream<StreamT> &archive_stream) {
    archive_stream << static_cast<NineBits>(sorted_codes.size());
    std::vector<size_t> count_with_lengths;
    for (const auto &[symbol, code] : sorted_codes) {
        archive_stream << symbol;
        while (count_with_lengths.size() < code.Size()) {
            count_with_lengths.push_back(0);
        }
        ++count_with_lengths.back();
    }
    for (auto count : count_with_lengths) {
        archive_stream << static_cast<NineBits>(count);
    }
}

template <typename StreamT>
static void ArchiveFile(const std::filesystem::path &file, BitsOStream<StreamT> &archive_stream, bool is_last_file) {
    SymbolsCounter counter;

    std::ifstream file_stream(file, std::ios::binary);
    file_stream.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);
    CountSymbols(std::istreambuf_iterator(file_stream), {}, counter);
    file_stream.seekg(0);

    std::string filename = file.filename();
    CountSymbols(filename.begin(), filename.end(), counter);
    ++counter[FILENAME_END];
    ++counter[ONE_MORE_FILE];
    ++counter[ARCHIVE_END];

    SortedHaffmanCodes sorted_codes = BuildCodes(counter);

    ArchiveCodes(sorted_codes, archive_stream);

    HaffmanCodes codes(sorted_codes.begin(), sorted_codes.end());

    ArchiveIt(filename.begin(), filename.end(), codes, archive_stream);
    archive_stream << codes.at(FILENAME_END);
    ArchiveIt(std::istreambuf_iterator(file_stream), {}, codes, archive_stream);
    if (is_last_file) {
        archive_stream << codes.at(ARCHIVE_END);
    } else {
        archive_stream << codes.at(ONE_MORE_FILE);
    }
}

void Archive(const std::vector<std::filesystem::path> &files, const std::filesystem::path &archive_name) {
    std::ofstream file_archive_stream(archive_name);
    file_archive_stream.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);
    BitsOStream archive_stream(file_archive_stream);

    for (size_t i = 0; i < files.size(); ++i) {
        ArchiveFile(files[i], archive_stream, i == files.size() - 1);
    }

    archive_stream.Flush();
}
