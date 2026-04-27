#pragma once
#include "core/core.cpp"
namespace bingo {
    inline void regpattern(PatternBase* pattern) {
        detail::patterns.push_back(pattern);
    }

    inline void stdconnect() {
        regpattern(new reduce::PowerAdd{});
        regpattern(new reduce::PowerSub{});
        regpattern(new reduce::PowerPower{});
        regpattern(new reduce::PowerInc{});
        regpattern(new reduce::PowerMul{});
        regpattern(new reduce::Square{});
        regpattern(new reduce::AddMul{});
        regpattern(new reduce::SubMul{});
        regpattern(new reduce::DivSelf{});
        regpattern(new reduce::Div{});
    }

    inline void optconnect() {
        regpattern(new reduce::ConstAdd{});
        regpattern(new reduce::ConstMul{});
        regpattern(new reduce::ConstDiv{});
        regpattern(new reduce::ConstPow{});
    }

    const auto I = -1_bn ^ (1_bn / 2);
    const auto c = 299792458_bn;
}
