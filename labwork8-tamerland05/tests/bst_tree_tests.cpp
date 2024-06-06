#include <lib/bst_tree.cpp>
#include <gtest/gtest.h>
#include <random>
#include <stack>

template<class T = BaseNode<int>, class A = std::allocator<T>>
void PreOrder(typename std::allocator_traits<A>::pointer node, IteratorPreOrder<T, A>& iterator, bool& f, std::stack<int>& stack) {
    if (node && f) {
        if (node != &*iterator) {
            f = false;
            return;
        }
        stack.push(node->key);
        PreOrder(node->left, ++iterator, f, stack);
        PreOrder(node->right, iterator, f, stack);
    }
}

template<class T = BaseNode<int>, class A = std::allocator<T>>
void InOrder(typename std::allocator_traits<A>::pointer  node, IteratorInOrder<T, A>& iterator, bool& f, std::stack<int>& stack) {
    if (node && f) {
        InOrder(node->left, iterator, f, stack);
        if (node != &*(iterator)) {
            f = false;
            return;
        }
        stack.push(node->key);
        InOrder(node->right, ++iterator, f, stack);
    }
}

template<class T = BaseNode<int>, class A = std::allocator<T>>
void PostOrder(typename std::allocator_traits<A>::pointer node, IteratorPostOrder<T, A>& iterator, bool& f, std::stack<int> stack) {
    if (node && f) {
        PostOrder(node->left, iterator, f, stack);
        PostOrder(node->right, iterator, f, stack);

        stack.push(node->key);
        if (node != &*(iterator++)) {
            f = false;
            return;
        }
    }
}

class BSTTreeTestsSuite : public testing::TestWithParam<std::tuple<int>> {
};

TEST_P(BSTTreeTestsSuite, InsertDeleteTest) {
    int to = std::get<0>(GetParam());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, to * 2);

    for (int j = 0; j < 10; ++j) {
        BaseBST<> my_tree{};
        bool f = true;
        for (int i = 0; i < to; ++i) {
            int key = distribution(gen);
            my_tree.insert(key);
            ASSERT_TRUE(my_tree.Search(key));
        }

        for (int i = 0; i < to; ++i) {
            int key = distribution(gen);
            my_tree.extract(key);
            ASSERT_FALSE(my_tree.Search(key));
        }

        ASSERT_TRUE(f);
    }
}

TEST_P(BSTTreeTestsSuite, PreOrderTest) {
    int to = std::get<0>(GetParam());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, to * 2);

    for (int j = 0; j < 10; ++j) {
        BaseBST<> my_tree{};
        bool f = true;
        for (int i = 0; i < to; ++i) {
            my_tree.insert(distribution(gen));
        }

        auto iter1 = my_tree.begin(pre_iter{});
        std::stack<int> stack;
        PreOrder(my_tree.root_, iter1, f, stack);
        iter1 = my_tree.end(pre_iter{});

        while (!stack.empty()) {
            if ((*(iter1--)).key != stack.top()) {
                f = false;
                break;
            }
            stack.pop();
        }

        ASSERT_TRUE(f);
    }
}

TEST_P(BSTTreeTestsSuite, InOrderTest) {
    int to = std::get<0>(GetParam());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, to * 2);

    for (int j = 0; j < 10; ++j) {
        BaseBST<> my_tree{};
        bool f = true;
        for (int i = 0; i < to; ++i) {
            int a = distribution(gen);
            my_tree.insert(a);
        }

        auto iter1 = my_tree.begin(in_iter{});
        std::stack<int> stack;
        InOrder(my_tree.root_, iter1, f, stack);
        iter1 = my_tree.end(in_iter{});

        while (!stack.empty()) {
            if ((*(iter1--)).key != stack.top()) {
                f = false;
                break;
            }
            stack.pop();
        }

        ASSERT_TRUE(f);
    }
}

TEST_P(BSTTreeTestsSuite, PostOrderTest) {
    int to = std::get<0>(GetParam());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, to * 2);

    for (int j = 0; j < 10; ++j) {
        BaseBST<> my_tree{};
        bool f = true;
        for (int i = 0; i < to; ++i) {
            int a = distribution(gen);
            my_tree.insert(a);
        }

        auto iter1 = my_tree.begin(post_iter{});
        std::stack<int> stack;
        PostOrder(my_tree.root_, iter1, f, stack);
        iter1 = my_tree.end(post_iter{});

        while (!stack.empty()) {
            if ((*(iter1--)).key != stack.top()) {
                f = false;
                break;
            }
            stack.pop();
        }

        ASSERT_TRUE(f);
    }
}

INSTANTIATE_TEST_SUITE_P(
        Group,
        BSTTreeTestsSuite,
        testing::Values(
                std::make_tuple(5),
                std::make_tuple(10),
                std::make_tuple(100),
                std::make_tuple(1000),
                std::make_tuple(10000),
                std::make_tuple(100000),
                std::make_tuple(200000),
                std::make_tuple(300000),
                std::make_tuple(400000),
                std::make_tuple(500000)
        )
);
