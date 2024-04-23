#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

std::vector<int> twoSumParallel(const std::vector<int>& nums, int target) {
    int n = nums.size();
    std::vector<int> result(2, -1);
    bool found = false;

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int start = (n / num_threads) * thread_id; // Розподіл роботи
        int end = (thread_id == num_threads - 1) ? n : start + (n / num_threads); // !!

        for (int i = start; i < end && !found; i++) {
            for (int j = i + 1; j < n && !found; j++) {
                if (nums[i] + nums[j] == target) {
                    #pragma omp critical
                    {
                        if (!found) {  // Перевірка всередині критичної секції
                            result[0] = i;
                            result[1] = j;
                            found = true;
                        }
                    }
                }
            }
        }
    }
    return result;
}

const int N = 150000;
// const int location1 = std::max(N - 1024, 1);
const int location1 = 10;
const int location2 = std::max(N - 2999, 1);

const int target = 900;

int main() {
    omp_set_num_threads(4);
     std::vector<int> nums(N);

    for(int i = 0; i < N; i++) nums[i] = 30000;
    nums[location1] = 200;
    nums[location2] = 700;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> indices = twoSumParallel(nums, target);
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "Indices: " << indices[0] << ", " << indices[1] << std::endl;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}
