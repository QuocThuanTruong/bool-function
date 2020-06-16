#ifndef _BOOL_H
#define _BOOL_H
#pragma once

/**
 *	BOOLE FUNCTION
 *	@author	Quoc Thuan Truong		18120583
 *	@since 2019-10-18
 */

#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

class Boole {
private:
	int numberOfVariables;
	int numberOfAllMinterms;
	int numberOfAllPIs;
	int numberOfAllEPIs;
	int numberOfRemainingMinterms;
	int numberOfRemainingPIs;
	int numberOfPossibleEPIs;
	int numberOfPotentialEPIs;
	int minNumberOfPIs;
	int numberOfMinNumPIs;
	int minAfterFilterDuplicate;
private:
	int* indexOfDecimalMinterms;
	int** binaryMinterms;
	int**** Table;
	int** indexOfPIs;
	int** indexOfEPIs;
	int* numAppearCounter;
	int** idxOfRemainingPIs;
	int* idxOfRemainingDecMTs;
	int** reducedPIChart;
	int* idxOfPIsCoverRemainPart;
	int** potentialEPIs;
	int* numberOfPIsForEPI;
	int* idxOfMinNumOfPIs;
	int*** Result;
private:
	int getIdxNumberOfOne();
	int getIdxOriginalPosition();
	int getIdxPositionOfX();
	int getIdxCheckGrouped();
private:
	bool isValidMinterm(string minterm);
	void listAllBinary(char** allBin, int& numOfAllBin, char* binTemp, int n, int i);
	int convertBinToDec(char* bin, int noOfDigits);
	void convertTermToDec(string minterm);
private:
	void convertAllDecToBin();
	int countNo1s(int* binary, int noOfDigits);
	void transformAndObtainInformationFromMT();
private:
	int calNumberofCombination(int n, int noColumn, int k);
	void initTable();
	void arrangeMintermsByNoOf1s();
private:	
	int isPowerOfTwo(int n);
	void handleGrouping();
private:
	void handlePIs();
private:
	void findEPIs();
private:
	void handleRemainPIs();
private:
	void generateReducedPIChart();
private:
	void listIdxPIsCanBeEPIs(int maxIdxRemainMT);
	void selectEPIsFromReducedPIChart();
private:
	void generateResult();
public:
	Boole();
	~Boole();
public:
	void inputFunction();
	void findAllBoolFunction();
};

#endif // !_BOOL_H
