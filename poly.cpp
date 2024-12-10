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
    if (coefficients.empty()) {
        return 0;  
    }

    for (auto it = coefficients.rbegin(); it != coefficients.rend(); ++it) {
        if (it->second != 0) {
            return it->first;
        }
    }
    return 0;
}

void polynomial::print() {

}

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

void poly_mult(const polynomial &p2, int power, coeff coeff, std::vector<polynomial*>& polys) {
    polynomial* newpoly = new polynomial(); 
    for (auto it2 = p2.coefficients.rbegin(); it2 != p2.coefficients.rend(); ++it2) {
        int power2 = it2->first;
        int coeff2 = it2->second;
        if (coeff2 == 0) continue;
        int new_power = power + power2;
        int new_coeff = coeff * coeff2;
        newpoly->coefficients[new_power] += new_coeff;
    }
    polys.push_back(newpoly); 
}

polynomial& polynomial::operator*(const polynomial &other) const {
    if (coefficients.empty() || other.coefficients.empty()) {
        return *new polynomial();
    }

    std::vector<std::thread> threads;
    std::vector<polynomial*> polys;

    for (auto it = coefficients.rbegin(); it != coefficients.rend(); ++it) {
        int power1 = it->first;
        int coeff1 = it->second;
        if (coeff1 == 0) continue;


        threads.emplace_back(poly_mult, std::cref(other), power1, coeff1, std::ref(polys));
    }


    for (auto &thread : threads) {
        thread.join();
    }

    polynomial* newpoly = new polynomial();
    for (auto* ply : polys) {
        *newpoly = *newpoly + *ply;
        delete ply; 
    }

    return *newpoly;
}

// polynomial& polynomial::operator*(const polynomial &other) const {
//     if (coefficients.empty() || other.coefficients.empty()) {
//         return *new polynomial(); 
//     }
//     polynomial* newpoly = new polynomial();
//     for (auto it = coefficients.rbegin(); it != coefficients.rend(); ++it) {
//         int power1 = it->first;
//         coeff coeff1 = it->second;
//         if (coeff1 == 0) continue;
//         for (auto it2 = other.coefficients.rbegin(); it2 != other.coefficients.rend(); ++it2) {
//             int power2 = it2->first;
//             coeff coeff2 = it2->second;
//             if (coeff2 == 0) continue;
//             int new_power = power1 + power2;
//             coeff new_coeff = coeff1 * coeff2;
//             newpoly->coefficients[new_power] += new_coeff;
//         }
//     }
//     return *newpoly;
// }

polynomial& polynomial::operator*(int other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = coefficients;
    for (auto it = newpoly->coefficients.rbegin(); it != newpoly->coefficients.rend(); ++it) {
        newpoly->coefficients[it->first] = newpoly->coefficients[it->first] * other;
    }
    return *newpoly; 
}

polynomial operator*(int left, const polynomial &right) {
    polynomial newpoly;  
    newpoly.coefficients = right.coefficients;  

    for (auto it = newpoly.coefficients.rbegin(); it != newpoly.coefficients.rend(); ++it) {
        newpoly.coefficients[it->first] = it->second * left;  
    }
    return newpoly;  
}

polynomial& polynomial::operator%(const polynomial& other) const {

    polynomial* remainder = new polynomial(*this);
    while (!remainder->coefficients.empty() && remainder->find_degree_of() >= other.find_degree_of()) {
        int degree_diff = remainder->find_degree_of() - other.find_degree_of();
        int leading_coeff_remainder = remainder->coefficients[remainder->find_degree_of()];
        int leading_coeff_other = other.coefficients.at(other.find_degree_of()); 

        if (leading_coeff_other == 0) {
            break; 
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
        auto highest_degree = remainder->find_degree_of();
        while (remainder->coefficients.find(highest_degree) != remainder->coefficients.end() &&
               remainder->coefficients[highest_degree] == 0) {
            remainder->coefficients.erase(highest_degree);
            highest_degree = remainder->find_degree_of();
        }
        if (remainder->coefficients.empty()) {
            remainder->coefficients[0]=0;
            break;
        }
    }

    return *remainder;
}