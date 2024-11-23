#include <iostream>
#include <chrono>
#include <optional>
#include <vector>
#include <cassert> 
#include "poly.h"

std::optional<double> poly_test(polynomial& p1,
                                polynomial& p2,
                                std::vector<std::pair<power, coeff>> solution)

{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    polynomial p3 = p1 * p2;

    auto p3_can_form = p3.canonical_form();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    p3.print();

    if (p3_can_form != solution)
    {
        return std::nullopt;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

void addition_test1() {
    std::vector<std::pair<power, coeff>> poly_input1 = {{2,1}, {1,2}, {0,1}};
    std::vector<std::pair<power, coeff>> poly_input2 = {{1,1}, {0,1}};
    polynomial p1(poly_input1.begin(), poly_input1.end());
    polynomial p2(poly_input2.begin(), poly_input2.end());
}

void zero_poly_test() {
    std::vector<std::pair<power, coeff>> poly_input1 = {{2,0}};
    polynomial p1(poly_input1.begin(), poly_input1.end());
}

void int_addition_test() {
    std::vector<std::pair<power, coeff>> poly_input1 = {{2,1}, {1,2}, {0,1}};
    polynomial p1(poly_input1.begin(), poly_input1.end());    
    polynomial p2 = p1 + 5;
    polynomial p3 = 5 + p1;
}

void modulo_test() {
    std::vector<std::pair<power, coeff>> poly_input1 = {{2,6}, {1,7}, {0,-20}};
    polynomial p1(poly_input1.begin(), poly_input1.end()); 
    std::vector<std::pair<power, coeff>> poly_input2 = {{1,2}, {0,5}};
    polynomial p2(poly_input2.begin(), poly_input2.end());  
    polynomial p3 = p1 % p2;
}
/*
int main()
{
    
    addition_test1();
    zero_poly_test();
    int_addition_test();
    modulo_test();
    //We're doing (x+1)^2, so solution is x^2 + 2x + 1
    std::vector<std::pair<power, coeff>> solution = {{2,1}, {1,2}, {0,1}};

    // This holds (x+1), which we'll pass to each polynomial
    std::vector<std::pair<power, coeff>> poly_input = {{1,1}, {0,1}};

    polynomial p1(poly_input.begin(), poly_input.end());
    polynomial p2(poly_input.begin(), poly_input.end());
    polynomial p3 = p1 * p2;
    std::optional<double> result = poly_test(p1, p2, solution);

    if (result.has_value())
    {
        std::cout << "Passed test, took " << result.value()/1000 << " seconds" << std::endl;
    } 
    else 
    {
        std::cout << "Failed test" << std::endl;
    }
}*/