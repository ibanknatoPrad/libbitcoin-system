/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_ELECTRUM_V1_HPP
#define LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_ELECTRUM_V1_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/data/string.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/addresses/witness_address.hpp>
#include <bitcoin/system/wallet/context.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/hd_private.hpp>
#include <bitcoin/system/wallet/mnemonics/dictionaries.hpp>
#include <bitcoin/system/wallet/mnemonics/dictionary.hpp>
#include <bitcoin/system/wallet/mnemonics/language.hpp>
#include <bitcoin/system/wallet/mnemonics/languages.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// TODO: determine if there is an Electrum v1 seed algorithm.
/// A wallet mnemonic, as defined by the first Electrum implementation.
class BC_API electrum_v1
  : public languages
{
public:
    typedef wallet::dictionary<1626> dictionary;
    typedef wallet::dictionaries<2, dictionary::size()> dictionaries;

    /// Publish Electrum v1 word lists.
    static const dictionary::words en;
    static const dictionary::words pt;

    /// Supports 128 or 256 bits of entropy.
    static constexpr size_t entropy_multiple = 4;
    static constexpr size_t entropy_minimum = 4u * entropy_multiple;
    static constexpr size_t entropy_maximum = 8u * entropy_multiple;

    /// Supports 12 or 24 words (128 or 256 bits) of entropy.
    static constexpr size_t word_multiple = 3;
    static constexpr size_t word_minimum = 4u * word_multiple;
    static constexpr size_t word_maximum = 8u * word_multiple;

    /// The two arrays of entropy sizes supported by Electrum v1.
    typedef byte_array<entropy_minimum> minimum_entropy;
    typedef byte_array<entropy_maximum> maximum_entropy;

    /// The dictionary, limited by identifier, that contains all words.
    /// If 'none' is specified all dictionaries are searched.
    static language contained_by(const string_list& words,
        language identifier=language::none);

    /// Valid dictionaries (en, pt).
    static bool is_valid_dictionary(language identifier);

    /// Valid entropy values (16 or 32 bytes).
    static bool is_valid_entropy_size(size_t size);

    /// Valid word counts (12 or 24 words).
    static bool is_valid_word_count(size_t count);

    /// This instance is initialized invalid, but can be assigned to.
    electrum_v1();

    electrum_v1(const electrum_v1& other);

    /// Construct from the "recovery seed" (mnemonic phrase or entropy).
    /// Validity should be checked after construction.
    electrum_v1(const std::string& sentence, language identifier=language::none);
    electrum_v1(const string_list& words, language identifier=language::none);
    electrum_v1(const data_chunk& entropy, language identifier=language::en);
    electrum_v1(const minimum_entropy& entropy, language identifier=language::en);
    electrum_v1(const maximum_entropy& entropy, language identifier=language::en);

    /// Derive the "wallet seed" from mnemonic entropy.
    /// The wallet seed is also the wallet "master private key".
    /// ec_private.point() is the wallet "master public key".
    ec_private to_seed(const context& context=btc_mainnet_p2kh) const;

    /// Derive the hd root private key from the wallet seed.
    /// The original seed cannot be obtained from the key.
    hd_private to_key(const context& context=btc_mainnet_p2kh) const;

protected:
    /// Constructors.
    electrum_v1(const data_chunk& entropy, const string_list& words,
        language identifier);

    /// Map entropy to entropy bit count (128 or 256 bits).
    static size_t entropy_bits(const data_slice& entropy);

    /// Map words to entropy bit count (128 or 256 bits).
    static size_t entropy_bits(const string_list& words);

    /// Map words to entropy size (16 or 32 bytes).
    static size_t entropy_size(const string_list& words);

    /// Map entropy size to word count (12 or 24 words).
    static size_t word_count(const data_slice& entropy);

    static data_chunk decoder(const string_list& words, language identifier);
    static string_list encoder(const data_chunk& entropy, language identifier);
    static ec_secret strecher(const data_chunk& entropy);

    electrum_v1 from_entropy(const data_chunk& entropy, language identifier) const;
    electrum_v1 from_words(const string_list& words, language identifier) const;

private:
    // All Electrum v1 dictionaries, from <dictionaries/electrum_v1.cpp>.
    static const dictionaries dictionaries_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
