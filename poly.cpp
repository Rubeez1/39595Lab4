#include <vector>
#include <iostream>
#include <chrono>
#include <optional>
#include <map>
#include <thread>
#include "poly.h"


polynomial::polynomial() {
    std::map<int, int> coefficients;
}



polynomial::~polynomial() {
}

polynomial::polynomial(const polynomial &other) {
    coefficients = std::map<int, int>(other.coefficients);
}

size_t polynomial::find_degree_of() const {
    return coefficients.rbegin()->first;
}

// void polynomial::print() {
//     for (int i = coefficients->size(); i < 0 ; i--) {
//        std::cout << (*coefficients)[i].last << "x^" << (*coefficients)[i].first << std::endl;
//     }
    
// }

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    std::vector<std::pair<power, coeff>> canonical;
    bool zero_poly = false;

    for (auto it = coefficients.rbegin(); it != coefficients.rend(); ++it) {
        if (it->second == 0) {
            zero_poly = true;
        } else {
            canonical.push_back(*it); 
        }
    }


    if (zero_poly && canonical.empty()) {
        canonical.push_back(std::make_pair(0, 0));
    }

    return canonical;
}

polynomial& polynomial::operator=(const polynomial &other){
    coefficients = other.coefficients; 
    return *this;
}

polynomial& polynomial::operator+(const polynomial &other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = coefficients; 

    for (const auto& it : other.coefficients) {
        newpoly->coefficients[it.first] += it.second; 
    }

    return *newpoly; 
}
polynomial& polynomial::operator+(int other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = coefficients;

    newpoly->coefficients[0] += other;
    return *newpoly;
}

polynomial operator+(int left, const polynomial &right) {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = right.coefficients;

    newpoly->coefficients[0] = left + newpoly->coefficients[0];
    return *newpoly; 
}

polynomial& polynomial::operator*(const polynomial &other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = std::map<int, int>();
    for (const auto& p1 : coefficients) {
        int power1 = p1.first;
        coeff coeff1 = p1.second;
        
        for (const auto& p2 : other.coefficients) {
            int power2 = p2.first;
            coeff coeff2 = p2.second;
            int new_power = power1 + power2;
            coeff new_coeff = coeff1 * coeff2;
            newpoly->coefficients[new_power] += new_coeff;
        }
    }

    return *newpoly;
}


polynomial& polynomial::operator*(int other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = coefficients;
    for (auto it = newpoly->coefficients.rbegin(); it != newpoly->coefficients.rend(); ++it) {
        newpoly->coefficients[it->first] = newpoly->coefficients[it->first] * other;
    }
    return *newpoly; 
}

polynomial operator*(int left, const polynomial &right){
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = right.coefficients;
    for (auto it = newpoly->coefficients.rbegin(); it != newpoly->coefficients.rend(); ++it) {
        newpoly->coefficients[it->first] = newpoly->coefficients[it->second] * left;
    }
    return *newpoly; 
}

polynomial& polynomial::operator%(const polynomial& other) const {
    polynomial* remainder = new polynomial(*this);  

    while (!remainder->coefficients.empty() && remainder->find_degree_of() >= other.find_degree_of()) {

        int degree_diff = remainder->find_degree_of() - other.find_degree_of();
        int leading_coeff_remainder = remainder->coefficients[remainder->find_degree_of()];
        int leading_coeff_other = 0;
        auto it = other.coefficients.find(other.find_degree_of());
        if (it != other.coefficients.end()) {
                leading_coeff_other = it->second;
        }
        int c_quot = leading_coeff_remainder / leading_coeff_other;

        for (const auto& term : other.coefficients) {
            int power = term.first;
            coeff coeff_other = term.second;
            int new_power = power + degree_diff;
            remainder->coefficients[new_power] -= c_quot * coeff_other;

            if (remainder->coefficients[new_power] == 0) {
                remainder->coefficients.erase(new_power);
            }
        }

        while (!remainder->coefficients.empty() && remainder->coefficients.count(remainder->find_degree_of()) == 0) {
            remainder->coefficients.erase(remainder->find_degree_of());
        }

        if (remainder->coefficients.empty()) {
            break;
        }
    }
    return *remainder;
}