//
// Created by X-ray on 6/3/2021.
//
#pragma once
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include "http_session.hpp"

namespace web_server {
    namespace session {
        namespace beast = boost::beast;
        namespace net = beast::net;
        namespace ssl = net::ssl;

        class ssl_http_session : public http_session<ssl_http_session>, public std::enable_shared_from_this<ssl_http_session>
        {
            beast::ssl_stream<beast::tcp_stream> stream_;

        public:
            // Create the http_session
            ssl_http_session(beast::tcp_stream&& stream,ssl::context& ctx,beast::flat_buffer&& buffer,std::shared_ptr<std::string const> const& doc_root) : http_session<ssl_http_session>(std::move(buffer), doc_root), stream_(std::move(stream), ctx)
            {
            }

            // Start the session
            void run();

            // Called by the base class
            beast::ssl_stream<beast::tcp_stream>& stream();

            // Called by the base class
            beast::ssl_stream<beast::tcp_stream> release_stream();

            // Called by the base class
            void do_eof();

        private:
            void on_handshake(beast::error_code ec, std::size_t bytes_used);

            void on_shutdown(beast::error_code ec);
        };
    } // session
} // web_server
