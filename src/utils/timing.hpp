//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

namespace cc {

    class timing {
    public:

        static std::shared_ptr<timing> make(const std::string &name) {
            return std::make_shared<timing>(name);
        }

    public:

        explicit timing(const std::string &name) : m_name(name) {
            m_start = std::chrono::steady_clock::now();
        }

        ~timing() {
            std::chrono::duration<int, std::milli> duration(std::chrono::duration_cast<std::chrono::duration<int, std::milli >>(std::chrono::steady_clock::now() - m_start));
            std::cout << "timing: " << m_name << ": " << duration.count() << std::endl;
        }

    private:

        std::string m_name;
        std::chrono::steady_clock::time_point m_start;

    };

}