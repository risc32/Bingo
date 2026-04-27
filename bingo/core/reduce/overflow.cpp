#pragma once

#include "reduce.cpp"

namespace bingo::detail {
    bool w_overflow_mul(const int64_t a, const int64_t b) {
        if (a == 0 || b == 0) return false;
        if (a > 0 && b > 0) return a > INT64_MAX / b;
        if (a > 0 && b < 0) return b < INT64_MIN / a;
        if (a < 0 && b > 0) return a < INT64_MIN / b;
        if (a < -1 && b < -1) {
            if (a < INT64_MAX / b) return true;
        }
        return false;
    }

    bool w_overflow_add(const int64_t a, const int64_t b) {
        if (a > 0 && b > 0) return a > INT64_MAX - b;
        if (a < 0 && b < 0) return a < INT64_MIN - b;
        return false;
    }

    bool w_overflow_pow(const int64_t base, const int64_t exp) {
        if (exp < 0) return true;
        if (base == 0) return false;
        if (base == 1) return false;
        if (base == -1) return false;

        uint64_t abs_base = std::abs(base);
        uint64_t result = 1;

        for (int64_t i = 0; i < exp; i++) {
            if (result > INT64_MAX / abs_base) return true;
            result *= abs_base;
        }

        if (base < 0 && exp % 2 == 0) {
            return result > INT64_MAX;
        }

        return result > INT64_MAX;
    }

    bool w_overflow(const int64_t a, const int64_t b, const OperationType op) {
        switch (op) {
            case OperationType::Add:      return w_overflow_add(a, b);
            case OperationType::Multiply: return w_overflow_mul(a, b);
            case OperationType::Power:    return w_overflow_pow(a, b);
            default:                      return false;
        }
    }

    bool can_fold(const int64_t a, const int64_t b, const OperationType op) {
        switch (op) {
            case OperationType::Add:      return !w_overflow_add(a, b);
            case OperationType::Multiply: return !w_overflow_mul(a, b);
            case OperationType::Power:    return !w_overflow_pow(a, b);
            default:                      return false;
        }
    }

    bool safe_apply(const int64_t a, const int64_t b, const OperationType op, int64_t &result) {
        if (!can_fold(a, b, op)) return false;

        switch (op) {
            case OperationType::Add:
                result = a + b;
                break;
            case OperationType::Multiply:
                result = a * b;
                break;
            case OperationType::Power: {
                if (b < 0) return false;
                result = 1;
                for (int64_t i = 0; i < b; i++) result *= a;
                break;
            }
            default: ;
        }
        return true;
    }
}
