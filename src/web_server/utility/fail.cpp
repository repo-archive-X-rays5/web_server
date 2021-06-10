//
// Created by X-ray on 6/8/2021.
//
#include "fail.hpp"

namespace web_server {
    namespace utility {
        void fail(boost::beast::error_code ec, char const* what)
        {
            if(ec == boost::beast::net::ssl::error::stream_truncated)
                return;

            std::cerr << what << ": " << ec.message() << "\n";
        }
    } // session
} // web_server