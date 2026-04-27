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

    struct PowerPower : PatternBase {
        PatternBuilder pattern = (x ^ a) ^ b;
        const PatternBuilder& getpattern() override {return pattern;}
        bool match(Number& num, const detail::Result& res) override {
            num = x(res) ^ (a(res) * b(res));
            return true;
        }
    };

    struct PowerInc : PatternBase {
        PatternBuilder pattern = (x ^ c) * x;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = x(res) ^ (c(res) + 1);
            return true;
        }
    };

    struct PowerMul : PatternBase {
        PatternBuilder pattern = x ^ a ^ b;
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = x(res) ^ (a(res) * b(res));
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

    struct Div : PatternBase {
        PatternBuilder pattern = x * (z / x);
        const PatternBuilder& getpattern() override {return pattern;}

        bool match(Number& num, const detail::Result& res) override {
            num = z(res);
            return true;
        }
    };
}