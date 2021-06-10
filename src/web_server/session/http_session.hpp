//
// Created by X-ray on 6/3/2021.
//
#pragma once
#include <utility>
#include <iostream>
#include <boost/beast.hpp>
#include "websocket.hpp"
#include "handle_request/handle_request.hpp"
#include "../utility/fail.hpp"

namespace web_server {
    namespace session {
        namespace beast = boost::beast;
        namespace http = beast::http;

        template<class Derived>
        class http_session
        {
            // Access the derived class, this is part of
            // the Curiously Recurring Template Pattern idiom.
            Derived& derived()
            {
                return static_cast<Derived&>(*this);
            }

            // This queue is used for HTTP pipelining.
            class queue
            {
                enum
                {
                    // Maximum number of responses we will queue
                    limit = 8
                };

                // The type-erased, saved work item
                struct work
                {
                    virtual ~work() = default;
                    virtual void operator()() = 0;
                };

                http_session& self_;
                std::vector<std::unique_ptr<work>> items_;

            public:
                explicit queue(http_session& self) : self_(self)
                {
                    static_assert(limit > 0, "queue limit must be positive");
                    items_.reserve(limit);
                }

                // Returns `true` if we have reached the queue limit
                [[nodiscard]] bool is_full() const {
                    return items_.size() >= limit;
                }

                // Called when a message finishes sending
                // Returns `true` if the caller should initiate a read
                bool on_write() {
                    BOOST_ASSERT(! items_.empty());
                    auto const was_full = is_full();
                    items_.erase(items_.begin());
                    if(! items_.empty())
                        (*items_.front())();
                    return was_full;
                }

                // Called by the HTTP handler to send a response.
                template<bool isRequest, class Body, class Fields>
                void operator()(http::message<isRequest, Body, Fields>&& msg) {
                    // This holds a work item
                    struct work_impl : work
                    {
                        http_session& self_;
                        http::message<isRequest, Body, Fields> msg_;

                        work_impl(http_session& self, http::message<isRequest, Body, Fields>&& msg) : self_(self), msg_(std::move(msg))
                        {
                        }

                        void operator()()
                        {
                            http::async_write(self_.derived().stream(), msg_, beast::bind_front_handler(&http_session::on_write, self_.derived().shared_from_this(), msg_.need_eof()));
                        }
                    };

                    // Allocate and store the work
                    items_.push_back(boost::make_unique<work_impl>(self_, std::move(msg)));

                    // If there was no previous work, start this one
                    if(items_.size() == 1)
                        (*items_.front())();
                }
            };

            std::shared_ptr<std::string const> doc_root_;
            queue queue_;

            // The parser is stored in an optional container so we can
            // construct it from scratch it at the beginning of each new message.
            boost::optional<http::request_parser<http::string_body>> parser_;

        protected:
            beast::flat_buffer buffer_;

        public:
            // Construct the session
            http_session(beast::flat_buffer buffer, std::shared_ptr<std::string const>  doc_root) : doc_root_(std::move(doc_root)), queue_(*this), buffer_(std::move(buffer))
            {
            }

            void do_read()
            {
                // Construct a new parser for each message
                parser_.emplace();

                // Apply a reasonable limit to the allowed size
                // of the body in bytes to prevent abuse.
                parser_->body_limit(10000);

                // Set the timeout.
                beast::get_lowest_layer(derived().stream()).expires_after(std::chrono::seconds(30));

                // Read a request using the parser-oriented interface
                http::async_read(derived().stream(), buffer_, *parser_, beast::bind_front_handler(&http_session::on_read, derived().shared_from_this()));
            }

            void on_read(beast::error_code ec, std::size_t bytes_transferred) {
                boost::ignore_unused(bytes_transferred);

                // This means they closed the connection
                if(ec == http::error::end_of_stream)
                    return derived().do_eof();

                if(ec)
                    return utility::fail(ec, "read");

                // See if it is a WebSocket Upgrade
                if(beast::websocket::is_upgrade(parser_->get()))
                {
                    // websocket connections are disabled
                    std::cout << "Websocket connection refused\n";
                    return;

                    //beast::get_lowest_layer(derived().stream()).expires_never();
                    //return make_websocket_session(derived().release_stream(), parser_->release());
                }

                // Send the response
                handle_request::handle request_handler;
                request_handler.run(*doc_root_, parser_->release(), queue_);
                //handle_request::handle_request(*doc_root_, parser_->release(), queue_);

                // If we aren't at the queue limit, try to pipeline another request
                if(!queue_.is_full())
                    do_read();
            }

            void on_write(bool close, beast::error_code ec, std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);

                if(ec)
                    return utility::fail(ec, "write");

                if(close)
                {
                    // This means we should close the connection, usually because
                    // the response indicated the "Connection: close" semantic.
                    return derived().do_eof();
                }

                // Inform the queue that a write completed
                if(queue_.on_write())
                {
                    // Read another request
                    do_read();
                }
            }
        };
    } // session
} // web_server