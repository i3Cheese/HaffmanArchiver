#include <string>
#include <sstream>

#include <catch.hpp>

#include "bits_stream.h"
#include "nine_bits.h"

TEST_CASE("BitsOStream_OneBit") {
    std::ostringstream x;
    BitsOStream stream(x);
    stream << Bit{1};
    stream.Flush();
    std::string expected;
    expected.push_back(static_cast<char>(0b10000000));
    REQUIRE(x.str() == expected);
}

TEST_CASE("BitsOStream_OneByte") {
    std::ostringstream x;
    BitsOStream stream(x);
    stream << Bit{1} << Bit{1} << Bit{0} << Bit{1} << Bit{0} << Bit{1} << Bit{1} << Bit{1};
    stream.Flush();
    std::string expected;
    expected.push_back(static_cast<char>(0b11010111));
    REQUIRE(x.str() == expected);
}

TEST_CASE("BitsOStream_OneNineBits") {
    std::ostringstream x;
    BitsOStream stream(x);
    stream << NineBits{0b110101111};
    stream.Flush();
    std::string expected;
    expected.push_back(static_cast<char>(0b11010111));
    expected.push_back(static_cast<char>(0b10000000));
    REQUIRE(x.str().size() == 2);
    REQUIRE(x.str() == expected);
}

TEST_CASE("BitsOStream_OneNineBitsSevenBits") {
    std::ostringstream osstream;
    BitsOStream stream(osstream);
    stream << NineBits{0b110101111} << Bit{0} << Bit{0} << Bit{0} << Bit{1} << Bit{0} << Bit{1} << Bit{1};
    std::string expected;
    expected.push_back(static_cast<char>(0b11010111));
    expected.push_back(static_cast<char>(0b10001011));
    REQUIRE(osstream.str().size() == 2);
    REQUIRE(osstream.str() == expected);
}

TEST_CASE("BitsIStream_OneBit") {
    std::string content;
    content.push_back(static_cast<char>(0b10000000));
    std::istringstream isstream(content);
    BitsIStream stream(isstream);

    Bit bit;
    stream >> bit;
    REQUIRE(bit == Bit{1});
}

TEST_CASE("BitsIStream_OneByte") {
    std::string content;
    content.push_back(static_cast<char>(0b11010111));
    std::istringstream isstream(content);
    BitsIStream stream(isstream);

    std::vector<Bit> readed(8);
    for (size_t i = 0; i < readed.size(); ++i) {
        stream >> readed[i];
    }
    std::vector<Bit> expected{Bit{1}, Bit{1}, Bit{0}, Bit{1}, Bit{0}, Bit{1}, Bit{1}, Bit{1}};
    REQUIRE(readed == expected);
}

TEST_CASE("BitsIStream_NineBits") {
    std::string content;
    content.push_back(static_cast<char>(0b11010111));
    content.push_back(static_cast<char>(0b10000000));

    std::istringstream isstream(content);
    BitsIStream stream(isstream);

    NineBits nine_bits;
    stream >> nine_bits;
    REQUIRE(nine_bits == NineBits{0b110101111});
}
