#include "Matrix.h"


/*
* ham tao khong doi so
* @param: ma tran NULL
* @return: ma tran voi hang = 0, cot =0, gia tri cac phan tu trong ma tran = NULL
*/
Matrix::Matrix() {
	_row = 0;
	_column = 0;
	this->arr = NULL;
}

/*
* ham tao sao chep
* @param: ma tran null, ma tran a
* @return: ma tran la ban sao cua ma tran a
*/
Matrix::Matrix(const Matrix & a)
{
	// gan cac gia tri cua ma tran
	_row = a._row;
	_column = a._column;

	// cap phat bo nho
	arr = new float*[_row];
	for (int i = 0; i < _row; i++) {
		arr[i] = new float[_column];
	}

	// gan cac gia tri cua cac phan tu
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _column; j++) {
			arr[i][j] = a.arr[i][j];
		}
	}
}


/*
* ham tao co doi so
* @param: ma tran null, gia tri cua hang, cot
* @return: ma tran duoc cap phat voi hang va cot o input
*/
Matrix::Matrix(int row, int column) {
	// gan cac gia tri cua ma tran
	_row = row;
	_column = column;

	// cap phat
	arr = new float*[_row];

	for (int i = 0; i < _row; i++) {
		arr[i] = new float[_column];
	}
}

/*
* ham huy ma tran
* @param: ma tran
* @return: ma tran duoc giai phong, xoa cac phan tu trong ma tran
*/
Matrix::~Matrix() {


	for (int i = 0; i < _row; i++)
		if (arr[i] != NULL)
			delete[] arr[i];
	if (arr != NULL)
		delete[] arr;

}


/*
* ham dinh nghia toan tu =
* @param: ma tran thu nhat, ma tran duoc gan ( thu 2 )
* @return: ma tran thu 1 la ban sao cua ma tran thu 2 sau toan tu =
*/
Matrix & Matrix::operator=(const Matrix & a)
{
	/* kiem tra ma tran thu nhat co khac ma tran thu 2, neu dung thi moi thuc hien,
	* neu khong dung thi tra ve vi hai ma tran co gia tri bang nhau */
	if (this != &a) {

		if (this->arr != NULL) {
			for (int i = 0; i < _row; i++)
				delete[]arr[i];
			delete[]arr;
		}

		// gan va cap phat lai cac gia tri cua ma tran thu nhat theo cac gia tri ma tran thu 2
		_row = a._row;
		_column = a._column;
		arr = new float*[_row];

		for (int i = 0; i < _row; i++) {
			arr[i] = new float[_column];
		}

		// gan gia tri cua cac phan tu 
		for (int i = 0; i < _row; i++) {
			for (int j = 0; j < _column; j++) {
				arr[i][j] = a.arr[i][j];
			}
		}
	}
	// tra ve ma tran thu nhat
	return *this;
}

/*
* ham nhap ma tran
* @param: ma tran khong co gia tri
* @return: ma tran voi cac gia tri duoc nhap tu ban phim
*/
void Matrix::importMatrix() {

	// nhap cac gia tri hang cot
	cout << " Nhap so hang: ";
	cin >> _row;
	cout << "\n Nhap so cot: ";
	cin >> _column;
	cout << endl;
	// cap phat vung nho cho mang 2 chieu
	arr = new float*[_row];

	for (int i = 0; i < _row; i++) {
		arr[i] = new float[_column];
	}
	// nhap tung gia tri cua mang
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _column; j++) {
			cout << "arr[" << i << "][" << j << "]=";
			cin >> arr[i][j];
		}
	}
}

/*
* ham xuat ma tran ( in ma tran )
* @param: ma tran
* @return: in ra man hinh gia tri cua ma tran
*/
void Matrix::printMatrix() {

	// duyet mang, xuat tung gia tri
	for (int i = 0; i < this->_row; i++) {
		cout << endl;
		for (int j = 0; j < this->_column; j++) {
			cout << this->arr[i][j] << "\t";
		}
	}
}


float Matrix::caculateDeterminant() {

	// truong hop dong=cot=1
	if (_row == 1 && _column == 1) {

		return arr[0][0];
	}

	// truong hop dong bang cot bang 2
	if (_row == 2 && _column == 2) {
		return (arr[0][0] * arr[1][1] - arr[0][1] * arr[1][0]);

	}

	// cac truong hop con lai dinh thuc duoc tinh bang tich duong cheo cua ma tran khi chuyen ve ma tran vuong

	float result = 1.0;
	// su dung 1 ma tran tam thoi co gia tri bang ma tran return khi chuyen ve ma tran vuong
	Matrix temp = this->converttoEchelonMatrix();
	// ket qua se la tich duong cheo
	for (int i = 0; i < temp._row; i++) {
		result *= temp.arr[i][i];
	}
	// tra ve ket qua la dinh thuc cua ma tran
	if (result == -0) {
		result = -result;
	}

	return result;
}

/*
* ma tran xoa hang m cot n
* @param: ma tran bi xoa, hang m cot n bi xoa
* @return: ma tran bi xoa hang m cot n
*/
Matrix Matrix::subMatrix(int m, int n) {

	// kiem tra cac gia tri n, m co thoa khong, neu khong thi tra ve ma tran NULL
	if (_column < n && _row < m) {
		return Matrix();
	}

	// tao ma tran ket qua voi so hang, so cot - 1, vectoc vec de luu cac gia tri hang cot
	Matrix result(_row - 1, _column - 1);
	vector <float> vec;

	// lay tat ca cac phan tu khong thuoc hang m cot n dua vao vector vec
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _column; j++) {
			if (i + 1 != m && j + 1 != n) {
				vec.push_back(arr[i][j]);
			}
		}
	}

	// dua cac gia tri cua vector vec vao ma tran ket qua ( ma tran da bi xoa cac gia tri hang m cot n )
	for (int i = result._row - 1; i >= 0; i--) {
		for (int j = result._column - 1; j >= 0; j--) {
			result.arr[i][j] = vec.back();
			vec.pop_back();
		}
	}
	// tra ve ma tran ket qua
	return result;
}

/*
* ma tran phu hop
* @param: ma tran
* @return: ma tran phu hop cua ma tran input
*/
Matrix Matrix::atjugateMatrix() {

	// tao ma tran moi voi gia tri hang cot nhu ma tran ban dau
	Matrix result(_row, _column);

	// gan cac gia tri cua ma tran ket qua theo cong thuc cua ma tran phu hop
	// cong thuc:  a[j][i]=(-1)^(i+j+2) * dinh thuc cua ma tran xoa hang i cot j
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _column; j++) {
			result.arr[j][i] = pow(-1, i + j + 2)*subMatrix(i + 1, j + 1).caculateDeterminant();
		}
	}

	// tra ve ma tran ket qua
	return result;
}

/*
* ma tran nghich dao
* @param: ma tran
* @return: ma tran nghich dao cua ma tran input
*/
Matrix Matrix::inverseMatrix() {

	// khoi tao ma tran ket qua
	Matrix result(_row, _column);

	// chuyen ma tran thanh ma tran phu hop
	result = this->atjugateMatrix();

	// gan cac gia tri theo cong thuc 
	// cong thuc: a[i][j]= (1/ det)*a[i][j] cua ma tran phu hop
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _column; j++) {
			result.arr[i][j] = result.arr[i][j] / this->caculateDeterminant();
		}
	}

	// tra ve ma tran ket qua
	return result;
}

/*
* ham nhan ma tran
* @param: ma tran 1, ma tran 2
* @return: 1 neu nhan duoc , 0 neu khong nhan duoc
*/
int Matrix::Multiplication(const Matrix & a) {

	// dieu kien de nhan la so cot ma tran a phai bang so dong ma tran b
	if (_column != a._row)
		return 0;

	// tạo vùng nhớ chứa ma trận mới
	float ** elementsNew = new float *[_row];
	for (int i = 0; i < _row; i++)
		elementsNew[i] = new float[a._column];

	// nhân theo công thức
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < a._column; j++)
		{
			elementsNew[i][j] = 0;
			for (int k = 0; k < _column; k++)
				elementsNew[i][j] = elementsNew[i][j] + arr[i][k] * a.arr[k][j];
		}
	}

	// xóa vùng nhớ cũ
	for (int i = 0; i < _row; i++)
		delete[] arr[i];
	delete[]arr;

	// cập nhật lại kích thước ma trận ( khi ma tran mn * np thi se ra ma tran mp )
	_row = _row;
	_column = a._column;
	arr = elementsNew; // gán lại vùng nhớ mới.
	return 1;

}

/*
* ham tim rank cua ma tran
* @param: ma tran
* @return: rank cua ma tran do ( int )
*/
int Matrix::findRankofMatrix() {

	// tao ma tran temp = ma tran input chuyen ve dang bac thang
	Matrix temp = this->converttoEchelonMatrix();

	// khoi tao gia tri cua ket qua = so hang cua ma tran
	int result = _row;

	// kiem tra cot 0 cua ma tran, neu bang 0 thi bat dau duyet hang do, neu tat ca cac gia tri cua hang deu bang khong
	// thi hang cua ma tran se bi tru di 1 
	for (int i = 0; i < temp._row; i++) {
		if (temp.arr[i][0] == 0) {
			int count = 0;
			for (int j = 1; j < temp._column; j++) {
				if (temp.arr[i][j] == 0) count++;
			}
			if (count == temp._column - 1) result--;
		}
	}

	// tra ve ket qua la rank cua ma tran
	return result;
}

/*
* ham doi  2 hang cua ma tran
* @param: ma tran, row1 row2 can hoan vi
* @return: ma tran khi doi row1 va row2 cho nha
*/
void Matrix::swapRow(int row1, int row2) {

	for (int i = 0; i < _column; i++) {

		int temp = arr[row1][i];
		arr[row1][i] = arr[row2][i];
		arr[row2][i] = temp;
	}
}
// ham doi 2 so
void Matrix::swapNumber(float &a, float &b) {
	float t;
	t = a;
	a = b;
	b = t;
}


/*
* ham chuyen ma tran thanh ma tran bac thang
* @param: ma tran
* @return: dang bac thang cua ma tran
*/
Matrix Matrix::converttoEchelonMatrix() {

	// su dung thuat toan Gauss
	// gan ma tran ket qua bang ma tran input, cac gia tri i j bang 0
	Matrix result = *this;
	int i = 0; int j = 0;

	// dieu kien dung la i >= so hang hoac j>= so cot
	while (i < _row && j < _column) {
		// neu Aij=0 thi thuc hien phep bien doi tren dong ( dk-(Akj/Aij)di voi k>i
		if (result.arr[i][j] != 0) {

			for (int k = i + 1; k < result._row; k++) {

				float t = (float)-result.arr[k][j] / result.arr[i][j];

				for (int d = j; d < result._column; d++) result.arr[k][d] += t * result.arr[i][d];
			}
			// sau do tang gia tri cua i j
			i++; j++;
		}
		// neu Aij=0 thi kiem tra cac gia tri Akj ( k>i ), neu Akj!=0 thi hoan vi hang k va hang i 
		// neu khong co gia tri Akj nao khac 0 thi tang gia tri cua j 
		else if (result.arr[i][j] == 0) {
			int count = 0;
			for (int k = i + 1; k < result._row; k++) {
				if (result.arr[k][j] != 0) result.swapRow(i, k);
				else if (result.arr[k][j] == 0) count++;
			}
			if (count == result._row - i - 1) j = j + 1;
		}
	}
	// tra ve ma tran ket qua
	return result;
}

/*
* ham giai he phuong trinh tuyen tinh
* @param:  he phuong trinh tuyen tinh  (bao gom ma tran he so, cot he so tu do)
* @return: nghiem cua phuong trinh
*/
void Matrix::solveLinearSystem() {

	// nhap ma tran he so va cot he so tu do
	cout << "He phuong trinh tuyen tinh co dang AX=B" << endl;
	cout << "Voi A la ma tran he so, B la cot cac he so tu do." << endl;
	cout << "Vi du" << endl;
	cout << "x1+x2+2x4=3" << endl;
	cout << "2x1-x2+3x4=1" << endl;
	cout << "5x1+x2-2x4=-3" << endl;
	cout << "Thi A=";
	cout << "1  1  2" << endl;
	cout << "      2 -1  3" << endl;
	cout << "      5  1 -2" << endl;
	cout << "B={3,1,-3}" << endl;
	cout << "Nhap ma tran he so A: " << endl;
	this->importMatrix();
	int n = _row;
	float b[100];
	cout << "Nhap cot cac he so tu do B" << endl;
	for (int i = 0; i < _row; i++) {
		cout << "nhap B[" << i << "]:=";
		cin >> b[i];
		cout << endl;
	}

	// chuyen ma tran he so ve dang bac thang ( thuat toan tuong tu chuyen ve ma tran o tren )
	// ben canh do cung chuyen cot b theo cac phep bien doi cua ma tran he so
	Matrix result = *this;
	int i = 0; int j = 0;

	while (i < _row && j < _column) {
		if (result.arr[i][j] != 0) {

			for (int k = i + 1; k < result._row; k++) {

				float t = (float)-result.arr[k][j] / result.arr[i][j];
				b[k] += t * b[i];
				for (int d = j; d < result._column; d++) result.arr[k][d] += t * result.arr[i][d];
			}
			i++; j++;
		}
		else if (result.arr[i][j] == 0) {
			int count = 0;
			for (int k = i + 1; k < result._row; k++) {
				if (result.arr[k][j] != 0) {
					result.swapRow(i, k);
					result.swapNumber(b[i], b[k]);
				}
				else if (result.arr[k][j] == 0) count++;
			}
			if (count == result._row - i - 1) j = j + 1;
		}
	}
	cout << endl;

	// tim hang cua ma tran do ( tuong tu thuat toan tim rank ma tran o tren )
	int r = _row;
	for (int i = 0; i < result._row; i++) {
		if (result.arr[i][0] == 0) {
			int count = 0;
			for (int j = 1; j < result._column; j++) {
				if (result.arr[i][j] == 0) count++;
			}
			if (count == result._column - 1) r--;
		}
	}

	// sau khi chuyen ve dang bac thang cua ma tran mo rong ( ma tran dang A|B ) thi tien hanh giai

	// truong hop vo nghiem
	// khi ton tai mot hang cua A = 0 va gia tri tuong ung cua cot B != 0 thi he vo nghiem ( vd 0x1+0x2+0x3=1 )
	if (r < result._row) {
		for (int i = r; i < result._row; i++) {
			if (b[i] != 0) {
				cout << "He phuong trinh vo nghiem";
				return;
			}
		}
	}

	// truong hop co duy nhat mot nghiem, ta se luu tap nghiem do vao mot mang c
	float c[100];
	if (r == result._row && r == result._column) {
		for (int i = r - 1; i >= 0; i--) {
			c[i] = b[i];
			for (int j = i + 1; j < r; j++) {
				if (j != i) c[i] = c[i] - result.arr[i][j] * c[j];
			}
			c[i] = c[i] / result.arr[i][i];
		}
		cout << "he phuong trinh co nghiem: ";
		for (int i = 0; i < r; i++) {
			cout << c[i] << endl;
		}
		return;
	}
	// truong hop vo so nghiem la cac truong hop con lai
	cout << "he phuong trinh vo so nghiem";
	return;
}

/*
* ham kiem tra ma tran co vuong khong, vuong thi tra ve 1, khong vuong tra ve 0
* @param: ma tran
* @return: 1: neu ma tran vuong, 0 neu ma tran khong vuong
*/
int Matrix::isSquare()
{
	if (_row == _column) return 1;
	return 0;
}
