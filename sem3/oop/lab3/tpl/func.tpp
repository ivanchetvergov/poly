// func.tpp

template <class T>
void Swap(T& x, T& y) {
    T temp = x;
    x = y;
    y = temp;
}