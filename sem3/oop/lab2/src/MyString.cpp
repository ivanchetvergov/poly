// MyString.cpp
#include "../include/stdafx.h"
#include "../include/MyString.h"

#include <algorithm>
#include <cstring> 

MyString::MyString() : m_pStr(nullptr) {
    Copy("default");
}

MyString::MyString(const char* s) : m_pStr(nullptr) {
    Copy(s);
}

MyString::MyString(const MyString& other) : m_pStr(nullptr) {
    Copy(other.m_pStr);
}

MyString::~MyString() {
    delete[] m_pStr;
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        Copy(other.m_pStr);
    }
    return *this;
}

const char* MyString::GetString() const {
    return m_pStr;
}

int MyString::GetLength() const {
    return (m_pStr) ? strlen(m_pStr) : 0;
}

bool operator==(const MyString& l, const MyString& r) {
    if (l.m_pStr == nullptr && r.m_pStr == nullptr) {
        return true;
    }
    if (l.m_pStr == nullptr || r.m_pStr == nullptr) {
        return false;
    }
    return strcmp(l.m_pStr, r.m_pStr) == 0;
}

void MyString::Copy(const char* s) {
    if (m_pStr) {
        delete[] m_pStr;
    }
    if (s) {
        size_t len = strlen(s) + 1;
        m_pStr = new char[len];
        std::copy(s, s + len, m_pStr);
    } else {
        m_pStr = nullptr;
    }
}

void MyString::Copy(const MyString& other) {
    Copy(other.m_pStr);
}

std::ostream& operator<<(std::ostream& os, const MyString& myStr) {
    os << myStr.GetString();
    return os;
}