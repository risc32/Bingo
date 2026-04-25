#pragma once

#include "../core.cpp"

namespace bingo {
    struct PatternBase {
        virtual ~PatternBase() = default;

        virtual const PatternBuilder& getpattern() = 0;
        virtual bool match(Number&, const detail::Result&) = 0;
    };

    namespace detail {
        std::vector<PatternBase*> patterns;
    }

    void do_reduce(Number& num) {
        if (num.has_childs()) {
            Number& a = *Number::ref(num.pattern.a);
            Number& b = *Number::ref(num.pattern.b);
            do_reduce(a);
            do_reduce(b);

            std::tie(
                num.pattern.a->a,
                num.pattern.a->b,
                num.pattern.a->type,
                num.pattern.a->value,
                num.pattern.a->level) = std::tie(
                    a.pattern.a,
                    a.pattern.b,
                    a.pattern.type,
                    a.pattern.value,
                    a.pattern.level);

            std::tie(
                num.pattern.b->a,
                num.pattern.b->b,
                num.pattern.b->type,
                num.pattern.b->value,
                num.pattern.b->level) = std::tie(
                    b.pattern.a,
                    b.pattern.b,
                    b.pattern.type,
                    b.pattern.value,
                    b.pattern.level);
        }

        bool needreduce = false;
        bool repeat = false;
        do {
            repeat = false;
            for (auto pattern: detail::patterns) {
                std::vector<Number> left = {};
                const detail::OResult&& oresult = num.apply_pattern(pattern->getpattern(), &left);
                bool success = static_cast<bool>(oresult);
                if (success) {
                    if (left.size()) {
                        detail::CommutativeNumber comm;
                        comm.type = num.pattern.type;
                        comm.operands = left;
                        comm.operands.push_back({});
                        success = pattern->match(comm.operands.back(), oresult.value());
                        if (success) {
                            num = comm.getmany(comm.operands.begin(), comm.operands.end());
                            //num.pattern.type = comm.type;
                        }
                    } else {
                        success = pattern->match(num, oresult.value());
                    }
                }
                repeat = repeat || success;
            }
            needreduce = needreduce || repeat;
        } while (repeat);

        if (needreduce) {
            do_reduce(num);
        }
    }

    bool isconst(const Number& num) {
        return num.pattern.type == detail::OperationType::Constant;
    }
}

#include "basic.cpp"
#include "opt.cpp"
#include "overflow.cpp"