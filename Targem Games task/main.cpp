#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "String.h"

void BasicStringTests() {
	using namespace std;
	{
		string str("hello\0");
		assert(str.size() == 5);
	}

	{
		string str1("hello");
		string str2("world");
		str1 += str2;
		assert(str2 == "world");
		assert(str1 == "helloworld");
	}
}

void TestConstructors() {
	// Протестируем все конструкторы.
	
	{
		// Конструктор по умолчанию
		String empty_str;
		assert(empty_str.Size() == 0);
	}
	
	size_t str_lenght = 10;
	{
		// Конструктор на основании разрмера строки
		String str_without_values(str_lenght);
		assert(str_without_values.Size() == str_lenght);
	}
	
	{
		// Конструктор на основании размера и заданного символа
		char test_ch = 'a';
		String str_with_ch(str_lenght, test_ch);
		assert(str_with_ch.Size() == str_lenght);
		//assert(str_with_ch == "aaaaaaaaaa");
	}
	
	{
		// Конструктор на основании строки в стиле C
		String c_str("hello\0");
		assert(c_str.Size() == 5);
		assert(c_str == "hello");
	}

	{
		// Копирующий конструктор
		String s1("test\0");
		String s2(s1);
		assert(s1.Size() == s2.Size());
		assert(s1 == s2);
	}

	{
		// Конструктор перемещения
		String s1("move\0");
		String s2(std::move(s1));
		assert(s2 == "move");
	}
	std::cout << "All contructors tests passed successfully" << std::endl;
}

void TestOperators() {

	{
		// Оператор =
		String str1 = "hello";
		String str2;
		str2 = str1;
		assert(str1.Size() == str2.Size());
		assert(str1 == str2);

		str1 = "aaaa";
		str2 = "bbb";
		str1 = str2;
		assert(str1.Size() == str2.Size());
		assert(str1 == str2);

		str1 = "";
		str2 = "";
		str1 = str2;
		assert(str1.Size() == str2.Size());
		assert(str1 == str2);
		
	}

	{
		// Оператор +=
		String str1("hello");
		String str2("world");
		str1 += str2;
		assert(str2 == "world");
		assert(str1 == "helloworld");
		
		str1 = "";
		str2 = "aaa";
		str1 += str2;
		assert(str1.Size() == str2.Size());
		assert(str1 == str2);

		str1 = "";
		str2 = "";
		str1 += str2;
		assert(str1 == "");
		assert(str1.Size() == 0);

		str1 = "aaaa";
		str2 = "";
		str1 += str2;
		assert(str1.Size() == 4);
		assert(str1 == "aaaa");

	}

	{
		// Оператор <
		String str1 = "aaa";
		String str2 = "b";
		assert(str1 < str2);

		str1 = "aabb";
		str2 = "aaab";
		assert(str2 < str1);

		str1 = "";
		str2 = "";
		assert(!(str1 < str2));

	}

	{
		// Оператор ==
		String str1 = "aaa";
		String str2 = "bbb";
		assert(!(str1 == str2));

		str2 = "aaa";
		assert(str1 == str2);

		str2 = "aa";
		assert(!(str1 == str2));

	}
}


int main() {
	BasicStringTests();
	TestConstructors();
	TestOperators();
	return 0;
}