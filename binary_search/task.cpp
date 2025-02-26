#include <iostream>
#include <vector>

size_t FindIndex(int value, std::vector<int>& vec) {

    size_t left_index = 0;
    size_t right_index = vec.size() - 1;
    size_t middle_index = (left_index + right_index) / 2;

    while (vec[middle_index] != value) {

        if (vec[left_index] == value)  // Не получилось без этих проверок решить,
            return left_index;         // возможно, я просто чего-то не знаю, но
        if (vec[right_index] == value) // происходило зацикливание, хотя пробовал
            return right_index;        // и с округлением, и без.

        if (vec[middle_index] > value) {
            if (vec[right_index] >= value && vec[left_index] > value && vec[middle_index] > vec[left_index]) {
                left_index = middle_index;
                middle_index = (left_index + right_index) / 2;
            } else {
                right_index = middle_index;
                middle_index = (left_index + right_index) / 2;
            }
        } else {
            if (vec[right_index] >= value) {
                left_index = middle_index;
                middle_index = (left_index + right_index) / 2;
            } else {
                right_index = middle_index;
                middle_index = (left_index + right_index) / 2;
            }
        }
    }
    return middle_index;
}

int main() {
    std::vector<int> v = {5, 6, 7, 8, 9, 1, 2, 3, 4};
    for (int i = 1; i <= 9; ++i)
        std::cout << FindIndex(i, v) << std::endl;
}