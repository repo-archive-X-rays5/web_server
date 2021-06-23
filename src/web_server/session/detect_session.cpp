//
// Created by X-ray on 6/7/2021.
//
#pragma once
#include "detect_session.hpp"

namespace web_server {
    namespace session {
        void detect_session::run() {
            // Set the timeout.
            stream_.expires_after(std::chrono::seconds(30));

            beast::async_detect_ssl(stream_,buffer_,beast::bind_front_handler(&detect_session::on_detect,this->shared_from_this()));
        }

        void detect_session::on_detect(beast::error_code ec, boost::tribool result) {
            if(ec)
                return utility::fail(ec, "detect");

            auto config = config::Instance->GetConfig();
            if(config.ssl_settings.accept_https && result)
            {
                std::cout << "Incoming ssl request from: " << ip_ << "\n";
                // Launch SSL session
                std::make_shared<ssl_http_session>(std::move(stream_), ctx_, std::move(buffer_), doc_root_)->run();
                return;
            }

            if (config.ssl_settings.accept_http || ip_ == "127.0.0.1" || ip_ == "::1") {
                std::cout << "Request incoming: " << ip_ << "\n";
                // Launch plain session
                std::make_shared<plain_http_session>(std::move(stream_), std::move(buffer_), doc_root_)->run();
                return;
            }

            beast::error_code error;
            stream_.socket().shutdown(tcp::socket::shutdown_send, error);
            if (error)
                return utility::fail(error, "shutdown");
        }
    } // session
} // web_server
