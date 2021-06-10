//
// Created by X-ray on 6/7/2021.
//
#pragma once
#include "plain_session.hpp"

namespace web_server {
    namespace session {

        void plain_http_session::run() {
            this->do_read();
        }

        beast::tcp_stream &plain_http_session::stream() {
            return stream_;
        }

        beast::tcp_stream plain_http_session::release_stream() {
            return std::move(stream_);
        }

        void plain_http_session::do_eof() {
            // Send a TCP shutdown
            beast::error_code ec;
            stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

            // At this point the connection is closed gracefully
        }
    } // session
} // web_server
