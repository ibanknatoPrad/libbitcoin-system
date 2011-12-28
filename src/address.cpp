#include <bitcoin/address.hpp>

#include <bitcoin/constants.hpp>
#include <bitcoin/data_helpers.hpp>
#include <bitcoin/util/base58.hpp>
#include <bitcoin/util/ripemd.hpp>
#include <bitcoin/util/sha256.hpp>

namespace libbitcoin {

std::string public_key_to_address(const data_chunk& public_key)
{
    data_chunk unhashed_address;
    unhashed_address.push_back(0);
    extend_data(unhashed_address, generate_ripemd_hash(public_key));
    uint32_t checksum = generate_sha256_checksum(unhashed_address);
    extend_data(unhashed_address, uncast_type(checksum));
    return encode_base58(unhashed_address);
}

short_hash address_to_short_hash(const std::string& address)
{
    const data_chunk decoded_address = decode_base58(address);
    // version + 20 bytes short hash + 4 bytes checksum
    if (decoded_address.size() != 25)
        return null_short_hash;
    const uint8_t version = decoded_address[0];
    if (version != 0)
        return null_short_hash;
    const data_chunk checksum_bytes(
        decoded_address.end() - 4, decoded_address.end());
    // version + short hash
    const data_chunk main_body(
        decoded_address.begin(), decoded_address.end() - 4);
    // verify checksum bytes
    if (generate_sha256_checksum(main_body) != 
            cast_chunk<uint32_t>(checksum_bytes))
        return null_short_hash;
    short_hash result;
    std::copy(main_body.begin() + 1, main_body.end(), result.begin());
    return result;
}

} // libbitcoin
