#include "Menu.h"

/**
 *	Ham setFontSize - Thiet lap kich thuoc font chu
 *  @param  int   Kich thuoc font chu can thiet lap
 *	@return NULL
 */
void Menu::setFontSize(int size) {
	CONSOLE_FONT_INFOEX info = { 0 };

	info.cbSize = sizeof(info);
	info.dwFontSize.Y = size;
	info.FontWeight = FW_NORMAL;
	wcscpy_s(info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

/**
 *	Ham resizeConsole - Thiet lap lai kich thuoc man hinh
 *  @param  NULL
 *	@return NULL
 */
void Menu::resizeConsole() {
	HWND console = GetConsoleWindow();
	RECT rect;
	GetWindowRect(console, &rect);

	MoveWindow(console, rect.left, rect.top, this->width, this->height, TRUE);
}

/**
 *	Ham gotoXY - Di chuyen con chay tren console den toa do (x, y)
 *  @param  int	  Toa do truc nam ngang
 *  @param  int	  Toa do truc thang dung
 *	@return NULL
 */
void Menu::gotoXY(int x, int y) {
	COORD coord = { (SHORT)x, (SHORT)y };

	SetConsoleCursorPosition(consoleHandle, coord);
}

/**
 *	Ham putCharacterToConsole - Dua ki tu vao vi tri (x, y) tren man hinh
 *  @param  char  Ki tu can dua ra man hinh
 *  @param  int	  Toa do truc nam ngang
 *  @param  int	  Toa do truc thang dung
 *	@return NULL
 */
void Menu::putCharacterToConsole(char c, int x, int y)
{
	DWORD written;
	COORD coord = { (SHORT)x, (SHORT)y };

	FillConsoleOutputCharacter(consoleHandle, c, 1, coord, &written);
}

/**
 *	Ham putStringToConsole - Dua chuoi vao vi tri (x, y) tren man hinh
 *  @param  const char*  Chuoi can dua ra man hinh
 *  @param  int	         Toa do truc nam ngang
 *  @param  int	         Toa do truc thang dung
 *	@return NULL
 */
void Menu::putStringToConsole(const char str[], int x, int y) {
	DWORD written;
	COORD coord = { (SHORT)x, (SHORT)y };

	WriteConsoleOutputCharacter(consoleHandle, str, strlen(str), coord, &written);
}

/**
 *	Ham drawTitle - Ve tieu de cua tung muc menu ra man hinh, voi lua chon co khung hoac khong co khung (mac dinh)
 *  @param  const char*  Tieu de
 *  @param  int	         Toa do truc nam ngang
 *  @param  int	         Toa do truc thang dung
 *  @param  bool	     Co khong hay khong (mac dinh la khong)
 *	@return NULL
 */
void Menu::drawTitle(const char title[], int x, int y, bool haveFrame) {

	putStringToConsole(title, x, y);

	putCharacterToConsole(201, 0, 0);
	putCharacterToConsole(187, 53, 0);

	if (haveFrame) {
		putCharacterToConsole(200, 0, 34);
		putCharacterToConsole(188, 53, 34);

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 0);
			putCharacterToConsole(205, i, 34);
		}

		for (int i = 1; i < 34; i++) {
			putCharacterToConsole(186, 0, i);
			putCharacterToConsole(186, 53, i);
		}

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 4);
		}
	}
	else {
		putCharacterToConsole(200, 0, 4);
		putCharacterToConsole(188, 53, 4);

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 0);
		}

		for (int i = 1; i < 4; i++) {
			putCharacterToConsole(186, 0, i);
			putCharacterToConsole(186, 53, i);
		}

		for (int i = 1; i < 53; i++) {
			putCharacterToConsole(205, i, 4);
		}
	}
}

/**
 *	Ham draw - Ve menu
 *  @param  NULL
 *	@return NULL
 */
void Menu::draw() {
	system("cls");

	//So ki tu toi da theo truc x la 0-53, truc y la 0-34
	while (1) {

		drawTitle("DO AN 1 - CAU TRUC DU LIEU & GIAI THUAT", 7, 2, true);

		for (int i = 16; i <= 38; i++) {
			putCharacterToConsole(196, i, 11);
			putCharacterToConsole(196, i, 15);
			putCharacterToConsole(196, i, 19);
			putCharacterToConsole(196, i, 23);
			putCharacterToConsole(196, i, 27);
		}

		putStringToConsole("1. Ham Boole", 22, 13);
		putStringToConsole("2. Vector", 22, 17);
		putStringToConsole("3. Matrix", 22, 21);
		putStringToConsole("4. Exit", 22, 25);

		putCharacterToConsole(219, 20, 29);
		putStringToConsole("Lua chon:", 22, 29);

		gotoXY(32, 29);

		cin >> option;

		if (1 <= option && option <= 3) {
			break;
		}

		if (option == 4) {
			system("cls");
			exit(0);
		}
	}
}

/**
 *	Ham run - Khoi chay menu
 *  @param  NULL
 *	@return NULL
 */
void Menu::run() {
	int inputChar;

	resizeConsole();
	setFontSize(FONT_SIZE);

	while (1) {
		draw();

		switch (option) {
		case 1:
			do {
				system("cls");
				gotoXY(0, 0);
				drawTitle("TIM CONG THUC DA THUC TOI THIEU", 10, 2);
				gotoXY(0, 6);

				Boole* boole = new Boole();
				boole->inputFunction();
				boole->findAllBoolFunction();
				delete boole;

				cout << "\nNhap Enter de tiep tuc, phim bat ki de quay ve menu!" << endl;
				inputChar = _getch();
			} while (inputChar == 13);

			break;
		case 2:
			do {
				cin.ignore();
				system("cls");
				gotoXY(0, 0);
				drawTitle("TINH TOAN TREN VECTOR", 17, 2);
				gotoXY(0, 6);

				Vector* vector1 = new Vector();

				cout << "1. Tinh tong 2 vector" << endl;
				cout << "2. Tinh tich vector voi so alpha" << endl;

				do {
					cout << "\nLua chon : ";
					cin >> option;

					if (option > 2 || option < 0) {
						cout << "Lua chon khong hop le!" << endl;
					}
				} while (option > 2 || option < 0);

				if (option == 1) {
					Vector* vector2 = new Vector();
					Vector* res = new Vector();

					cout << "\nNhap vector 1: " << endl;
					vector1->inputData();
					cout << "-> Vector 1: ";
					vector1->showData();
					cout << "\nNhap vector 2: " << endl;
					vector2->inputData();
					cout << "-> Vector 2: ";
					vector2->showData();

					cout << "\n> Ket qua cong 2 vector: " << endl;
					res = vector1->sumWith(vector2);
					if (res->Size() != 0) {
						res->showData();
					}

					delete vector2, res;
				}
				else {
					double alpha;
					Vector* res = new Vector();
					cout << "\nNhap vector: " << endl;
					vector1->inputData();
					cout << "-> Vector: ";
					vector1->showData();
					cout << "\nNhap vao so alpha: ";
					cin >> alpha;

					cout << "\n> Ket qua nhan vector voi " << alpha << ": " << endl;
					res = vector1->multiplyWithAlpha(alpha);
					res->showData();

					delete res;
				}

				delete vector1;

				cout << "\nNhap Enter de tiep tuc, phim bat ki de quay ve menu!" << endl;
				inputChar = _getch();

			} while (inputChar == 13);

			break;
		case 3:
			do {
				system("cls");
				gotoXY(0, 0);

				drawTitle("TINH TOAN TREN MA TRAN", 17, 2);
				gotoXY(0, 6);

				cout << "1. Tim dinh thuc cua ma tran" << endl;
				cout << "2. Ngich dao ma tran" << endl;
				cout << "3. Tich hai ma tran" << endl;
				cout << "4. Tim hang cua ma tran" << endl;
				cout << "5. He phuong trinh tuyen tinh" << endl;
				do {
					cout << "\nLua chon : ";
					cin >> option;

					if (option > 5 || option < 0) {
						cout << "Lua chon khong hop le!" << endl;
					}
				} while (option > 5 || option < 0);


				Matrix* matrix = new Matrix();
				if (option == 1) {
					cout << "\nDe tinh dinh thuc thi ma tran nhap can phai la ma tran vuong! " << endl;
					cout << "Nhap ma tran" << endl;
					matrix->importMatrix();
					if (matrix->isSquare() == 1) {
						float det = matrix->caculateDeterminant();
						cout << "\n Dinh thuc cua ma tran vua nhap la: " << det << endl;
					}
					else cout << "\n Ma tran vua nhap khong phai la ma tran vuong!" << endl;
				}
				if (option == 2) {
					cout << "\nNhap ma tran" << endl;
					matrix->importMatrix();
					if (matrix->isSquare() == 0) {
						cout << "Ma tran vua nhap khong the nghich dao, yeu cau nhap ma tran vuong" << endl;
					}
					else {
						Matrix matrix2;
						matrix2 = matrix->inverseMatrix();
						cout << "Ma tran nghich dao cua ma tran vua nhap la: " << endl;
						matrix2.printMatrix();
					}
				}
				if (option == 3) {
					cout << "\nDieu kien de nhan 2 ma tran la so cot ma tran A phai bang so hang cua ma tran B" << endl;
					cout << "Nhap ma tran A:" << endl;
					matrix->importMatrix();
					cout << "Nhap ma tran B:" << endl;
					Matrix matrix3;
					matrix3.importMatrix();
					if (matrix->Multiplication(matrix3) != 1) {
						cout << "Hai ma tran vua nhap khong du dieu kien de nhan" << endl;
					}
					else {
						cout << "Tich cua hai ma tran vua nhap:" << endl;
						matrix->printMatrix();
					}
				}
				if (option == 4) {
					cout << "\nNhap ma tran:" << endl;
					matrix->importMatrix();
					int r = matrix->findRankofMatrix();
					cout << "Hang cua ma tran vua nhap la:" << r << endl;
				}
				if (option == 5) {
					matrix->solveLinearSystem();
				}

				delete matrix;
				cout << "\nNhap Enter de tiep tuc, phim bat ki de quay ve menu!" << endl;
				inputChar = _getch();

			} while (inputChar == 13);

			break;
		}
	}
}

/**
 *	Constructor - Thiet lap kich thuoc man hinh
 *  @param  NULL
 *	@return NULL
 */
Menu::Menu() {
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	this->width = WIDTH;
	this->height = HEIGHT;
}
