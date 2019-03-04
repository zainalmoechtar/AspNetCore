// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include "signalrclient/transport_type.h"
#include "logger.h"

namespace signalr
{
    class transport
    {
    public:
        virtual void connect(const std::string &url, const std::function<void(const std::exception_ptr&)>& completion_callback) = 0;

        virtual void send(const std::string &data, const std::function<void(const std::exception_ptr&)>& completion_callback) = 0;

        virtual void disconnect(const std::function<void(const std::exception_ptr&)>& completion_callback) = 0;

        virtual transport_type get_transport_type() const = 0;

        virtual ~transport();

    protected:
        transport(const logger& logger, const std::function<void(const std::string &)>& process_response_callback,
            std::function<void(const std::exception&)> error_callback);

        void process_response(const std::string &message);
        void error(const std::exception &e);

        logger m_logger;

    private:
        std::function<void(const std::string &)> m_process_response_callback;

        std::function<void(const std::exception&)> m_error_callback;
    };
}
