// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "stdafx.h"
#include "default_websocket_client.h"

namespace signalr
{
    namespace
    {
        static web::websockets::client::websocket_client_config create_client_config(const signalr_client_config& signalr_client_config) noexcept
        {
            auto websocket_client_config = signalr_client_config.get_websocket_client_config();
            websocket_client_config.headers() = signalr_client_config.get_http_headers();

            return websocket_client_config;
        }
    }

    default_websocket_client::default_websocket_client(const signalr_client_config& signalr_client_config) noexcept
        : m_underlying_client(create_client_config(signalr_client_config))
    { }

    void default_websocket_client::connect(const std::string& url, const std::function<void(const std::exception_ptr&)>& completion_callback)
    {
        m_underlying_client.connect(utility::conversions::to_string_t(url))
            .then([completion_callback](pplx::task<void> task)
            {
                try
                {
                    task.get();
                    completion_callback(nullptr);
                }
                catch (const std::exception& ex)
                {
                    completion_callback(std::make_exception_ptr(ex));
                }
            });
    }

    void default_websocket_client::send(const std::string &message, const std::function<void(const std::exception_ptr&)>& completion_callback)
    {
        web::websockets::client::websocket_outgoing_message msg;
        msg.set_utf8_message(message);
        m_underlying_client.send(msg)
            .then([completion_callback](pplx::task<void> task)
            {
                try
                {
                    task.get();
                    completion_callback(nullptr);
                }
                catch (const std::exception & ex)
                {
                    completion_callback(std::make_exception_ptr(ex));
                }
            });
    }

    void default_websocket_client::receive(const std::function<void(const std::exception_ptr&, const std::string&)>& completion_callback)
    {
        // the caller is responsible for observing exceptions
        m_underlying_client.receive()
            .then([completion_callback](pplx::task<web::websockets::client::websocket_incoming_message> msg_task)
            {
                try
                {
                    auto msg = msg_task.get();
                    msg.extract_string().then([completion_callback](std::string m)
                    {
                        completion_callback(nullptr, m);
                    });
                }
                catch (...)
                {
                    completion_callback(std::current_exception(), std::string());
                }
            });
    }

    pplx::task<void> default_websocket_client::close()
    {
        return m_underlying_client.close();
    }
}
