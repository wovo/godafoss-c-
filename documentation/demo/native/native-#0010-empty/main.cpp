template<unsigned N>
struct FixedString {
    char buf[N + 1]{};
    constexpr FixedString(char const* s) {
        for (unsigned i = 0; i != N; ++i) buf[i] = s[i];
    }
    constexpr operator char const*() const { return buf; }
};
template<unsigned N> FixedString(char const (&)[N]) -> FixedString<N - 1>;

template<FixedString T>
class Foo {
    static constexpr char const* Name = T;
public:
    void hello() const;
};

int main() {
    Foo<"Hello!"> foo;
    foo.hello();
}
