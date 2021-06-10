//
// Created by X-ray on 6/9/2021.
//
#pragma once
#include <sol/sol.hpp>
#include <utility>

namespace config {
    namespace lua {
        class lua_exception {
        public:
            lua_exception(std::string what) : what_(std::move(what))
            {
            }

            std::string what() {
                return what_;
            }
        private:
            std::string what_;
        };

        class lua_state {
        public:
            lua_state() {
                lua_state_.open_libraries(sol::lib::base);
                lua_state_.open_libraries(sol::lib::os);
            }

            template<typename... Args>
            void RegisterFunction(std::string name, Args&&... args) {
                lua_state_.set_function(name, std::forward<Args>(args)...);
            }

            void ExcecuteFile(std::string path) {
                sol::load_result script = lua_state_.load_file(path);

                if (!script.valid()) {
                    sol::error e = script;
                    throw(lua_exception(e.what()));
                } else {
                    ExcecuteLua(script);
                }
            }
        private:
            sol::state lua_state_;
        private:
            void ExcecuteLua(sol::load_result& lua_file) {
                auto result = lua_file.call<>();
                if (!result.valid()) {
                    sol::error e = result;
                    throw(lua_exception(e.what()));
                }
            }
        };
    } // lua
} // config
