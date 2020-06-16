#ifndef _VECTOR_H
#define _VECTOR_H

/**
 *	VECTOR
 *	@author	Quoc Thuan Truong		18120583
 *	@since 2019-10-18
 */

#include <iostream>
#include <string>
using namespace std;

class Vector {
private:
	double* vector;
	int size;
public:
	int Size();
public:
	void inputData();
	void showData();
public:
	Vector* sumWith(const Vector* other);
	Vector* multiplyWithAlpha(double alpha);
public:
	Vector();
	Vector(int size);
	~Vector();
public:
	friend Vector* operator+(const Vector& a, const Vector& b);
	friend Vector* operator*(const Vector& a, double alpha);
};

#endif // !_VECTOR_H
