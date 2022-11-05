#pragma once

#include <bit>
#include <exception>
#include <ios>
#include <iostream>
#include <stdexcept>

#include "nine_bits.h"
#include "bits.h"

constexpr bool IsBigEndian = std::endian::native == std::endian::big;
constexpr bool IsLittleEndian = std::endian::native == std::endian::little;
static_assert(IsBigEndian || IsLittleEndian);

template <class OStreamT>
class BitsOStream {
public:
    explicit BitsOStream(OStreamT& stream) : stream_(stream) {
    }

    BitsOStream& operator<<(NineBits symbol) {
        for (size_t i = 0; i < 9; ++i) {
            *this << static_cast<Bit>(((static_cast<uint16_t>(symbol) >> (9 - 1 - i)) & 1) == 1);
        }
        return *this;
    }

    BitsOStream& operator<<(const Bits& bits) {
        for (Bit bit : bits) {
            *this << bit;
        }
        return *this;
    }

    void Flush() {
        if (buffer_count_ > 0) {
            stream_ << buffer_;
        }
        stream_.flush();
    }

    BitsOStream& operator<<(Bit bit) {
        if (bit == Bit::ONE) {
            if constexpr (IsBigEndian) {
                buffer_ |= (1 << buffer_count_);
            } else if constexpr (IsLittleEndian) {
                buffer_ |= (1 << (8 - 1 - buffer_count_));
            }
        }
        if (++buffer_count_ == 8) {
            stream_.put(buffer_);
            buffer_count_ = 0;
            buffer_ = 0;
        }
        return *this;
    }

private:
    uint8_t buffer_ = 0;
    unsigned buffer_count_ = 0;
    OStreamT& stream_;
};

template <class IStreamT>
class BitsIStream {
public:
    explicit BitsIStream(IStreamT& stream) : stream_(stream) {
    }

    BitsIStream& operator>>(NineBits& symbol) {
        symbol = {0};
        for (size_t i = 0; i < 9; ++i) {
            Bit bit;
            *this >> bit;
            symbol |= static_cast<NineBits>(bit) << (9 - 1 - i);
        }
        return *this;
    }

    BitsIStream& operator>>(Bit& bit) {
        if (buffer_count_ == 0) {
            buffer_ = stream_.get();
        }
        if constexpr (IsBigEndian) {
            bit = static_cast<Bit>((buffer_ >> buffer_count_) & 1);
        } else if constexpr (IsLittleEndian) {
            bit = static_cast<Bit>((buffer_ >> (8 - 1 - buffer_count_)) & 1);
        }
        if (++buffer_count_ == 8) {
            buffer_count_ = 0;
        }
        return *this;
    }

private:
    uint8_t buffer_ = 0;
    unsigned buffer_count_ = 0;
    IStreamT& stream_;
};
