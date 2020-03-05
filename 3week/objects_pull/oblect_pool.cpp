
#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
	T* Allocate() {
		if (deallocator.empty()) {
			auto temp = new T;
			allocator.insert(temp);
			return *allocator.find(temp);
		}

		auto element = deallocator.front();
		allocator.insert(element);
		deallocator.pop();

		return element;
	};

	T* TryAllocate() {
		if (deallocator.empty()) {
			return nullptr;
		}

		return Allocate();
	};

	void Deallocate(T* object) {
		if (allocator.find(object) == allocator.end()) throw invalid_argument("Invalid object!");
		deallocator.push(object);
		allocator.erase(object);
	};

	~ObjectPool() {
		allocator.clear();
		while (!deallocator.empty()) deallocator.pop();

	};

private:
	queue <T*> deallocator; // освобожденные
	set <T*> allocator; // выделенные
};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);
	return 0;
}