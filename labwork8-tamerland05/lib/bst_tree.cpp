#include "bst_tree.h"
#include <vector>

template<class T, class A, class C>
BaseBST<T, A, C>::BaseBST(const C& comp) {
    compare_ = comp;
}

template<class T, class A, class C>
BaseBST<T, A, C>::BaseBST(auto i, auto& j, C comp) {
    compare_ = comp;

    while (i != j) {
        this->insert(*i);
    }
}

template<class T, class A, class C>
BaseBST<T, A, C>::BaseBST(const std::initializer_list<T>& il, C comp) {
    compare_ = comp;
    auto pointer = il.begin();

    while (pointer != il.end()) {
        this->insert(*(pointer));
        ++pointer;
    }
}

template<class T, class A, class C>
BaseBST<T, A, C>::BaseBST(const BaseBST& copy) {
    if (this == &copy) {
        return;
    }

    auto iter_beg = copy.begin(pre_iter{});
    auto iter_en = copy.end(pre_iter{});

    while (*iter_beg) {
        this->insert((*(iter_beg++)).key);
    }
}

template<class T, class A, class C>
BaseBST<T, A, C>& BaseBST<T, A, C>::operator=(const BaseBST& other) {
    if (this == &other) {
        return *this;
    }

    this->clear();
    *this = BaseBST<T, A, C>(other);
    return *this;
}

template<class T, class A, class C>
BaseBST<T, A, C>& BaseBST<T, A, C>::operator=(const std::initializer_list<T>& il) {
    *this = BaseBST<T, A, C>(il);
}

template<class T, class A, class C>
bool BaseBST<T, A, C>::operator==(const BaseBST& comp_bst) const {
    return root_ == comp_bst.root_;
}

template<class T, class A, class C>
bool BaseBST<T, A, C>::operator!=(const BaseBST& comp_bst) const {
    return root_ != comp_bst.root_;
}

template<class T, class A, class C>
size_t BaseBST<T, A, C>::erase(key_type key) {
    if (Search(key)) {
        extract(key);
        return 1;
    } else {
        return 0;
    }
}

template<class T, class A, class C>
void BaseBST<T, A, C>::swap(BaseBST& other) {
    std::swap(root_, other.root_);
    std::swap(alloc_, other.alloc_);
}

namespace std {
    template<class T, class A, class C>
    void swap(BaseBST<T, A, C>& lhs, BaseBST<T, A, C>& rhs) {
        lhs.swap(rhs);
    }
}

template<class T, class A, class C>
void BaseBST<T, A, C>::merge(const BaseBST<T, A, C>& merging) {
    if (this == &merging) {
        return;
    }

    auto iter_beg = merging.begin(pre_iter{});
    auto iter_en = merging.end(pre_iter{});

    while (*iter_beg) {
        this->insert((*(iter_beg++)).key);
    }
}


template<class T, class A, class C>
BaseBST<T, A, C>::size_type BaseBST<T, A, C>::size() const {
    return size_;
}

template<class T, class A, class C>
BaseBST<T, A, C>::size_type BaseBST<T, A, C>::max_size() const {
    return alloc_traits_.max_size(alloc_);
}

template<class T, class A, class C>
bool BaseBST<T, A, C>::empty() const {
    return root_;
}

template<class T, class A, class C>
A& BaseBST<T, A, C>::get_allocator() const {
    return alloc_;
}

template<class T, class A, class C>
BaseBST<T, A, C>::pointer BaseBST<T, A, C>::SmartInsert(key_type key, pointer node){
    if (!node) {
        node = alloc_traits_.allocate(alloc_, 1);
        alloc_traits_.construct(alloc_, node, key);
        ++size_;

        auto roo = root_;
        while (!roo->right) {
            roo = roo->right;
        }
        max_ = roo->key;

        roo = root_;
        while (!roo->left) {
            roo = roo->left;
        }
        max_ = roo->key;

        return node;
    }

    if (compare_(key, node->key)) {
        node->left = SmartInsert(key, node->left);
        if (node->left) node->left->parent = node;
    } else if (compare_(node->key, key)) {
        node->right = SmartInsert(key, node->right);
        if (node->right) node->right->parent = node;
    }

    return node;
}

template<class T, class A, class C>
BaseBST<T, A, C>::pointer BaseBST<T, A, C>::SmartSearch(key_type key, pointer node) const {
    if (!node) {
        return nullptr;
    }
    if (compare_(key, node->key)) {
        return SmartSearch(key, node->left);
    } else if (compare_(node->key, key)){
        return SmartSearch(key, node->right);
    } else {
        return node;
    }
}

template<class T, class A, class C>
BaseBST<T, A, C>::pointer BaseBST<T, A, C>::Minimum(pointer node) const {
    if (!node->left) {
        return node;
    }
    return Minimum(node->left);
}

template<class T, class A, class C>
BaseBST<T, A, C>::pointer BaseBST<T, A, C>::SmartDelete(key_type key, pointer node) {
    if (!node) {
        return node;
    }
    if (compare_(key, node->key)) {
        node->left = SmartDelete(key, node->left);
    } else if (compare_(node->key, key)) {
        node->right = SmartDelete(key, node->right);
    } else if (node->left && node->right) {
        node->key = Minimum(node->right)->key;
        node->right = SmartDelete(node->key, node->right);
    } else {
        auto copy = node;

        if (node->left) {
            node = node->left;
        } else if (node->right) {
            node = node->right;
        } else {
            alloc_traits_.deallocate(alloc_, copy, 1);
            return nullptr;
        }

        node->parent = nullptr;
        alloc_traits_.deallocate(alloc_, copy, 1);
        --size_;

        auto roo = root_;
        while (!roo->right) {
            roo = roo->right;
        }
        max_ = roo->key;

        roo = root_;
        while (!roo->left) {
            roo = roo->left;
        }
        max_ = roo->key;
    }

    return node;
}

template<class T, class A, class C>
void BaseBST<T, A, C>::DeletingTree(pointer node) {
    if (node) {
        if (node->right) DeletingTree(node->right);
        if (node->left) DeletingTree(node->left);
        alloc_traits_.deallocate(alloc_, node, 1);
    }
}

template<class T, class A, class C>
IteratorPreOrder<T, A> BaseBST<T, A, C>::begin(pre_iter) {
    return IteratorPreOrder<T, A>(root_);
}

template<class T, class A, class C>
IteratorPreOrder<T, A> BaseBST<T, A, C>::end(pre_iter) {
    auto current = root_;

    while (current->right || current->left) {
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    }

    return IteratorPreOrder<T, A>(current);

}

template<class T, class A, class C>
IteratorInOrder<T, A> BaseBST<T, A, C>::begin(in_iter) {
    pointer current = root_;

    while (current->left) {
        current = current->left;
    }

    return IteratorInOrder<T, A>(current);
}

template<class T, class A, class C>
IteratorInOrder<T, A> BaseBST<T, A, C>::end(in_iter) {
    auto current = root_;

    while (current->right) {
        current = current->right;
    }

    return IteratorInOrder<T, A>(current);

}

template<class T, class A, class C>
IteratorPostOrder<T, A> BaseBST<T, A, C>::begin(post_iter) {
    auto current = root_;

    while (current->left || current->right) {
        if (current->left) {
            current = current->left;
        } else if (current->right) {
            current = current->right;
        }
    }

    return IteratorPostOrder<T, A>(current);
}

template<class T, class A, class C>
IteratorPostOrder<T, A> BaseBST<T, A, C>::end(post_iter) {
    return IteratorPostOrder<T, A>(root_);
}
