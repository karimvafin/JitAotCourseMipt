#ifndef VALUE_HPP
#define VALUE_HPP

namespace Compiler {

struct Value {
    int value_;

    bool IsZero() {
        return value_ == 0;
    }

    static Value Add(const Value& v1, const Value& v2) {
        return Value{v1.value_ + v2.value_};
    }

    Value Shr(const Value& v1, size_t shift) {
        return Value{value_ >> shift};
    }

    static bool Or(const Value& v1, const Value& v2) {
        return v1.value_ || v2.value_;
    }

    void SetZero() {
        value_ = 0;
    }
    
    void SetOne() {
        value_ = 1;
    }

    bool GreaterThan(const Value& v) {
        return value_ > v.value_;
    }

    bool Compare(const Value& v) {
        return value_ == v.value_;
    }
};

} // namespace Compiler

#endif // VALUE_HPP