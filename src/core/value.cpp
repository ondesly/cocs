//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <algorithm>
#include <stdexcept>

#include "value.hpp"

cc::value cc::value::object() {
    return value(value_t::object);
}

cc::value cc::value::array() {
    return value(value_t::array);
}

cc::value::value() {
}

cc::value::value(const cc::value &other) : m_type(other.m_type) {
    switch (m_type) {
        case value_t::object:
            m_value = *other.m_value.object;
            break;
        case value_t::array:
            m_value = *other.m_value.array;
            break;
        case value_t::string:
            m_value = *other.m_value.string;
            break;
        case value_t::boolean:
            m_value = other.m_value.boolean;
            break;
        case value_t::integer_number:
            m_value = other.m_value.number_long_long;
            break;
        case value_t::unsigned_number:
            m_value = other.m_value.unsigned_long_long;
            break;
        case value_t::float_number:
            m_value = other.m_value.number_double;
            break;
        case value_t::vec:
            m_value = other.m_value.vec;
            break;
        default:
            break;
    }
}

cc::value::value(cc::value &&other) : m_type(std::move(other.m_type)), m_value(std::move(other.m_value)) {
    other.m_type = value_t::null;
    other.m_value = {};
}

cc::value &cc::value::operator=(cc::value other) {
    std::swap(m_type, other.m_type);
    std::swap(m_value, other.m_value);

    return *this;
}

cc::value::value(const char *v) : m_type(value_t::string), m_value(v) {
}

cc::value::value(const std::string &v) : m_type(value_t::string), m_value(v) {
}

cc::value::value(const bool v) : m_type(value_t::boolean), m_value(v) {
}

cc::value::value(const int v) : m_type(value_t::integer_number), m_value(v) {
}

cc::value::value(const long v) : m_type(value_t::integer_number), m_value(v) {
}

cc::value::value(const long long v) : m_type(value_t::integer_number), m_value(v) {
}

cc::value::value(const unsigned int v) : m_type(value_t::unsigned_number), m_value(v) {
}

cc::value::value(const unsigned long v) : m_type(value_t::unsigned_number), m_value(v) {
}

cc::value::value(const unsigned long long v) : m_type(value_t::unsigned_number), m_value(v) {
}

cc::value::value(const float v) : m_type(value_t::float_number), m_value(v) {
}

cc::value::value(const double v) : m_type(value_t::float_number), m_value(v) {
}

cc::value::value(const cc::float2 &v) : m_type(value_t::vec), m_value(v) {

}

cc::value::value(std::initializer_list<cc::value> init) {
    bool is_an_object = std::all_of(init.begin(), init.end(), [](const value &element) {
        return (element.is_array() && element.size() == 2 && element[0].is_string());
    });

    if (is_an_object) {
        m_type = value_t::object;
        m_value = value_t::object;

        for (const value &element : init) {
            auto &element_key = *(element.m_value.array->front().m_value.string);
            auto &element_value = element.m_value.array->back();

            m_value.object->emplace(element_key, element_value);
        }
    } else {
        m_type = value_t::array;
        m_value = value_t::array;

        m_value.array->insert(m_value.array->end(), init.begin(), init.end());
    }
}

cc::value::~value() {
    m_value.destroy(m_type);
}

cc::value &cc::value::operator[](const char *key) const {
    return m_value.object->operator[](key);
}

cc::value &cc::value::operator[](const char *key) {
    if (is_null()) {
        m_type = value_t::object;
        m_value = value_t::object;
    }

    return m_value.object->operator[](key);
}

cc::value &cc::value::operator[](const std::string &key) const {
    return m_value.object->operator[](key);
}

cc::value &cc::value::operator[](const std::string &key) {
    if (is_null()) {
        m_type = value_t::object;
        m_value = value_t::object;
    }

    return m_value.object->operator[](key);
}

cc::value &cc::value::operator[](const int index) const {
    return m_value.array->operator[](size_t(index));
}

cc::value &cc::value::operator[](const int index) {
    return m_value.array->operator[](size_t(index));
}

cc::value &cc::value::operator[](const size_t index) const {
    return m_value.array->operator[](index);
}

cc::value &cc::value::operator[](const size_t index) {
    return m_value.array->operator[](index);
}

const cc::value &cc::value::front() const {
    return m_value.array->front();
}

const cc::value &cc::value::back() const {
    return m_value.array->back();
}

void cc::value::push_back(const cc::value &v) {
    if (is_null()) {
        m_type = value_t::array;
        m_value = value_t::array;
    }

    m_value.array->push_back(v);
}

bool cc::value::has(const std::string &key) const {
    if (m_type == value_t::object) {
        return m_value.object->find(key) != m_value.object->end();
    } else {
        return false;
    }
}

bool cc::value::empty() const {
    switch (m_type) {
        case value_t::object:
            return m_value.object->empty();
        case value_t::array:
            return m_value.array->empty();
        case value_t::string:
            return m_value.string->empty();
        case value_t::null:
            return true;
        default:
            return false;
    }
}

cc::value_iterator cc::value::begin() const {
    value_iterator result(this);
    result.set_begin();
    return result;
}

cc::value_iterator cc::value::end() const {
    value_iterator result(this);
    result.set_end();
    return result;
}

cc::value::operator const std::string &() const {
    assert_type(value_t::string);
    return *m_value.string;
}

cc::value::operator bool() const {
    assert_type(value_t::boolean);
    return m_value.boolean;
}

cc::value::operator int() const {
    assert_type(value_t::integer_number);
    return static_cast<int>(m_value.number_long_long);
}

cc::value::operator long() const {
    assert_type(value_t::integer_number);
    return m_value.number_long_long;
}

cc::value::operator long long() const {
    assert_type(value_t::integer_number);
    return m_value.number_long_long;
}

cc::value::operator unsigned int() const {
    assert_type(value_t::unsigned_number);
    return static_cast<unsigned int>(m_value.unsigned_long_long);
}

cc::value::operator unsigned long() const {
    assert_type(value_t::unsigned_number);
    return m_value.unsigned_long_long;
}

cc::value::operator unsigned long long() const {
    assert_type(value_t::unsigned_number);
    return m_value.unsigned_long_long;
}

cc::value::operator float() const {
    assert_type(value_t::float_number);
    return static_cast<float>(m_value.number_double);
}

cc::value::operator double() const {
    assert_type(value_t::float_number);
    return m_value.number_double;
}

cc::value::operator cc::float2() const {
    assert_type(value_t::vec);
    return m_value.vec;
}

bool cc::value::is_null() const {
    return m_type == value_t::null;
}

bool cc::value::is_object() const {
    return m_type == value_t::object;
}

bool cc::value::is_array() const {
    return m_type == value_t::array;
}

bool cc::value::is_string() const {
    return m_type == value_t::string;
}

size_t cc::value::size() const {
    switch (m_type) {
        case value_t::null:
            return 0;
        case value_t::array:
            return m_value.array->size();
        case value_t::object:
            return m_value.object->size();
        default:
            return 1;
    }
}

cc::value::value(const cc::value::value_t type) : m_type(type), m_value(type) {
}

void cc::value::assert_type(const cc::value::value_t expected) const {
    if (m_type != expected) {
        throw std::invalid_argument(std::string("expected: ") + get_name(expected) + ", was: " + get_name(m_type));
    }
}

const char *cc::value::get_name(const cc::value::value_t t) const {
    switch (t) {
        case value_t::object:
            return "object";
        case value_t::array:
            return "array";
        case value_t::string:
            return "string";
        case value_t::boolean:
            return "boolean";
        case value_t::integer_number:
            return "integer";
        case value_t::unsigned_number:
            return "unsigned";
        case value_t::float_number:
            return "float";
        case value_t::vec:
            return "vec";
        case value_t::null:
            return "null";
    }
}

cc::value_iterator::value_iterator(cc::value_iterator::pointer object) : m_object(object) {
    switch (object->m_type) {
        case value::value_t::object:
            m_object_iterator = std::unordered_map<std::string, value_type>::iterator();
            break;
        case value::value_t::array:
            m_array_iterator = std::vector<value_type>::iterator();
            break;
        default:
            break;
    }
}

const cc::value &cc::value_iterator::operator*() const {
    switch (m_object->m_type) {
        case value::value_t::object:
            return m_object_iterator->second;
        case value::value_t::array:
            return *m_array_iterator;
        default:
            return *m_object;
    }
}

const cc::value_iterator cc::value_iterator::operator++(int) {
    auto result = *this;
    ++(*this);
    return result;
}

cc::value_iterator &cc::value_iterator::operator++() {
    switch (m_object->m_type) {
        case value::value_t::object:
            std::advance(m_object_iterator, 1);
            break;
        case value::value_t::array:
            std::advance(m_array_iterator, 1);
            break;
        default:
            break;
    }

    return *this;
}

bool cc::value_iterator::operator==(const cc::value_iterator &other) const {
    switch (m_object->m_type) {
        case value::value_t::object:
            return m_object_iterator == other.m_object_iterator;
        case value::value_t::array:
            return m_array_iterator == other.m_array_iterator;
        default:
            return false;
    }
}

bool cc::value_iterator::operator!=(const cc::value_iterator &other) const {
    return !(operator==(other));
}

void cc::value_iterator::set_begin() {
    switch (m_object->m_type) {
        case value::value_t::object:
            m_object_iterator = m_object->m_value.object->begin();
            break;
        case value::value_t::array:
            m_array_iterator = m_object->m_value.array->begin();
            break;
        default:
            break;
    }
}

void cc::value_iterator::set_end() {
    switch (m_object->m_type) {
        case value::value_t::object:
            m_object_iterator = m_object->m_value.object->end();
            break;
        case value::value_t::array:
            m_array_iterator = m_object->m_value.array->end();
            break;
        default:
            break;
    }
}
