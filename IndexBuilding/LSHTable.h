#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

#include "stdafx.h"
#include "commons.h"


#define MAX_KEYLEN 60
#define MAX_VECLEN 260

class LSHTable
{
private:
	Table tab;
	int keyLen;
	int dataNum,vecLen;//input data is dataNum*vecLen double 2D-array 
	double stdndVecList[MAX_KEYLEN][MAX_VECLEN];//keyLen hash vectors with length vecLen each.(keyLen*vecLen)
	double ignThresh;//bits with |av-intercept|<ignThresh ignored
	double intercept;//use super-plane av=intercept
	
public:
	LSHTable();
	LSHTable(ifstream &fin);//read an existing table from file
	LSHTable(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double itcptRate);
	//create table from data matrix
	Table getTable();
	int getTableSize();
	void printTable();
	void writeToFile(ofstream &fout);
	void readFromFile(ifstream &fin);

	void setIntercept(double itcptRate);
	void getAV(double *dataVec,double* av);
	string getLSHKey(double *dataVec);
	int getIgnTag(double *dataVec,int* ignTag);
	vector<int> getCandIDset(double *dataVec,int maxEDist);//nth&ngen
	vector<int> getCandIDset(double *dataVec,int maxEDist,bool useThresh);//th&ngen
	vector<int> getCandIDs(double *dataVec,int maxEDist);//nth&gen
	vector<int> getCandIDss(double *dataVec,int maxEDist);//th&gen
	vector<int> get0EDistIDs(double *dataVec);
	vector<int> get1EDistIDs(double *dataVec);
	vector<int> getId4KeySet(string *candKeys,int keyNum);
	void get1EDistKeys(double *dataVec,string *ED1Keys);
	void get2EDistKeys(double *dataVec,string *ED2Keys);
	void get3EDistKeys(double *dataVec,string *ED3Keys);
	//return 1-edit-distance keys in array ED1Keys,its length will be (keyLen)

	int getKeysEDist(double *vec1,double *vec2,int vecLength);//only for debugging
	//get edit distance of two vector's keys
	void getAvgStdndVec(double *avgVec);//only for debugging
	void getItermSize(int* tSize,int &tNum);//only for debugging
};
