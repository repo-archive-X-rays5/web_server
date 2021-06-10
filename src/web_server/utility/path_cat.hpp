//
// Created by X-ray on 6/4/2021.
//
#pragma once
#include <boost/beast.hpp>

namespace web_server {
    namespace utility {
        std::string path_cat(boost::beast::string_view base, std::string_view path);
    } // utility
} // web_server
