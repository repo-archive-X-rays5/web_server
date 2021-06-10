//
// Created by X-ray on 6/3/2021.
//
#pragma once
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "http_session.hpp"

namespace web_server {
    namespace session {
        namespace beast = boost::beast;
        using tcp = boost::asio::ip::tcp;

        // Handles a plain HTTP connection
        class plain_http_session : public http_session<plain_http_session>, public std::enable_shared_from_this<plain_http_session>
        {
            beast::tcp_stream stream_;

        public:
            // Create the session
            plain_http_session(beast::tcp_stream&& stream, beast::flat_buffer&& buffer, std::shared_ptr<std::string const> const& doc_root) : http_session<plain_http_session>( std::move(buffer), doc_root), stream_(std::move(stream))
            {
            }

            // Start the session
            void run();

            // Called by the base class
            beast::tcp_stream& stream();

            // Called by the base class
            beast::tcp_stream release_stream();

            // Called by the base class
            void do_eof();
        };
    } // session
} // web_server
