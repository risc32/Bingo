#pragma once
#include "core/core.cpp"
namespace bingo {
    inline void regpattern(PatternBase* pattern) {
        detail::patterns.push_back(pattern);
    }

    inline void stdconnect() {
        regpattern(new reduce::PowerAdd{});
        regpattern(new reduce::PowerSub{});
        regpattern(new reduce::Square{});
        regpattern(new reduce::AddMul{});
        regpattern(new reduce::SubMul{});
        regpattern(new reduce::DivSelf{});
    }

    inline void optconnect() {
        regpattern(new reduce::ConstAdd{});
        regpattern(new reduce::ConstMul{});
        regpattern(new reduce::ConstDiv{});
        regpattern(new reduce::ConstPow{});
    }
}
