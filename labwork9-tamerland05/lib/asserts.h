#define GetChecker(CheckerName, ret_type, operator)                                  \
template<class T>                                                                    \
struct CheckerName {                                                                 \
    typedef ret_type(T::*P)();                                                       \
                                                                                     \
    template<class U, P = &U::operator>                                              \
    struct True{ int dummy; };                                                       \
                                                                                     \
    static void detect(...) {};                                                      \
                                                                                     \
    template<class U>                                                                \
    static True<U> detect(U*) { return True<U>{}; };                                 \
                                                                                     \
    static const bool exists = (sizeof(bool) != sizeof(detect(static_cast<T*>(0)))); \
};

char p1[1]; auto& pp1 = p1; char p2[2]; auto& pp2 = p2;

#define GetIterChecker(CheckerName, iter_type)                                  \
template<typename T> struct CheckerName {                                       \
    struct Fallback { bool iter_type; };                                        \
    struct Derived : T, Fallback { };                                           \
                                                                                \
    template<typename C, C> struct ChT;                                         \
                                                                                \
    template<typename C>                                                        \
    static char (&f(ChT<bool Fallback::*, &C::iter_type>*))[1] { return pp1; }; \
                                                                                \
    template<typename C>                                                        \
    static char (&f(...))[2] { return pp2; };                                   \
                                                                                \
    static bool const value = sizeof(f<Derived>(0)) == 2;                       \
};

GetChecker(CheckBegin, T::iterator, begin)
GetChecker(CheckEnd, T::iterator, end)
GetChecker(CheckPP, T&, operator++)
GetChecker(CheckMM, T&, operator--)
GetIterChecker(CheckIterator, iterator)
GetIterChecker(CheckConstIterator, const_iterator)

template<class container>
void GeneralCheck() {
    static_assert(CheckIterator<container>::value, "Container hasn't variable iterator");
    static_assert(CheckConstIterator<container>::value, "Container hasn't variable const_iterator");

    static_assert(CheckBegin<container>::exists, "Don't exists operator begin()");
    static_assert(CheckEnd<container>::exists, "Don't exists operator end()");
    static_assert(CheckPP<typename container::iterator>::exists, "Don't exists operator++()");
    static_assert(CheckMM<typename container::iterator>::exists, "Don't exists operator--()");
}