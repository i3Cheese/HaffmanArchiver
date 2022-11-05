#pragma once

#include <array>

#include "nine_bits.h"
#include "bits.h"

class SymbolsCounter {
public:
    inline size_t& operator[](NineBits symbol) {
        return array_[static_cast<size_t>(symbol)];
    }
    inline size_t operator[](NineBits symbol) const {
        return array_[static_cast<size_t>(symbol)];
    }
    inline size_t& operator[](size_t symbol) {
        return array_[static_cast<size_t>(symbol)];
    }
    inline size_t operator[](size_t symbol) const {
        return array_[static_cast<size_t>(symbol)];
    }
    inline auto begin() const {  // NOLINT
        return array_.begin();
    }
    inline auto end() const {  // NOLINT
        return array_.end();
    }
    inline auto size() const {  // NOLINT
        return array_.size();
    }

    inline auto GetUnder() const {
        return array_;
    }

    inline auto GetUnder() {
        return array_;
    }

private:
    std::array<size_t, NINE_BITS_MAX + 1> array_ = {0};
};
