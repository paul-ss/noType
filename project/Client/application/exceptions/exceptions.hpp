#pragma once

#include <exception>
#include <string>

class ClientException : public std::exception {
    public:
        explicit ClientException(const std::string& l_msg) : _errorMsg(l_msg) {}
        const char* what() const noexcept override {
            return _errorMsg.c_str();
        }

    private:
        std::string _errorMsg;
};

class InvalidFile : public ClientException {
    public:
        InvalidFile() : ClientException("Error occured while reading from file") {}
};

class InvalidCmd : public ClientException {
    public:
        InvalidCmd() : ClientException("Invalid command in keys.cfg") {}
};
