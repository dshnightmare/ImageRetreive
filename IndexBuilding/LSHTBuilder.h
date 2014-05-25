#ifndef _LSHTBuilder_H 
#define _LSHTBuilder_H

#include "LSHTable.h"

class LSHTBuilder
{
private:
	LSHTable *tableArray;
	int tableNum;
	
public:
	LSHTBuilder();
	LSHTBuilder(ifstream &fin,int keyLen,int tabNum);
	LSHTBuilder(ifstream &fin,int keyLen,int tabNum,double thresh);//cur
	LSHTBuilder(ifstream &fin,int keyLen,int tabNum,double thresh,ofstream &fout);
	LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh);
	LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,ofstream &fout);
	//LSHTBuilder(ifstream &fin);
	LSHTBuilder(LSHTable *tabArray,int tabNum);
	
	void writeToFile(ofstream &fout);
	void getTabSize(int* tSize);
	void ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double lthresh,double thresh,double uthresh,int maxIter);
};
///*
void builder(ImageFeature *imgFeatArray,int dataNum)//main();
{
	
	int keyLen[6]={20,20,20,20,20,20};
	int tabNum[6]={20,20,20,20,20,20};
	double thresh[6]={0.5,0.5,0.5,0.5,0.5,0.5};
	for(int fid=1;fid<=MAX_FEAT_ID;fid++)
	{
		string lshFName=getlshFName(fid);//"E:/lshTable(fid).txt";
		ifstream fin(lshFName);
		if(fin)//cur index file exists
		{
			continue;
			fin.close();
		}
		else
		{
			ofstream fout(lshFName);
			int vecLen=getlength(fid);
			double** vecList=allocDoubleVecList(dataNum,vecLen,0);
			for(int i=0;i<dataNum;i++)
				vecList[dataNum]=imgFeatArray[dataNum].getFeat(fid);
			//LSHTBuilder bld=LSHTBuilder(fin,keyLen[fid],tabNum[fid],thresh[fid]);
			//LSHTBuilder bld=LSHTBuilder(fin,keyLen[fid],tabNum[fid],thresh[fid],"E:/"+to_string(fid)+".txt");
			LSHTBuilder bld=LSHTBuilder(vecList,dataNum,vecLen,keyLen[fid],tabNum[fid],thresh[fid],fout);
			fout.close();
		}
	}
	return;
}
#endif
