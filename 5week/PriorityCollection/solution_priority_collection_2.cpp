#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:

	using Id = size_t;/* тип, используемый для идентификаторов */

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
	Id Add(T object)
	{
		Id id = objects.size();

		priority.insert({ 0, id });
		objects.push_back(make_pair<T, int>(move(object), 0));

		return id;
	};

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin)
	{
		while (range_begin != range_end)
		{
			*ids_begin = Add(*range_begin);

			++ids_begin;
			++range_begin;
		}

	};

	// Определить, принадлежит ли идентификатор какому-либо
	// хранящемуся в контейнере объекту
	bool IsValid(Id id) const
	{
		return (id < objects.size());
	};

	// Получить объект по идентификатору
	const T& Get(Id id) const
	{
		return objects[id].first;
	};

	// Увеличить приоритет объекта на 1
	void Promote(Id id)
	{
		int new_priority = objects[id].second + 1;

		auto node_it = priority.find({ objects[id].second, id });
		auto node = move(priority.extract(node_it).value());
		
		node.first = new_priority;
		objects[node.second].second = new_priority;
	
		priority.insert(move(node));
	};

	// Получить объект с максимальным приоритетом и его приоритет
	pair<const T&, int> GetMax() const
	{
		auto obj = priority.rbegin();
		return { objects[obj->second].first, obj->first };
	};

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, int> PopMax()
	{
		auto obj_priority = prev(priority.end());
		Id id = obj_priority->second;
		
		pair<T, int> res = make_pair(move(objects[id].first), objects[id].second);		 
		objects.erase(objects.begin() + id);
		priority.erase(obj_priority);

		return res;
	};

private:
	// Приватные поля и методы
	set<pair<int, Id>> priority; // firts - priority, second - number object
	vector<pair<T, int>>objects;// firts - object, second - priority
};


class StringNonCopyable : public string {
public:
	using string::string;  // Позволяет использовать конструкторы строки
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestNoCopy);
	return 0;
}
