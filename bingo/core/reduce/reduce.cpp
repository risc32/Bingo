#pragma once

#include "../core.cpp"

namespace bingo {
    struct PatternBase {
        virtual ~PatternBase() = default;
        virtual bool match(Number&) = 0;
    };

    namespace detail {
        std::vector<PatternBase*> patterns;
    }

    void do_reduce(Number& num) {
        if (num.has_childs()) {
            do_reduce(*Number::ref(num.pattern.a));
            do_reduce(*Number::ref(num.pattern.b));
        }

        bool repeat = false;
        do {
            repeat = false;
            for (auto pattern: detail::patterns) {
                repeat = repeat || pattern->match(num);
            }
        } while (repeat);
    }
}

#include "basic.cpp"
