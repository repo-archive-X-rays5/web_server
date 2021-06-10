//
// Created by X-ray on 6/9/2021.
//
#pragma once
#include <memory>
#include <utility>
#include <filesystem>
#include <mutex>
#include "lua.hpp"
#include "argshandler.hpp"

namespace config {
    class instance {
    public:
        struct ssl {
            bool accept_https = false;
            bool accept_http = true;
            std::string certificate_file;
            std::string private_key_file;
        };

        struct settings {
          uint16_t port;
          std::string doc_root;
          int max_threads;
          ssl ssl_settings;
        };
    public:
        instance(int argc, char *argv[]) : args_("--", true) {
            RegisterKnowArgs();
            args_.HandleArgs(argc, argv);

            cfg_.port = args_.GetArg("p").empty() ? 80 : std::stoi(args_.GetArg("p"));
            cfg_.doc_root = args_.GetArg("dir").empty() ? "" : args_.GetArg("dir");
            cfg_.max_threads = args_.GetArg("t").empty() ? 50 : std::stoi(args_.GetArg("t"));

            if (!args_.GetArg("c").empty()) {
                if (std::filesystem::exists(args_.GetArg("c"))) {
                    RegisterSetters();

                    try {
                        lua_.ExcecuteFile(args_.GetArg("c"));
                    } catch (lua::lua_exception &e) {
                        std::cout << "Lua error\n" << e.what() << "\n";
                    }
                }
            }
            if (cfg_.doc_root.empty()) {
                std::cout << "Document root was not specified\n";
                exit(EXIT_FAILURE);
            }
        }

        settings GetConfig() {
            std::lock_guard lock(mtx_);
            return cfg_;
        }

    private:
        std::mutex mtx_; // when different threads try to access the config it will automatically lock
        argshandler args_;
        settings cfg_;
        lua::lua_state lua_;
    private:
        void RegisterKnowArgs();
        void RegisterSetters();
    };
    // a global ptr is needed so it can be accessed from anywhere
    inline std::unique_ptr<instance> Instance;
} // config
