//
// Created by X-ray on 5/30/2021.
//
#pragma once
#include <boost/beast.hpp>

namespace web_server {
    namespace utility {
        boost::beast::string_view mime_type(boost::beast::string_view path);
    } // utility
} // web_server
