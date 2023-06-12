#include "String.h"

Allocator::Allocator(size_t capacity) {
	if (capacity > 0) {
		data_ = static_cast<char*>(operator new (capacity * sizeof(char)));
		capacity_ = capacity;
	}
}

Allocator::Allocator(char* str) : data_(str) {

}

Allocator::~Allocator() {
	operator delete (data_);
	capacity_ = 0;
}

Allocator::Allocator(Allocator&& other) noexcept : data_(std::move(other.data_)), capacity_(std::move(other.capacity_)) {
	other.data_ = nullptr;
	other.capacity_ = 0;
}

Allocator& Allocator::operator=(Allocator&& other) noexcept {
	if (this != &other) {
		data_ = std::move(other.data_);
		capacity_ = std::move(other.capacity_);
		other.data_ = nullptr;
		other.capacity_ = 0;
	}
	return *this;
}

char& Allocator::operator[](size_t index) noexcept {
	assert(index < capacity_);
	return data_[index];
}

const char& Allocator::operator[](size_t index) const noexcept {
	assert(index < capacity_);
	return data_[index];
}

char* Allocator::GetAddress() const noexcept {
	return data_;
}

size_t Allocator::Capacity() const noexcept {
	return capacity_;
}

void Allocator::Swap(Allocator& other) {
	std::swap(data_, other.data_);
	std::swap(capacity_, other.capacity_);
}

String::String(size_t size) : String(size, char{}) {

}

String::String(size_t size, char ch) : str_(Allocator(size)), size_(size) {
	for (size_t i = 0; i < size; ++i) {
		str_[i] = ch;
	}
}

String::String(const char* str) {
	size_t str_length = std::strlen(str);
	str_ = Allocator(str_length);
	for (size_t i = 0; i < str_length; ++i) {
		str_[i] = str[i];
	}
	size_ = str_length;
}

String::String(const String& other) : str_(Allocator(other.str_.Capacity())), size_(other.size_) {
	for (size_t i = 0; i < size_; ++i) {
		str_[i] = other.str_[i];
	}
}

String::String(String&& other) noexcept : str_(std::move(other.str_)), size_(std::move(other.size_)) {

}

String& String::operator=(String&& other) noexcept {
	// Защитимся от присваивания самому себе.
	if (this != &other) {
		str_ = std::move(other.str_);
		size_ = std::move(other.size_);
	}
	return *this;
}

String& String::operator=(const String& other) {
	// Защитимся от присваивания самому себе.
	if (this != &other) {
		// Безопасное присваивание.
		String copy(other);
		Swap(copy);
	}
	return *this;
}




String& String::operator+=(const String& other) {
	size_t new_str_size = size_ + other.size_;
	size_t cur_capacity = str_.Capacity();
	// Проверим, хватает ли нам запаса сырой памяти, чтобы объединить две строки.
	if (new_str_size > cur_capacity) {
		// Если запаса мало, то выделим новую память и объединим старую строку с новой.
		String new_str(std::max(new_str_size, cur_capacity * 2));
		// Избавимся от копипаста циклов
		CopyToNewStr(*this, new_str, 0, size_);
		CopyToNewStr(other, new_str, size_, new_str_size);
		Swap(new_str);
		return *this;
	}
	CopyToNewStr(other, *this, size_, new_str_size);
	size_ = new_str_size;
	return *this;
}

String& String::operator+=(const char* str) {
	size_t new_str_size = std::strlen(str) + size_;
	size_t cur_capacity = str_.Capacity();
	// Аналогично методу выше.
	if (new_str_size > cur_capacity) {
		String new_str(std::max(new_str_size, cur_capacity * 2));
		CopyToNewStr(*this, new_str, 0, size_);
		CopyToNewStr(str, new_str, size_, new_str_size);
		Swap(new_str);
		return *this;
	}
	CopyToNewStr(str, *this, size_, new_str_size);
	size_ = new_str_size;
	return *this;
}


char& String::operator[](size_t index) noexcept {
	return str_[index];
}

const char& String::operator[](size_t index) const noexcept {
	return str_[index];
}

size_t String::Size() const noexcept {
	return size_;
}

void String::Swap(String& other) {
	str_.Swap(other.str_);
	std::swap(size_, other.size_);
}

void CopyToNewStr(const String& old_str, String& new_str, size_t begin, size_t end) {
	for (size_t i = begin, j = 0; i < end; ++i, ++j) {
		new_str[i] = old_str[j];
	}
}

void CopyToNewStr(const char* old_str, String& new_str, size_t begin, size_t end) {
	for (size_t i = begin, j = 0; i < end; ++i, ++j) {
		new_str[i] = old_str[j];
	}
}


String operator+(const String& lhs, const String& rhs) {
	size_t new_str_len = lhs.Size() + rhs.Size();
	String new_str(new_str_len);
	CopyToNewStr(lhs, new_str, 0, lhs.Size());
	CopyToNewStr(rhs, new_str, new_str.Size(), new_str_len);
	return new_str;
}

String operator+(const char* lhs, const String& rhs) {
	size_t lhs_len = std::strlen(lhs);
	size_t new_str_len = lhs_len + rhs.Size();
	String new_str(new_str_len);
	CopyToNewStr(lhs, new_str, 0, lhs_len);
	CopyToNewStr(rhs, new_str, lhs_len, new_str_len);
	return new_str;
}

String operator+(const String& lhs, const char* rhs) {
	return rhs + lhs;
}

bool operator<(const String& lhs, const String& rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = lhs.Size();
	size_t r_end = rhs.Size();
	for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
		if (lhs[l_iter] < rhs[r_iter]) {
			return true;
		}
		if (rhs[r_iter] < lhs[l_iter]) {
			return false;
		}
	}
	return (l_iter == l_end - 1) && (r_iter != r_end - 1);
}

bool operator<(const String& lhs, const char* rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = lhs.Size();
	size_t r_end = std::strlen(rhs);
	for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
		if (lhs[l_iter] < rhs[r_iter]) {
			return true;
		}
		if (rhs[r_iter] < lhs[l_iter]) {
			return false;
		}
	}
	return (l_iter == l_end - 1) && (r_iter != r_end - 1);
}


bool operator==(const String& lhs, const String& rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = lhs.Size();
	size_t r_end = rhs.Size();
	if (l_end == r_end) {
		for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
			if (lhs[l_iter] != rhs[r_iter]) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool operator==(const String& lhs, const char* rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = lhs.Size();
	size_t r_end = std::strlen(rhs);
	if (l_end == r_end) {
		for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
			if (lhs[l_iter] != rhs[r_iter]) {
				return false;
			}
		}
		return true;
	}
	return false;
}