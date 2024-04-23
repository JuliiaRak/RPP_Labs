#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
// Функція twoSum, яка повертає індекси двох чисел, що додаються до цільового числа.
std::vector<int> twoSumBruteForce(const std::vector<int>& nums, int target) {

    for (int i = 0; i < nums.size(); i++) {

        for (int j = i + 1; j < nums.size(); j++) {

            if (nums[i] + nums[j] == target) {
                return {i, j}; // Знайдено два індекси
            }

        }
        
    }

    return {}; // Якщо пару не знайдено, повертаємо порожній вектор

}


const int N = 150000;
const int location1 = std::max(N - 1024, 1);
const int location2 = std::max(N - 2999, 1);
const int target = 900;

int main() {
    std::vector<int> nums(N);

    for(int i = 0; i < N; i++) nums[i] = 3;
    nums[location1] = 200;
    nums[location2] = 700;

    

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> result = twoSumBruteForce(nums, target);
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;


    if (!result.empty()) 
    {
        std::cout << "Indices: " << result[0] << ", " << result[1] << std::endl;
    } 
    else 
    {
        std::cout << "No two sum solution found." << std::endl;
    }
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}
