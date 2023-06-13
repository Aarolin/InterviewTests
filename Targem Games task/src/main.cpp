#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "String.h"

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

		str1 = "aaa";
		assert(str1 < "b");
		assert(str1 < "aab");
		assert(!(str1 < "aaa"));
		assert(str1 < "aaaa");
		assert("aa" < str1);
		assert(!("aaa" < str1));
		assert(!("ab" < str1));

		// Протестируем сравнение строк с разным регистром.
		str1 = "AAA";
		assert(str1 < "aab");
		assert(str1 < "b");
		assert(!(str1 < "aaa"));

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

		assert(str2 == "aa");
		assert("aa" == str2);
		assert(!(str2 == "ab"));
		assert(!("ab" == str2));
		assert(str2 == "AA");
		assert("AA" == str2);
	}

	{
		// Push
		String str;
		str.Push('a');
		assert(str.Size() == 1);
		str.Push('b');
		assert(str.Size() == 2);
		assert(str == "ab");
		str.Push('c');
		assert(str.Size() == 3);
		assert(str == "abc");
	}
}

void TestContainers() {
	{
		// Протестируем вектор
		std::vector<String> v1(3, "aa");
		std::vector<String> v2 = { "aa", "aa", "aa" };
		assert(v1.size() == 3);
		assert(v1 == v2);
		// Добавление элемента в конец
		v1.push_back("c_string");
		v1.push_back(String(2));
		v1.push_back(String("cc_string"));
		v1.push_back(std::move(String("moved_string")));
		assert(v1.size() == 7);

		// Вставка элемента в середину.
		v1.insert(v1.begin() + 3, std::move("new_str"));
		assert(v1.size() == 8);
		// Удаление элемента из конца вектора
		v1.pop_back();
		assert(v1.size() == 7);
		// Удаление элемента из середины.
		v1.erase(v1.begin() + 2);
		assert(v1.size() == 6);
	}

	{
		// Протестируем map
		std::map<String, int> dict = { {"aa", 0}, {"bb", 1}, {"cc", 2} };
		std::vector<String> v1 = { "aa", "bb", "cc" };
		assert(dict.size() == 3);
		assert(dict.count("aa") == 1);
		assert(dict.count("bb") == 1);
		assert(dict.count("cc") == 1);

		// C++ 17
		for (const auto& [str, ind] : dict) {
			// Проверим правильно ли map расположил String
			assert(str == v1[ind]);
		}
		// Вставка элемента
		dict[String("dd")] = 3;
		assert(dict.size() == 4);
		assert(dict.count("dd") == 1);
		// Нахождение значения по ключу
		const auto& bb_ind = dict.at("bb");
		assert(bb_ind == 1);
		// Нахождение ключа
		const auto aa_iter = dict.find("aa");
		assert(aa_iter->first == "aa");
		assert(aa_iter->second == 0);
		// Удаление элемента
		dict.erase("bb");
		assert(dict.size() == 3);
		assert(dict.count("bb") == 0);

	}

	{
		// Протестируем set
		std::set<String> dict = { "aa", "bb", "cc"};
		std::vector<String> v1 = { "aa", "bb", "cc" };
		assert(dict.size() == 3);
		assert(dict.count("aa") == 1);
		assert(dict.count("bb") == 1);
		assert(dict.count("cc") == 1);
		size_t iter = 0;
		for (const auto& str : dict) {
			// Проверим правильно ли set расположил String
			assert(str == v1[iter++]);
		}
		// Вставка элемента
		dict.insert("dd");
		assert(dict.size() == 4);
		assert(dict.count("dd") == 1);
		// Удаление элемента
		dict.erase("aa");
		assert(dict.size() == 3);
		assert(dict.count("aa") == 0);
	}
}

void TestOutput() {
	String str = "Hello world\n";
	std::cout << str;
}

int main() {
	//TestConstructors();
	//TestOperators();
	//TestContainers();
	//TestOutput();
	std::vector<String> vc;
	String str;
	std::cout << "Enter ctrl + z to stop input" << std::endl;
	while (std::cin >> str) {
		vc.push_back(std::move(str)); 
	}

	std::sort(vc.begin(), vc.end(), [](const String& lhs, const String& rhs) {
		return rhs < lhs;
		});

	for (const String& str : vc) { 
		std::cout << str << std::endl;
	}

	return 0;
}