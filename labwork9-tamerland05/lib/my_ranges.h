#include "asserts.h"
#include <typeinfo>

class ranges {
private:
    struct base_fp_ch { auto operator()(auto) { return true; } };
    struct base_fp { auto operator()(auto i) { return i; } };
    struct reverse_fp : base_fp {};

    struct BaseView {};

    typedef typeof(base_fp) fp;
    typedef typeof(base_fp_ch) fp_ch;
    typedef typeof(reverse_fp) rev_fp;
    static inline const BaseView base{};

public:
    template<class Fp>
    static auto transform(Fp) {
        return BaseRange<Fp, fp_ch>();
    }

    template<class Fp_ch>
    static auto filter(Fp_ch) {
        return BaseRange<fp, Fp_ch>();
    }

    static auto take(long long num) {
        return BaseRange<fp, fp_ch>(num);
    }

    static auto drop(long long num) {
        return BaseRange<fp, fp_ch>(-num);
    }

    static auto reverse() {
        return BaseRange<rev_fp, fp_ch>();
    }

    static auto keys() {
        return BaseRange<typeof([](auto i) { return i.first; }), fp_ch>();
    }

    static auto values() {
        return BaseRange<typeof([](auto i) { return i.second; }), fp_ch>();
    }

    template<class Fp, class Fp_ch, class Range=BaseView>
    class BaseRange {
    private:
        const Range& iterable_;

        class BaseIterator {
        private:
            typedef Range::const_iterator iterator;

            iterator pointer_;
            iterator end_pointer_;

        public:
            explicit BaseIterator(const iterator& end_pointer)
                    : pointer_(end_pointer), end_pointer_(end_pointer) {};

            BaseIterator(const iterator& pointer,
                         const iterator& end_pointer)
                         : pointer_(pointer), end_pointer_(end_pointer) {};

            auto operator*() {
                while (pointer_ != end_pointer_ && !Fp_ch()(*pointer_)) {
                    ++pointer_;
                }

                return Fp()(*pointer_);
            }

            BaseIterator& operator++() {
                if (IsReverse()) {
                    --pointer_;
                } else {
                    while (++pointer_ != end_pointer_ && !Fp_ch()(*pointer_)) {}
                }

                return *this;
            }

            BaseIterator& operator--() {
                if (IsReverse()) {
                    ++pointer_;
                } else {
                    while (--pointer_ != end_pointer_ && !Fp_ch()(*pointer_)) {}
                }

                return *this;
            }

            bool operator!=(const BaseIterator& any) const {
                return pointer_ != any.pointer_;
            }
        };

        static bool IsReverse() {
            return typeid(Fp) == typeid(typeof(reverse_fp));
        }

    public:
        typedef BaseIterator const_iterator;
        typedef const_iterator iterator;

        BaseRange(Range& iterable, long long num) : iterable_(iterable), num_(num) {}

        explicit BaseRange(long long num = 0) : iterable_(base), num_(num) {}

        [[nodiscard]] BaseIterator begin() const {
            auto i = iterable_.begin();
            auto end_all = iterable_.end();

            while (i != end_all && !Fp_ch()(*i)) {
                ++i;
            }

            if (IsReverse()) {
                return BaseIterator(--end_all, --i);
            } else if (num_ < 0) {
                long long k = -num_;

                while (k-- && i != end_all) {
                    ++i;
                }
            }

            return BaseIterator(i, end_all);
        }

        [[nodiscard]] BaseIterator end() const {
            auto i = iterable_.begin();

            if (IsReverse()) {
                return BaseIterator(--i);
            } else if (num_ > 0) {
                auto end_all = iterable_.end();
                long long k = num_;

                while (k-- && i != end_all) {
                    ++i;
                }

                if (i != end_all) {
                    ++i;
                }
            } else {
                i = iterable_.end();
            }

            return BaseIterator(i);
        }

        const long long num_;

        auto operator|(const auto& rhs) {
            return BaseRange<Fp, Fp_ch, const BaseRange>(*this, rhs.num_);
        }

        template<class container>
        friend auto operator|(const container& lhs, const BaseRange& rhs) {
            GeneralCheck<container>();

            return BaseRange<Fp, Fp_ch, const container>(lhs, rhs.num_);
        }
    };
};
