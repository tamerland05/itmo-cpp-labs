add_library(bst_tree bst_tree.h bst_tree.cpp)
add_library(iterators iterators.h iterators.cpp)

target_link_libraries(bst_tree PUBLIC iterators)
