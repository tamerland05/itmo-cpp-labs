#include <lib/scheduler.h>
#include <gtest/gtest.h>
#include <random>
#include <cmath>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distribution(1, 1000000000);

class QuadraticEquation : public testing::TestWithParam<std::tuple<float, float, float>> {
};

TEST_P(QuadraticEquation, Execute) {
    TTaskScheduler scheduler;

    float a = std::get<0>(GetParam());
    float b = std::get<1>(GetParam());
    float c = std::get<2>(GetParam());

    auto id1 = scheduler.Add([](float a, float c){return -4 * a * c;}, a, c);

    auto id2 = scheduler.Add([](float b, float v){return b * b + v;}, b, scheduler.GetFutureResult<float>(id1));

    auto id3 = scheduler.Add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));

    auto id4 = scheduler.Add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));

    auto id5 = scheduler.Add([](float a, float v){return v/(2*a);}, a, scheduler.GetFutureResult<float>(id3));

    auto id6 = scheduler.Add([](float a, float v){return v/(2*a);}, a, scheduler.GetFutureResult<float>(id4));

    scheduler.ExecuteAll();

    ASSERT_EQ(scheduler.GetResult<float>(id5), (-b + std::sqrt(b * b -4 * a * c)) / 2 / a);
    ASSERT_EQ(scheduler.GetResult<float>(id6), (-b - std::sqrt(b * b -4 * a * c)) / 2 / a);

    std::cout << "x1 = " << scheduler.GetResult<float>(id5) << std::endl;
    std::cout << "x2 = " << scheduler.GetResult<float>(id6) << std::endl;
}

TEST_P(QuadraticEquation, GetResult) {
    TTaskScheduler scheduler;

    float a = std::get<0>(GetParam());
    float b = std::get<1>(GetParam());
    float c = std::get<2>(GetParam());

    auto id1 = scheduler.Add([](float a, float c){return -4 * a * c;}, a, c);

    auto id2 = scheduler.Add([](float b, float v){return b * b + v;}, b, scheduler.GetFutureResult<float>(id1));

    auto id3 = scheduler.Add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));

    auto id4 = scheduler.Add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));

    auto id5 = scheduler.Add([](float a, float v){return v/(2*a);}, a, scheduler.GetFutureResult<float>(id3));

    auto id6 = scheduler.Add([](float a, float v){return v/(2*a);}, a, scheduler.GetFutureResult<float>(id4));

    ASSERT_EQ(scheduler.GetResult<float>(id5), (-b + std::sqrt(b * b -4 * a * c)) / 2 / a);
    ASSERT_EQ(scheduler.GetResult<float>(id6), (-b - std::sqrt(b * b -4 * a * c)) / 2 / a);

    std::cout << "x1 = " << scheduler.GetResult<float>(id5) << std::endl;
    std::cout << "x2 = " << scheduler.GetResult<float>(id6) << std::endl;
}

INSTANTIATE_TEST_SUITE_P(
    Group,
    QuadraticEquation,
    testing::Values(
        std::make_tuple(1, -2, 0),
        std::make_tuple(1, 2, 1),
        std::make_tuple(4, 4, 1),
        std::make_tuple(169, 26 * 3, 9),
        std::make_tuple(11, 228, 15)
    )
);

TEST(BallTest, Test3) {
    double r = 4.0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.Add([](double r) { return static_cast<double >(M_PI) * static_cast<double >(pow(r, 2)); }, r);

    auto id2 = scheduler.Add([](double r, double v) { return (4.0f / 3.0f) * v * r; }, r, scheduler.GetFutureResult<double>(id1));

    auto id3 = scheduler.Add([](double v) { return 4 * v; }, scheduler.GetFutureResult<double>(id1));

    auto t1 = round(scheduler.GetResult<double>(id2) * 10) / 10;
    auto t2 = round(scheduler.GetResult<double>(id3) * 10) / 10;

    scheduler.ExecuteAll();
    ASSERT_EQ(t1, 268.1);
    ASSERT_EQ(t2, 201.1);
}
TEST(TriangleArea, Test4) {
    double a = 5.0;
    double b = 5.0;
    double c = 8.0;
    double h = 4.8;
    double sin = 0.96;
    TTaskScheduler scheduler;

    auto id1 = scheduler.Add([](double a) { return static_cast<double >(1.0 / 2.0) * a; }, a);

    auto id2 = scheduler.Add([](double a, double h) { return a * h; }, scheduler.GetFutureResult<double>(id1), h);

    auto id3 = scheduler.Add([](double a, double b) { return a * b; }, scheduler.GetFutureResult<double>(id1), b);
    auto id4 = scheduler.Add([](double a, double b) { return a * b; }, scheduler.GetFutureResult<double>(id3), sin);

    scheduler.ExecuteAll();
    ASSERT_EQ(scheduler.GetResult<double>(id4), scheduler.GetResult<double>(id2));
}

