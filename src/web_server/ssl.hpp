#pragma once
#include <fstream>
#include <boost/beast/ssl.hpp>
#include "../config/config.hpp"

namespace web_server {
    namespace ssl {
        inline void load_server_certificate(boost::beast::net::ssl::context& ctx)
        {
            auto config = config::Instance->GetConfig();
            ctx.set_options(boost::asio::ssl::context::default_workarounds |
                            boost::asio::ssl::context::sslv23 |
                            boost::asio::ssl::context::no_sslv2 |
                            boost::asio::ssl::context::no_sslv3 |
                            boost::asio::ssl::context::single_dh_use);
            ctx.set_verify_mode(0);

            if (!std::filesystem::exists(config.ssl_settings.certificate_file)) {
                std::cout << "Failed to load ssl certificate file\n";
                exit(EXIT_FAILURE);
            }
            if (!std::filesystem::exists(config.ssl_settings.private_key_file)) {
                std::cout << "Failed to load ssl private key file\n";
                exit(EXIT_FAILURE);
            }

            std::ifstream certreader(config.ssl_settings.certificate_file);

            if (!certreader.is_open()) {
                std::cout << "Failed to open ssl certificate file\n";
                exit(EXIT_FAILURE);
            }

            std::string buffer;
            std::string cert;
            while (std::getline(certreader, buffer))
                cert += "\n" + buffer;
            certreader.close();

            std::ifstream keyreader(config.ssl_settings.private_key_file);

            if (!keyreader.is_open()) {
                std::cout << "Failed to open ssl private key file\n";
                exit(EXIT_FAILURE);
            }

            std::string key;
            while (std::getline(keyreader, buffer))
                key += "\n" + buffer;
            keyreader.close();

            ctx.use_certificate_chain(boost::asio::buffer(cert.data(), cert.size()));
            ctx.use_private_key(boost::asio::buffer(key.data(), key.size()), boost::asio::ssl::context::file_format::pem);
        }
    } // ssl
} // web_server