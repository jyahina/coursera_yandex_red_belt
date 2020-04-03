#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) 
{
    //1. Если диапазон содержит меньше 2 элементов, выйти из функции.
    if (std::distance(range_begin, range_end) < 2) return;

    //2. Создать вектор, содержащий все элементы текущего диапазона.
    vector<RandomIt::value_type> new_vec(
        make_move_iterator(range_begin), 
        make_move_iterator(range_end));

    //3. Разбить вектор на три равные части.
    size_t size_block = new_vec.size() / 3;
    
    auto first_block = new_vec.begin();
    auto second_block = first_block + size_block;
    auto third_block = second_block + size_block;

    //4. Вызвать функцию MergeSort от каждой части вектора.
    MergeSort(first_block, second_block);
    MergeSort(second_block, third_block);
    MergeSort(third_block, new_vec.end());

    ////5. Слить первые две трети вектора с помощью алгоритма merge, сохранив результат во временный вектор с помощью back_inserter.
    std::vector<int> temp_vec;
    std::merge(make_move_iterator(first_block), make_move_iterator(second_block),
        make_move_iterator(second_block), make_move_iterator(third_block),
        std::back_inserter(temp_vec));

    ////6. Слить временный вектор из предыдущего пункта с последней третью вектора из п. 2, записав полученный отсортированный диапазон вместо исходного.
    std::merge(make_move_iterator(temp_vec.begin()), make_move_iterator(temp_vec.end()),
        make_move_iterator(third_block), make_move_iterator(new_vec.end()), range_begin);

}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
