#ifndef VALUE_HPP
#define VALUE_HPP

#include <assert.h>

namespace Compiler {

enum class ValueType {
    S32,
    S64,
    U32,
    U64,
    Float,
    Double,
    None
};

template <typename T>
constexpr ValueType getValueType() {
    if constexpr (std::is_same_v<T, int32_t>)
        return ValueType::S32;
    if constexpr (std::is_same_v<T, int64_t>)
        return ValueType::S64;
    if constexpr (std::is_same_v<T, uint32_t>)
        return ValueType::U32;
    if constexpr (std::is_same_v<T, uint64_t>)
        return ValueType::U64;
    if constexpr (std::is_same_v<T, float>)
        return ValueType::Float;
    if constexpr (std::is_same_v<T, double>)
        return ValueType::Double;
    return ValueType::None;
}

struct Value {
private:
    std::variant<int32_t, int64_t, uint32_t, uint64_t, float, double> value_;
    ValueType type_;

public:
    template <typename T>
    Value(T value) : value_(value), type_(getValueType<T>()) {} 

    Value(Value* value) : value_(value->value_), type_(value->type_) {}

    template <typename T>
    T get() const {
        return std::get<T>(value_);
    }

    template <typename T>
    T& get() {
        return std::get<T>(value_);
    }

    void setZero() {
        switch (type_) {
        case ValueType::S32:
            get<int32_t>() = int32_t(0);
            return;
        case ValueType::S64:
            get<int64_t>() = int64_t(0);
            return;
        case ValueType::U32:
            get<uint32_t>() = uint32_t(0);
            return;
        case ValueType::U64:
            get<uint64_t>() = uint64_t(0);
            return;
        case ValueType::Float:
            get<float>() = float(0);
            return;
        case ValueType::Double:
            get<double>() = double(0);
            return;
        default:
            return;
        }
    }

    void setOne() {
        switch (type_) {
        case ValueType::S32:
            get<int32_t>() = int32_t(1);
            return;
        case ValueType::S64:
            get<int64_t>() = int64_t(1);
            return;
        case ValueType::U32:
            get<uint32_t>() = uint32_t(1);
            return;
        case ValueType::U64:
            get<uint64_t>() = uint64_t(1);
            return;
        case ValueType::Float:
            get<float>() = float(1);
            return;
        case ValueType::Double:
            get<double>() = double(1);
            return;
        default:
            return;
        }
    }

    bool isZero() {
        switch (type_) {
        case ValueType::S32:
            return get<int32_t>() == int32_t(0);
        case ValueType::S64:
            return get<int64_t>() == int64_t(0);
        case ValueType::U32:
            return get<uint32_t>() == uint32_t(0);
        case ValueType::U64:
            return get<uint64_t>() == uint64_t(0);
        case ValueType::Float:
            return get<float>() == float(0);
        case ValueType::Double:
            return get<double>() == double(0);
        default:
            return false;
        }
    }

    bool greaterThan(Value* val) {
        assert(type_ == val->type_);
        switch (type_) {
        case ValueType::S32:
            return get<int32_t>() > val->get<int32_t>();
        case ValueType::S64:
            return get<int64_t>() > val->get<int64_t>();
        case ValueType::U32:
            return get<uint32_t>() > val->get<uint32_t>();
        case ValueType::U64:
            return get<uint64_t>() > val->get<uint64_t>();
        case ValueType::Float:
            return get<float>() > val->get<float>();
        case ValueType::Double:
            return get<double>() > val->get<double>();
        default:
            return false;
        }
    }

    bool greaterThan(unsigned int val) {
        switch (type_) {
        case ValueType::S32:
            return get<int32_t>() > val;
        case ValueType::S64:
            return get<int64_t>() > val;
        case ValueType::U32:
            return get<uint32_t>() > val;
        case ValueType::U64:
            return get<uint64_t>() > val;
        case ValueType::Float:
            return get<float>() > val;
        case ValueType::Double:
            return get<double>() > val;
        default:
            return false;
        }
    }

    bool compare(Value* val) {
        assert(type_ == val->type_);
        switch (type_) {
        case ValueType::S32:
            return get<int32_t>() == val->get<int32_t>();
        case ValueType::S64:
            return get<int64_t>() == val->get<int64_t>();
        case ValueType::U32:
            return get<uint32_t>() == val->get<uint32_t>();
        case ValueType::U64:
            return get<uint64_t>() == val->get<uint64_t>();
        default:
            return false;
        }
    }

};

} // namespace Compiler

#endif // VALUE_HPP