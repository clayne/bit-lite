//
// Copyright (c) 2020-2020 Martin Moene
//
// https://github.com/martinmoene/bit-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "bit-main.t.hpp"

#include <climits>      // CHAR_BIT, when bit_USES_STD_BIT
#include <iostream>

#if bit_CPP11_90
# include <cstdint>
    namespace std11 { using std::uint64_t; }
#else
    namespace std11 { typedef unsigned long long uint64_t; }
#endif

#if bit_USES_STD_BIT
namespace std {
#else
namespace nonstd { namespace bit {
#endif

inline std::string to_string( endian const & e )
{
    using nonstd::endian;

    if ( e == endian::little )
        return "[little endian]";
    else if ( e == endian::big )
        return "[big endian]";
    else if ( e == endian::native )
        return "[native endian (distinct)]";
    else
        return "[??? endian]";
}

inline std::ostream & operator<<( std::ostream & os, endian const & e )
{
    return os << to_string( e );
}

#if bit_USES_STD_BIT
} // namespace std
#else
}} // namespace nonstd::bit
#endif

template< typename T >
inline T bitmask( int i )
{
    return static_cast<T>( T(1) << i );
}

using namespace nonstd;

//
// bit_cast<>()
//

// nonstd::bit_cast is not const_expr:

#define no_constexpr /*constexpr*/

CASE( "bit_cast<>(): ..." )
{
    no_constexpr double f64v = 19880124.0;
    no_constexpr ::std11::uint64_t u64v = nonstd::bit_cast< ::std11::uint64_t >(f64v);

    no_constexpr ::std11::uint64_t u64v2 = 0x3fe9000000000000ull;
    no_constexpr double  f64v2 = nonstd::bit_cast<double>(u64v2);

    std::cout
        << std::fixed << f64v << ": f64.to_bits() == 0x"
        << std::hex   << u64v << ": u64\n" << std::dec;

    std::cout
        << std::hex   << "f64::from_bits(0x" << u64v2 << "u64) == " << std::dec
        << std::fixed << f64v2 << "f64\n";

    EXPECT( true );
}

CASE( "has_single_bit(): single bit yields true for single bits set" )
{
    typedef unsigned long type;
    const int N = CHAR_BIT * sizeof(type);

    for( int i = 0; i < N; ++i )
    {
        // std::cout << "i:" << i << " mask:" << std::hex << bitmask<type>(i) << std::dec << '\n';
        EXPECT( has_single_bit( bitmask<type>(i) ) );
    }
}

CASE( "has_single_bit(): single bit yields false for mutiple bits set" )
{
    typedef unsigned long type;
    const int N = CHAR_BIT * sizeof(type);

    for( int i = 0; i < N; ++i )
    {
        if ( i != N-i )
        {
            EXPECT_NOT( has_single_bit( static_cast<type>(bitmask<type>(i) | bitmask<type>(N-i-1)) ) );
        }
    }
}

#include <bitset>

CASE( "bit_ceil(): implement" )
{
    EXPECT( !!"bit_ceil(): implement" );

    typedef std::bitset<8> bin;

    for (unsigned x = 0; x != 10; ++x)
    {
        unsigned const z = nonstd::bit_ceil(x); // `ceil2` before P1956R1

#if defined(_MSC_VER) && _MSC_VER < 1700
        std::cout << "bit_ceil(" << bin(int(x)) << ") = " << bin(int(z)) << '\n';
#else
        std::cout << "bit_ceil(" << bin(x) << ") = " << bin(z) << '\n';
#endif
    }
}

CASE( "bit_floor(): implement" )
{
    EXPECT( !!"bit_floor(): implement" );

    typedef std::bitset<8> bin;

    for (unsigned x = 0; x != 10; ++x)
    {
        unsigned const z = nonstd::bit_floor(x); // `floor2` before P1956R1

#if defined(_MSC_VER) && _MSC_VER < 1700
        std::cout << "bit_floor(" << bin(int(x)) << ") = " << bin(int(z)) << '\n';
#else
        std::cout << "bit_floor(" << bin(x) << ") = " << bin(z) << '\n';
#endif
    }
}

CASE( "bit_width: implement" )
{
    EXPECT( !!"bit_width()(): implement" );

    for (unsigned x = 0; x != 8; ++x)
    {
        std::cout
            << "bit_width( "
#if defined(_MSC_VER) && _MSC_VER < 1700
            << std::bitset<4>(int(x)) << " ) = "
#else
            << std::bitset<4>(x) << " ) = "
#endif
            << nonstd::bit_width(x) << '\n';
    }
}

CASE( "rotl(): implement" )
{
    EXPECT( !!"rotl(): implement" );

    typedef unsigned char uint8_t;
    uint8_t i = 29; // 0b00011101;
    std::cout << "i          = " << std::bitset<8>(i) << '\n';
    std::cout << "rotl(i,0)  = " << std::bitset<8>(nonstd::rotl(i,0)) << '\n';
    std::cout << "rotl(i,1)  = " << std::bitset<8>(nonstd::rotl(i,1)) << '\n';
    std::cout << "rotl(i,4)  = " << std::bitset<8>(nonstd::rotl(i,4)) << '\n';
    std::cout << "rotl(i,9)  = " << std::bitset<8>(nonstd::rotl(i,9)) << '\n';
    std::cout << "rotl(i,-1) = " << std::bitset<8>(nonstd::rotl(i,-1)) << '\n';
}

CASE( "rotr(): implement" )
{
    EXPECT( !!"rotr(): implement" );

    typedef unsigned char uint8_t;
    uint8_t i = 29; // 0b00011101;
    std::cout << "i          = " << std::bitset<8>(i) << '\n';
    std::cout << "rotr(i,0)  = " << std::bitset<8>(nonstd::rotr(i,0)) << '\n';
    std::cout << "rotr(i,1)  = " << std::bitset<8>(nonstd::rotr(i,1)) << '\n';
    std::cout << "rotr(i,9)  = " << std::bitset<8>(nonstd::rotr(i,9)) << '\n';
    std::cout << "rotr(i,-1) = " << std::bitset<8>(nonstd::rotr(i,-1)) << '\n';
}

CASE( "countl_zero(): implement" )
{
    EXPECT( !!"countl_zero(): implement" );

    typedef unsigned char type;
    type table[] = { 0u, 255u, 28u };

    for ( int i = 0; i < 3; ++i )
    {
        type x = table[i];

        std::cout
            << "countl_zero(0b" << std::bitset<8>(x) << ") = "
            << nonstd::countl_zero(x) << '\n';
    }
}

CASE( "countl_one(): implement" )
{
    EXPECT( !!"countl_one(): implement" );

    typedef unsigned char type;
    type table[] = { 0u, 255u, 227u };

    for ( int i = 0; i < 3; ++i )
    {
        type x = table[i];
        std::cout
            << "countl_one(0b" << std::bitset<8>(x) << ") = "
            << nonstd::countl_one(x) << '\n';
    }
}

CASE( "countr_zero(): implement" )
{
    EXPECT( !!"countr_zero(): implement" );

    typedef unsigned char type;
    type table[] = { 0u, 255u, 28u };

    for ( int i = 0; i < 3; ++i )
    {
        type x = table[i];

        std::cout
            << "countr_zero(0b" << std::bitset<8>(x) << ") = "
            << nonstd::countr_zero(x) << '\n';
    }
}

CASE( "countr_one(): implement" )
{
    EXPECT( !!"countr_one(): implement" );

    // for (std::uint8_t i : { 0, 0b11111111, 0b11100011 })

    typedef unsigned char type;
    type table[] = { 0u, 255u, 227u };

    for ( int i = 0; i < 3; ++i )
    {
        type x = table[i];

        std::cout
            << "countr_one(0b" << std::bitset<8>(x) << ") = "
            << nonstd::countr_one(x) << '\n';
    }
}

CASE( "popcount(): implement" )
{
    EXPECT( !!"popcount(): implement" );

    // for (std::uint8_t i : { 0, 0b11111111, 0b00011101 })

    typedef unsigned char type;
    type table[] = { 0u, 255u, 29u };

    for ( int i = 0; i < 3; ++i )
    {
        type x = table[i];

        std::cout
            << "popcount(0b" << std::bitset<8>(x) << ") = "
            << nonstd::popcount(x) << '\n';
    }
}

CASE( "endian: little differs from big (corner-case when all scalars have size of 1 byte)" )
{
    EXPECT( nonstd::endian::little != nonstd::endian::big );
}

//
// Extensions: endian conversions
//

CASE( "to_big_endian(): [extension]" )
{
#if bit_USES_STD_BIT
    EXPECT( !!"Extension to_big_endian() not available (bit_USES_STD_BIT)" );
#elif bit_CONFIG_STRICT
    EXPECT( !!"Extension to_big_endian() not available (bit_CONFIG_STRICT)" );
#else
    std::cout << "to_big_endian(0xabcd) => " << std::showbase << std::hex << to_big_endian(0xabcdU) << std::dec << '\n';
    std::cout << "to_big_endian(0xabcd, is_little_endian_t()) => " << std::showbase << std::hex << to_big_endian(0xabcdU, is_little_endian_t()) << std::dec << '\n';
    std::cout << "to_big_endian(0xabcd, is_big_endian_t()) => " << std::showbase << std::hex << to_big_endian(0xabcdU, is_big_endian_t()) << std::dec << '\n';
#endif
}

CASE( "to_little_endian(): [extension]" )
{
#if bit_USES_STD_BIT
    EXPECT( !!"Extension to_little_endian() not available (bit_USES_STD_BIT)" );
#elif bit_CONFIG_STRICT
    EXPECT( !!"Extension to_little_endian() not available (bit_CONFIG_STRICT)" );
#else
    std::cout << "to_little_endian(0xabcd) => " << std::showbase << std::hex << to_little_endian(0xabcdU) << std::dec << '\n';
    std::cout << "to_little_endian(0xabcd, is_little_endian_t()) => " << std::showbase << std::hex << to_little_endian(0xabcdU, is_little_endian_t()) << std::dec << '\n';
    std::cout << "to_little_endian(0xabcd, is_big_endian_t()) => " << std::showbase << std::hex << to_little_endian(0xabcdU, is_big_endian_t()) << std::dec << '\n';
#endif
}

CASE( "to_native_endian(): [extension]" )
{
#if bit_USES_STD_BIT
    EXPECT( !!"Extension to_native_endian() not available (bit_USES_STD_BIT)" );
#elif bit_CONFIG_STRICT
    EXPECT( !!"Extension to_native_endian() not available (bit_CONFIG_STRICT)" );
#else
    std::cout << "to_native_endian(0xabcd) => " << std::showbase << std::hex << to_native_endian(0xabcdU) << std::dec << '\n';
#endif
}

CASE( "as_big_endian(): [extension]" )
{
#if bit_USES_STD_BIT
    EXPECT( !!"Extension as_big_endian() not available (bit_USES_STD_BIT)" );
#elif bit_CONFIG_STRICT
    EXPECT( !!"Extension as_big_endian() not available (bit_CONFIG_STRICT)" );
#else
    std::cout << "as_big_endian(0xabcd) => " << std::showbase << std::hex << as_big_endian(0xabcdU) << std::dec << '\n';
#endif
}

CASE( "as_little_endian(): [extension]" )
{
#if bit_USES_STD_BIT
    EXPECT( !!"Extension as_little_endian() not available (bit_USES_STD_BIT)" );
#elif bit_CONFIG_STRICT
    EXPECT( !!"Extension as_little_endian() not available (bit_CONFIG_STRICT)" );
#else
    std::cout << "as_little_endian(0xabcd) => " << std::showbase << std::hex << as_little_endian(0xabcdU) << std::dec << '\n';
#endif
}

CASE( "as_native_endian(): [extension]" )
{
#if bit_USES_STD_BIT
    EXPECT( !!"Extension as_native_endian() not available (bit_USES_STD_BIT)" );
#elif bit_CONFIG_STRICT
    EXPECT( !!"Extension as_native_endian() not available (bit_CONFIG_STRICT)" );
#else
    std::cout << "as_native_endian(0xabcd) => " << std::showbase << std::hex << as_native_endian(0xabcdU) << std::dec << '\n';
#endif
}

// g++ -std=c++11 -I../include -o bit.t.exe bit.t.cpp && bit.t.exe
// cl -EHsc -I../include bit.t.cpp && bit.t.exe
