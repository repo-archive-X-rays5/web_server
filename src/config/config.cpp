//
// Created by X-ray on 6/9/2021.
//
#include "config.hpp"

namespace config {

    void instance::RegisterKnowArgs() {
        args_.AddKnownArg("p", "Specify the port number the server should run on");
        args_.AddKnownArg("dir", "Specify the document root");
        args_.AddKnownArg("t", "Specify the amount of threads the server will use");
        args_.AddKnownArg("c", "Supply a path to a config file");
    }

    void instance::RegisterSetters() {
        lua_.RegisterFunction("SetPort", [&](uint16_t port){cfg_.port = port;});
        lua_.RegisterFunction("SetDocumentRoot", [&](std::string path){cfg_.doc_root = std::move(path);});
        lua_.RegisterFunction("SetMaxThreads", [&](int max){cfg_.max_threads = max;});
        lua_.RegisterFunction("Https", [&](bool enabled){cfg_.ssl_settings.accept_https = enabled;});
        lua_.RegisterFunction("Http", [&](bool enabled){cfg_.ssl_settings.accept_http = enabled;});
        lua_.RegisterFunction("CertificateFile", [&](std::string path){cfg_.ssl_settings.certificate_file = std::move(path);});
        lua_.RegisterFunction("PrivateKeyFile", [&](std::string path){cfg_.ssl_settings.private_key_file = std::move(path);});
    }
} // config