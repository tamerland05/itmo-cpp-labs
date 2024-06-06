#include "lib/my_ranges.h"

#include <gtest/gtest.h>
#include <random>
#include <string>
#include <ranges>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distribution(1, 10000000);

template<class T>
void check_trans(const auto func, std::vector<T>& v) {
    auto std_trans = v | std::views::transform(func);
    auto std_beg = std_trans.begin();
    auto std_end = std_trans.end();

    for (auto my_trans : v | ranges::transform(func)) {
        ASSERT_EQ(my_trans, *std_beg++);
    }
}

TEST(TransformTestsSuite, Funcs) {
    std::vector<int> v_i;
    for (int i = 0; i < 100; ++i) {
        v_i.emplace_back(distribution(gen));
    }
    check_trans<int>([](const int i) { return i + 1; }, v_i);
    check_trans<int>([](const int i) { return i * 10; }, v_i);
    check_trans<int>([](const int i) { return i * i; }, v_i);
    check_trans<int>([](const int i) { return std::to_string(i); }, v_i);
    check_trans<int>([](const int i) { return std::vector<int>{10, i}; }, v_i);

    std::vector<std::string> v_s;
    for (int i = 0; i < 100; ++i) {
        v_s.emplace_back(std::to_string(distribution(gen)));
    }
    check_trans<std::string>([](const std::string& i) { return i + i; }, v_s);
    check_trans<std::string>([](const std::string&  i) { return (int)i[0] + (int)i[1]; }, v_s);
}

template<class T>
void check_filter(const auto func, const std::vector<T>& v) {
    auto std_trans = v | std::views::filter(func);
    auto std_beg = std_trans.begin();
    auto std_end = std_trans.end();

    for (auto my_trans : v | ranges::filter(func)) {
        ASSERT_EQ(my_trans, *std_beg);
        ++std_beg;
    }
}

TEST(FilterTestsSuite, Funcs) {
    std::vector<int> v_i;
    for (int i = 0; i < 100; ++i) {
        v_i.emplace_back(distribution(gen));
    }
    check_filter<int>([](const int i) { return i % 2 == 0; }, v_i);
    check_filter<int>([](const int i) { return i * 10 < 100000; }, v_i);
    check_filter<int>([](const int i) { return true; }, v_i);
    check_filter<int>([](const int i) { return std::to_string(i).size() > 1; }, v_i);

    std::vector<std::string> v_s;
    for (int i = 0; i < 100; ++i) {
        v_s.emplace_back(std::to_string(distribution(gen)));
    }
    check_filter<std::string>([](const std::string& i) { return (i + i).size() > 14; }, v_s);
    check_filter<std::string>([](const std::string& i) { return (int)i[0] + (int)i[1] > 10; }, v_s);
}

template<class T>
void check_take_drop(const long long num, const std::vector<T>& v) {
    auto std_trans = v | std::views::take(num) | std::views::drop(num);
    auto std_beg = std_trans.begin();
    auto std_end = std_trans.end();

    for (auto my_trans : v | ranges::take(num) | ranges::drop(num)) {
        ASSERT_EQ(my_trans, *std_beg);
        ++std_beg;
    }
}

TEST(TakeDropTestsSuite, Funcs) {
    std::vector<int> v_i;
    for (int i = 0; i < 100; ++i) {
        v_i.emplace_back(distribution(gen));
    }
    check_take_drop<int>(1, v_i);
    check_take_drop<int>(10, v_i);
    check_take_drop<int>(20, v_i);
    check_take_drop<int>(50, v_i);
}

template<class T>
void check_reverse(const std::vector<T>& v) {
    auto std_rev = std::views::reverse(v);
    auto std_beg = std_rev.begin();
    auto std_end = std_rev.end();

    for (auto my_trans : v | ranges::reverse()) {
        ASSERT_EQ(my_trans, *std_beg);
        ++std_beg;
    }
}

TEST(ReverceTestsSuite, Funcs) {
    for (int j = 0; j < 5; ++j) {
        std::vector<int> v_i;
        for (int i = 0; i < 100; ++i) {
            v_i.emplace_back(distribution(gen));
        }
        check_reverse(v_i);
    }
}
