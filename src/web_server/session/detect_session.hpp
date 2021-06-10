//
// Created by X-ray on 6/3/2021.
//
#pragma once
#include <utility>
#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "ssl_session.hpp"
#include "plain_session.hpp"
#include "../utility/fail.hpp"
#include "../../config/config.hpp"

namespace web_server {
    namespace session {
        namespace beast = boost::beast;
        namespace net = beast::net;
        namespace ssl = net::ssl;
        using tcp = boost::asio::ip::tcp;

        class detect_session : public std::enable_shared_from_this<detect_session>
        {
            std::string ip_;
            beast::tcp_stream stream_;
            ssl::context& ctx_;
            std::shared_ptr<std::string const> doc_root_;
            beast::flat_buffer buffer_;

        public:
            explicit detect_session(std::string ip, tcp::socket&& socket, ssl::context& ctx, std::shared_ptr<std::string const>  doc_root) : ip_(std::move(ip)), stream_(std::move(socket)), ctx_(ctx), doc_root_(std::move(doc_root))
            {
            }

            // Launch the detector
            void run();

            void on_detect(beast::error_code ec, boost::tribool result);
        };
    } // session
} // web_server
