#pragma once
#include <cstdint>
#include <string>

namespace bingo::detail {
    enum struct OperationType {
        Add,
        Multiply,
        Power,
        Constant
    };

    struct Number {
        OperationType type{};

        Number* a{};
        Number* b{};
        int64_t value{};
        Number* sibling{};
        uint64_t level{};

        bool operator==(const Number& another) const {
            if (type != another.type) {
                return false;
            }
            if (!has_childs() && !another.has_childs()) {
                return value == another.value;
            }

            return *this->a == *another.a and *this->b == *another.b;
        }



        static Number* constant(const int64_t value) {
            const auto n = new Number();
            n->type = OperationType::Constant;
            n->a = nullptr;
            n->b = nullptr;
            n->value = value;

            return n;
        }

        static Number* opvar(int64_t value);

        static Number* create(Number* a, Number* b, const OperationType type, const bool commutativity = false) {
            const auto n = new Number();
            n->type = type;

            n->a = a;
            n->b = b;

            n->level = n->a->level + 1;

            n->a->sibling = n->b;
            n->b->sibling = n->a;

            return n;
        }

        static Number* add(Number* a, Number* b) {
            return create(a, b, OperationType::Add, true);
        }

        static Number* sub(Number* a, Number* b) {
            return add(a, neg(b));
        }

        static Number* neg(Number* a) {
            return mul(constant(-1), a);
        }

        static Number* mul(Number* a, Number* b) {
            return create(a, b, OperationType::Multiply, true);
        }

        static Number* div(Number* a, Number* b) {
            return mul(a, pow(b, constant(-1)));
        }

        static Number* pow(Number* a, Number* b) {
            return create(a, b, OperationType::Power);
        }

        Number(const Number& num) {
            *this = num;
        }

        Number& operator=(Number&& other) noexcept {
            if (other.has_childs()) {
                type = other.type;
                a = new Number(*other.a);
                b = new Number(*other.b);
            } else {
                value = other.value;
            }
            type = other.type;
            other.~Number();
            return *this;
        }

        [[nodiscard]] bool has_childs() const {
            return type != OperationType::Constant and type != static_cast<OperationType>(-1);
        }

        Number& operator=(const Number& other) {
            if (other.has_childs()) {
                type = other.type;
                a = new Number(*other.a);
                b = new Number(*other.b);
            } else {
                value = other.value;
            }
            type = other.type;
            return *this;
        }

        ~Number() {
            if (has_childs()) {
                //delete a;
                //delete b;
            }
        }

        Number() = default;


        bool operator<(const Number& other) const {
            const auto scmp = std::tie(type, value) < std::tie(other.type, other.value);
            if (std::tie(type, value) != std::tie(other.type, other.value)) return scmp;

            auto compare_ptr = [](const Number* p1, const Number* p2) -> bool {
                if (!p1 && !p2) return false;
                if (!p1) return true;
                if (!p2) return false;
                return *p1 < *p2;
            };

            if (compare_ptr(a, other.a)) return true;
            if (compare_ptr(other.a, a)) return false;

            return compare_ptr(b, other.b);
        }
    };
}

#include "engine.cpp"
#include "variable.cpp"
#include "reduce/reduce.cpp"
