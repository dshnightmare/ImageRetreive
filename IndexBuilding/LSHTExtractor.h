#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include "../ImageFeature/Features.h"
#include "LSHTable.h"
#include "LSHTBuilder.h"

class LSHTExtractor
{
private:
	int tableNum;
	
public:
	LSHTable *tableArray;
	LSHTExtractor(ifstream &fin);//cur
	LSHTExtractor(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,double itcptRate);
	//test for itcptRate
	LSHTExtractor(LSHTable *tabArray,int tabNum);
	LSHTExtractor(LSHTBuilder bld);//API
	
	~LSHTExtractor();
	void printTables();
	
	vector<int> getCandIDset(double *dataVec,int maxEDist);
	vector<int> getCandIDsetf(double *dataVec,int maxEDist);
	vector<int> getCandIDseta(double *dataVec,int maxEDist);
	vector<int> getCandIDset(double *dataVec,int maxEDist,bool useThresh);
	vector<int> getCandIDsetf(double *dataVec,int maxEDist,bool useThresh);
	vector<int> getCandIDseta(double *dataVec,int maxEDist,bool useThresh);
	vector<int> getCandIDs(double *dataVec,int maxEDist);
	vector<int> getCandIDsf(double *dataVec,int maxEDist);
	vector<int> getCandIDss(double *dataVec,int maxEDist);
	vector<int> getCandIDssf(double *dataVec,int maxEDist);
	void getTabSize(int* tSize);//only for builder
	void ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double itcptRate,double lthresh,double uthresh,int maxIter);
	vector<int> resultFilter(vector<int>* candSetList,double lth,double uth);
	vector<int> resultAssembler(vector<int>* candSetList);//pair-cross&union
};


DLLEXPORT vector<int> Extractor(LSHTBuilder bld,int fid,ImageFeature &imgfeat);
typedef vector<int> (*PExtractor)(LSHTBuilder bld,int fid,ImageFeature &imgfeat);