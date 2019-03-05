// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include "cpprest/ws_client.h"
#include "signalrclient/signalr_client_config.h"
#include "websocket_client.h"

namespace signalr
{
    class default_websocket_client : public websocket_client
    {
    public:
        explicit default_websocket_client(const signalr_client_config& signalr_client_config = signalr_client_config{}) noexcept;

        void connect(const std::string& url, const std::function<void(const std::exception_ptr&)>& completion_callback) override;

        void send(const std::string& message, const std::function<void(const std::exception_ptr&)>& completion_callback) override;

        void receive(const std::function<void(const std::exception_ptr&, const std::string&)>& completion_callback) override;

        void close(const std::function<void(const std::exception_ptr&)>& completion_callback) override;

    private:
        web::websockets::client::websocket_client m_underlying_client;
    };
}
