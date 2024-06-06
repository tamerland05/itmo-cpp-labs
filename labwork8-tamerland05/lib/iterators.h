#include <memory>
#include <iostream>
#include <variant>

template<class T = int>
struct BaseNode {
    explicit BaseNode(T& value) : key(value) {};

    T key{};
    BaseNode<T>* left{};
    BaseNode<T>* right{};
    BaseNode<T>* parent{};
};

template<class T = int, class A = std::allocator<BaseNode<T>>>
class BaseIteratorTraversal {
public:
    typedef A allocator_type;
    typedef std::allocator_traits<A>::value_type& reference;
    typedef reference const_reference;
    typedef typename std::allocator_traits<A>::pointer pointer;

    explicit BaseIteratorTraversal(pointer p) : pointer_(p) {} ;

    BaseIteratorTraversal(const BaseIteratorTraversal&);

    ~BaseIteratorTraversal() = default;

    BaseIteratorTraversal& operator=(const BaseIteratorTraversal&);

    bool operator==(const BaseIteratorTraversal&) const;

    bool operator!=(const BaseIteratorTraversal&) const;

    virtual BaseIteratorTraversal& operator++() = 0 ;

    virtual BaseIteratorTraversal& operator--() = 0;

    reference operator* () const;

public:
    pointer pointer_{};
};

template<class T = int, class A = std::allocator<T>>
class IteratorPreOrder : public BaseIteratorTraversal<T, A> {
public:
    explicit IteratorPreOrder(BaseNode<T>* p) : BaseIteratorTraversal<T, A>(p) {};

    IteratorPreOrder& operator++();
    IteratorPreOrder& operator--();
};

template<class T = int, class A = std::allocator<T>>
class IteratorInOrder : public BaseIteratorTraversal<T, A> {
public:
    explicit IteratorInOrder(BaseNode<T>* p) : BaseIteratorTraversal<T, A>(p) {};

    IteratorInOrder& operator++();
    IteratorInOrder& operator--();
};

template<class T = int, class A = std::allocator<T>>
class IteratorPostOrder : public BaseIteratorTraversal<T, A> {
public:
    explicit IteratorPostOrder(BaseNode<T>* p) : BaseIteratorTraversal<T, A>(p) {};

    IteratorPostOrder& operator++();
    IteratorPostOrder& operator--();
};

struct pre_iter{};
struct in_iter{};
struct post_iter{};