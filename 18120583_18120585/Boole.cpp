#include "Boole.h"

/**
 *	Constructor - Thiet lap cac gia tri mac dinh cho cac thuoc tinh cua ham boole
 *	@param  NULL
 *	@return NULL
 */
Boole::Boole() {
	//Cap phat gia tri ban dau
	numberOfAllEPIs = 0;
	numberOfAllPIs = 0;
	numberOfPotentialEPIs = 0;
	numberOfPossibleEPIs = 1;
	numberOfRemainingMinterms = 0;
	numberOfRemainingPIs = 0;
	numberOfMinNumPIs = 0;
	numberOfAllMinterms = 0;

	indexOfDecimalMinterms = NULL;
	binaryMinterms = NULL;
	Table = NULL;
	Result = NULL;
	indexOfPIs = NULL;
	indexOfEPIs = NULL;
	numAppearCounter = NULL;
	idxOfRemainingPIs = NULL;
	idxOfRemainingDecMTs = NULL;
	idxOfPIsCoverRemainPart = NULL;
	reducedPIChart = NULL;
	potentialEPIs = NULL;
	idxOfMinNumOfPIs = NULL;
	numberOfPIsForEPI = NULL;
}

/**
 *	Destructor - Giai phong cac vung nho da cap phat de xu li ham boole
 *	@param  NULL
 *	@return NULL
 */
Boole::~Boole() {
	//Neu F != 1, xoa du lieu duoc cap phat de tim cong thuc cua F
	if (numberOfAllMinterms != pow(2, numberOfVariables)) {
		delete[] idxOfMinNumOfPIs;
		delete[] numberOfPIsForEPI;
		delete[] idxOfPIsCoverRemainPart;
		delete[] idxOfRemainingDecMTs;
		delete[] numAppearCounter;
		delete[] indexOfDecimalMinterms;

		for (int i = 0; i < numberOfPotentialEPIs; i++) {
			if (potentialEPIs[i] != NULL) {
				delete[] potentialEPIs[i];
			}
		}
		if (potentialEPIs != NULL) {
			delete[] potentialEPIs;
		}

		for (int i = 0; i < numberOfRemainingMinterms; i++) {
			if (reducedPIChart[i] != NULL) {
				delete[] reducedPIChart[i];
			}
		}
		if (reducedPIChart != NULL) {
			delete[] reducedPIChart;
		}

		for (int i = 0; i < numberOfAllPIs - numberOfAllEPIs; i++) {
			if (idxOfRemainingPIs[i] != NULL) {
				delete[] idxOfRemainingPIs[i];
			}
		}
		if (idxOfRemainingPIs != NULL) {
			delete[] idxOfRemainingPIs;
		}

		if (indexOfEPIs != NULL) {
			delete[] indexOfEPIs;
		}

		if (indexOfPIs != NULL) {
			delete[] indexOfPIs;
		}

		for (int i = 0; i < numberOfVariables + 1; i++) {
			for (int j = 0; j < numberOfVariables + 1 - i; j++) {
				for (int k = 0; k < calNumberofCombination(numberOfVariables, i, j); k++) {
					if (Table[i][j][k] != NULL) {
						delete[] Table[i][j][k];
					}
				}

				if (Table[i][j] != NULL) {
					delete[] Table[i][j];
				}
			}
		}

		for (int i = 0; i < numberOfVariables + 1; i++) {
			if (Table[i] != NULL) {
				delete[] Table[i];
			}
		}

		if (Table != NULL) {
			delete[] Table;
		}

		for (int i = 0; i < minAfterFilterDuplicate; i++) {
			if (Result[i] != NULL) {
				delete[] Result[i];
			}
		}
		if (Result != NULL) {
			delete[] Result;
		}

		if (binaryMinterms != NULL) {
			delete[] binaryMinterms;
		}
	}
	else { //F = 1, xoa mang luu vi tri cua minterm
		delete[] idxOfRemainingDecMTs;
	}
}

/**
 *	Ham getIdxNumberOfOne - Lay index luu so luong so '1' trong Table
 *	@param  NULL
 *	@return int  Index luu so luong so '1' trong Table
 */
int Boole::getIdxNumberOfOne()
{
	return numberOfVariables;
}

/**
 *	Ham getIdxOriginalPosition - Lay index luu vi tri ban dau cua minterm khi chua ghep cap trong Table
 *	@param  NULL
 *	@return int  Index luu vi tri ban dau cua minterm (so thap phan duoc chuyen tu don thuc)
 */
int Boole::getIdxOriginalPosition()
{
	return numberOfVariables + 3;
}

/**
 *	Ham getIdxPositionOfX - Lay index luu vi tri trung nhau DAU TIEN cua 2 minterm trong Table
 *	@param  NULL
 *	@return int  Index luu vi tri trung nhau DAU TIEN cua 2 mintems (X duoc luu bang '2') trong Table
 */
int Boole::getIdxPositionOfX()
{
	return numberOfVariables + 2;
}

/**
 *	Ham getIdxCheckGrouped - Lay index luu gia tri kiem tra minterm da duoc goup hay khong (1 grouped, 0 not grouped) trong Table
 *	@param  NULL
 *	@return int  Index luu gia tri kiem tra minterm da duoc goup hay khong trong Table
 */
int Boole::getIdxCheckGrouped()
{
	return numberOfVariables + 1;
}

/**
 *	Ham isValidMinterm - Kiem tra tinh hop le cua minterm do nguoi dung nhap vao
 *	@param  string  Minterm do nguoi dung nhap vao
 *	@return bool    True neu minterm hop le va false neu nguoc lai
 */
bool Boole::isValidMinterm(string minterm) {
	int countV = 0; //Dem so luong bien
	int countP = 0; //Dem so luong '
	int error = 0;

	//Kiem tra minterm
	for (int i = 0; i < minterm.length(); i++) {
		if (int(minterm[i]) == 39) { // Ki tu '
			if (int(minterm[i + 1]) == 39) { //2 dau ' lien tiep
				error++;
				break;
			}
			else {
				countP++;
			}
		}
		else if (toupper(minterm[i]) > (int)'Z' || toupper(minterm[i]) < (int)'A'){ //Minterm khong phai la ki tu alphabet
			error++;
			break;
		}
		else {
			countV++;
		}

		if (toupper(minterm[i]) > 65 + numberOfVariables - 1) { //Ki tu vuot ra khoi pham vi cua so luong bien VD: ham boole 4 bien thi nhap F la khong hop le
			error++;
			break;
		}

		//Kiem tra cac bien co trung nhau hay khong
		if (minterm[i] != 39 && i < minterm.length() - 1) {
			for (int j = i + 1; j < minterm.length(); j++) {
				if (minterm[j] != 39) {
					if (minterm[i] == minterm[j] || minterm[i] > minterm[j]) {
						error++;
						break;
					}
				}
			}
		}
	}

	if (countV > numberOfVariables || countP > countV || error) {
		return false;
	}

	return true;
}

/**
 *	Ham listAllBinary - Quay lui liet ke cac chuoi nhi phan co do dai n
 *	@param  char**  Mang 2D luu tat ca cac chuoi nhi phan duoc liet ke
 *	@param  int     So luong tat ca cac chuoi duoc liet ke
 *	@param  char*   Mang 1D luu chuoi nhi phan dang duoc quay lui
 *	@param  int     Do dai chuoi nhi phan can liet ke
 *	@param  int     Buoc thu thu i (khoi tao i = 0)
 *	@return NULL 
 */
void Boole::listAllBinary(char** allBin, int& numOfAllBin, char* binTemp, int n, int i) {
	for (int k = 0; k < 2; k++) {
		binTemp[i] = k + '0';

		if (i == n - 1) {
			for (int j = 0; j < n; j++) {
				allBin[numOfAllBin][j] = binTemp[j];
			}
			numOfAllBin++;
		}
		else {
			listAllBinary(allBin, numOfAllBin, binTemp, n, i + 1);
		}
	}
}

/**
 *	Ham convertBinToDec - Chuyen chuoi nhi phan sang so thap phan
 *	@param  char*  Chuoi nhi phan can chuyen doi
 *	@param  int    So luong chu so co trong chuoi
 *	@return int    So thap phan duoc chuyen tu chuoi
 */
int Boole::convertBinToDec(char* bin, int noOfDigits) {
	int dec = 0;

	for (int i = noOfDigits - 1, j = 0; i >= 0; i--) {
		dec += (int)(bin[i] - '0') * pow(2, j++);
	}

	return dec;
}

/**
 *	Ham convertTermToDec - Chuyen chuoi term sang vi tri cua cac minterm, cac vi tri se duoc luu trong indexOfDecimalMinterms
 *	@param  string  Term do nguoi dung nhap vao
 *	@return NULL
 */
void Boole::convertTermToDec(string term) {
	int numOfAllBin = 0; //So luong chuoi nhi phan
	int countNo2s = 0; //Dem so 2
	int begin = 0;
	char* binary = new char[numberOfVariables];

	for (int i = 0; i < term.length(); i++) { //Dua tat ca ki tu ve dang in hoa
		term[i] = toupper(term[i]);
	}

	for (int i = 0; i < numberOfVariables; i++) { //Chuyen ki tu cua bien sang nhi phan, vi tri bien khong ton tai duoc luu la 2 VD ABD->1121
		for (int j = begin; j < term.length(); j++) {

			if (int(term[j]) == 65 + i) {
				if (int(term[j + 1]) == 39) {
					binary[i] = '0';
					begin += 2;
				}
				else {
					binary[i] = '1';
					begin++;
				}
			}
			break;
		}

		if (binary[i] != '1' && binary[i] != '0') {
			binary[i] = '2';
			countNo2s++;
		}
	}

	if (countNo2s) { //Neu so luong 2 != 0, thi dien 0 hoac 1 vao tri tri 2 VD: 1121 -> 1111 va 1101
		char** allBin = new char* [pow(2, countNo2s)];
		char* binTemp = new char[countNo2s];

		for (int i = 0; i < pow(2, countNo2s); i++) {
			allBin[i] = new char[numberOfVariables];
		}

		listAllBinary(allBin, numOfAllBin, binTemp, countNo2s, 0);

		//Thay the 2 bang chuoi nhi phan da duoc liet ke VD:1221 thi 2 vi tri '2' se duoc thay bang cac chuoi nhi phan co do dai la 2 -> 1101, 1111, 1011, 1001
		for (int i = 0; i < pow(2, countNo2s); i++) {
			for (int j = 0; j < numberOfVariables; j++) {
				if (binary[j] != '2') {
					for (int k = numberOfVariables - 2; k >= j; k--) {
						allBin[i][k + 1] = allBin[i][k];
					}
					allBin[i][j] = binary[j];
				}
			}
		}
		
		//Loai bo nhung vi tri trung nhau
		for (int i = 0; i < pow(2, countNo2s); i++) {
			bool isDuplicateDec = false;
			int dec = convertBinToDec(allBin[i], numberOfVariables);
			
			for (int j = 0; j < numberOfAllMinterms; j++) {
				if (dec == indexOfDecimalMinterms[j]) {
					isDuplicateDec = true;
					break;
				}
			}

			if (!isDuplicateDec) { //neu khong trung thi luu chung
				indexOfDecimalMinterms[numberOfAllMinterms++] = dec;
			}
		}

		//Xoa bo nho tam da cap phat
		for (int i = 0; i < pow(2, countNo2s); i++) {
			if (allBin[i] != NULL) {
				delete[] allBin[i];
			}
		}
		if (allBin != NULL) {
			delete[] allBin;
		}

		if (binTemp != NULL) {
			delete[] binTemp;
		}
	}
	else { //Neu so luong 2 = 0 -> cac bien ton tai day du -> chuyen nhi phan va luu chung VD: ABCD -> 1111 -> 15
		indexOfDecimalMinterms[numberOfAllMinterms++] = convertBinToDec(binary, numberOfVariables);
	}
}

/**
 *	Ham inputFunction - Cho nguoi dung nhap vao ham boole, gom nhap so luong bien, nhap cac don thuc co trong ham
 *	@param  NULL
 *	@return NULL
 */
void Boole::inputFunction() {
	int flag = 1; //Co hieu su dung cho vong lap

	do {
		cout << "- Nhap so luong bien: ";
		cin >> numberOfVariables;

		if (numberOfVariables <= 0) {
			cout << "So luong bien phai la so duong! Vui long nhap lai!" << endl;
		}
	} while (numberOfVariables <= 0);

	indexOfDecimalMinterms = new int[pow(2, numberOfVariables)];

	cin.ignore();
	cout << "\n***Don thuc duoc nhap phai theo thu tu alphabet \nva bat dau tu ki tu 'a'" << endl;
	cout << "VD: abcd, a'b la cac  don thuc hop le" << endl;
	cout << "VD: bcd, a'', bb la cac don thuc khong hop le" << endl;
	cout << "Ket thuc nhap don thuc bang phim Enter\n" << endl;

	while (flag != -1) {
		string term;

		do {
			cout << "- Nhap don thuc thu " << flag << ": ";
			getline(cin, term);

			if (term[0] == NULL) {
				flag = -1;
				break;
			}

			if (!isValidMinterm(term)) {
				cout << "*Don thuc toi thieu khong hop le! Vui long nhap lai." << endl;
			}
		} while (!isValidMinterm(term));

		if (flag != -1) {
			convertTermToDec(term);
			flag++;
		}
	}
	//Sap xep cac vi tri tang dan
	sort(indexOfDecimalMinterms, indexOfDecimalMinterms + numberOfAllMinterms, less<int>());
}

/**
 *	Ham convertAllDecToBin - Chuyen tat cac cac so thap phan trong indexOfDecimalMinterms thanh nhi phan luu trong binaryMinterms
 *	@param  NULL
 *	@return NULL
 */
void Boole::convertAllDecToBin() {
	int devide;

	for (int i = 0;i < numberOfAllMinterms;i++) {
		devide = indexOfDecimalMinterms[i];

		for (int j = numberOfVariables - 1; j >= 0; j--) {
			binaryMinterms[i][j] = devide % 2;
			devide = devide / 2;
		}
	}
}

/**
 *	Ham countNo1s - Dem so luong so '1' co trong mang nhi phan
 *	@param  int*  Mang nhi phan (duoc trich ra tu binaryMinterms)
 *	@param  int   So luong chu so co trong mang
 *	@return int   So luong so '1'
 */
int Boole::countNo1s(int* binary, int noOfDigits) {
	int count = 0;

	for (int i = 0; i <= noOfDigits - 1; i++)
	{
		if (binary[i] == 1)
			count++;
	}

	return count;
}

//Step 1
/**
 *	Ham transformAndObtainInformationFromMT - Chuyen tat ca cac minterm sang mang duoc quy uoc cach to chuc cac phan tu 
 *  (gom phan chuoi nhi phan, so luong so 1, checkGrouped, vi tri cua X, vi tri ban dau cua minterm) - tam thoi luu trong binaryMinterms
 *	@param  string  Minterm do nguoi dung nhap vao
 *	@return bool    True neu minterm hop le va false neu nguoc lai
 */
void Boole::transformAndObtainInformationFromMT() {
	binaryMinterms = new int* [numberOfAllMinterms];

	for (int i = 0; i < numberOfAllMinterms; i++) {
		binaryMinterms[i] = new int[numberOfVariables + 4];
	}
	
	convertAllDecToBin();

	for (int i = 0; i < numberOfAllMinterms; i++) {
		binaryMinterms[i][getIdxNumberOfOne()] = countNo1s(binaryMinterms[i], numberOfVariables);
		binaryMinterms[i][getIdxCheckGrouped()] = 0;
		//gan tam thoi, vi chua so sanh nen vi tri cua X se khong co
		binaryMinterms[i][getIdxPositionOfX()] = indexOfDecimalMinterms[i];
		binaryMinterms[i][getIdxOriginalPosition()] = indexOfDecimalMinterms[i];
	}
}

/**
 *	Ham calNumberofCombination - Ham tinh so luong minterm toi da (so minterm toi da co the ket hop voi nhau) co trong 1 o cua Table,
 *  ung voi moi hang, cot so luong toi da se khac nhau (cong thuc duoc chung minh trong file bao cao)
 *	@param  int  So luong bien toi da trong minterm
 *	@param  int  Vi tri cua cot trong Table
 *	@param  int  Vi tri cua hang trong cot noColumn
 *	@return int  So luong minterm toi da (so minterm toi da co the ket hop voi nhau)
 */
int Boole::calNumberofCombination(int n, int noColumn, int k) {
		int Comb;
		int NtoK = 1, Kto1 = 1;

		//n(n - 1)...(n - i - j + 1)
		for (int i = n; i >= n - k + 1 - noColumn; i--) {
			NtoK = i * NtoK;
		}

		//k!
		for (int i = k; i >= 1; i--) {
			Kto1 = i * Kto1;
		}

		Comb = NtoK / Kto1;

		return noColumn <= 1 ? Comb : 2 * Comb;
}

/**
 *	Ham initTable - Ham khoi tao bang 4D (Table) de tien hanh ghep cap va danh dau minterm
 *	@param  NULL
 *	@return NULL
 */
void Boole::initTable() {
	Table = new int***[numberOfVariables + 1];

	for (int i = 0; i < numberOfVariables + 1; i++) {
		Table[i] = new int**[numberOfVariables + 1 - i];
	}

	//Cap phat kich thuoc toi da cho Table[i][j] 
	for (int i = 0; i < numberOfVariables + 1; i++) {
		for (int j = 0; j < numberOfVariables + 1 - i; j++) {
			Table[i][j] = new int*[calNumberofCombination(numberOfVariables, i, j)];

			for (int k = 0; k < calNumberofCombination(numberOfVariables, i, j); k++) {
				Table[i][j][k] = NULL;
			}
		}
	}
}

//Step 2
/**
 *	Ham arrangeMintermsByNoOf1s - Dua minterm tu binaryMinterms vao Table[0] (cot dau tien cua bang) theo SO LUONG cua so '1'
 *	@param  NULL
 *	@return NULL
 */
void Boole::arrangeMintermsByNoOf1s() {
	initTable();

	for (int i = 0; i < numberOfVariables + 1; i++) {
		for (int j = 0, k = 0; j < numberOfAllMinterms; j++) {
			if (binaryMinterms[j][getIdxNumberOfOne()] == i) {
				Table[0][i][k++] = binaryMinterms[j];
			}
		}
	}
}

/**
 *	Ham isPowerOfTwo - Kiem tra so chinh phuong
 *	@param  int  So can kiem tra
 *	@return int  True neu la so chinh phuong va nguoc lai
 */
int Boole::isPowerOfTwo(int n) {
	return(floor(log(n) / log(2)) == (log(n) / log(2)));
}

//Step 3
/**
 *	Ham handleGrouping - Xu li qua trinh nhom cac minterm (cac minterm duoc nhom voi nhau neu thoa yeu cau trong bao cao)
 *  sau do danh dau cac minterm duoc nhom cho den khi khong nhom duoc nua (toi da la Table[numberOfVariables + 1])
 *	@param  NULL
 *	@return NULL
 */
void Boole::handleGrouping() {
	int isDuplicatedPosOfX;
	bool isGroupable = true;

	for (int i = 0; i < numberOfVariables + 1; i++) {
		if (isGroupable) {
			isGroupable = false;

			for (int j = 0; j < numberOfVariables - i; j++) {
				int position, len;
				int m = 0; //So luong phan tu o Table[i+i][j]

				//So sanh 2 nhom ke tiep nhau (chenh lenh so luong 1s la 1 don vi)
				for (int k = 0; k < calNumberofCombination(numberOfVariables, i, j); k++) {
					if (Table[i][j][k] != NULL) {
						for (int l = 0; l < calNumberofCombination(numberOfVariables, i, j + 1); l++) {
							if (Table[i][j + 1][l] != NULL
								&& Table[i][j + 1][l][getIdxOriginalPosition() + i] > Table[i][j][k][getIdxOriginalPosition() + i]
								&& isPowerOfTwo(Table[i][j + 1][l][getIdxOriginalPosition() + i] - Table[i][j][k][getIdxOriginalPosition() + i])) { //Kiem tra dieu kien de ghep cap

								isDuplicatedPosOfX = 0 - i + 1; //true if == 1 else < 1, o i = 0 thi isisDuplicatedPosOfX luon = 1 (luon dung vi chua xuat hien X nen xem nhu bo qua)
								
								//So sanh tu vi tri originalPosition
								for (int n = 1; n <= i; n++) {
									for (int p = 1; p <= i; p++) {
										if (Table[i][j + 1][l][getIdxCheckGrouped() + n] == Table[i][j][k][getIdxCheckGrouped() + p]) {
											isDuplicatedPosOfX++;
										}
									}
								}
								
								if (isDuplicatedPosOfX == 1) {
									isGroupable = true;

									//Danh dau minterm da ghep cap
									Table[i][j][k][getIdxCheckGrouped()] = 1; 
									Table[i][j + 1][l][getIdxCheckGrouped()] = 1;

									//Cap phat o cot tiep theo de tao group moi
									len = numberOfVariables + 4 + i + pow(2, i + 1);
									Table[i + 1][j][m] = new int[len];

									for (int n = 0; n <= numberOfVariables + 1 + i; n++) {
										Table[i + 1][j][m][n] = Table[i][j][k][n];
									}

									Table[i + 1][j][m][numberOfVariables + 3 + i] = Table[i][j][k][getIdxPositionOfX() + i];

									for (int n = numberOfVariables + 4 + i; n < len; n++) {
										Table[i + 1][j][m][n] = 0;
									}

									//Tim vi tri sai khac (vi tri cua X)
									position = log((Table[i][j + 1][l][getIdxPositionOfX() + i] - Table[i][j][k][getIdxPositionOfX() + i])) / log(2);

									Table[i + 1][j][m][numberOfVariables - 1 - position] = 2;
									Table[i + 1][j][m][getIdxCheckGrouped()] = 0;
									Table[i + 1][j][m][getIdxPositionOfX() + i] = position;

									//Sao chep vi tri cua minterm o cot j vao minterm moi
									for (int p = 0; p < pow(2, i); p++) {										
										Table[i + 1][j][m][numberOfVariables + 4 + i + p] = Table[i][j][k][numberOfVariables + 3 + i + p];
									}

									//Sao chep vi tri cua mintem o cot j + 1 vaoi mintem moi
									for (int p = pow(2, i); p < pow(2, i + 1); p++) {										
										Table[i + 1][j][m][numberOfVariables + 4 + i + p] = Table[i][j + 1][l][numberOfVariables + 3 + i + p - (int)pow(2, i)];
									}
									
									m++;
								}
							}
						}
					}
				}
			}
		}
	}
}

//Step 4
/**
 *	Ham handlePIs - Chon PIs tu Table (chon nhung minterm co checkGrouped == 0) 
 *  bang cach luu index cua PIs vao indexOfPIs, dong thoi dem so lan xuat hien cac vi tri cua minterm trong PIs
 *	@param  NULL
 *	@return NULL
 */
void Boole::handlePIs() {	
	int isDuplicatedPI;

	//Khoi tao so bo dem so vi tri xuat hien
	numAppearCounter = new int[pow(2, numberOfVariables)];

	for (int i = 0; i < pow(2, numberOfVariables); i++) {
		numAppearCounter[i] = 0;
	}

	indexOfPIs = new int*[numberOfAllMinterms];

	for (int i = 0; i < numberOfAllMinterms; i++) {
		indexOfPIs[i] = new int[3]; //3 vi gom co i, j, k de truy xuat den minterm
	}

	//Loc cac PI trung nhau
	for (int i = 0; i < numberOfVariables + 1; i++) {
		for (int j = 0; j < numberOfVariables + 1 - i; j++) {
			for (int k = 0; k < calNumberofCombination(numberOfVariables, i, j); k++) {
				if (Table[i][j][k] != NULL && Table[i][j][k][getIdxCheckGrouped()] == 0) {
					isDuplicatedPI = 0 - pow(2, i) + 1;

					for (int l = k - 1; l >= 0; l--) {
						if (isDuplicatedPI != 1) {
							isDuplicatedPI = 0 - pow(2, i) + 1;

							for (int m = 0; m < pow(2, i); m++) {
								for (int n = 0; n < pow(2, i); n++) {
									if (Table[i][j][l][getIdxOriginalPosition() + i + m] == Table[i][j][k][getIdxOriginalPosition() + i + n]) {
										isDuplicatedPI++;
									}
								}
							}
						}
					}

					if (isDuplicatedPI != 1) { //Khong trung nhau
						indexOfPIs[numberOfAllPIs][0] = i;
						indexOfPIs[numberOfAllPIs][1] = j;
						indexOfPIs[numberOfAllPIs][2] = k;

						numberOfAllPIs++;

						for (int l = 0; l < pow(2, i); l++) { //Dem so lan xuat hien cua cac vi tri trong PI
							numAppearCounter[Table[i][j][k][getIdxOriginalPosition() + i + l]]++;
						}
					}
				}
			}
		}
	}
}

//Step 5
/**
 *	Ham findEPIs - Tim EPIs bang cach chon nhung PIs co vi tri minterm xuat hien la 1
 *	@param  NULL
 *	@return NULL
 */
void Boole::findEPIs() {

	indexOfEPIs = new int* [numberOfAllMinterms];

	//Tim PI co vi tri chi duoc danh dau 1 lan lam EPI
	for (int i = 0; i < pow(2, numberOfVariables); i++) {
		if (numAppearCounter[i] == 1) {
			for (int j = 0;j < numberOfAllPIs; j++) {
				for (int k = 0; k < pow(2, indexOfPIs[j][0]); k++) {
					if (Table[indexOfPIs[j][0]][indexOfPIs[j][1]][indexOfPIs[j][2]][getIdxOriginalPosition() + indexOfPIs[j][0] + k] == i) {
						indexOfEPIs[numberOfAllEPIs] = indexOfPIs[j];

						for (int l = 0; l < pow(2, indexOfPIs[j][0]); l++) {
							numAppearCounter[Table[indexOfPIs[j][0]][indexOfPIs[j][1]][indexOfPIs[j][2]][getIdxOriginalPosition() + indexOfPIs[j][0] + l]] = 0;
						}

						numberOfAllEPIs++;
						break;
					}
				}
			}
		}
	}
}

//Step 6
/**
 *	Ham handleRemainPIs - Xu li PIs con thua sau khi chon EPIs, neu khong PIs con thua (numberOfRemainingPIs == 0) thi chuyen den buoc in ket qua
 *	@param  NULL
 *	@return NULL
 */
void Boole::handleRemainPIs() {
	//Dem so minterm con lai
	for (int i = 0; i < pow(2, numberOfVariables); i++) {
		if (numAppearCounter[i] != 0) {
			numberOfRemainingMinterms++;
		}
	}

	idxOfRemainingDecMTs = new int[numberOfRemainingMinterms];

	for (int i = 0; i < numberOfRemainingMinterms; i++) {
		idxOfRemainingDecMTs[i] = -1;
	}

	idxOfRemainingPIs = new int* [numberOfAllPIs - numberOfAllEPIs];

	for (int i = 0; i < numberOfAllPIs - numberOfAllEPIs; i++) {
		idxOfRemainingPIs[i] = NULL;
	}

	for (int i = 0, j = 0; j < pow(2, numberOfVariables); j++) {
		if (numAppearCounter[j] != 0) {
			idxOfRemainingDecMTs[i] = j;
			i++;
		}
	}
	//Dem so luong PI con lai
	for (int i = 0; i < numberOfAllPIs; i++) {
		for (int j = 0;j < pow(2, indexOfPIs[i][0]);j++) {
			if (numAppearCounter[Table[indexOfPIs[i][0]] [indexOfPIs[i][1]] [indexOfPIs[i][2]] [getIdxOriginalPosition() + indexOfPIs[i][0] + j]] != 0) {
				idxOfRemainingPIs[numberOfRemainingPIs] = indexOfPIs[i];
				numberOfRemainingPIs++;
				break;
			}
		}
	}
}

//Step 7
/**
 *	Ham generateReducedPIChart - Tao bang danh dau PI rut gon va danh dau cac PI vao do, 1 la ticked va 0 nguoc lai.
 *	@param  NULL
 *	@return NULL
 */
void Boole::generateReducedPIChart() {

	reducedPIChart = new int* [numberOfRemainingMinterms];

	for (int i = 0; i < numberOfRemainingMinterms; i++) {
		reducedPIChart[i] = new int[numberOfRemainingPIs];
	}

	for (int i = 0; i < numberOfRemainingMinterms; i++) {
		for (int j = 0; j < numberOfRemainingPIs; j++) {
			reducedPIChart[i][j] = 0;
		}
	}

	//Danh dau vao reducedPIChart 1 tuc la duoc chon va nguoc lai
	for (int i = 0; i < numberOfRemainingPIs; i++) {
		for (int j = 0; j < pow(2, idxOfRemainingPIs[i][0]); j++) {
			for (int k = 0; k < numberOfRemainingMinterms; k++) {
				if (Table[idxOfRemainingPIs[i][0]][idxOfRemainingPIs[i][1]][idxOfRemainingPIs[i][2]][getIdxOriginalPosition() + idxOfRemainingPIs[i][0] + j] == idxOfRemainingDecMTs[k]) {
					reducedPIChart[k][i] = 1;
				}
			}
		}
	}
}

/**
 *	Ham listIdxPIsCanBeEPIs - Quay lui Liet ke tat cac cac PIs co the duoc chon lam EPIs
 *	@param  int  Chieu dai toi da cua so minterm con lai (tinh tu 0)
 *	@return NULL
 */
void Boole::listIdxPIsCanBeEPIs(int maxIdxRemainMT) {
	int idxOfRemainMT = maxIdxRemainMT; //So luong minterm con lai - 1

	for (idxOfPIsCoverRemainPart[idxOfRemainMT] = 0; idxOfPIsCoverRemainPart[idxOfRemainMT] < numberOfRemainingPIs; idxOfPIsCoverRemainPart[idxOfRemainMT]++) {
		if (reducedPIChart[numberOfRemainingMinterms - 1 - idxOfRemainMT][idxOfPIsCoverRemainPart[idxOfRemainMT]]) { //Neu vi tri duoc danh danh 1 tren Reduced PI Chart
			if (idxOfRemainMT > 0) { //Quay lui tim cac vi tri tiep theo de phu phan con lai
				maxIdxRemainMT = idxOfRemainMT;
				maxIdxRemainMT--;
				listIdxPIsCanBeEPIs(maxIdxRemainMT);
			}
			else if (idxOfRemainMT == 0) {
				for (int i = 0; i < numberOfRemainingMinterms; i++) { //vi tri cac PI phu phan con lai duoc xem la potential EPI
					potentialEPIs[numberOfPotentialEPIs][i] = idxOfPIsCoverRemainPart[numberOfRemainingMinterms - 1 - i];
				}
				numberOfPotentialEPIs++;
			}
		}
	}
}

//Step 8
/**
 *	Ham selectEPIsFromReducedPIChart - Chon EPIs tu bang danh dau PI rut gon bang cach chon nhung EPIs tiem nang co so luong PIs IT NHAT
 *	@param  NULL
 *	@return NULL
 */
void Boole::selectEPIsFromReducedPIChart() {
	
	//Tinh so luong EPI toi da co the co
	for (int i = 0; i < numberOfRemainingMinterms; i++) {
		numberOfPossibleEPIs *= numAppearCounter[idxOfRemainingDecMTs[i]];
	}

	potentialEPIs = new int*[numberOfPossibleEPIs];

	for (int i = 0; i < numberOfPossibleEPIs; i++) {
		potentialEPIs[i] = new int[numberOfRemainingMinterms];
	}

	idxOfPIsCoverRemainPart = new int[numberOfRemainingMinterms];

	for (int i = 0;i < numberOfRemainingMinterms; i++) {
		idxOfPIsCoverRemainPart[i] = -1;
	}

	listIdxPIsCanBeEPIs(numberOfRemainingMinterms - 1);

	numberOfPIsForEPI = new int[numberOfPossibleEPIs];

	for (int i = 0; i < numberOfPossibleEPIs; i++) {
		numberOfPIsForEPI[i] = 0;
	}
	//Loc cac potential EPI co index PI trung nhau
	for (int i = 0;i < numberOfPossibleEPIs;i++) {
		for (int j = 0;j < numberOfRemainingMinterms;j++) {
			if (potentialEPIs[i][j] != -1) {
				numberOfPIsForEPI[i]++;
				//Neu phan tu phia sau bi trung, no se duoc chuyen thanh -1, VD: 0 2 2 -> 0 2 -1
				for (int k = j + 1;k < numberOfRemainingMinterms; k++) {
					if (potentialEPIs[i][k] == potentialEPIs[i][j]) {
						potentialEPIs[i][k] = -1;
					}
				}
			}
		}
	}

	minNumberOfPIs = numberOfRemainingMinterms;
	idxOfMinNumOfPIs = new int[numberOfPossibleEPIs];

	//Tim so luong PI phu phan co lai it nhat (*)
	for (int i = 1; i < numberOfPossibleEPIs; i++) {
		if (numberOfPIsForEPI[i] < minNumberOfPIs) {
			minNumberOfPIs = numberOfPIsForEPI[i];
		}
	}
	
	//Tim index thoa dieu kien tren (*)
	for (int j = 0; j < numberOfPossibleEPIs; j++) {
		if (numberOfPIsForEPI[j] == minNumberOfPIs) {
			idxOfMinNumOfPIs[numberOfMinNumPIs++] = j;
		}
	}

	//Sap xep cac index tang dan de loc trung nhau
	for (int i = 0; i < numberOfMinNumPIs; i++) {
		sort(potentialEPIs[idxOfMinNumOfPIs[i]], potentialEPIs[idxOfMinNumOfPIs[i]] + numberOfRemainingMinterms, less<int>());
	}

	minAfterFilterDuplicate = numberOfMinNumPIs;

	//Loc cac vi tri trung nhau
	for (int i = 0; i < numberOfMinNumPIs - 1; i++) {
		for (int j = i + 1; j < numberOfMinNumPIs; j++) {
			if (idxOfMinNumOfPIs[i] != -1 && idxOfMinNumOfPIs[j] != -1) {
				int isDuplicatedEPI = numberOfRemainingMinterms + 1;

				for (int k = 0; k < numberOfRemainingMinterms; k++) {
					for (int l = 0; l < numberOfRemainingMinterms; l++) {
						if (potentialEPIs[idxOfMinNumOfPIs[i]][k] == potentialEPIs[idxOfMinNumOfPIs[j]][l]) {
							isDuplicatedEPI--;
						}
					}
				}

				if (isDuplicatedEPI == 1) { //Neu trung nhau thi gan no = -1 (bo di), va giam so luong so index it nhat phu phan con lai
					idxOfMinNumOfPIs[j] = -1;
					minAfterFilterDuplicate--;
				}
			}
		}
	}
}

//Step 9
/**
 *	Ham generateResult - Ket hop cac EPIs ban dau voi EPIs vua chon de tao ket qua cuoi cung
 *	@param  NULL
 *	@return NULL
 */
void Boole::generateResult() {
	int row = 0;
	Result = new int** [minAfterFilterDuplicate];

	//Ket qua la so EPI chon dau + so EPI chon sau
	for (int i = 0; i < minAfterFilterDuplicate; i++) {
		Result[i] = new int* [numberOfAllEPIs + minNumberOfPIs];
	}
	
	//Sao chep EPI da chon ban dau
	for (int i = 0; i < minAfterFilterDuplicate; i++) {
		for (int j = 0; j < numberOfAllEPIs; j++) {
			Result[i][j] = indexOfEPIs[j];
		}
	}
	//Sao chep EPI chon sau
	for (int j = 0; j < numberOfMinNumPIs; j++) {
		if (idxOfMinNumOfPIs[j] != -1) {
			int posOfLastIdxEPI = numberOfAllEPIs;

			for (int i = 0;i < numberOfRemainingMinterms; i++) {
				if (potentialEPIs[idxOfMinNumOfPIs[j]][i] != -1) {
					Result[row][posOfLastIdxEPI++] = idxOfRemainingPIs[potentialEPIs[idxOfMinNumOfPIs[j]][i]];
				}
			}
			row++;
		}
	}

	numberOfAllEPIs += minNumberOfPIs;
}

/**
 *	Ham findAllBoolFunction - Thiet lap cac buoc xu li de tim cac cong thuc da thuc toi thieu cua ham boole o inputFunction
 *  Neu so luong minterm == 2 ^ numberOfVariables tuc la tat cac cac minterm da phu het toan bo bang -> F = 1 (chan tri cua F luon luon dung)
 *  Neu numberOfRemainingPIs == 0 sau khi chon EPIs tuc la da tim duoc ket qua -> tien hanh in ket qua
 *  Nguoc lai numberOfRemainingPIs != 0 can tien hanh them cac buoc dem tim cac EPIs tu PIs con thua sau do moi in ket qua
 *	@param  NULL
 *	@return NULL
 */
void Boole::findAllBoolFunction() {
	
	if (numberOfAllMinterms != pow(2, numberOfVariables)) { //Cac minterm khong phu toan bo cac o
		transformAndObtainInformationFromMT();
		arrangeMintermsByNoOf1s();
		handleGrouping();
		handlePIs();
		findEPIs();
		handleRemainPIs();

		if (numberOfRemainingPIs != 0) { //Neu con PI thua
			generateReducedPIChart();
			selectEPIsFromReducedPIChart();
			generateResult();

			cout << "\n> Cac cong thuc da thuc toi thieu cua ham boole da cho la: " << endl;

			for (int i = 0; i < minAfterFilterDuplicate; i++) {
				cout << "- F = ";
				for (int x = 0;x < numberOfAllEPIs;x++) {
					for (int y = 0; y < numberOfVariables; y++) {
						if (Table[Result[i][x][0]][Result[i][x][1]][Result[i][x][2]][y] == 1) {
							cout << char(65 + y);
						}
						else if (Table[Result[i][x][0]][Result[i][x][1]][Result[i][x][2]][y] == 0) {
							cout << char(65 + y) << "'";
						}
					}
					if (x < numberOfAllEPIs - 1) {
						cout << " + ";
					}
				}
				cout << endl;
			}
		}
		else { //Khong co PI thua
			cout << "\n> Cong thuc da thuc toi thieu cua ham boole da cho la: " << endl;
			cout << "- F = ";

			for (int x = 0;x < numberOfAllEPIs;x++) {
				for (int y = 0;y < numberOfVariables;y++) {
					if (Table[indexOfEPIs[x][0]][indexOfEPIs[x][1]][indexOfEPIs[x][2]][y] == 1) {
						cout << char(65 + y);
					}
					else if (Table[indexOfEPIs[x][0]][indexOfEPIs[x][1]][indexOfEPIs[x][2]][y] == 0) {
						cout << char(65 + y) << "'";
					}
				}
				if (x < numberOfAllEPIs - 1) {
					cout << " + ";
				}
			}
			cout << endl;
		}
	}
	else { //Cac minterm phu toan bo cac o
		cout << "\n> Cong thuc da thuc toi thieu cua ham boole da cho la: " << endl;
		cout << "- F = 1" << endl;
	}
}