#include "poly.h"
#include <vector>


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
    return coefficients->size();
}

void polynomial::print() {
    //dummy function
}

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    std::vector<std::pair<power, coeff>> canonical;
    for (size_t i = 0; i < coefficients->size(); i++) {
        canonical.push_back(std::make_pair(i, (*coefficients)[i]));
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
        (*newpoly->coefficients)[i] = (*right.coefficients)[i]; 
    }
    (*newpoly->coefficients)[0] = left + (*newpoly->coefficients)[0];
    return *newpoly; 
}

polynomial& polynomial::operator*(const polynomial &other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>;
    for (size_t i = 0; i < coefficients->size(); i++) {
        for (size_t j = 0; i < other.coefficients->size(); i++) {
            (*newpoly->coefficients)[i + j] = (*coefficients)[i] * (*other.coefficients)[j]; 
        }
    }
    return *newpoly; 
}

polynomial& polynomial::operator*(int other) const {
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>;
    for (size_t i = 0; i < coefficients->size(); i++) {
        (*newpoly->coefficients)[i] = (*coefficients)[i] * other; 
    }
    return *newpoly; 
}

polynomial operator*(int left, const polynomial &right){
    polynomial* newpoly = new polynomial();
    newpoly->coefficients = new std::vector<int>;    
    for (size_t i = 0; i < right.coefficients->size(); i++) {
        (*newpoly->coefficients)[i] = (*right.coefficients)[i] * left; 
    }
    return *newpoly; 
}

polynomial& polynomial::operator%(const polynomial& other) const {
    polynomial* remainder = new polynomial(*this);

    while (remainder->find_degree_of() >= other.find_degree_of()) {
        power p_diff = remainder->find_degree_of() - other.find_degree_of();
        coeff c_quot = (*remainder->coefficients)[remainder->find_degree_of() - 1] / (*other.coefficients)[other.find_degree_of() - 1];

        for (size_t i = 0; i < other.coefficients->size(); ++i) {
            if (i + p_diff < remainder->coefficients->size()) {
                (*remainder->coefficients)[i + p_diff] -= c_quot * (*other.coefficients)[i];
            }
        }
        while (!remainder->coefficients->empty() && remainder->coefficients->back() == 0) {
            remainder->coefficients->pop_back();
        }
    }
    return *remainder;
}
