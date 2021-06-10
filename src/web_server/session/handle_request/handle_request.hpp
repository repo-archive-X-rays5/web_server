//
// Created by X-ray on 5/30/2021.
//
#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <boost/beast.hpp>
#include "../../utility/mime.hpp"
#include "../../utility/path_cat.hpp"
#include "response_builder.hpp"

namespace web_server {
    namespace session {
        namespace handle_request {
            namespace beast = boost::beast;
            namespace http = beast::http;

            struct request {
                http::verb method;
                std::string url;
                std::map<std::string, std::string> parameters;
                http::header<true, http::basic_fields<std::allocator<char>>> headers;
            };

            std::string ErrorResponse(std::string title, std::string what, int error_code);

            std::map<std::string, std::string> ParseUrlParameters(std::string url);

            class handle_tools {
            public:
                boost::beast::http::message<false, boost::beast::http::basic_string_body<char>> HandleError(http_status status, std::string error);

                using error_handler_t = std::function<boost::beast::http::response<boost::beast::http::string_body>(std::string)>;
                void AddErrorHandler(http_status status, error_handler_t handler);

            private:
                std::map<http_status, error_handler_t> error_handlers;
            };

            class handle {
            public:
                handle() {
                    RegisterErrorHandlers();
                }

                template<bool isRequest, class Body, class Allocator = std::allocator<char>, class Send>
                void run(beast::string_view doc_root, http::message<isRequest, Body, Allocator>&& req, Send&& send) {
                    try {
                        request Req{
                                static_cast<http::verb>(req.method()),
                                std::string(req.target()),
                                ParseUrlParameters(std::string(req.target())),
                                req.base(),
                        };

                        // Make sure we can handle the method
                        if(Req.method != http::verb::get && Req.method != http::verb::head)
                            return send(tools_.HandleError(http_status::bad_request, "Unknown HTTP-method"));

                        // Request path must be absolute and not contain "..".
                        if(Req.url.empty() || req.target()[0] != '/' || Req.url.find("..") != std::string::npos)
                            return send(tools_.HandleError(http_status::bad_request, "Illegal request-target"));

                        // Build the path to the requested file
                        std::string path = utility::path_cat(doc_root, Req.url);
                        if(Req.url.ends_with('/'))
                            path.append("index.html");

                        // Attempt to open the file
                        beast::error_code ec;
                        if (path.find('?') != std::string::npos) {
                            path.erase(path.find('?'));
                        }
                        http::file_body::value_type body;
                        body.open(path.c_str(), beast::file_mode::scan, ec);

                        // Handle the case where the file doesn't exist
                        if(ec == beast::errc::no_such_file_or_directory)
                            return send(tools_.HandleError(http_status::not_found, "The requested url was not found: " + std::string(Req.headers["Host"]) +Req.url));

                        // Handle an unknown error
                        if(ec)
                            return send(tools_.HandleError(http_status::internal_server_error, ec.message()));

                        if (Req.method == http::verb::head) {
                            responsebuilder res(http_status::ok, std::string(utility::mime_type(path)));
                            return send(res.GetStringResponse());
                        } else if (Req.method == http::verb::get) {
                            responsebuilder res(http_status::ok, std::string(utility::mime_type(path)), body);
                            return send(res.GetFileResponse());
                        }
                        return send(tools_.HandleError(http_status::internal_server_error, "Request was not handled"));
                    } catch(std::exception &e) {
                        return send(tools_.HandleError(http_status::internal_server_error, e.what()));
                    }
                }
            private:
                handle_tools tools_;
            private:
                void RegisterErrorHandlers();
            };
        } // handle_request
    } // session
} // web_server
