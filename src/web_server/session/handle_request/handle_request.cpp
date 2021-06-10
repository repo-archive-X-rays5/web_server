//
// Created by X-ray on 6/8/2021.
//
#pragma once
#include "handle_request.hpp"

namespace web_server {
    namespace session {
        namespace handle_request {

            std::string ErrorResponse(std::string title, std::string what, int error_code) {
                std::string body = "<!doctype html>\n<html>\n<head>\n<title>"+std::to_string(error_code)+" "+title+"</title>\n</head>\n<body>\n<h1>"+std::to_string(error_code)+" "+title+"</h1>\n<hr>\n<p>"+what+"</p>\n<hr>\n<p><i>Server: Boost.Beast/"+std::to_string(BOOST_BEAST_VERSION)+" Error: "+std::to_string(error_code)+"</i></p>\n</body>\n</html>\n";
                return body;
            }

            std::map<std::string, std::string> ParseUrlParameters(std::string url) {
               if (url.find('?') != std::string::npos) {
                   std::map<std::string, std::string> parameters;
                   std::string parameterstring = url.substr(url.find('?') + 1, url.size());

                   while (parameterstring.find('=') != std::string::npos) {
                       int ispos = parameterstring.find('=');
                       int nextpos = parameterstring.find('&');
                       if (nextpos == std::string::npos)
                           nextpos = parameterstring.size();
                       std::string kv = parameterstring.substr(0, nextpos);
                       parameters.insert({kv.substr(0, ispos), kv.substr(ispos + 1, kv.size() - 1)});
                       if (nextpos != std::string::npos) {
                           parameterstring.erase(0, nextpos + 1);
                       } else {
                           return parameters;
                       }
                   }
                   return parameters;
               } else {
                   return std::map<std::string, std::string>{};
               }
            }

            boost::beast::http::message<false, boost::beast::http::basic_string_body<char>>
            handle_tools::HandleError(http_status status, std::string error) {
                auto handler = error_handlers.find(status);

                if (handler != error_handlers.end()) {
                    return std::invoke(handler->second, error);
                } else {
                    responsebuilder res(http_status::internal_server_error, "text/html");
                    res.SetBody(ErrorResponse("Internal Server Error", "Unable to handle error with code: " + std::to_string(static_cast<int>(status)), static_cast<int>(http_status::internal_server_error)));
                    std::cout << "Unknown error occurred: " << static_cast<int>(status) << "\n";
                    return res.GetStringResponse();
                }
            }

            void handle_tools::AddErrorHandler(http_status status, handle_tools::error_handler_t handler) {
                error_handlers[status] = std::move(handler);
            }

            void handle::RegisterErrorHandlers() {
                tools_.AddErrorHandler(http_status::bad_request, [=](std::string error)
                {
                    responsebuilder res(http_status::bad_request, "text/html");
                    res.SetBody(ErrorResponse("Bad Request", std::move(error), static_cast<int>(http_status::bad_request)));
                    return res.GetStringResponse();
                });
                tools_.AddErrorHandler(http_status::not_found, [=](std::string error)
                {
                    responsebuilder res(http_status::not_found, "text/html");
                    res.SetBody(ErrorResponse("Not Found", std::move(error), static_cast<int>(http_status::not_found)));
                    return res.GetStringResponse();
                });
                tools_.AddErrorHandler(http_status::internal_server_error, [=](std::string error)
                {
                    responsebuilder res(http_status::internal_server_error, "text/html");
                    res.SetBody(ErrorResponse("Internal Server Error", std::move(error), static_cast<int>(http_status::internal_server_error)));
                    return res.GetStringResponse();
                });
                tools_.AddErrorHandler(http_status::service_unavailable, [=](std::string error)
                {
                    responsebuilder res(http_status::service_unavailable, "text/html");
                    res.SetBody(ErrorResponse("Service unavailable", std::move(error), static_cast<int>(http_status::service_unavailable)));
                    return res.GetStringResponse();
                });
                tools_.AddErrorHandler(http_status::Im_a_teapot, [=](std::string error)
                {
                    responsebuilder res(http_status::Im_a_teapot, "text/html");
                    res.SetBody(ErrorResponse("I'm a teapot", "I'm refusing to brew coffee because I'm, permanently, a teapot.", static_cast<int>(http_status::Im_a_teapot)));
                    return res.GetStringResponse();
                });
            }
        } // handle_request
    } // session
} // web_server