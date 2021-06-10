cmake_minimum_required(VERSION 3.19)
project(cdn_backend)

set(CMAKE_CXX_STANDARD 20)

if (${MSVC})
    set(COMPILER_FLAGS "/bigobj -D_WIN32_WINNT=0x0601")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}" )
endif ()

set(LIBSSL_DIR third-party/libssl)
set(LIBCRYPTO_DIR third-party/libcrypto)

include(FindLua)

find_package(Boost 1.66 COMPONENTS REQUIRED)
find_package(Lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)

find_library(LIBSSL NAMES libssl PATHS ${LIBSSL_DIR} NO_DEFAULT_PATH)
find_library(LIBCRYPTO NAMES libcrypto PATHS ${LIBCRYPTO_DIR} NO_DEFAULT_PATH)

add_executable(cdn_backend main.cpp web_server/utility/mime.hpp config/argshandler.hpp web_server/session/listener.hpp web_server/session/handle_request/handle_request.hpp web_server/ssl.hpp web_server/utility/fail.hpp web_server/session/websocket.hpp web_server/session/detect_session.hpp web_server/session/ssl_session.hpp web_server/session/plain_session.hpp web_server/session/http_session.hpp web_server/session/handle_request/response_builder.hpp web_server/session/handle_request/status_codes.hpp web_server/utility/path_cat.cpp web_server/utility/path_cat.hpp web_server/utility/mime.cpp web_server/session/detect_session.cpp web_server/session/listener.cpp web_server/session/plain_session.cpp web_server/session/ssl_session.cpp web_server/session/handle_request/handle_request.cpp main.hpp web_server/utility/fail.cpp config/lua.hpp config/config.hpp config/config.cpp)

target_include_directories(cdn_backend PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(cdn_backend PRIVATE ${LUA_INCLUDE_DIR})

target_link_libraries(cdn_backend PRIVATE ${Boost_LIBRARIES})
target_link_libraries(cdn_backend PRIVATE ${LUA_LIBRARIES})
target_link_libraries(cdn_backend PRIVATE sol2::sol2)
target_link_libraries(cdn_backend PRIVATE ${LIBSSL})
target_link_libraries(cdn_backend PRIVATE ${LIBCRYPTO})