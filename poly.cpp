#include "poly.h"
#include <vector>


polynomial::polynomial() {
    coefficients = new std::vector<int>;
}

template <typename Iter>
polynomial::polynomial(Iter begin, Iter end)
{


polynomial &operator=(const polynomial &other) {
    newvec = new std:vector<int>;
    for (int i = 0; i < newvec.size(); i++) {
        newvec.push_back(other->coefficients[i]);
    }
    newpoly = new Polynomial;
    newpoly->coefficients = &newvec;
    return newpoly;
}

polynomial &operator+(const polynomial &other) {
    newpoly = new polynomial();
    newpoly.coefficients = new std::vector<int>;
    if (coefficients.size() > other.coefficients.size()) {
        for (int i = 0; i < coefficients.size(); i++) {
            if ((i+1) < other.coefficients.size()) {
                newpoly.push_back(other.coefficients[i] + coefficients[i]);
            }
            else {
                newpoly.push_back(coefficients[i]);
            }
        }
    }
    else {
        for (int i = 0; i < other.coefficients.size(); i++) {
            if ((i+1) < coefficients.size()) {
                newpoly.push_back(other.coefficients[i] + coefficients[i]);
            }
            else {
                newpoly.push_back(other.coefficients[i]);
            }
        }        
    }

}

polynomial &operator+(int other) {
    newpoly = new polynomial();
    newpoly.coefficients = new std:vector<int>;
    for (int i = 0; i < coefficients.size(); i++) {
        newpoly.coefficients[i] = coefficients[i];
    }
    newpoly.coefficients[0] = newpoly.coefficients[0] + other;
    return newpoly;
}

polynomial &operator*(const polynomial &other) {
    newpoly = new polynomial();
    newpoly.coefficients = new std::vector<int>;
    for (int i = 0; i < coefficients.size(); i++) {
        for (int j = 0; i < other.coefficients.size(); i++) {
            newpoly.coefficients[i + j] = coefficients[i] * other.coefficients[j]; 
        }
    }
    return newpoly;
}

polynomial &operator%(const polynomial &other) {
    
}