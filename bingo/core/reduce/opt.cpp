#pragma once


#include "overflow.cpp"

namespace bingo::reduce {
    struct ConstMul : PatternBase {
        PatternBuilder pattern = a * b;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            if (!isconst(a(res)) || !isconst(b(res))) return false;

            int64_t a_val = a(res).pattern.value;
            int64_t b_val = b(res).pattern.value;

            if (detail::w_overflow_mul(a_val, b_val)) {
                return false;
            }

            num = Number(a_val * b_val);
            return true;
        }
    };

    struct ConstDiv : PatternBase {
        PatternBuilder pattern = a / b;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            if (!isconst(a(res)) || !isconst(b(res))) return false;

            int64_t a_val = a(res).pattern.value;
            int64_t b_val = b(res).pattern.value;

            if (detail::w_overflow_mul(a_val, b_val)) {
                return false;
            }

            int64_t r = a_val / b_val;
            if (r * b_val != a_val) return false;
            num = Number(r);
            return true;
        }
    };

    struct ConstPow : PatternBase {
        PatternBuilder pattern = a ^ b;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            if (!isconst(a(res)) || !isconst(b(res))) return false;

            int64_t base = a(res).pattern.value;
            int64_t exp = b(res).pattern.value;

            if (exp < 0) return false;  // Отрицательная степень

            if (detail::w_overflow_pow(base, exp)) {
                return false;
            }

            num = Number(static_cast<int64_t>(std::pow(base, exp)));
            return true;
        }
    };

    struct ConstAdd : PatternBase {
        PatternBuilder pattern = a + b;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            if (!isconst(a(res)) || !isconst(b(res))) return false;

            int64_t a_val = a(res).pattern.value;
            int64_t b_val = b(res).pattern.value;

            if (detail::w_overflow_add(a_val, b_val)) {
                return false;
            }

            num = Number(a_val + b_val);
            return true;
        }
    };
}
