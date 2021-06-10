//
// Created by X-ray on 6/3/2021.
//
#pragma once
#include <boost/beast.hpp>
#include "../utility/fail.hpp"

namespace web_server {
    namespace session {
        namespace beast = boost::beast;
        namespace http = beast::http;
        namespace websocket = beast::websocket;

        template<class Derived>
        class websocket_session
        {
            // Access the derived class, this is part of
            // the Curiously Recurring Template Pattern idiom.
            Derived& derived()
            {
                return static_cast<Derived&>(*this);
            }

            beast::flat_buffer buffer_;

            // Start the asynchronous operation
            template<class Body, class Allocator>
            void
            do_accept(http::request<Body, http::basic_fields<Allocator>> req)
            {
                // Set suggested timeout settings for the websocket
                derived().ws().set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

                // Set a decorator to change the Server of the handshake
                derived().ws().set_option(websocket::stream_base::decorator([](websocket::response_type& res)
                {
                    res.set(http::field::server,std::string(BOOST_BEAST_VERSION_STRING));
                }));

                // Accept the websocket handshake
                derived().ws().async_accept(req, beast::bind_front_handler(&websocket_session::on_accept,derived().shared_from_this()));
            }

            void on_accept(beast::error_code ec)
            {
                if(ec)
                    return utility::fail(ec, "accept");

                // Read a message
                do_read();
            }

            void do_read()
            {
                // Read a message into our buffer
                derived().ws().async_read(buffer_, beast::bind_front_handler(&websocket_session::on_read, derived().shared_from_this()));
            }

            void on_read(beast::error_code ec, std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);

                // This indicates that the websocket_session was closed
                if(ec == websocket::error::closed)
                    return;

                if(ec)
                    utility::fail(ec, "read");

                // Echo the message
                derived().ws().text(derived().ws().got_text());
                derived().ws().async_write(buffer_.data(), beast::bind_front_handler(&websocket_session::on_write, derived().shared_from_this()));
            }

            void on_write(beast::error_code ec, std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);

                if(ec)
                    return utility::fail(ec, "write");

                // Clear the buffer
                buffer_.consume(buffer_.size());

                // Do another read
                do_read();
            }

        public:
            // Start the asynchronous operation
            template<class Body, class Allocator>
            void run(http::request<Body, http::basic_fields<Allocator>> req)
            {
                // Accept the WebSocket upgrade request
                do_accept(std::move(req));
            }
        };

//------------------------------------------------------------------------------

// Handles a plain WebSocket connection
        class plain_websocket_session : public websocket_session<plain_websocket_session>, public std::enable_shared_from_this<plain_websocket_session>
        {
            websocket::stream<beast::tcp_stream> ws_;

        public:
            // Create the session
            explicit plain_websocket_session(beast::tcp_stream&& stream) : ws_(std::move(stream))
            {
            }

            // Called by the base class
            websocket::stream<beast::tcp_stream>& ws()
            {
                return ws_;
            }
        };

//------------------------------------------------------------------------------

// Handles an SSL WebSocket connection
        class ssl_websocket_session : public websocket_session<ssl_websocket_session>, public std::enable_shared_from_this<ssl_websocket_session>
        {
            websocket::stream<
                    beast::ssl_stream<beast::tcp_stream>> ws_;

        public:
            // Create the ssl_websocket_session
            explicit ssl_websocket_session(beast::ssl_stream<beast::tcp_stream>&& stream) : ws_(std::move(stream))
            {
            }

            // Called by the base class
            websocket::stream<beast::ssl_stream<beast::tcp_stream>>& ws()
            {
                return ws_;
            }
        };

//------------------------------------------------------------------------------

        template<class Body, class Allocator>
        void make_websocket_session(beast::tcp_stream stream, http::request<Body, http::basic_fields<Allocator>> req)
        {
            std::make_shared<plain_websocket_session>(std::move(stream))->run(std::move(req));
        }

        template<class Body, class Allocator>
        void make_websocket_session(beast::ssl_stream<beast::tcp_stream> stream, http::request<Body, http::basic_fields<Allocator>> req)
        {
            std::make_shared<ssl_websocket_session>(std::move(stream))->run(std::move(req));
        }
    } // session
} // web_server
