//
// Created by X-ray on 6/7/2021.
//
#pragma once
#include "listener.hpp"

namespace web_server {
    namespace session {

        void listener::run() {
            do_accept();
        }

        void listener::do_accept() {
            // The new connection gets its own strand
            acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&listener::on_accept, shared_from_this()));
        }

        void listener::on_accept(beast::error_code ec, tcp::socket socket) {
            if(ec)
            {
                utility::fail(ec, "accept");
            } else {
                // Create the detector http_session and run it
                std::make_shared<detect_session>(socket.remote_endpoint().address().to_string(), std::move(socket), ctx_, doc_root_)->run();
            }

            // Accept another connection
            do_accept();
        }
    } // session
} // web_server