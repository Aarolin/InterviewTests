#include "String.h"

RawMemory::RawMemory(size_t capacity) {
	if (capacity > 0) {
		data_ = static_cast<char*>(operator new (capacity * sizeof(char)));
		capacity_ = capacity;
	}
}

RawMemory::RawMemory(char* str) : data_(str) {

}

RawMemory::~RawMemory() {
	Clear();
}

RawMemory::RawMemory(RawMemory&& other) noexcept : data_(std::move(other.data_)), capacity_(std::move(other.capacity_)) {
	other.data_ = nullptr;
	other.capacity_ = 0;
}

RawMemory& RawMemory::operator=(RawMemory&& other) noexcept {
	if (this != &other) {
		data_ = std::move(other.data_);
		capacity_ = std::move(other.capacity_);
		other.data_ = nullptr;
		other.capacity_ = 0;
	}
	return *this;
}

char& RawMemory::operator[](size_t index) noexcept {
	assert(index < capacity_);
	return data_[index];
}

const char& RawMemory::operator[](size_t index) const noexcept {
	assert(index < capacity_);
	return data_[index];
}


char* RawMemory::GetAddress() const noexcept {
	return data_;
}

size_t RawMemory::Capacity() const noexcept {
	return capacity_;
}

void RawMemory::Swap(RawMemory& other) {
	std::swap(data_, other.data_);
	std::swap(capacity_, other.capacity_);
}

void RawMemory::Clear() {
	if (data_) {
		operator delete(data_);
		capacity_ = 0;
		data_ = nullptr;
	}
}

String::String(size_t size) : String(size, char{}) {

}

String::String(size_t size, char ch) : str_(RawMemory(size)), size_(size) {
	for (size_t i = 0; i < size; ++i) {
		str_[i] = ch;
	}
}

String::String(const char* str) {
	size_t str_length = std::strlen(str);
	str_ = RawMemory(str_length);
	/*for (size_t i = 0; i < str_length; ++i) {
		str_[i] = str[i];
	}*/
	// Ётот вариант короче и без copy/paste.
	CopyFromStr(str, 0, str_length);
	size_ = str_length;
}

String::String(const String& other) : str_(RawMemory(other.str_.Capacity())){
	/*for (size_t i = 0; i < size_; ++i) {
		str_[i] = other.str_[i];
	}*/
	// Ётот вариант короче и без copy/paste.
	CopyFromStr(&other, 0, other.size_);
	size_ = other.size_;
}

String::String(String&& other) noexcept : str_(std::move(other.str_)), size_(std::move(other.size_)) {

}

String& String::operator=(String&& other) noexcept {
	// «ащитимс€ от присваивани€ самому себе.
	if (this != &other) {
		str_ = std::move(other.str_);
		size_ = std::move(other.size_);
	}
	return *this;
}

String& String::operator=(const String& other) {
	// «ащитимс€ от присваивани€ самому себе.
	if (this != &other) {
		// Ѕезопасное присваивание.
		String copy(other);
		Swap(copy);
	}
	return *this;
}

String& String::operator+=(const String& other) {
	size_t new_str_size = size_ + other.size_;
	size_t cur_capacity = str_.Capacity();
	// ѕроверим, хватает ли нам запаса сырой пам€ти, чтобы объединить две строки.
	if (new_str_size > cur_capacity) {
		// ≈сли запаса мало, то выделим новую пам€ть и объединим старую строку с новой.
		String new_str(std::max(new_str_size, cur_capacity * 2));
		// »збавимс€ от копипаста циклов
		/*CopyToNewStr(*this, new_str, 0, size_);
		CopyToNewStr(other, new_str, size_, new_str_size);*/
		new_str.CopyFromStr(this, 0, size_);
		new_str.CopyFromStr(&other, size_, new_str_size);
		Swap(new_str);
		return *this;
	}
	//CopyToNewStr(str, *this, size_, new_str_size);
	CopyFromStr(&other, size_, new_str_size);
	size_ = new_str_size;
	return *this;
}

String& String::operator+=(const char* str) {
	size_t new_str_size = std::strlen(str) + size_;
	size_t cur_capacity = str_.Capacity();
	// јналогично методу выше.
	if (new_str_size > cur_capacity) {
		String new_str(std::max(new_str_size, cur_capacity * 2));
		/*CopyToNewStr(*this, new_str, 0, size_);
		CopyToNewStr(str, new_str, size_, new_str_size);*/
		new_str.CopyFromStr(this, 0, size_);
		new_str.CopyFromStr(str, size_, new_str_size);
		Swap(new_str);
		return *this;
	}
	//CopyToNewStr(str, *this, size_, new_str_size);
	CopyFromStr(str, size_, new_str_size);
	size_ = new_str_size;
	return *this;
}

void String::Push(char ch) {
	size_t new_size = size_ + 1;
	if (new_size > str_.Capacity()) {
		String new_str(new_size * 2);
		//CopyToNewStr(*this, new_str, 0, size_);
		new_str.CopyFromStr(this, 0, size_);
		new_str[size_] = ch;
		//new_str.size_ = new_size;
		Swap(new_str);
		return;
	}
	str_[size_] = ch;
	size_++;
}

bool String::Empty() {
	return size_ == 0;
}

void String::Clear() {
	str_.Clear();
	size_ = 0;
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

//void CopyToNewStr(const String& old_str, String& new_str, size_t begin, size_t end) {
//	for (size_t i = begin, j = 0; i < end; ++i, ++j) {
//		new_str[i] = old_str[j];
//	}
//}
//
//void CopyToNewStr(const char* old_str, String& new_str, size_t begin, size_t end) {
//	for (size_t i = begin, j = 0; i < end; ++i, ++j) {
//		new_str[i] = old_str[j];
//	}
//}

char GetLowCaseChar(char ch) {
	return ch >= 'A' && ch <= 'Z' ? ch + 32 : ch;
}

String operator+(const String& lhs, const String& rhs) {
	size_t new_str_len = lhs.Size() + rhs.Size();
	size_t lhs_end = lhs.Size();
	String new_str(new_str_len);

	// lhs_end - индекс в новой строке до которого будет скопирована лева€ строка, и начина€ с которого будет копироватьс€ права€.
	new_str.CopyFromStr(&lhs, 0, lhs_end);
	new_str.CopyFromStr(&rhs, lhs_end, new_str_len);

	return new_str;
}

String operator+(const char* lhs, const String& rhs) {
	size_t lhs_len = std::strlen(lhs);
	size_t new_str_len = lhs_len + rhs.Size();
	String new_str(new_str_len);

	/*CopyToNewStr(lhs, new_str, 0, lhs_len);
	CopyToNewStr(rhs, new_str, lhs_len, new_str_len);*/

	// lhs_len - индекс в новой строке до которого будет скопирована лева€ строка, и начина€ с которого будет копироватьс€ права€.
	new_str.CopyFromStr(&lhs, 0, lhs_len);
	new_str.CopyFromStr(&rhs, lhs_len, new_str_len);

	return new_str;
}

String operator+(const String& lhs, const char* rhs) {

	// ќшибчный return, т.к. операци€ не ассоциативна.
	return rhs + lhs;
}

std::ostream& operator<<(std::ostream& os, const String& str) {
	for (size_t i = 0; i < str.Size(); ++i) {
		os << str[i];
	}
	return os;
}

std::istream& operator>>(std::istream& is, String& str) {
	if (!str.Empty()) {
		str.Clear();
	}
	while (is && (is.peek() != '\n' && is.peek() != '\0')) {
		str.Push(is.get());
	}
	is.get();
	return is;
}

bool operator<(const String& lhs, const String& rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = lhs.Size();
	size_t r_end = rhs.Size();
	for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
		// »гнорируем регистр
		char lhs_ch = GetLowCaseChar(lhs[l_iter]);
		char rhs_ch = GetLowCaseChar(rhs[r_iter]);
		if (lhs_ch < rhs_ch) {
			return true;
		}
		if (rhs_ch < lhs_ch) {
			return false;
		}
	}
	return (l_iter == l_end) && (r_iter != r_end);
}

bool operator<(const String& lhs, const char* rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = lhs.Size();
	size_t r_end = std::strlen(rhs);
	for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
		char lhs_ch = GetLowCaseChar(lhs[l_iter]);
		char rhs_ch = GetLowCaseChar(rhs[r_iter]);
		if (lhs_ch < rhs_ch) {
			return true;
		}
		if (rhs_ch < lhs_ch) {
			return false;
		}
	}
	return (l_iter == l_end) && (r_iter != r_end);
}

bool operator<(const char* lhs, const String& rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = std::strlen(lhs);
	size_t r_end = rhs.Size();
	for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
		char lhs_ch = GetLowCaseChar(lhs[l_iter]);
		char rhs_ch = GetLowCaseChar(rhs[r_iter]);
		if (lhs_ch < rhs_ch) {
			return true;
		}
		if (rhs_ch < lhs_ch) {
			return false;
		}
	}
	return (l_iter == l_end) && (r_iter != r_end);
}

bool operator==(const String& lhs, const String& rhs) {
	size_t l_iter = 0, r_iter = 0;
	size_t l_end = lhs.Size();
	size_t r_end = rhs.Size();
	if (l_end == r_end) {
		for (; l_iter < l_end && r_iter < r_end; ++l_iter, ++r_iter) {
			char lhs_ch = GetLowCaseChar(lhs[l_iter]);
			char rhs_ch = GetLowCaseChar(rhs[r_iter]);
			if (lhs_ch != rhs_ch) {
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
			char lhs_ch = GetLowCaseChar(lhs[l_iter]);
			char rhs_ch = GetLowCaseChar(rhs[r_iter]);
			if (lhs_ch != rhs_ch) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool operator==(const char* lhs, const String& rhs) {
	// »спользуем уже готовый оператор
	return rhs == lhs;
}

bool operator!=(const String& lhs, const String& rhs) {
	return !(lhs == rhs);
}

bool operator!=(const String& lhs, const char* rhs) {
	return !(lhs == rhs);
}

bool operator!=(const char* lhs, const String& rhs) {
	return !(lhs == rhs);
}
