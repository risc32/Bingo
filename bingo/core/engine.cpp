#pragma once
#include <map>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>
#include <algorithm>

#include "core.cpp"

namespace bingo {
    struct PatternBuilder;
    PatternBuilder var(int64_t number);


    struct PatternBase {
        virtual ~PatternBase() = default;

        virtual std::optional<PatternBuilder> match(PatternBuilder);
    };

    namespace detail {
        constexpr OperationType opvar = static_cast<OperationType>(-1);

        using Result = std::map<int64_t, PatternBuilder>;
        using OResult = std::optional<Result>;

        bool merge(Result& a, const Result& b);
    }

    struct PatternBuilder {
        detail::Number pattern;
        [[nodiscard]] PatternBuilder a() const {
            return {*pattern.a};
        }
        [[nodiscard]] PatternBuilder b() const {
            return {*pattern.b};
        }

        auto operator +(PatternBuilder b)  {
            return PatternBuilder{*detail::Number::add(&pattern, &b.pattern)};
        }

        auto operator -(PatternBuilder b) {
            return PatternBuilder{*detail::Number::sub(&pattern, &b.pattern)};
        }

        auto operator *(PatternBuilder b) {
            return PatternBuilder{*detail::Number::mul(&pattern, &b.pattern)};
        }

        auto operator /(PatternBuilder b) {
            return PatternBuilder{*detail::Number::div(&pattern, &b.pattern)};
        }

        auto operator +() const {
            return *this;
        }

        auto operator -() {
            return PatternBuilder{*detail::Number::neg(&pattern)};
        }

        auto operator ^(PatternBuilder b) {
            return PatternBuilder{*detail::Number::pow(&pattern, &b.pattern)};
        }

        PatternBuilder(int64_t value) : pattern{*detail::Number::constant(value)}{}

        PatternBuilder() = default;

        PatternBuilder(const detail::Number &n) : pattern{n} {}

        // mul(var("x") + 1)
        // mul(add(3, 4)) + 1)

        [[nodiscard]] detail::OResult check(const PatternBuilder& _b) const ;
        // {
        //     detail::Result res;
        //
        //     if (pattern.type == detail::opvar) {
        //         res[pattern.value] = _b;
        //     } else if (alltp(_b, detail::OperationType::Constant)) {
        //         return _b.pattern.value == pattern.value ? detail::OResult(res) : std::nullopt;
        //     } else {
        //         if (_b.pattern.type != pattern.type) {
        //             return std::nullopt;
        //         }
        //
        //         auto res_a = a().check(_b.a());
        //         auto res_b = b().check(_b.b());
        //         if (!res_a || !res_b) return std::nullopt;
        //         if (!detail::merge(res, res_a.value())) return std::nullopt;
        //         if (!detail::merge(res, res_b.value())) return std::nullopt;
        //     }
        //
        //     return res;
        // }

    private:
        [[nodiscard]] bool alltp(const PatternBuilder& b, const detail::OperationType t) const {
            return b.pattern.type == t and pattern.type == t;
        }
    };

    struct Number : PatternBuilder {
        [[nodiscard]] Number a() const {
            return PatternBuilder::a();
        };
        [[nodiscard]] Number b() const {
            return PatternBuilder::b();
        };

        template<typename T> requires
    std::is_arithmetic_v<T> Number(const T value) : PatternBuilder{*detail::Number::constant(value)} {
            //a = new Number(PatternBuilder::a());
        }
        Number(int value) : Number(static_cast<int64_t>(value)) {}
        Number() : Number(0) {}
        Number(PatternBuilder pb) : PatternBuilder(std::move(pb)) {}

        [[nodiscard]] bool has_childs() const {
            return pattern.has_childs();
        }

    private:
        //PatternBuilder a();
        //PatternBuilder b();
    };

    namespace detail {
        bool isCommutative(const OperationType type) {
            return type == OperationType::Add or type == OperationType::Multiply;
        }

        struct CommutativeNumber {
            std::vector<bingo::Number> operands{};
            OperationType type;

            // mul(mul(2, 3), 4)
            // c_mul(2, 3, 4)
            explicit CommutativeNumber(const bingo::Number &num)
                    : type(num.pattern.type)
            {
                collect(num);
                //normalize();
            }

        private:
            void collect(const bingo::Number &num) {
                if (!num.has_childs()) {
                    operands.push_back(num);
                    return;
                }

                if (num.pattern.type == type && isCommutative(type)) {
                    collect(num.a());
                    collect(num.b());
                } else {
                    operands.push_back(num);
                }
            }

            void normalize() {
                std::ranges::sort(operands, [&](const bingo::Number& a, const bingo::Number& b) {
                    return a.pattern < b.pattern;
                });
            }
        };

        Number *Number::opvar(const int64_t value) {
            const auto res = constant(value);
            res->type = detail::opvar;
            return res;
        }

        bool merge(Result &a, const Result &b) {
            for (const auto&[k, v] : b) {
                if (a.contains(k)) {
                    if (a[k].pattern != v.pattern) {
                        return false;
                    }
                } else {
                    a[k] = v;
                }
            }

            return true;
        }
    }

    detail::OResult PatternBuilder::check(const PatternBuilder &_b) const {
        detail::Result res;

        if (pattern.type == detail::opvar) {
            if (pattern.value == -128) return res;
            res[pattern.value] = _b;
            return res;
        }


        if (pattern.type == detail::OperationType::Constant) {
            return _b.pattern.value == pattern.value ? detail::OResult(res) : std::nullopt;
        }

        if (!detail::isCommutative(pattern.type)) {
            if (_b.pattern.type != pattern.type) {
                return std::nullopt;
            }

            auto res_a = a().check(_b.a());
            auto res_b = b().check(_b.b());
            if (!res_a || !res_b) return std::nullopt;
            if (!detail::merge(res, res_a.value())) return std::nullopt;
            if (!detail::merge(res, res_b.value())) return std::nullopt;
            return res;
        }

        const detail::CommutativeNumber commA{*this};
        detail::CommutativeNumber commB{_b};

        if (commA.type != commB.type || commA.operands.size() != commB.operands.size()) {
            return std::nullopt;
        }

        std::vector<const Number*> aop;
        std::vector<Number>& bop = commB.operands;

        // commA.operands [a, c, b, r, h, e, d]
        // commB.operands [b, a, c, d, e, f, g]
        // to [r, h] and [f, g]

        for (const auto& operand : commA.operands) {
            bool found = false;
            for (int i = 0; i < bop.size(); ++i) {
                if (operand.pattern == bop[i].pattern) {
                    found = true;
                    bop.erase(bop.begin() + i);
                    break;
                }
            }
            if (!found) aop.push_back(&operand);
        }
        if (aop.size() - bop.size()) {
            throw std::runtime_error{"Error in library, sorry."};
        }

        for (size_t i = 0; i < aop.size(); ++i) {
            auto tmp = aop[i]->check(bop[i]);
            if (!tmp) return std::nullopt;
            if (!detail::merge(res, tmp.value())) return std::nullopt;
        }

        //     for (const auto& c : to_check) {
        //         if (auto tmp = c.first.check(c.second)) {
        //             if (!detail::merge(res, tmp.value())) {
        //                 break;
        //             }
        //         } else {
        //             break;
        //         }
        //     }
        // }

        // for (size_t i = 0; i < commA.operands.size(); ++i) {
        //     auto tmp = commA.operands[i].check(commB.operands[i]);
        //     if (!tmp) return std::nullopt;
        //     if (!detail::merge(res, tmp.value())) return std::nullopt;
        // }

        return res;
    }

    struct InvalidVarNumberException : std::exception{};

    PatternBuilder var(const int64_t number) {
        if (number == 128) throw InvalidVarNumberException{};
        return PatternBuilder{
        *detail::Number::opvar(number)
        };
    }

    auto any() {
        return PatternBuilder{
            *detail::Number::opvar(-128)
            };
    }
}

bingo::Number operator""_bn(unsigned long long int value) {
    return bingo::Number{value};
}

bingo::PatternBuilder operator""_vr(unsigned long long int value) {
    return bingo::var(value);
}

