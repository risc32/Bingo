#pragma once

#include "reduce.cpp"

namespace bingo::reduce {
    using namespace vars;



    struct PowerAdd : PatternBase {
        bool match(Number& num) override {
            auto res = num.apply_pattern((var(3) ^ var(1)) * (var(0) ^ var(2)));
            if (!res) return false;
            num = res.value()[0] ^ (res.value()[1] + res.value()[2]);
            return true;
        }
    };

}
