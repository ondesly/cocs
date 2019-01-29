//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iterator>

#include "math/float2.hpp"

namespace cc {

    class value_iterator;

    class value {
        friend class value_iterator;

    public:

        static value object();

        static value array();

    public:

        value();

        value(const value &other);

        value(value &&other);

        value &operator=(value other);

        value(const char *v);

        value(const std::string &v);

        value(const bool v);

        value(const int v);

        value(const long v);

        value(const long long v);

        value(const unsigned int v);

        value(const unsigned long v);

        value(const unsigned long long v);

        value(const float v);

        value(const double v);

        value(const cc::float2 &v);

        value(std::initializer_list<value> init);

        value &operator[](const char *key) const;

        value &operator[](const char *key);

        value &operator[](const std::string &key) const;

        value &operator[](const std::string &key);

        value &operator[](int index) const;

        value &operator[](int index);

        value &operator[](size_t index) const;

        value &operator[](size_t index);

        const value &front() const;

        const value &back() const;

        ~value();

        void push_back(const value &v);

        bool has(const std::string &key) const;

        bool empty() const;

        value_iterator begin() const;

        value_iterator end() const;

        operator const std::string &() const;

        operator bool() const;

        operator int() const;

        operator long() const;

        operator long long() const;

        operator unsigned int() const;

        operator unsigned long() const;

        operator unsigned long long() const;

        operator float() const;

        operator double() const;

        operator cc::float2() const;

        bool is_null() const;

        bool is_object() const;

        bool is_array() const;

        bool is_string() const;

        size_t size() const;

    private:

        enum class value_t : std::uint8_t {
            object,
            array,
            string,
            boolean,
            integer_number,
            unsigned_number,
            float_number,
            vec,
            null
        };

        union value_u {

            std::unordered_map<std::string, value> *object;

            std::vector<value> *array;

            std::string *string;

            bool boolean;

            long long number_long_long;

            unsigned long long unsigned_long_long;

            double number_double;

            cc::float2 vec;

            //

            value_u() = default;

            value_u(const std::unordered_map<std::string, value> &v) {
                object = new std::unordered_map<std::string, value>(v);
            }

            value_u(const std::vector<value> &v) {
                array = new std::vector<value>(v);
            }

            value_u(const char *v) {
                string = new std::string(v);
            }

            value_u(const std::string &v) {
                string = new std::string(v);
            }

            value_u(const bool v) : boolean(v) {
            }

            value_u(const int v) : number_long_long(v) {
            }

            value_u(const long v) : number_long_long(v) {
            }

            value_u(const long long v) : number_long_long(v) {
            }

            value_u(const unsigned int v) : unsigned_long_long(v) {
            }

            value_u(const unsigned long v) : unsigned_long_long(v) {
            }

            value_u(const unsigned long long v) : unsigned_long_long(v) {
            }

            value_u(const float v) : number_double(v) {
            }

            value_u(const double v) : number_double(v) {
            }

            value_u(const cc::float2 &v) : vec(v) {
            }

            value_u(const value_t t) {
                switch (t) {
                    case value_t::object:
                        object = new std::unordered_map<std::string, value>{};
                        break;
                    case value_t::array:
                        array = new std::vector<value>{};
                        break;
                    case value_t::string:
                        string = new std::string{};
                        break;
                    case value_t::boolean:
                        boolean = false;
                        break;
                    case value_t::integer_number:
                        number_long_long = 0;
                        break;
                    case value_t::unsigned_number:
                        unsigned_long_long = 0;
                        break;
                    case value_t::float_number:
                        number_double = 0;
                        break;
                    case value_t::vec:
                        vec = {0.f, 0.f};
                        break;
                    case value_t::null:
                        break;
                }
            }

            void destroy(const value_t t) {
                switch (t) {
                    case value_t::object:
                        delete object;
                        break;
                    case value_t::array:
                        delete array;
                        break;
                    case value_t::string:
                        delete string;
                        break;
                    default:
                        break;
                }
            }

        };

    private:

        value_t m_type = value_t::null;

        value_u m_value = {};

    private:

        value(value_t type);

        void assert_type(value_t expected) const;

        const char *get_name(value_t t) const;

    };

}

namespace cc {

    class value_iterator {
        friend class value;

    public:

        using iterator_category = std::bidirectional_iterator_tag;

        using value_type = value;

        using difference_type = std::ptrdiff_t;

        using pointer = const value *;

        using reference = const value &;

    public:

        explicit value_iterator(pointer object);

        reference operator*() const;

        value_iterator const operator++(int);

        value_iterator &operator++();

        bool operator==(const value_iterator &other) const;

        bool operator!=(const value_iterator &other) const;

    private:

        pointer m_object = nullptr;

        std::unordered_map<std::string, value_type>::iterator m_object_iterator;
        std::vector<value_type>::iterator m_array_iterator;

    private:

        void set_begin();

        void set_end();

    };

}
