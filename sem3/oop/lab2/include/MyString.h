#pragma once
#include <string>

class MyString {
public:
    MyString();
    explicit MyString(const char* s); // запрет неявного приведения
    MyString(const MyString& other);
    ~MyString();

    MyString& operator=(const MyString& other);

    const char* GetString() const;
    int GetLength() const;

    friend bool operator==(const MyString& l, const MyString& r);
    friend std::ostream& operator<<(std::ostream& os, const MyString& myStr);
private:
    char* m_pStr;
	void Copy(const char* s);
	void Copy(const MyString& other);
};