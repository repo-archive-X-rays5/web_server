//
// Created by X-ray on 6/3/2021.
//
namespace web_server {
    namespace session {
        namespace handle_request {
            enum class http_status : unsigned
            {
                /** An unknown status-code.

                    This value indicates that the value for the status code
                    is not in the list of commonly recognized status codes.
                    Callers interested in the exactly value should use the
                    interface which provides the raw integer.
                */
                unknown = 0,

                continue_                           = 100,

                /** Switching Protocols

                    This status indicates that a request to switch to a new
                    protocol was accepted and applied by the server. A successful
                    response to a WebSocket Upgrade HTTP request will have this
                    code.
                */
                switching_protocols                 = 101,

                processing                          = 102,

                ok                                  = 200,
                created                             = 201,
                accepted                            = 202,
                non_authoritative_information       = 203,
                no_content                          = 204,
                reset_content                       = 205,
                partial_content                     = 206,
                multi_status                        = 207,
                already_reported                    = 208,
                im_used                             = 226,

                multiple_choices                    = 300,
                moved_permanently                   = 301,
                found                               = 302,
                see_other                           = 303,
                not_modified                        = 304,
                use_proxy                           = 305,
                temporary_redirect                  = 307,
                permanent_redirect                  = 308,

                bad_request                         = 400,
                unauthorized                        = 401,
                payment_required                    = 402,
                forbidden                           = 403,
                not_found                           = 404,
                method_not_allowed                  = 405,
                not_acceptable                      = 406,
                proxy_authentication_required       = 407,
                request_timeout                     = 408,
                conflict                            = 409,
                gone                                = 410,
                length_required                     = 411,
                precondition_failed                 = 412,
                payload_too_large                   = 413,
                uri_too_long                        = 414,
                unsupported_media_type              = 415,
                range_not_satisfiable               = 416,
                expectation_failed                  = 417,
                Im_a_teapot                         = 418,
                misdirected_request                 = 421,
                unprocessable_entity                = 422,
                locked                              = 423,
                failed_dependency                   = 424,
                upgrade_required                    = 426,
                precondition_required               = 428,
                too_many_requests                   = 429,
                request_header_fields_too_large     = 431,
                connection_closed_without_response  = 444,
                unavailable_for_legal_reasons       = 451,
                client_closed_request               = 499,

                internal_server_error               = 500,
                not_implemented                     = 501,
                bad_gateway                         = 502,
                service_unavailable                 = 503,
                gateway_timeout                     = 504,
                http_version_not_supported          = 505,
                variant_also_negotiates             = 506,
                insufficient_storage                = 507,
                loop_detected                       = 508,
                not_extended                        = 510,
                network_authentication_required     = 511,
                network_connect_timeout_error       = 599
            };
        } // handle_request
    } // session
} // web_server

