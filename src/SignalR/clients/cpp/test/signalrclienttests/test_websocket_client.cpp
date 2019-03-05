// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "stdafx.h"
#include "test_websocket_client.h"

test_websocket_client::test_websocket_client()
    : m_connect_function([](const std::string&, const std::function<void(const std::exception_ptr&)> completion){ completion(nullptr); }),
    m_send_function ([](const std::string msg, const std::function<void(const std::exception_ptr&)> completion){ completion(nullptr); }),
    m_receive_function([](const std::function<void(const std::exception_ptr&, const std::string&)> completion){ completion(nullptr, ""); }),
    m_close_function([](const std::function<void(const std::exception_ptr&)> completion){ completion(nullptr); })

{ }

void test_websocket_client::connect(const std::string& url, const std::function<void(const std::exception_ptr&)>& completion_callback)
{
    m_connect_function(url, completion_callback);
}

void test_websocket_client::send(const std::string &msg, const std::function<void(const std::exception_ptr&)>& completion_callback)
{
    m_send_function(msg, completion_callback);
}

void test_websocket_client::receive(const std::function<void(const std::exception_ptr&, const std::string&)>& completion_callback)
{
    m_receive_function(completion_callback);
}

void test_websocket_client::close(const std::function<void(const std::exception_ptr&)>& completion_callback)
{
    return m_close_function(completion_callback);
}

void test_websocket_client::set_connect_function(const std::function<void(const std::string& url, const std::function<void(const std::exception_ptr&)>& completion_callback)>& connect_function)
{
    m_connect_function = connect_function;
}

void test_websocket_client::set_send_function(const std::function<void(const std::string& msg, const std::function<void(const std::exception_ptr&)>& completion_callback)>& send_function)
{
    m_send_function = send_function;
}

void test_websocket_client::set_receive_function(const std::function<void(const std::function<void(const std::exception_ptr&, const std::string&)>& completion_callback)>& receive_function)
{
    m_receive_function = receive_function;
}

void test_websocket_client::set_close_function(const std::function<void(const std::function<void(const std::exception_ptr&)>& completion_callback)>& close_function)
{
    m_close_function = close_function;
}
