#include <iostream>

#include "bingo/bingo.hpp"
#include "tests/basic.cpp"

int main() {
    // bingo::Number f{1222};
    //
    // f = (10_bn*f)+(10_bn*782);
    // auto pattern = (10_bn*1222)+(10_bn*1_vr);
    // auto res = pattern.check(f);

    test();
    test_comm();
    // res[1]: constant(782)
}
