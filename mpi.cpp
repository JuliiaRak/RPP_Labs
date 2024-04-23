#include <mpi.h>
#include <vector>
#include <iostream>
#include <chrono>

// Функція пошуку пари
std::vector<int> findPair(const std::vector<int>& nums, int target) {
    std::vector<int> result(2, -1);
    for (int i = 0; i < nums.size(); i++) {
        for (int j = i + 1; j < nums.size(); j++) {
            if (nums[i] + nums[j] == target) {
                result[0] = i;
                result[1] = j;
                return result;
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


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> nums(N);

    for(int i = 0; i < N; i++) nums[i] = 30000;
    nums[location1] = 200;
    nums[location2] = 700;

    std::vector<int> local_nums;
    if (rank == 0) {
        local_nums = nums;
    }

    // Розділення даних на частини
    std::vector<int> local_size(1);
    int local_N = N; // Створюємо копію N для передачі через MPI_Bcast
    MPI_Bcast(&local_N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_size[0] = N / size;
    local_nums.resize(local_size[0]);
    MPI_Scatter(nums.data(), local_size[0], MPI_INT, local_nums.data(), local_size[0], MPI_INT, 0, MPI_COMM_WORLD);

    // Запуск таймера
    MPI_Barrier(MPI_COMM_WORLD);
    auto start = std::chrono::high_resolution_clock::now();

    // Виконання пошуку на процесі з рангом 0
    std::vector<int> global_result(2, -1);
    if (rank == 0) {
        global_result = findPair(local_nums, target);
    }

    // Розсилання результатів
    MPI_Bcast(global_result.data(), 2, MPI_INT, 0, MPI_COMM_WORLD);

    // Вимір часу виконання
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    if (rank == 0) {
        if (global_result[0] != -1) {
            std::cout << "Indices: " << global_result[0] << ", " << global_result[1] << std::endl;
        } else {
            std::cout << "No two sum solution found." << std::endl;
        }
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    }

    MPI_Finalize();
    return 0;
}
