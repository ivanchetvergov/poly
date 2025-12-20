// func.tpp

template <typename T>
void printContainer(const T& cont) {
    for (const auto& item : cont) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void printPop(T container) {
    while (!container.empty()) {
        // проверяем std::queue
        // constexpr директива компилятора
        if constexpr (std::is_same_v<T, std::queue<typename T::value_type>>) {
            std::cout << container.front() << " ";
        } else {
            std::cout << container.top() << " ";
        }
        container.pop();
    }
    std::cout << std::endl;
}

template <typename T>
void printMap(const T& container) {
    for (const auto& pair : container) {
        std::cout << "{" << pair.first << ": " << pair.second << "} ";
    }
    std::cout << std::endl;
}

template<typename T>
void print(const T& item) {
    std::cout << item << " ";
}

