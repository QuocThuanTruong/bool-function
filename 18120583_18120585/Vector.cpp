#include "Vector.h"

/**
 *	Constructor - Thiet lap gia tri mac dinh cho vector
 *	@param  NULL
 *	@return NULL
 */
Vector::Vector() {
	this->vector = NULL;
	this->size = 0;
}

/**
 *	Constructor with param - Khoi tao vector voi chieu dai cho truoc
 *	@param  int  Chieu dai cua vector(size)
 *	@return NULL
 */
Vector::Vector(int size) {
	this->size = size;
	this->vector = new double[size];

	//Cap phat mac dinh
	for (int i = 0; i < this->size; i++) {
		vector[i] = 0;
	}
}

/**
 *	Destructor - Giai phong vung nho da cap phat cho vector
 *	@param  NULL
 *	@return NULL
 */
Vector::~Vector() {
	if (this->vector != NULL) {
		delete[] this->vector;
		this->size = 0;
	}
}

/**
 *	Ham Size - Dung de lay thuoc tinh chieu dai cua vector
 *	@param  NULL
 *	@return int  Chieu dai cua vector hien tai (this->size)
 */
int Vector::Size() {
	return this->size;
}

/**
 *	Ham inputData - Cho phep nguoi dung nhap du lieu cho vector (gom chieu dai va cac phan tu kem theo)
 *	@param  NULL
 *	@return NULL
 */
void Vector::inputData() {
	cout << "- Nhap chieu dai cua vector: ";
	cin >> this->size;
	
	this->vector = new double[size];
	for (int i = 0; i < this->size; i++) {
		cout << "+ Nhap phan tu thu " << i + 1 << ": ";
		cin >> vector[i];
	}
}

/**
 *	Ham showData - Hien thi du lieu cua vector, vd: (1, 2, 3, 4)
 *	@param  NULL
 *	@return NULL
 */
void Vector::showData() {
	if (this->size > 0) { //Neu vector != rong
		cout << "(";
		for (int i = 0; i < this->size; i++) {
			cout << this->vector[i];
			if (i < this->size - 1) {
				cout << ", ";
			}
		}
		cout << ")" << endl;
	}
	else { //Vector rong
		cout << "()" << endl;
	}
}

/**
 *	Ham sumWith - Tinh tong cua hai vector neu chung co cung chieu dai
 *	@param  Vector*  Mot vector dung de tinh tong voi vector hien tai (Vector* other)
 *	@return Vector*  Neu 2 vector co cung chieu dai se tra ve ket qua tinh tong cua 2 vector do, nguoc lai tra ve vector rong (size = 0)
 */
Vector* Vector::sumWith(const Vector* other) {
	if (this->size == other->size) { //Kiem tra chieu dai 2 vector
		Vector* result = new Vector(this->size);

		for (int i = 0; i < this->size; i++) {
			result->vector[i] = this->vector[i] + other->vector[i];
		}

		return result;
	}
	else { //this->size != other->size
		cout << "Khong the tinh tong vi hai vector khong cung chieu dai!" << endl;
	}

	return new Vector(0);
}

/**
 *	Ham multiplyWithAlpha - Nhan vector voi mot so alpha cho truoc
 *	@param  double   So thuc alpha cho truoc
 *	@return Vector*  Neu vector hien tai khac rong (size > 0) thi tra ve ket qua cua nhan vector voi so alpha, nguoc lai tra ve vector rong (size = 0)
 */
Vector* Vector::multiplyWithAlpha(double alpha) {
	if (this->size > 0) { //Kiem tra vector rong
		Vector* result = new Vector(this->size);

		for (int i = 0; i < this->size; i++) {
			result->vector[i] = this->vector[i] * alpha;
		}

		return result;
	}

	return new Vector(0);
}

/**
 *	@Overload toan tu '+' - Tinh tong cua hai vector neu chung co cung chieu dai
 *  @param  const Vector&  Vector hien tai goi den toan tu (a)
 *	@param  const Vector&  Vector tinh tong voi vector hien tai (b)
 *	@return Vector         Neu 2 vector co cung chieu dai se tra ve ket qua tinh tong cua 2 vector do, nguoc lai tra ve vector rong (size = 0)
 */
Vector* operator+(const Vector& a, const Vector& b) {
	if (a.size == b.size) { //Kiem tra chieu dai
		Vector* result = new Vector(a.size);

		for (int i = 0; i < result->size; i++) {
			result->vector[i] = a.vector[i] + b.vector[i];
		}

		return result;
	}
	else { //this->size != other->size
		cout << "Khong the tinh tong vi hai vector khong cung chieu dai!" << endl;
	}

	return new Vector(0);
}

/**
 *	@Overload toan tu '*' - Nhan vector voi mot so alpha cho truoc
 *  @param  const Vector&  Vector hien tai goi den toan tu
 *	@param  double         So thuc alpha cho truoc
 *	@return Vector         Neu vector hien tai khac rong (size > 0) thi tra ve ket qua cua nhan vector voi so alpha, nguoc lai tra ve vector rong (size = 0)
 */
Vector* operator*(const Vector& a, double alpha) {
	if (a.size > 0) { //Kiem tra vector rong
		Vector* result = new Vector(a.size);

		for (int i = 0; i < result->size; i++) {
			result->vector[i] = a.vector[i] * alpha;
		}

		return result;
	}

	return new Vector(0);
}
