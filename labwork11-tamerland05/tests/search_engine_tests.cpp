#include <lib/search_engine.h>
#include <gtest/gtest.h>


TEST(Test1, Execute) {
    SearchEngine search_engine("C:\\C++\\test");
    std::vector<std::string> request{"for", "OR", "green", "OR", "anybody"};
    search_engine.DoSearch(request);
}

TEST(Test2, Execute) {
    SearchEngine search_engine("C:\\C++\\test");
    std::vector<std::string> request{"for"};
    search_engine.DoSearch(request);
}

TEST(Test3, Execute) {
    SearchEngine search_engine("C:\\C++\\test");
    std::vector<std::string> request{"look", "OR", "mom"};
    search_engine.DoSearch(request);
}
