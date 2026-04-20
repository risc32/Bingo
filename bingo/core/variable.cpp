#pragma once

#include "core.cpp"

namespace vars {
    union __attribute__((packed)) variable {
        struct {
            char letter;
            uint64_t number : 7*8;
        } __attribute__((packed));
        int64_t raw;
    };

    bingo::PatternBuilder a(const uint64_t num) {
        return bingo::var(variable{'a', num}.raw);
    }
    bingo::PatternBuilder b(const uint64_t num) {
        return bingo::var(variable{'b', num}.raw);
    }
    bingo::PatternBuilder c(const uint64_t num) {
        return bingo::var(variable{'c', num}.raw);
    }
    bingo::PatternBuilder d(const uint64_t num) {
        return bingo::var(variable{'d', num}.raw);
    }
    bingo::PatternBuilder e(const uint64_t num) {
        return bingo::var(variable{'e', num}.raw);
    }
    bingo::PatternBuilder f(const uint64_t num) {
        return bingo::var(variable{'f', num}.raw);
    }
    bingo::PatternBuilder g(const uint64_t num) {
        return bingo::var(variable{'g', num}.raw);
    }
    bingo::PatternBuilder h(const uint64_t num) {
        return bingo::var(variable{'h', num}.raw);
    }
    bingo::PatternBuilder i(const uint64_t num) {
        return bingo::var(variable{'i', num}.raw);
    }
    bingo::PatternBuilder j(const uint64_t num) {
        return bingo::var(variable{'j', num}.raw);
    }
    bingo::PatternBuilder k(const uint64_t num) {
        return bingo::var(variable{'k', num}.raw);
    }
    bingo::PatternBuilder l(const uint64_t num) {
        return bingo::var(variable{'l', num}.raw);
    }
    bingo::PatternBuilder m(const uint64_t num) {
        return bingo::var(variable{'m', num}.raw);
    }
    bingo::PatternBuilder n(const uint64_t num) {
        return bingo::var(variable{'n', num}.raw);
    }
    bingo::PatternBuilder o(const uint64_t num) {
        return bingo::var(variable{'o', num}.raw);
    }
    bingo::PatternBuilder p(const uint64_t num) {
        return bingo::var(variable{'p', num}.raw);
    }
    bingo::PatternBuilder q(const uint64_t num) {
        return bingo::var(variable{'q', num}.raw);
    }
    bingo::PatternBuilder r(const uint64_t num) {
        return bingo::var(variable{'r', num}.raw);
    }
    bingo::PatternBuilder s(const uint64_t num) {
        return bingo::var(variable{'s', num}.raw);
    }
    bingo::PatternBuilder t(const uint64_t num) {
        return bingo::var(variable{'t', num}.raw);
    }
    bingo::PatternBuilder u(const uint64_t num) {
        return bingo::var(variable{'u', num}.raw);
    }
    bingo::PatternBuilder v(const uint64_t num) {
        return bingo::var(variable{'v', num}.raw);
    }
    bingo::PatternBuilder w(const uint64_t num) {
        return bingo::var(variable{'w', num}.raw);
    }
    bingo::PatternBuilder x(const uint64_t num) {
        return bingo::var(variable{'x', num}.raw);
    }
    bingo::PatternBuilder y(const uint64_t num) {
        return bingo::var(variable{'y', num}.raw);
    }
    bingo::PatternBuilder z(const uint64_t num) {
        return bingo::var(variable{'z', num}.raw);
    }

    bingo::PatternBuilder A(const uint64_t num) {
        return bingo::var(variable{'A', num}.raw);
    }
    bingo::PatternBuilder B(const uint64_t num) {
        return bingo::var(variable{'B', num}.raw);
    }
    bingo::PatternBuilder C(const uint64_t num) {
        return bingo::var(variable{'C', num}.raw);
    }
    bingo::PatternBuilder D(const uint64_t num) {
        return bingo::var(variable{'D', num}.raw);
    }
    bingo::PatternBuilder E(const uint64_t num) {
        return bingo::var(variable{'E', num}.raw);
    }
    bingo::PatternBuilder F(const uint64_t num) {
        return bingo::var(variable{'F', num}.raw);
    }
    bingo::PatternBuilder G(const uint64_t num) {
        return bingo::var(variable{'G', num}.raw);
    }
    bingo::PatternBuilder H(const uint64_t num) {
        return bingo::var(variable{'H', num}.raw);
    }
    bingo::PatternBuilder I(const uint64_t num) {
        return bingo::var(variable{'I', num}.raw);
    }
    bingo::PatternBuilder J(const uint64_t num) {
        return bingo::var(variable{'J', num}.raw);
    }
    bingo::PatternBuilder K(const uint64_t num) {
        return bingo::var(variable{'K', num}.raw);
    }
    bingo::PatternBuilder L(const uint64_t num) {
        return bingo::var(variable{'L', num}.raw);
    }
    bingo::PatternBuilder M(const uint64_t num) {
        return bingo::var(variable{'M', num}.raw);
    }
    bingo::PatternBuilder N(const uint64_t num) {
        return bingo::var(variable{'N', num}.raw);
    }
    bingo::PatternBuilder O(const uint64_t num) {
        return bingo::var(variable{'O', num}.raw);
    }
    bingo::PatternBuilder P(const uint64_t num) {
        return bingo::var(variable{'P', num}.raw);
    }
    bingo::PatternBuilder Q(const uint64_t num) {
        return bingo::var(variable{'Q', num}.raw);
    }
    bingo::PatternBuilder R(const uint64_t num) {
        return bingo::var(variable{'R', num}.raw);
    }
    bingo::PatternBuilder S(const uint64_t num) {
        return bingo::var(variable{'S', num}.raw);
    }
    bingo::PatternBuilder T(const uint64_t num) {
        return bingo::var(variable{'T', num}.raw);
    }
    bingo::PatternBuilder U(const uint64_t num) {
        return bingo::var(variable{'U', num}.raw);
    }
    bingo::PatternBuilder V(const uint64_t num) {
        return bingo::var(variable{'V', num}.raw);
    }
    bingo::PatternBuilder W(const uint64_t num) {
        return bingo::var(variable{'W', num}.raw);
    }
    bingo::PatternBuilder X(const uint64_t num) {
        return bingo::var(variable{'X', num}.raw);
    }
    bingo::PatternBuilder Y(const uint64_t num) {
        return bingo::var(variable{'Y', num}.raw);
    }
    bingo::PatternBuilder Z(const uint64_t num) {
        return bingo::var(variable{'Z', num}.raw);
    }
}
