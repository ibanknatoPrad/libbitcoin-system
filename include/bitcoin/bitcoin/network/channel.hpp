/**
 * Copyright (c) 2011-2016 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see COPYING.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_CHANNEL_HPP
#define LIBBITCOIN_CHANNEL_HPP

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <bitcoin/bitcoin/config/authority.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/network/channel_proxy.hpp>
#include <bitcoin/bitcoin/utility/logger.hpp>
#include <bitcoin/bitcoin/utility/serializer.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

namespace libbitcoin {
namespace network {
    
class channel;
typedef std::shared_ptr<channel> channel_ptr;

class BC_API channel
{
public:
    channel(channel_proxy_ptr proxy);
    channel(threadpool& pool, socket_ptr socket, const timeout& timeouts);
    ~channel();

    /// This class is not copyable.
    channel(const channel&) = delete;
    void operator=(const channel&) = delete;

    void start();
    void stop(const std::error_code& ec);

    uint64_t nonce() const;
    void set_nonce(uint64_t nonce);
    config::authority address() const;
    hash_digest own_threshold();
    void set_own_threshold(const hash_digest& threshold);
    hash_digest peer_threshold();
    void set_peer_threshold(const hash_digest& threshold);

    void reset_poll();
    void reset_sync();
    void set_poll_handler(channel_proxy::poll_handler handler);
    void set_sync_handler(channel_proxy::sync_handler handler);

    void subscribe_version(
        channel_proxy::receive_version_handler handle_receive);
    void subscribe_verack(
        channel_proxy::receive_verack_handler handle_receive);
    void subscribe_address(
        channel_proxy::receive_address_handler handle_receive);
    void subscribe_get_address(
        channel_proxy::receive_get_address_handler handle_receive);
    void subscribe_inventory(
        channel_proxy::receive_inventory_handler handle_receive);
    void subscribe_get_data(
        channel_proxy::receive_get_data_handler handle_receive);
    void subscribe_get_blocks(
        channel_proxy::receive_get_blocks_handler handle_receive);
    void subscribe_transaction(
        channel_proxy::receive_transaction_handler handle_receive);
    void subscribe_block(
        channel_proxy::receive_block_handler handle_receive);
    void subscribe_ping(
        channel_proxy::receive_ping_handler handle_receive);
    void subscribe_pong(
        channel_proxy::receive_pong_handler handle_receive);
    void subscribe_raw(
        channel_proxy::receive_raw_handler handle_receive);
    void subscribe_stop(
        channel_proxy::stop_handler handle_stop);

    template <typename Message>
    void send(const Message& packet, channel_proxy::send_handler handle_send)
    {
        proxy_->send(packet, handle_send);
    }

    void send_raw(const header_type& packet_header,
        const data_chunk& payload, channel_proxy::send_handler handle_send);

private:
    channel_proxy_ptr proxy_;
    std::atomic<uint64_t> nonce_;
    hash_digest own_threshold_;
    hash_digest peer_threshold_;
    std::mutex own_threshold_mutex_;
    std::mutex peer_threshold_mutex_;
};

} // namespace network
} // namespace libbitcoin

#endif
