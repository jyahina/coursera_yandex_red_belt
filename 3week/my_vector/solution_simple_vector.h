#pragma once
#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
	SimpleVector() = default;

	explicit SimpleVector(size_t size)
	{
		size_data = size;
		capacity_data = size;
		data = new T[size];
	};

	~SimpleVector()
	{
		delete[] data;
	};

	T& operator[](size_t index)
	{
		return data[index];
	};

	T* begin()
	{
		return data;
	};

	T* end()
	{
		return data + size_data;
	};

	size_t Size() const
	{
		return size_data;
	};

	size_t Capacity() const
	{
		return capacity_data;
	};

	void PushBack(const T& value)
	{
		if (Empty()) {

			capacity_data = 1;
			data = new T[capacity_data];
		}

		if (Size() >= Capacity()) {

			capacity_data = 2 * Capacity();
			T* temp = new T[capacity_data];

			std::copy(begin(), end(), temp);
			delete[] data;

			data = temp;
		}

		data[size_data] = value;
		size_data++;
	};

private:
	T* data = nullptr;
	size_t size_data = 0;
	size_t capacity_data = 0;

	bool Empty() const 
	{
		return Size() == 0u && Capacity() == 0u;
	};

};