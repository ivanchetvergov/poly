// MyStack.tpp

#include "../include/MyStack.h" 

template <class T, int N>
MyStack<T, N>::MyStack() : size_(-1) {}

template <class T, int N>
void MyStack<T, N>::Push(const T& element) {
    if (size_ >= N - 1) {
        throw StackOverflow();
    }
    arr_[++size_] = element;
}

template <class T, int N>
T MyStack<T, N>::Pop() {
    if (size_ < 0) {
        throw StackUnderflow();
    }
    return arr_[size_--];
}

template <class T, int N>
int MyStack<T, N>::GetSize() const {
    return size_ + 1;
}

template <class T, int N>
int MyStack<T, N>::Capacity() const {
    return N;
}

template <class T, int N>
T& MyStack<T, N>::operator[](int index) {
    if (index < 0 || index > size_) {
        throw StackOutOfRange(index);
    }
    return arr_[index];
}

template <class T, int N>
const T MyStack<T, N>::operator[](int index) const {
    if (index < 0 || index > size_) {
        throw StackOutOfRange(index);
    }
    return arr_[index];
}