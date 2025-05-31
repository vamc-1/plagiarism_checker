#include <iostream>
#include <vector>

double calculate_average(const std::vector<int>& nums) {
    if (nums.empty()) return 0.0;
    int sum = 0;
    for (int num : nums) sum += num;
    return static_cast<double>(sum) / nums.size();
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::cout << "Average: " << calculate_average(data) << std::endl;
    return 0;
}
