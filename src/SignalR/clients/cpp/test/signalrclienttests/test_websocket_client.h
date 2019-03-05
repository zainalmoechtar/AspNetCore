// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include <functional>
#include "websocket_client.h"

using namespace signalr;

class test_websocket_client : public websocket_client
{
public:
    test_websocket_client();

    void connect(const std::string& url, const std::function<void(const std::exception_ptr&)>& completion_callback) override;

    void send(const std::string& msg, const std::function<void(const std::exception_ptr&)>& completion_callback) override;

    void receive(const std::function<void(const std::exception_ptr&, const std::string&)>& completion_callback) override;

    void close(const std::function<void(const std::exception_ptr&)>& completion_callback) override;

    void set_connect_function(const std::function<void(const std::string& url, const std::function<void(const std::exception_ptr&)>& completion_callback)>& connect_function);

    void set_send_function(const std::function<void(const std::string& msg, const std::function<void(const std::exception_ptr&)>& completion_callback)>& send_function);

    void set_receive_function(const std::function<void(const std::function<void(const std::exception_ptr&, const std::string&)>& completion_callback)>& receive_function);

    void set_close_function(const std::function<void(const std::function<void(const std::exception_ptr&)>& completion_callback)>& close_function);

private:
    std::function<void(const std::string& url, const std::function<void(const std::exception_ptr&)>&)> m_connect_function;

    std::function<void(const std::string& msg, const std::function<void(const std::exception_ptr&)>&)> m_send_function;

    std::function<void(const std::function<void(const std::exception_ptr&, const std::string&)>&)> m_receive_function;

    std::function<void(const std::function<void(const std::exception_ptr&)>&)> m_close_function;
};
