#include <vector>
#include <iostream>
#include <chrono>
#include <optional>
#include <thread>
#include "poly.h"


polynomial::polynomial() {
    coefficients = new std::vector<int>;
}



polynomial::~polynomial() {
    delete coefficients; 
}

polynomial::polynomial(const polynomial &other) {
    coefficients = new std::vector<int>(*other.coefficients);
}

size_t polynomial::find_degree_of() const {
    return (coefficients->size() - 1);
}

void polynomial::print() {
    for (int i = coefficients->size(); i < 0 ; i--) {
       std::cout << (*coefficients)[i] << "x^" << i << std::endl;
    }
    
}

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    std::vector<std::pair<power, coeff>> canonical;
    bool zero_poly = false;
    for (int i = coefficients->size() - 1; i >= 0; i--) {
        if ((*coefficients)[i] == 0) {
            zero_poly = true;
        }
        else {
        canonical.push_back(std::make_pair(i, (*coefficients)[i]));
        }
    }
    if ((zero_poly) && (canonical.size() == 0)) {
        canonical.push_back(std::make_pair(0, 0));
    }
    return canonical;
}

polynomial& polynomial::operator=(const polynomial &other){
    coefficients = new std::vector<int>(*other.coefficients); 
    return *this;
}

polynomial& polynomial::operator+(const polynomial &other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>(*coefficients);
    
    for (size_t i = 0; i < other.coefficients->size(); i++) {
        if (i < newpoly->coefficients->size()) {
            (*newpoly->coefficients)[i] += (*other.coefficients)[i];
        } else {
            newpoly->coefficients->push_back((*other.coefficients)[i]);
        }
    }
    return *newpoly;
}

polynomial& polynomial::operator+(int other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>(*coefficients);
    (*newpoly->coefficients)[0] += other;
    return *newpoly;
}

polynomial operator+(int left, const polynomial &right) {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>;    
    for (size_t i = 0; i < right.coefficients->size(); i++) {
        newpoly->coefficients->push_back((*right.coefficients)[i]);
    }
    (*newpoly->coefficients)[0] = left + (*newpoly->coefficients)[0];
    return *newpoly; 
}

void mult_poly(const polynomial & p1, const polynomial &p2, size_t deg, int * result){
    *result = 0; 
    for(size_t dg = 0; dg < deg; dg += 1){
        if(dg < p1.coefficients->size() && (deg - dg) < p2.coefficients->size()){
            *result += (*p1.coefficients)[dg] * (*p2.coefficients)[dg];
        }
    }
}

polynomial& polynomial::operator*(const polynomial &other) const {
    polynomial* newpoly = new polynomial();

    size_t max_power1 = coefficients->size();
    size_t max_power2 = other.coefficients->size();
    newpoly->coefficients->resize(max_power1 + max_power2 - 1, 0);  

    std::thread * threads = new std::thread[max_power1 + max_power2 - 1];
    int ** saves = new int*[max_power1 + max_power2 - 1]; 
    for(size_t i = 0; i <  max_power1 + max_power2; i += 1){
        saves[i] = new int[1];
        saves[i][0] = 0;
        threads[i] = std::thread(mult_poly,*this, other, i, saves[i]);
    }

    for(size_t i = 0; i <  max_power1 + max_power2; i += 1){
        threads[i].join();
    }

    for(size_t i = 0; i <  max_power1 + max_power2; i += 1){
        (*newpoly->coefficients)[i] = saves[i][0];
        int * sv = saves[i];
        delete []sv;
    }

    delete []saves;
    delete []threads;

    // polynomial* newpoly2 = new polynomial();

    // for (size_t i = 0; i < max_power1; i++) {
    //     for (size_t j = 0; j < max_power2; j++) {
    //         (*newpoly2->coefficients)[i + j] += (*coefficients)[i] * (*other.coefficients)[j];  // Add the product
    //     }
    // }

    // for(size_t i = 0; i <  max_power1 + max_power2; i += 1){
    //     std::cout<<"IDx: " << i << "Mine: " << (*newpoly->coefficients)[i] << "Correct: " << (*newpoly2->coefficients)[i] << std::endl;
    // }


    return *newpoly;
}

polynomial& polynomial::operator*(int other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>;
    for (size_t i = 0; i < coefficients->size(); i++) {
        newpoly->coefficients->push_back((*coefficients)[i] * other);
    }
    return *newpoly; 
}

polynomial operator*(int left, const polynomial &right){
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>;    
    for (size_t i = 0; i < right.coefficients->size(); i++) {
        newpoly->coefficients->push_back((*right.coefficients)[i] * left);
    }
    return *newpoly; 
}

polynomial& polynomial::operator%(const polynomial& other) const {
    polynomial* remainder = new polynomial(*this);

    while (remainder->find_degree_of() >= other.find_degree_of()) {
        size_t p_diff = remainder->find_degree_of() - other.find_degree_of();
        int c_quot = (*remainder->coefficients)[remainder->find_degree_of()] / (*other.coefficients)[other.find_degree_of()];

        for (size_t i = 0; i < other.coefficients->size(); ++i) {
            if (i + p_diff < remainder->coefficients->size()) {
                (*remainder->coefficients)[i + p_diff] -= c_quot * (*other.coefficients)[i];
            }
        }

        while (!remainder->coefficients->empty() && remainder->coefficients->back() == 0) {
            remainder->coefficients->pop_back();
        }

        if (remainder->coefficients->empty()) {
            break; 
        }
    }

    return *remainder;
}