//
// Created by X-ray on 6/7/2021.
//
#pragma once
#include "ssl_session.hpp"

namespace web_server {
    namespace session {

        void ssl_http_session::run() {
            // Set the timeout.
            beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

            // Perform the SSL handshake
            // Note, this is the buffered version of the handshake.
            stream_.async_handshake(ssl::stream_base::server, buffer_.data(), beast::bind_front_handler(&ssl_http_session::on_handshake, shared_from_this()));
        }

        beast::ssl_stream<beast::tcp_stream> &ssl_http_session::stream() {
            return stream_;
        }

        beast::ssl_stream<beast::tcp_stream> ssl_http_session::release_stream() {
            return std::move(stream_);
        }

        void ssl_http_session::do_eof() {
            // Set the timeout.
            beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

            // Perform the SSL shutdown
            stream_.async_shutdown(beast::bind_front_handler(&ssl_http_session::on_shutdown, shared_from_this()));
        }

        void ssl_http_session::on_handshake(beast::error_code ec, std::size_t bytes_used) {
            if(ec)
                return utility::fail(ec, "handshake");

            // Consume the portion of the buffer used by the handshake
            buffer_.consume(bytes_used);

            do_read();
        }

        void ssl_http_session::on_shutdown(beast::error_code ec) {
            if(ec)
                return utility::fail(ec, "shutdown");

            // At this point the connection is closed gracefully
        }
    } // session
} // web_server
