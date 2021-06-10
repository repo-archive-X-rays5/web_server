#include "main.hpp"

namespace beast = boost::beast;
namespace net = beast::net;
namespace ssl = net::ssl;
using tcp = boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    config::Instance = std::make_unique<config::instance>(argc, argv);
    auto config = config::Instance->GetConfig();

    // The io_context is required for all I/O
    net::io_context ioc{config.max_threads};

    // setup ssl
    ssl::context ctx{ssl::context::sslv23};
    if (config.ssl_settings.accept_https)
        web_server::ssl::load_server_certificate(ctx);

    // Create and launch a listening port
    std::make_shared<web_server::session::listener>(ioc, ctx, tcp::endpoint{net::ip::make_address(std::to_string(INADDR_ANY)), config.port}, std::make_shared<const std::string>(config.doc_root))->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&](boost::system::error_code e, int)
    {
        std::cout << "Unhandled exception: " << e.message() << "\n";
        ioc.stop();
    });

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(config.max_threads - 1);
    for(auto i = config.max_threads - 1; i > 0; --i) {
        v.emplace_back([&ioc]
        {
            try {
                ioc.run();
            } catch(std::exception &e) {
                std::cerr << "std exception: " << e.what() << "\n";
            } catch(...) {
                std::cerr << "Unkown error\n";
            }
        });
    }
    std::cout << "Server started on port: " << config.port << "\n";
    try {
        ioc.run();
    } catch(std::exception &e) {
        std::cerr << "std exception: " << e.what() << "\n";
    } catch(...) {
        std::cerr << "Unkown error\n";
    }

    // (If we get here, it means we got a SIGINT or SIGTERM)

    // Block until all the threads exit
    for(auto& t : v)
        t.join();

    return EXIT_FAILURE;
}
