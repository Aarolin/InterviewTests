#pragma once
#include <iostream>
#include <cassert>
#include <cstring>
#include <utility>

class Allocator {
public:
	Allocator() = default;
	explicit Allocator(size_t size);
	explicit Allocator(char* str);
	
	Allocator(const Allocator& other) = delete;
	Allocator(Allocator&& other) noexcept;

	Allocator& operator=(const Allocator& other) = delete;
	Allocator& operator=(Allocator&& other) noexcept;

	char& operator[](size_t index) noexcept;
	const char& operator[](size_t index) const noexcept;

	char* GetAddress() const noexcept;
	size_t Capacity() const noexcept;

	void Swap(Allocator& other);
	void Clear();

	~Allocator();

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

	void Push(char ch);
	bool Empty();
	void Clear();

	char& operator[](size_t index) noexcept;
	const char& operator[](size_t index) const noexcept;

	size_t Size() const noexcept;

private:
	Allocator str_;
	size_t size_ = 0;

	void Swap(String& other);
};

void CopyToNewStr(const String& old_str, String& new_str, size_t begin, size_t end);
void CopyToNewStr(const char* old_str, String& new_str, size_t begin, size_t end);

char GetLowCaseChar(char ch);

bool operator<(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const char* rhs);
bool operator<(const char* lhs, const String& rhs);

bool operator==(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const char* rhs);
bool operator==(const char* lhs, const String& rhs);

String operator+(const String& lhs, const String& rhs);
String operator+(const char* lhs, const String& rhs);
String operator+(const String& lhs, const char* rhs);

std::ostream& operator<<(std::ostream& os, const String& str);
std::istream& operator>>(std::istream& is, String& str);
