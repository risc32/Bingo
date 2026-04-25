#pragma once

#include "reduce.cpp"

namespace bingo::reduce {
    using namespace vars;

    struct PowerAdd : PatternBase {
        PatternBuilder pattern = (x ^ a) * (x ^ b);
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = x(res) ^ (a(res) + b(res));
            return true;
        }
    };

    struct PowerSub : PatternBase {
        PatternBuilder pattern = (x ^ a) / (x ^ b);
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = x(res) ^ (a(res) - b(res));
            return true;
        }
    };

    struct Square : PatternBase {
        PatternBuilder pattern = x * x;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = x(res) ^ 2;
            return true;
        }
    };

    struct AddMul : PatternBase {
        PatternBuilder pattern = x * y + x * z;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = x(res) * (y(res) + z(res));
            return true;
        }
    };

    struct SubMul : PatternBase {
        PatternBuilder pattern = x * y - x * z;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = x(res) * (y(res) - z(res));
            return true;
        }
    };

    struct DivSelf : PatternBase {
        PatternBuilder pattern = x / x;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = 1;
            return true;
        }
    };
}