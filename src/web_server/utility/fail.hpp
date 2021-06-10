//
// Created by X-ray on 6/3/2021.
//
#pragma once
#include <iostream>
#include <boost/beast/ssl.hpp>

namespace web_server {
    namespace utility {
        void fail(boost::beast::error_code ec, char const* what);
    } // utility
} // web_server
