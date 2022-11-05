#pragma once

#include <array>
#include <functional>
#include <memory>
#include <cstddef>
#include <vector>

/**
 * @brief Trie with O(height) operations and O(size * AlphabetSize) memory
 *
 * @tparam T: type stored in nodes
 * @tparam AlphabetSize: Alphabet is numbers in [0; AlphabetSize)
 *
 */
template <typename T, size_t AlphabetSize>
class TrieNode {
public:
    using AlphabetT = size_t;
    using ArrayT = std::array<std::unique_ptr<TrieNode>, AlphabetSize>;

    TrieNode(bool is_terminal, T value) : value_(std::move(value)), is_terminal_(is_terminal) {
    }

    explicit TrieNode(bool is_terminal) : is_terminal_(is_terminal) {
    }

    void SetChildren(AlphabetT key, std::unique_ptr<TrieNode> child) {
        children_[key] = std::move(child);
    }

    bool IsTerminal() const {
        return is_terminal_;
    }

    /**
     * @return Pointer to TrieNode if children exists and nullptr otherwise
     */
    const TrieNode *GetChild(AlphabetT key) const {
        return children_.at(key).get();
    }

    TrieNode *GetChild(AlphabetT key) {
        return *children_.at(key).get();
    }

    T Value() const {
        return value_;
    }

    void WalkTrie(std::function<void(const std::vector<AlphabetT> &, const T &)> on_terminal) {
        std::vector<AlphabetT> way;
        WalkTrie(way, on_terminal);
    }

    /**
     * @brief Creates way. Make at the end terminated node with `value`
     *
     * @tparam It forward iterator of AlphabetT
     * @param way_first
     * @param way_last
     * @param value
     */
    template <typename It>
    void AddWay(It way_first, It way_last, T value) {
        if (way_first == way_last) {
            is_terminal_ = true;
            value_ = value;
        } else {
            if (!children_[*way_first]) {
                children_[*way_first] = std::make_unique<TrieNode>(false);
            }
            children_[*way_first]->AddWay(++way_first, way_last, value);
        }
    }

private:
    void WalkTrie(std::vector<AlphabetT> &way,
                  std::function<void(const std::vector<AlphabetT> &, const T &)> on_terminal) {
        if (IsTerminal()) {
            on_terminal(way, value_);
        }
        for (AlphabetT i = 0; i < AlphabetSize; ++i) {
            if (!children_[i]) {
                continue;
            }
            way.push_back(i);
            children_[i]->WalkTrie(way, on_terminal);
            way.pop_back();
        }
    }
    T value_;
    ArrayT children_;
    bool is_terminal_;
};
