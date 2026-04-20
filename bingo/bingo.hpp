#pragma once
#include "core/core.cpp"
namespace bingo {
    inline void regpattern(PatternBase* pattern) {
        detail::patterns.push_back(pattern);
    }

    inline void stdconnect() {
        regpattern(new reduce::PowerAdd{});
    }
}
