#include <iostream>
#include <numeric>

#include "bingo/bingo.hpp"
#include "tests/basic.cpp"

int main() {
    bingo::stdconnect();
    // bingo::Number f{1222};
    //
    // f = (10_bn*f)+(10_bn*782);
    // auto pattern = (10_bn*1222)+(10_bn*1_vr);
    // auto res = pattern.check(f);

    // bingo::Number f{1222};
    //auto res = bingo::pattern(1222_vr, 1_vr).check(a);
    // f = (10_bn*f)*(10_bn);
    // auto pattern = (10_bn*1222)*(10_bn*1_vr);
    // auto res = pattern.check(f);
    test();
    test_comm();

    // std::cout << "1.2 Variables and constants mixed order:" << std::endl;
    // auto p2 = (5_bn + 1_vr + 3_bn);
    // auto e2a = (10_bn + 5_bn + 3_bn);
    // auto e2b = (3_bn + 10_bn + 5_bn);
    // auto e2c = (5_bn + 3_bn + 10_bn);
    //
    // std::cout << "  Pattern: 5 + var1 + 3" << std::endl;
    // std::cout << "  Expr A:  10 + 5 + 3 -> ";
    // printResult(p2.check(e2a));
    // std::cout << "  Expr B:  3 + 10 + 5 -> ";
    // printResult(p2.check(e2b));
    // std::cout << "  Expr C:  5 + 3 + 10 -> ";
    // printResult(p2.check(e2c));
    // std::cout << std::endl;

    auto i = (3_bn ^ 2) * (3_bn ^ 4);
    bingo::do_reduce(i);
}
