#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include "commons.h"

class LSHTable
{
private:
	Table tab;
	int keyLen;
	int dataNum,vecLen;//input data is dataNum*vecLen double 2D-array 
	double **stdndVecList;//keyLen hash vectors with length vecLen each.(keyLen*vecLen)
	double mu,sigma;//estimated mu and sigma of each dimension of the data.
	//Default(mu,sigma)=(0.5,0.15)(for nomalized data,cos [mu-3sigma,mu+3sigma]->[0,1])
	//sigma(av)=(vecLen*(mu^2+sigma^2))^0.5;(as the scale-measurement for av)
	double ignThresh;//bits with |av|<ignThresh ignored
	double threshRatio;//default as 0.5
	//ignThresh=threshRatio*sigma(av)
	
	
public:
	LSHTable();
	LSHTable(ifstream &fin);//read an existing table from file
	LSHTable(ifstream &fin,int keyLength);//read dataVecList from file and create a table
	LSHTable(double **dataVecList,int dataNumber,int vecLength,int keyLength);
	LSHTable(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh);
	LSHTable(double **dataVecList,int dataNumber,int vecLength,double u,double sig,double thrRatio,int keyLength);
	
	Table getTable();
	int getTableSize();
	void printTable();
	void writeToFile(ofstream &fout);
	void readFromFile(ifstream &fin);

	void setThreshRatio(double thrRatio);
	void getAV(double *dataVec,double* av);
	string getLSHKey(double *dataVec);
	int getIgnTag(double *dataVec,int* ignTag);
	vector<int> getCandIDset(double *dataVec,int maxEDist);
	vector<int> getCandIDset(double *dataVec,int maxEDist,bool useThresh);
	vector<int> getCandIDs(double *dataVec,int maxEDist);
	vector<int> getCandIDss(double *dataVec,int maxEDist);
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
