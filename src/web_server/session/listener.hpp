//
// Created by X-ray on 5/30/2021.
//
#pragma once
#include <utility>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "../utility/fail.hpp"
#include "detect_session.hpp"

namespace web_server {
    namespace session {
        namespace beast = boost::beast;
        namespace net = beast::net;
        namespace ssl = net::ssl;
        using tcp = boost::asio::ip::tcp;

        // Accepts incoming connections and launches the sessions
        class listener : public std::enable_shared_from_this<listener>
        {
            net::io_context& ioc_;
            ssl::context& ctx_;
            tcp::acceptor acceptor_;
            std::shared_ptr<std::string const> doc_root_;

        public:
            listener(net::io_context& ioc,ssl::context& ctx,const tcp::endpoint& endpoint,std::shared_ptr<std::string const>  doc_root): ioc_(ioc), ctx_(ctx), acceptor_(net::make_strand(ioc)), doc_root_(std::move(doc_root))
            {
                beast::error_code ec;

                // Open the acceptor
                acceptor_.open(endpoint.protocol(), ec);
                if(ec)
                {
                    utility::fail(ec, "open");
                    return;
                }

                // Allow address reuse
                acceptor_.set_option(net::socket_base::reuse_address(true), ec);
                if(ec)
                {
                    utility::fail(ec, "set_option");
                    return;
                }

                // Bind to the server address
                acceptor_.bind(endpoint, ec);
                if(ec)
                {
                    utility::fail(ec, "bind");
                    exit(EXIT_FAILURE);
                    return;
                }

                // Start listening for connections
                acceptor_.listen(net::socket_base::max_listen_connections, ec);
                if(ec)
                {
                    utility::fail(ec, "listen");
                    return;
                }
            }

            // Start accepting incoming connections
            void run();

        private:
            void do_accept();

            void on_accept(beast::error_code ec, tcp::socket socket);
        };
    } // session
} // web_server
