// Exceptions.h
#pragma once

#include <stdexcept>
#include <string>

class MultisetException : public std::runtime_error {
public:
    explicit MultisetException(const std::string& message)
        : std::runtime_error("\nmultiset Error: " + message) {}
};

class InvalidOperationException : public MultisetException {
public:
    explicit InvalidOperationException(const std::string& message)
        : MultisetException("\ninvalid Operation - " + message) {}
};

class InvalidValueException : public MultisetException {
public:
    explicit InvalidValueException(const std::string& message)
        : MultisetException("\ninvalid Value - " + message) {}
};