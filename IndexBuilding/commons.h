#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include<cv.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime> 
#include <algorithm>
#include <functional>
#include <random>
#include <math.h>
#include <stdlib.h>

using namespace std;

typedef unordered_map<string,vector<int>> Table; 
typedef pair<string,vector<int>> TableTerm; 
typedef Table::iterator TableIter;

struct dAndI
{
	double value;
	int idx;
};


double * allocDoubleVector(int col,double initValue);

double ** allocDoubleVecList(int row,int col,double initValue);

void getNDVector(double *vec,int vecLen,double *muVec,double *sigVec);

void getStdNDVector(double *vec,int vecLen);

void getNDVectorList(double **vecList,int row,int vecLen,double *muVec,double *sigVec);

double getBasicLSH(double *dataVec,double *stdndVec,int vecLen);

int maxOfArray(int *vec,int vecLen);

int sumOfArray(int *vec,int vecLen);

double sumOfArray(double *vec,int vecLen);

double avgOfArray(int *vec,int vecLen);

double avgOfArray(double *vec,int vecLen);

void ajustTowardsAvg(int *vec,int vecLen,double lthresh,double uthresh,int maxIter);

void unionSortedIntVec(vector<int> vec1,vector<int> vec2,vector<int>& gVec);

void intersectSortedIntVec(vector<int> vec1,vector<int> vec2,vector<int>& gVec);

int cmp(const void* it1, const void* it2);

int dAndICmp(const void* it1, const void* it2);

void sortDataAndIdx(double *vec,int *sortedIdx,int vecLen);

int countBeyondThresh(double *vec,int vecLen,double thresh);

void get0DIgnStringSet(string bKey,int *ignTag,int ignNum,string *res);

void get1DIgnStringSet(string bKey,int *ignTag,int ignNum,string *res);

void get2DIgnStringSet(string bKey,int *ignTag,int ignNum,string *res);
//***************** distance-calculating functions **********************//

int hammingDist(string str1,string str2);

int hammingDist(string str1,string str2,int* ignTag,int keyLen);

double dist(double *vec1,double *vec2,int vecLen);

vector<int> distOrderToOthers(double **vecList,int row,int col,int id,double pcnt);

double minDistToOthers(double **vecList,int row,int col,int id,int &nnId);

double minorAvgDistToOthers(double **vecList,int row,int col,int id,double thresh);

double qntDistToOthers(double **vecList,int row,int col,int id,double thresh);

double avgDistToOthers(double **vecList,int row,int col,int id);

//***************** printing functions **********************//

void readVectorFromFile(vector<int> &vec,ifstream &fin);

void writeVectorToFile(vector<int> vec,ofstream &fout);

void readDArrayFromFile(double *vec,int vecLen,ifstream &fin);

void writeDArrayToFile(double *vec,int vecLen,ofstream &fout);

void readDArrayListFromFile(double** &vecList,int &row,int &vecLen,ifstream &fin);

void writeDArrayListToFile(double ** vecList,int row,int vecLen,ofstream &fout);

void printInt(int i);

void printlnVector(string *vec,int vecLen);

void printVector(vector<int> vec);

void printVector(double *vec,int vecLen);

void printVector(int *vec,int vecLen);

void printVectorList(double **VectorList,int row,int col);

//***************** other functions **********************//
string getlshFName(int fid);
