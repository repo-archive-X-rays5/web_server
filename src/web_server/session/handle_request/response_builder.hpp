//
// Created by X-ray on 6/3/2021.
//
#pragma once
#include <boost/beast.hpp>
#include <utility>
#include "status_codes.hpp"

namespace web_server {
    namespace session {
        namespace handle_request {
            namespace http = boost::beast::http;

            class responsebuilder {
            public:
                responsebuilder(http_status status_code, std::string content_type) {
                    file = false;
                    http::response<http::string_body> res{static_cast<http::status>(status_code), 11};
                    string_response_ = std::move(res);
                    string_response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    string_response_.set(http::field::content_type, content_type);
                    file_response_.set(http::field::cache_control, "public, max-age=60, immutable");
                }

                responsebuilder(http_status status_code, std::string content_type, http::file_body::value_type& body) {
                    file = true;
                    http::response<http::file_body> res{std::piecewise_construct, std::make_tuple(std::move(body)), std::make_tuple(static_cast<http::status>(status_code), 11)};
                    file_response_ = std::move(res);
                    file_response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    file_response_.set(http::field::content_type, content_type);
                    file_response_.set(http::field::cache_control, "public, max-age=60, immutable");
                }

                void SetHeader(std::string key, std::string value) {
                    if (file) {
                        file_response_.set(key, value);
                    } else {
                        string_response_.set(key, value);
                    }
                }

                void SetBody(std::string body) {
                    if (!file) {
                        string_response_.content_length(body.size());
                        string_response_.body() = std::move(body);
                    }
                }

                http::response<http::string_body> GetStringResponse() {
                    string_response_.prepare_payload();
                    return string_response_;
                }

                http::response<http::file_body> GetFileResponse() {
                    file_response_.prepare_payload();
                    return std::move(file_response_);
                }
            private:
                bool file;
                http::response<http::string_body> string_response_;
                http::response<http::file_body> file_response_;
            };
        } // handle_request
    } // session
} // web_server