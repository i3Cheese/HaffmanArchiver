#pragma once

#include <algorithm>
#include <vector>
#include <ostream>

enum class Bit : uint8_t {
    ZERO = 0,
    ONE = 1,
};

class Bits {
public:
    inline Bits& operator++() {
        for (auto it = code_.rbegin(); it != code_.rend(); ++it) {
            if (*it == Bit::ONE) {
                *it = Bit::ZERO;
            } else {
                *it = Bit::ONE;
                return *this;
            }
        }
        code_.emplace(code_.begin(), Bit::ONE);  // executes almost never
        return *this;
    }
    template <class size_t>
    inline Bits& operator<<=(size_t shift) noexcept {
        code_.resize(code_.size() + shift, Bit::ZERO);
        return *this;
    }
    template <class size_t>
    inline Bits operator<<(size_t shift) noexcept {
        Bits result;
        result.code_.assign(code_.size() + shift, Bit::ZERO);
        std::copy(code_.begin(), code_.end(), result.code_.begin());
        return result;
    }

    inline size_t Size() const {
        return code_.size();
    }

    /**
     * @brief starts at most significant bit
     *
     */
    inline auto begin() const {  // NOLINT
        return code_.begin();
    }

    inline auto end() const {  // NOLINT
        return code_.end();
    }

private:
    std::vector<Bit> code_;  // stores in big-endian format
};

inline std::ostream& operator<<(std::ostream& stream, Bit bit) {
    return stream << static_cast<int>(bit);
}

inline std::ostream& operator<<(std::ostream& stream, const Bits& bits) {
    for (auto b : bits) {
        stream << b;
    }
    return stream;
}
