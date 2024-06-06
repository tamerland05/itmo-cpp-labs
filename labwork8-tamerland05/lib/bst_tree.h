#include "iterators.cpp"

template<class T = int, class A = std::allocator<BaseNode<T>>, class C = std::less<T>>
class BaseBST {
private:
    typedef A allocator_type;
    typedef std::allocator_traits<A>::value_type reference;
    typedef const A::value_type& const_reference;
    typedef typename std::allocator_traits<A>::pointer pointer;
    typedef typename std::allocator_traits<A>::value_type value_type;
    typedef typename std::allocator_traits<A>::difference_type difference_type;
    typedef typename std::allocator_traits<A>::size_type size_type;
    typedef T key_type;
    typedef C comparator;
    typedef comparator value_compare;
    typedef comparator key_compare;

    pointer SmartInsert(key_type key, pointer node);
    pointer SmartSearch(key_type value, pointer node) const;
    pointer SmartDelete(key_type value, pointer node);
    void DeletingTree(pointer node);

    pointer Minimum(pointer node) const;

    size_t size_{};
    allocator_type alloc_{};
    std::allocator_traits<allocator_type> alloc_traits_{};
    comparator compare_{};
    key_type max_{};
    key_type min_{};

public:
    typedef BaseIteratorTraversal<T, A> iterator;
    typedef BaseIteratorTraversal<T, A> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<iterator> const_reverse_iterator;

    BaseBST() = default;
    explicit BaseBST(const comparator&);
    BaseBST(const BaseBST<T, A, C>&);
    BaseBST(auto, auto&, C);
    BaseBST(const std::initializer_list<key_type>& il, comparator comp=C{});

    ~BaseBST() { clear(); }

    BaseBST& operator=(const BaseBST&);
    BaseBST& operator=(const std::initializer_list<T>&);
    bool operator==(const BaseBST&) const;
    bool operator!=(const BaseBST&) const;

    void insert(key_type value) { root_ = SmartInsert(value, root_); }
    void insert(const std::initializer_list<key_type>& il) { *this = BaseBST<T, A, C>(il); }

    bool Search(key_type value) const { return SmartSearch(value, root_); }
    void extract(key_type value) { root_ = SmartDelete(value, root_); }

    comparator key_comp() { return key_compare(); }
    comparator value_comp() { return key_compare(); }

    key_type max() { return max_; }
    key_type min() { return min_; }

    size_t erase(key_type);

    void clear() { DeletingTree(root_); };

    void swap(BaseBST&);
    void merge(const BaseBST<T, A, C>&);
    size_type size() const;
    size_type max_size() const;
    [[nodiscard]] bool empty() const;

    A& get_allocator() const;

    IteratorPreOrder<T, A> begin(pre_iter);
    IteratorPreOrder<T, A> end(pre_iter);

    IteratorPreOrder<T, A> begin(pre_iter iter) const { return begin(iter); }
    IteratorPreOrder<T, A> cbegin(pre_iter iter) const { return begin(iter); }
    IteratorPreOrder<T, A> end(pre_iter iter) const { return end(iter); }
    IteratorPreOrder<T, A> cend(pre_iter iter) const { return end(iter); }
    IteratorPreOrder<T, A> rbegin(pre_iter iter) { return end(iter); }
    IteratorPreOrder<T, A> rbegin(pre_iter iter) const { return rbegin(iter); }
    IteratorPreOrder<T, A> crbegin(pre_iter iter) const { return rbegin(iter); }
    IteratorPreOrder<T, A> rend(pre_iter iter) { return begin(iter); }
    IteratorPreOrder<T, A> rend(pre_iter iter) const { return rend(iter); }
    IteratorPreOrder<T, A> crend(pre_iter iter) const { return rend(iter); }

    IteratorInOrder<T, A> begin(in_iter);
    IteratorInOrder<T, A> end(in_iter);
    IteratorInOrder<T, A> begin(in_iter iter) const { return begin(iter); }
    IteratorInOrder<T, A> cbegin(in_iter iter) const { return begin(iter); }
    IteratorInOrder<T, A> end(in_iter iter) const { return end(iter); }
    IteratorInOrder<T, A> cend(in_iter iter) const { return end(iter); }
    IteratorInOrder<T, A> rbegin(in_iter iter) { return end(iter); }
    IteratorInOrder<T, A> rbegin(in_iter iter) const { return rbegin(iter); }
    IteratorInOrder<T, A> crbegin(in_iter iter) const { return rbegin(iter); }
    IteratorInOrder<T, A> rend(in_iter iter) { return begin(iter); }
    IteratorInOrder<T, A> rend(in_iter iter) const { return rend(iter); }
    IteratorInOrder<T, A> crend(in_iter iter) const { return rend(iter); }

    IteratorPostOrder<T, A> begin(post_iter);
    IteratorPostOrder<T, A> end(post_iter);
    IteratorPostOrder<T, A> begin(post_iter iter) const { return begin(iter); }
    IteratorPostOrder<T, A> cbegin(post_iter iter) const { return begin(iter); }
    IteratorPostOrder<T, A> end(post_iter iter) const { return end(iter); }
    IteratorPostOrder<T, A> cend(post_iter iter) const { return end(iter); }
    IteratorPostOrder<T, A> rbegin(post_iter iter) { return reverse_iterator(end(iter)); }
    IteratorPostOrder<T, A> rbegin(post_iter iter) const { return reverse_iterator(rbegin(iter)); }
    IteratorPostOrder<T, A> crbegin(post_iter iter) const { return reverse_iterator(rbegin(iter)); }
    IteratorPostOrder<T, A> rend(post_iter iter) { return reverse_iterator(begin(iter)); }
    IteratorPostOrder<T, A> rend(post_iter iter) const { return reverse_iterator(rend(iter)); }
    IteratorPostOrder<T, A> crend(post_iter iter) const { return reverse_iterator(rend(iter)); }

    pointer root_{};
};
