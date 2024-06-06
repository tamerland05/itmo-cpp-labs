#include "iterators.h"

template<class T, class A>
BaseIteratorTraversal<T, A>::BaseIteratorTraversal(const BaseIteratorTraversal& copy) : pointer_(copy.pointer_) {}

template<class T, class A>
BaseIteratorTraversal<T, A>& BaseIteratorTraversal<T, A>::operator=(const BaseIteratorTraversal& other) {
    if (this == &other) {
        return *this;
    }

    this->pointer_ = other.pointer_;

    return *this;
}

template<class T, class A>
bool BaseIteratorTraversal<T, A>::operator==(const BaseIteratorTraversal& comp_iter) const {
    return pointer_ == comp_iter.pointer_;
}

template<class T, class A>
bool BaseIteratorTraversal<T, A>::operator!=(const BaseIteratorTraversal& comp_iter) const {
    return pointer_->key != comp_iter.pointer_->key;
}

template<class T, class A>
BaseIteratorTraversal<T, A>::reference BaseIteratorTraversal<T, A>::operator*() const {
    return *pointer_;
}

template<class C>
C operator++(C& iter, int) {
    C copy = iter;
    ++iter;

    return copy;
}

template<class C>
C operator--(C& iter, int) {
    C copy = iter;
    --iter;

    return copy;
}

template<class T, class A>
IteratorPreOrder<T, A>& IteratorPreOrder<T, A>::operator++() {
    auto& current = this->pointer_;

    if (!current) {
        std::cerr << "end of three";
        throw;
    } else if (current->left) {
        current = current->left;
    } else if (current->right) {
        current = current->right;
    } else if (current->parent) {
        auto parent = current->parent;

        while (parent && (!parent->right || parent->right == current)) {
            current = parent;
            parent = current->parent;
        }

        if (parent) {
            current = parent->right;
        } else {
            current = nullptr;
        }
    } else {
        current = nullptr;
    }

    return *this;
}

template<class T, class A>
IteratorPreOrder<T, A>& IteratorPreOrder<T, A>::operator--() {
    auto& current = this->pointer_;

    if (!current) {
        std::cerr << "end of three";
        throw;
    } else if (current->parent) {
        if (current == current->parent->left || !current->parent->left ) {
            current = current->parent;
        } else {
            current = current->parent->left;
            while (current->left || current->right) {
                if (current->right) {
                    current = current->right;
                } else {
                    current = current->left;
                }
            }
        }
    } else {
        current = nullptr;
    }

    return *this;
}

template<class T, class A>
IteratorInOrder<T, A>& IteratorInOrder<T, A>::operator++() {
    auto& current = this->pointer_;

    if (!current) {
        std::cerr << "end of three";
        throw;
    } else if (!current->right) {
        if (!current->parent) {
            current = nullptr;
        } else if (current->parent->left == current) {
            current = current->parent;
        } else {
            current = current->parent;
            while (current->parent && current->parent->left != current) {
                current = current->parent;
            }

            if (current->parent) {
                current = current->parent;
            } else if (current->right) {
                current = current->right;
            } else {
                current = nullptr;
            }
        }
    } else {
        current = current->right;
        while (current->left) {
            current = current->left;
        }
    }

    return *this;
}

template<class T, class A>
IteratorInOrder<T, A>& IteratorInOrder<T, A>::operator--() {
    auto& current = this->pointer_;

    if (!current) {
        std::cerr << "end of three";
        throw;
    } else if (!current->left) {
        while (current->parent && current->parent->right != current) {
            current = current->parent;
        }
        if (current->parent) {
            current = current->parent;
        } else {
            current = nullptr;
        }
    } else {
        current = current->left;
        while (current->right) {
            current = current->right;
        }
    }

    return *this;
}

template<class T, class A>
IteratorPostOrder<T, A>& IteratorPostOrder<T, A>::operator++() {
    auto& current = this->pointer_;

    if (!current->parent) {
        std::cerr << "end of tree";
    } else if (((current->parent->right
        && (!((current->left || current->right) || current == current->parent->right) || current->parent->left))) &&
        current->parent->right && current != current->parent->right) {
        current = current->parent->right;
        while (current->right || current->left) {
            if (current->left) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
    } else {
        current = current->parent;
    }

    return *this;
}

template<class T, class A>
IteratorPostOrder<T, A>& IteratorPostOrder<T, A>::operator--() {
    auto& current = this->pointer_;

    if (!current) {
        std::cerr << "end of tree";
    } else if (current->right) {
        current = current->right;
    } else if (current->left) {
        current = current->left;
    } else {
        while (!current->parent->left && current->parent->right == current || current->parent->left == current) {
            current = current->parent;
        }
        current = current->parent->left;
    }

    return *this;
}
