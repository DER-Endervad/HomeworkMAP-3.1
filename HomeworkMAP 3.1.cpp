#include <iostream>
#include <vector>
#include <Windows.h>
#include <future>

void selection_sort(std::vector<int>& list);
void search_min1(const std::vector<int>& list, const int& point, const int& full_point, std::promise<int> prom);
void search_min2(const std::vector<int>& list, const int& point, std::promise<int> prom);
void print_vector(const std::vector<int> &vec);

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    std::vector<int> vec = { 4, 2, 12, 7, 4, 5, 11, 7, 8, 3, 9 };
    std::cout << "Список без сортировки: "; print_vector(vec);
    selection_sort(vec);
    std::cout << "Список после сортировки: "; print_vector(vec);
}

void selection_sort(std::vector<int> &list) {
    int point = list.size() / 2;
    int full_point = 0;
    while (full_point < list.size()) {
        std::promise<int> promise_res1; std::promise<int> promise_res2;
        std::future<int> future_res1 = promise_res1.get_future();
        std::future<int> future_res2 = promise_res2.get_future();
        std::future<void> task1 = std::async(search_min1, list, point, full_point, std::move(promise_res1));
        std::future<void> task2 = std::async(search_min2, list, point, std::move(promise_res2));
        future_res1.wait(); future_res2.wait();
        int f_r1 = future_res1.get(); int f_r2 = future_res2.get();
        if (list[f_r1] > list[f_r2]) {
            int move = list[full_point];
            list[full_point] = list[f_r2];
            list[f_r2] = move;
        }
        else {
            int move = list[full_point];
            list[full_point] = list[f_r1];
            list[f_r1] = move;
        }
        full_point++;
        point = (list.size() + full_point) / 2;
    }
}

void search_min1(const std::vector<int>& list, const int& point, const int& full_point, std::promise<int> prom) {
    int min = list[point];
    int number = point;
    for (int i = point; i >= full_point; --i) {
        if (min > list[i]) {
            min = list[i];
            number = i;
        }
    }
    prom.set_value(number);
}

void search_min2(const std::vector<int>& list, const int& point, std::promise<int> prom) {
    int min = list[point];
    int number = point;
    for (int i = point; i < list.size(); ++i) {
        if (min > list[i]) {
            min = list[i];
            number = i;
        }
    }
    prom.set_value(number);
}

void print_vector(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}
