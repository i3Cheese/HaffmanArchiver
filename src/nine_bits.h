#pragma once

#include <cinttypes>
#include <ostream>
#include <type_traits>
#include <stddef.h>

enum class NineBits : uint16_t {};

inline const size_t NINE_BITS_MAX = (1 << 9) - 1;

inline std::ostream& operator<<(std::ostream& stream, NineBits nine_bits) {
    return stream << static_cast<int>(nine_bits);
}

inline NineBits CharToNineBits(char c) {
    return static_cast<NineBits>(static_cast<uint8_t>(c));
}

template <bool>
struct EnableIfIntegralImp {};
template <>
struct EnableIfIntegralImp<true> {
    using Type = NineBits;
};
template <class _Tp>
using EnableByteOverload = typename EnableIfIntegralImp<std::is_integral<_Tp>::value>::Type;

constexpr NineBits operator|(NineBits lhs, NineBits rhs) noexcept {
    return static_cast<NineBits>(
        static_cast<uint16_t>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs)));
}

constexpr NineBits& operator|=(NineBits& lhs, NineBits rhs) noexcept {
    return lhs = lhs | rhs;
}

constexpr NineBits operator&(NineBits lhs, NineBits rhs) noexcept {
    return static_cast<NineBits>(
        static_cast<uint16_t>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs)));
}

constexpr NineBits& operator&=(NineBits& lhs, NineBits rhs) noexcept {
    return lhs = lhs & rhs;
}

constexpr NineBits operator^(NineBits lhs, NineBits rhs) noexcept {
    return static_cast<NineBits>(
        static_cast<uint16_t>(static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs)));
}

constexpr NineBits& operator^=(NineBits& lhs, NineBits rhs) noexcept {
    return lhs = lhs ^ rhs;
}

constexpr NineBits operator~(NineBits b) noexcept {
    return static_cast<NineBits>(static_cast<uint16_t>(~static_cast<unsigned int>(b)));
}
template <class _Integer>
constexpr EnableByteOverload<_Integer>& operator<<=(NineBits& lhs, _Integer shift) noexcept {
    return lhs = lhs << shift;
}

template <class _Integer>
constexpr EnableByteOverload<_Integer> operator<<(NineBits lhs, _Integer shift) noexcept {
    return static_cast<NineBits>(static_cast<uint16_t>(static_cast<unsigned int>(lhs) << shift));
}

template <class _Integer>
constexpr EnableByteOverload<_Integer>& operator>>=(NineBits& lhs, _Integer shift) noexcept {
    return lhs = lhs >> shift;
}

template <class _Integer>
constexpr EnableByteOverload<_Integer> operator>>(NineBits lhs, _Integer shift) noexcept {
    return static_cast<NineBits>(static_cast<uint16_t>(static_cast<unsigned int>(lhs) >> shift));
}

template <class _Integer, class = EnableByteOverload<_Integer> >
[[nodiscard]] constexpr _Integer ToInteger(NineBits b) noexcept {
    return static_cast<_Integer>(b);
}
