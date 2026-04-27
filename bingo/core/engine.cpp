#pragma once
#include <map>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>

#include "core.cpp"

namespace vars {
    struct variable;
}

namespace bingo {
    struct Number;
    struct PatternBuilder;
    PatternBuilder var(const int64_t number);

    namespace detail {
        constexpr OperationType opvar = static_cast<OperationType>(-1);

        using Result = std::map<int64_t, PatternBuilder>;
        using OResult = std::optional<Result>;

        bool merge(Result& a, const Result& b);
    }

    struct PatternBuilder {
        mutable detail::Number pattern;
        [[nodiscard]] PatternBuilder a() const {
            return {*pattern.a};
        }
        [[nodiscard]] PatternBuilder b() const {
            return {*pattern.b};
        }

        auto operator +(const PatternBuilder& b) const {
            return PatternBuilder{*detail::Number::add(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }

        auto operator -(const PatternBuilder& b) const {
            return PatternBuilder{*detail::Number::sub(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }

        auto operator *(const PatternBuilder& b) const {
            return PatternBuilder{*detail::Number::mul(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }

        auto operator /(const PatternBuilder& b) const {
            return PatternBuilder{*detail::Number::div(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }

        auto operator +() const {
            return *this;
        }

        auto operator -() const {
            return PatternBuilder{*detail::Number::neg(&pattern)};
        }

        auto operator ^(const PatternBuilder& b) const {
            return PatternBuilder{*detail::Number::pow(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }

        PatternBuilder(const int64_t value) : pattern{*detail::Number::constant(value)}{}

        PatternBuilder() = default;

        PatternBuilder(const detail::Number &n) : pattern{n} {}

        // mul(var("x") + 1)
        // mul(add(3, 4)) + 1)

        [[nodiscard]] detail::OResult check(const PatternBuilder &_b, std::vector<Number> *left = {}) const ;
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
        Number(const int value) : Number(static_cast<int64_t>(value)) {}
        Number() : Number(0) {}
        Number(PatternBuilder pb) : PatternBuilder(std::move(pb)) {}

        Number(const vars::variable& v);

        static Number* ref(const detail::Number* num) {
            auto n = new Number{};
            n->pattern.a = num->a;
            n->pattern.b = num->b;
            n->pattern.value = num->value;
            n->pattern.type = num->type;
            n->pattern.sibling = num->sibling;
            n->pattern.level = num->level;
            return n;
        }

        [[nodiscard]] bool has_childs() const {
            return pattern.has_childs();
        }

        // all operators from PatternBuilder, but returns Number

        Number operator +(const Number& b) const {
            return Number{*detail::Number::add(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }
        Number operator -(const Number& b) const {
            return Number{*detail::Number::sub(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }
        Number operator *(const Number& b) const {
            return Number{*detail::Number::mul(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }
        Number operator /(const Number& b) const {
            return Number{*detail::Number::div(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }
        Number operator +() const {
            return *this;
        }
        Number operator -() const {
            return Number{*detail::Number::neg(&pattern)};
        }
        Number operator ^(const Number& b) const {
            return Number{*detail::Number::pow(&pattern, const_cast<detail::Number *>(&b.pattern))};
        }

        [[nodiscard]] detail::OResult apply_pattern(const PatternBuilder& b, std::vector<Number>* const left = nullptr) const {
            return b.check(*this, left);
        }

    private:
        //using PatternBuilder::check;
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
            }

            CommutativeNumber() = default;

            [[nodiscard]] bingo::Number getmany(std::vector<bingo::Number>::iterator begin, const std::vector<bingo::Number>::iterator end) const {
                bingo::Number res = *begin++;
                std::for_each(begin, end, [&](bingo::Number& n) {
                    res.pattern = *Number::create(new Number(res.pattern), &n.pattern, type, true);
                });

                return res;
            }

            void normalize() {
                std::ranges::sort(operands, [&](const bingo::Number& a, const bingo::Number& b) {
                    return a.pattern.type > b.pattern.type;
                });
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

        uint64_t ceil_div(const uint64_t a, const uint64_t d) {
            return (a + d - 1) / d;
        }
    }




    detail::OResult PatternBuilder::check(const PatternBuilder &_b, std::vector<Number>* const left) const {
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

        detail::CommutativeNumber commA{*this};
        detail::CommutativeNumber commB{_b};

        if (commA.type != commB.type || commA.operands.size() > commB.operands.size()) {
            return std::nullopt;
        }

        std::vector<const Number*> aop;
        std::vector<Number>& bop = commB.operands;

        // commA.operands [a, c, b, r, h, e, d]
        // commB.operands [b, a, c, d, e, f, g]
        // to [r, h] and [f, g]
        commA.normalize();
        commB.normalize();
        for (const auto& operand : commA.operands) {
            bool success = false;
            bool found = false;
            for (int i = 0; i < bop.size(); ++i) {
                if (auto cr_res = operand.check(bop[i])) {
                    detail::Result tmp_res = res;
                    if (detail::merge(tmp_res, cr_res.value())) {
                        res = std::move(tmp_res);
                        bop.erase(bop.begin() + i);
                        success = true;
                        break;  // нашли подходящий
                    }
                }
            }
            if (!success) return std::nullopt;
            if (!found) aop.push_back(&operand);
        }
        // if (aop.size() - bop.size()) {
        //     throw std::runtime_error{"Error in library, sorry."};
        // }

        // mul(mul(2, 3), 4)
        // mul(2, 3, 4)

        // x * y :
        // x * y * z
        if (aop.empty()) {
            if (!bop.empty()) {
                if (left) {
                    *left = bop;
                } else {
                    return std::nullopt;
                }
            }
            return res;
        }
        auto varsize = static_cast<int64_t>(detail::ceil_div(bop.size(), aop.size()));
        int i = 0;
        for (auto start = bop.begin(); start != bop.end(); i++) {
            auto end = (std::distance(start, bop.end()) >= varsize)
                       ? std::next(start, varsize)
                       : bop.end();

            auto tmp = aop[i]->check(commB.getmany(start, end));
            if (!tmp) return std::nullopt;
            if (!detail::merge(res, tmp.value())) return std::nullopt;

            start = end;
        }
        // for (size_t i = 0; i < aop.size(); ++i) {
        //     auto tmp = aop[i]->check(bop[i]);
        //     if (!tmp) return std::nullopt;
        //     if (!detail::merge(res, tmp.value())) return std::nullopt;
        // }

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
        if (number == -128) throw InvalidVarNumberException{};
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

bingo::Number operator""_bn(const unsigned long long int value) {
    return bingo::Number{value};
}

bingo::PatternBuilder operator""_vr(const unsigned long long int value) {
    return bingo::var(value);
}

