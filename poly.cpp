#include "poly.h"
#include <vector.h>

polynomial &operator=(const polynomial &other) {
    newvec = new std:vector<int>;
    for (int i = 0; i < newvec.size(); i++) {
        newvec.push_back(other->coefficients[i]);
    }
    newpoly = new Polynomial;
    newpoly->coefficients = &newvec;
    return newpoly;
}

void &operator+(const polynomial &other) {
    if (other.coefficients.size() > coefficients.size()) {
        for (int i = 0; i < other.coefficients.size(); i++) {
            if (i > (coefficients.size() - 1)) {
                coefficients.push_back(other.coefficients[i]);
            }
            coefficients[i] = other.coefficients[i] + coefficients[i];
        }
    }
    else {
        for (int i = 0; i < other.coefficients.size(); i++) {
            coefficients[i] = other.coefficients[i] + coefficients[i];
        }
    }
}

void &operator+(int other) {
    coefficients[0] = coefficients[0] + other;
}

void &operator*(const polynomial &other) {
    
}

void &operator%(const polynomial &other) {
    
}