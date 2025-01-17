/**
 * Copyright (c) 2011-2022 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(endian_tests)

// Templates must match uintx_t<unsigned> vs. uintx_t<size_t>.
BOOST_AUTO_TEST_CASE(endian__to_array__zero__expected)
{
    BOOST_REQUIRE_EQUAL(from_uintx(uint5_t{ 1 }), data_array<1>{ 1 });
    BOOST_REQUIRE_EQUAL(from_uintx(uint11_t{ 0 }), data_array<2>{});
    BOOST_REQUIRE_EQUAL(from_uintx(uint48_t{ 0 }), data_array<6>{});
    BOOST_REQUIRE_EQUAL(from_uintx(uint128_t{ 0 }), data_array<16>{});
    BOOST_REQUIRE_EQUAL(from_uintx(uint160_t{ 0 }), data_array<20>{});
    BOOST_REQUIRE_EQUAL(from_uintx(uint256_t{ 0 }), data_array<32>{});
    BOOST_REQUIRE_EQUAL(from_uintx(uint512_t{ 0 }), data_array<64>{});
}

// Templates must match uintx_t<unsigned> vs. uintx_t<size_t>.
BOOST_AUTO_TEST_CASE(endian__to_uintx__zero__expected)
{
    BOOST_REQUIRE_EQUAL(to_uintx(data_array<1>{ 1 }), uint5_t{ 1 });
    BOOST_REQUIRE_EQUAL(to_uintx(data_array<2>{}), uint11_t{ 0 });
    BOOST_REQUIRE_EQUAL(to_uintx(data_array<6>{}), uint48_t{ 0 });
    BOOST_REQUIRE_EQUAL(to_uintx(data_array<16>{}), uint128_t{ 0 });
    BOOST_REQUIRE_EQUAL(to_uintx(data_array<20>{}), uint160_t{ 0 });
    BOOST_REQUIRE_EQUAL(to_uintx(data_array<32>{}), uint256_t{ 0 });
    BOOST_REQUIRE_EQUAL(to_uintx(data_array<64>{}), uint512_t{ 0 });
}

// ENDIAN_ARRAY_UINTX

constexpr uint32_t value32 = 0x01020304;
const data_chunk data_forward{ 0x01, 0x02, 0x03, 0x04 };
const data_chunk data_reverse{ 0x04, 0x03, 0x02, 0x01 };
const data_array<4> array_forward{ { 0x01, 0x02, 0x03, 0x04 } };
const data_array<4> array_reverse{ { 0x04, 0x03, 0x02, 0x01 } };

BOOST_AUTO_TEST_CASE(endian__from_big_endian__data_to_uintx__expected)
{
    // Populating uintx8
    BOOST_REQUIRE_EQUAL(uintx_from_big_endian_chunk<1>(data_forward), value32 >> 24u);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__data_to_uintx__expected)
{
    // Populating uintx800
    BOOST_REQUIRE_EQUAL(uintx_from_little_endian_chunk<100>(data_reverse), value32);
}

BOOST_AUTO_TEST_CASE(endian__to_big_endian__uintx_to_data__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<4>(value32), array_forward);
}

BOOST_AUTO_TEST_CASE(endian__to_little_endian__uintx_to_data__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<4>(value32), array_reverse);
}

// ENDIAN_SYMMETRIC_ROUND_TRIPS

const mini_hash bytes6 = base16_array(
    "010203040506");
const quarter_hash bytes8 = base16_array(
    "0102030405060708");
const half_hash bytes16 = base16_array(
    "01020304050607080102030405060708");
const short_hash bytes20 = base16_array(
    "0102030405060708010203040506070801020304");
const hash_digest bytes32 = base16_array(
    "0102030405060708010203040506070801020304050607080102030405060708");
const long_hash bytes64 = base16_array(
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708");

// This is not a predefined hash size or uintx_t type.
const data_array<128> bytes128 = base16_array(
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708");

// big endian

BOOST_AUTO_TEST_CASE(endian__round_trip__uint48_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<6>(uintx_from_big_endian_array<6>(bytes6)), bytes6);
}

// This is uintx64 not uint64_t!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<8>(uintx_from_big_endian_array<8>(bytes8)), bytes8);
}

// This is uint64_t not uintx64!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_t_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian(from_big_endian<uint64_t>(bytes8)), bytes8);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint128_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<16>(uintx_from_big_endian_array<16>(bytes16)), bytes16);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint160_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<20>(uintx_from_big_endian_array<20>(bytes20)), bytes20);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint256_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<32>(uintx_from_big_endian_array<32>(bytes32)), bytes32);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint512_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<64>(uintx_from_big_endian_array<64>(bytes64)), bytes64);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint1024_big_endian__expected)
{
    // Reading more bytes than provided.
    BOOST_REQUIRE_EQUAL(to_big_endian<128>(uintx_from_big_endian_array<128>(bytes128)), bytes128);
}

// little endian

BOOST_AUTO_TEST_CASE(endian__round_trip__uint48_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<6>(uintx_from_little_endian_array<6>(bytes6)), bytes6);
}

// This is uintx64 not uint64_t!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<8>(uintx_from_little_endian_array<8>(bytes8)), bytes8);
}

// This is uint64_t not uintx64!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_t_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian(from_little_endian<uint64_t>(bytes8)), bytes8);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint128_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<16>(uintx_from_little_endian_array<16>(bytes16)), bytes16);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint160_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<20>(uintx_from_little_endian_array<20>(bytes20)), bytes20);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint256_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<32>(uintx_from_little_endian_array<32>(bytes32)), bytes32);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint512_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<64>(uintx_from_little_endian_array<64>(bytes64)), bytes64);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint1024_little_endian__expected)
{
    // Reading more bytes than provided.
    BOOST_REQUIRE_EQUAL(to_little_endian<128>(uintx_from_little_endian_array<128>(bytes128)), bytes128);
}

// ENDIAN_ASYMMETRIC_ROUND_TRIPS

BOOST_AUTO_TEST_CASE(endian__round_trip__uint48__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<6>(uintx_from_little_endian_array<6>(bytes6)), reverse_copy(bytes6));
    BOOST_REQUIRE_EQUAL(to_little_endian<6>(uintx_from_big_endian_array<6>(bytes6)), reverse_copy(bytes6));
}

// This is uintx64 not uint64_t!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<8>(uintx_from_little_endian_array<8>(bytes8)), reverse_copy(bytes8));
    BOOST_REQUIRE_EQUAL(to_little_endian<8>(uintx_from_big_endian_array<8>(bytes8)), reverse_copy(bytes8));
}

// This is uint64_t not uintx64!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_t__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian(from_little_endian<uint64_t>(bytes8)), reverse_copy(bytes8));
    BOOST_REQUIRE_EQUAL(to_little_endian(from_big_endian<uint64_t>(bytes8)), reverse_copy(bytes8));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint128__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<16>(uintx_from_little_endian_array<16>(bytes16)), reverse_copy(bytes16));
    BOOST_REQUIRE_EQUAL(to_little_endian<16>(uintx_from_big_endian_array<16>(bytes16)), reverse_copy(bytes16));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint160__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<20>(uintx_from_little_endian_array<20>(bytes20)), reverse_copy(bytes20));
    BOOST_REQUIRE_EQUAL(to_little_endian<20>(uintx_from_big_endian_array<20>(bytes20)), reverse_copy(bytes20));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint256__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<32>(uintx_from_little_endian_array<32>(bytes32)), reverse_copy(bytes32));
    BOOST_REQUIRE_EQUAL(to_little_endian<32>(uintx_from_big_endian_array<32>(bytes32)), reverse_copy(bytes32));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint512__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<64>(uintx_from_little_endian_array<64>(bytes64)), reverse_copy(bytes64));
    BOOST_REQUIRE_EQUAL(to_little_endian<64>(uintx_from_big_endian_array<64>(bytes64)), reverse_copy(bytes64));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint1024__expected)
{
    // Reading more bytes than provided.
    BOOST_REQUIRE_EQUAL(to_big_endian<128>(uintx_from_little_endian_array<128>(bytes128)), reverse_copy(bytes128));
    BOOST_REQUIRE_EQUAL(to_little_endian<128>(uintx_from_big_endian_array<128>(bytes128)), reverse_copy(bytes128));
}

BOOST_AUTO_TEST_SUITE_END()
