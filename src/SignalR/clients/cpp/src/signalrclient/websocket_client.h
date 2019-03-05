// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include "pplx/pplxtasks.h"

namespace signalr
{
    class websocket_client
    {
    public:
        virtual void connect(const std::string& url, const std::function<void(const std::exception_ptr&)>& completion_callback) = 0;

        virtual void send(const std::string& message, const std::function<void(const std::exception_ptr&)>& completion_callback) = 0;

        virtual void receive(const std::function<void(const std::exception_ptr&, const std::string&)>& completion_callback) = 0;

        virtual void close(const std::function<void(const std::exception_ptr&)>& completion_callback) = 0;

        virtual ~websocket_client() {};
    };
}
