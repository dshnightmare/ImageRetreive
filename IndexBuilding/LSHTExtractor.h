#ifndef _LSHTExtractor_H 
#define _LSHTExtractor_H

#include "LSHTable.h"

class LSHTExtractor
{
private:
	int tableNum;
	
public:
	LSHTable *tableArray;
	LSHTExtractor(ifstream &fin);//cur
	LSHTExtractor(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh);//test
	LSHTExtractor(LSHTable *tabArray,int tabNum);
	
	void printTables();
	
	vector<int> getCandIDset(double *dataVec,int maxEDist);
	vector<int> LSHTExtractor::getCandIDsetf(double *dataVec,int maxEDist);
	vector<int> getCandIDset(double *dataVec,int maxEDist,bool useThresh);
	vector<int> getCandIDs(double *dataVec,int maxEDist);
	vector<int> getCandIDsf(double *dataVec,int maxEDist);
	void getTabSize(int* tSize);//only for builder
	void ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double lthresh,double uthresh,int maxIter);
	vector<int> resultFilter(vector<int>* candSetList,double uth);
};
/*
vector<int> extractor(int fid,ImageFeature imgfeat)
{
	string lshFName=getlshFName(fid);
	ifstream fin(lshFName);
	vector<int> res;
	if(!fin)
		return res;//cout<<"File open failed!"<<endl;
	LSHTExtractor exct=LSHTExtractor(fin);
	fin.close();
	int vecLength=imgfeat.getlength(fid);
	double *dataVec=imgfeat.getFeat(fid);//problem: already alloc ?
	int maxEDist[6]={1,1,1,1,1,1};
	bool useThresh[6]={0,0,0,0,0,0};
	res=exct.getCandIDs(dataVec,maxEDist[fid]);
	return res;
}
*/
#endif
