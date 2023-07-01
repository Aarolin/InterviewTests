#pragma once
#include <iostream>
#include <cassert>
#include <cstring>
#include <utility>

class RawMemory {
public:
	RawMemory() = default;
	explicit RawMemory(size_t size);
	explicit RawMemory(char* str);
	
	RawMemory(const RawMemory& other) = delete;
	RawMemory(RawMemory&& other) noexcept;

	RawMemory& operator=(const RawMemory& other) = delete;
	RawMemory& operator=(RawMemory&& other) noexcept;

	char& operator[](size_t index) noexcept;
	const char& operator[](size_t index) const noexcept;

	char* GetAddress() const noexcept;
	size_t Capacity() const noexcept;

	void Swap(RawMemory& other);
	void Clear();

	~RawMemory();

private:
	char* data_ = nullptr;
	size_t capacity_ = 0;
};

class String {
public:
	String() = default;
	String(const char* str);

	String(size_t size);
	String(size_t size, char ch);

	String(const String& other);
	String& operator=(const String& other);

	String(String&& other) noexcept;
	String& operator=(String&& other) noexcept;

	String& operator+=(const String& other);
	String& operator+=(const char* other);

	template<typename Str>
	void CopyFromStr(const Str* str, size_t begin_d, size_t end_d);

	void Push(char ch);
	bool Empty();
	void Clear();

	char& operator[](size_t index) noexcept;
	const char& operator[](size_t index) const noexcept;

	size_t Size() const noexcept;

private:
	RawMemory str_;
	size_t size_ = 0;

	void Swap(String& other);


};

//void CopyToNewStr(const String& old_str, String& new_str, size_t begin, size_t end);
//void CopyToNewStr(const char* old_str, String& new_str, size_t begin, size_t end);
//template <typename Str>
//void CopyToNewStr(const Str* old_str, String& new_str, size_t new_str_begin_d, size_t new_str_end_d) {
//	for (size_t i = new_str_begin_d, j = 0; i < new_str_end_d; ++i, ++j) {
//		new_str[i] = old_str[j];
//	}
//}

template <typename Str>
void String::CopyFromStr(const Str* from, size_t begin_d, size_t end_d) {
	for (size_t i = begin_d, j = 0; i < end_d; ++i, ++j) {
		// Минус такого подхода в том, что счетчик j, который идет по строке откуда копируются символы не проверяется, поэтому, если мы выйдем за пределы, будет очень плохо.
		str_[i] = from[j];
	}
}


char GetLowCaseChar(char ch);

bool operator<(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const char* rhs);
bool operator<(const char* lhs, const String& rhs);

bool operator==(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const char* rhs);
bool operator==(const char* lhs, const String& rhs);

bool operator!=(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const char* rhs);
bool operator!=(const char* lhs, const String& rhs);

String operator+(const String& lhs, const String& rhs);
String operator+(const char* lhs, const String& rhs);
String operator+(const String& lhs, const char* rhs);

std::ostream& operator<<(std::ostream& os, const String& str);
std::istream& operator>>(std::istream& is, String& str);
