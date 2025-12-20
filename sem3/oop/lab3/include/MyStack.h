// MyStack.h
#pragma once

#pragma once

#include <iostream>
#include <stdexcept>

template <class T, int N>
class MyStack {
public:
    MyStack();
    void Push(const T& element);
    T Pop();
    int GetSize() const;
    int Capacity() const;
    T& operator[](int index);
    const T operator[](int index) const;

private:
    T arr_[N];
    int size_;
};

class StackOverflow : public std::runtime_error {
public:
    StackOverflow() : std::runtime_error("\nError! Stack is full!") {}
};

class StackUnderflow : public std::runtime_error {
public:
    StackUnderflow() : std::runtime_error("\nError! Stack is empty!") {}
};

class StackOutOfRange {
public:
    int index;
    StackOutOfRange(int i) : index(i) {}
    void Out() const {
        std::cout << "\nError! Index " << index << " is out of range!\n";
    }
};

#include "../tpl/MyStack.tpp"