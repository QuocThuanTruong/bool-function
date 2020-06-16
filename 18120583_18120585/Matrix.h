#pragma once

#include <iostream>
#include <vector>

using namespace std;


class Matrix {
private:
	int _row, _column;
	float **arr;
public:
	// ham tao khong doi so
	Matrix();

	// ham tao sao chep 
	Matrix(const Matrix& a);

	// ham tao co doi so
	Matrix(int row, int column);

	// ham huy
	~Matrix();

	// dinh nghia toan tu =
	Matrix& operator=(const Matrix &a);

	// ham nhap ma tran
	void importMatrix();

	// ham in ma tran
	void printMatrix();

	// ham tim dinh thuc cua ma tran
	float caculateDeterminant();

	// ma tran phu hop
	Matrix atjugateMatrix();

	//subMatrix
	Matrix subMatrix(int m, int n);

	// ham nghich dao ma tran
	Matrix inverseMatrix();

	// phep nhan hai ma tran
	int Multiplication(const Matrix& a);

	// tim hang cua ma tran
	int findRankofMatrix();

	// ham doi 2 hang cua ma tran
	void swapRow(int row1, int row2);

	// ham doi gia tri 2 so
	void swapNumber(float &a, float &b);

	// ham chuyen ve ma tran bac thang
	Matrix converttoEchelonMatrix();

	// ham nhap he phuong trinh tuyen tinh
	void solveLinearSystem();

	// ham kiem tra ma tran co vuong khong
	int isSquare();
};

