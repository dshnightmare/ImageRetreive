#include "LSHTBuilder.h"

LSHTBuilder::LSHTBuilder()
{
}

LSHTBuilder::LSHTBuilder(ifstream &fin,int keyLen,int tabNum)
{
	//fin.seekg(0,ios::beg);
	tableNum=tabNum;
	tableArray = new LSHTable[tableNum];
	double **dataVecList=NULL;
	int dataNum,vecLen;
	readDArrayListFromFile(dataVecList,dataNum,vecLen,fin);

	for (int i=0; i<tableNum; i++) 
	{
		tableArray[i] = LSHTable(dataVecList,dataNum,vecLen,keyLen);
		//tableArray[i].printTable();
	}
}

LSHTBuilder::LSHTBuilder(ifstream &fin,int keyLen,int tabNum,double thresh)
{
	//fin.seekg(0,ios::beg);
	tableNum=tabNum;
	tableArray = new LSHTable[tableNum];
	double **dataVecList=NULL;
	int dataNum,vecLen;
	readDArrayListFromFile(dataVecList,dataNum,vecLen,fin);

	for (int i=0; i<tableNum; i++) 
	{
		tableArray[i] = LSHTable(dataVecList,dataNum,vecLen,keyLen,thresh);
		//tableArray[i].printTable();
	}
	//**ajusting
	double lthresh=0.8;
	double uthresh=3;
	int maxIter=5;
	ajustTowardsAvg(dataVecList,dataNum,vecLen,keyLen,thresh,lthresh,uthresh,maxIter);
}


LSHTBuilder::LSHTBuilder(ifstream &fin,int keyLen,int tabNum,double thresh,ofstream &fout)
{
}

LSHTBuilder::LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh)
{
	tableNum=tabNum;
	tableArray = new LSHTable[tableNum];
	for (int i=0; i<tableNum; i++) 
		tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh);
	//**ajusting
	double lthresh=0.8;
	double uthresh=3;
	int maxIter=5;
	ajustTowardsAvg(dataVecList,dataNumber,vecLength,keyLength,thresh,lthresh,uthresh,maxIter);
}

LSHTBuilder::LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,ofstream &fout)
{	
	tableNum=tabNum;
	tableArray = new LSHTable[tableNum];
	for (int i=0; i<tableNum; i++) 
		tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh);
	//**ajusting
	double lthresh=0.8;
	double uthresh=3;
	int maxIter=5;
	ajustTowardsAvg(dataVecList,dataNumber,vecLength,keyLength,thresh,lthresh,uthresh,maxIter);
	writeToFile(fout);
}

LSHTBuilder::LSHTBuilder(LSHTable *tabArray,int tabNum)
{
	tableArray=tabArray;
	tableNum=tabNum;
}
	
void LSHTBuilder::writeToFile(ofstream &fout)
{
	fout.seekp(0,ios::beg);
	fout<<tableNum<<endl;
	for (int i=0; i<tableNum; i++) 
		tableArray[i].writeToFile(fout);

}

void LSHTBuilder::getTabSize(int* tSize)
{
	for (int i=0; i<tableNum; i++) 
		tSize[i]=tableArray[i].getTableSize();
	return;
}

void LSHTBuilder::ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double lthresh,double uthresh,int maxIter)
{
	int it=0;
	int* tSize=new int[tableNum];
	getTabSize(tSize);
	bool nend=true;
	double orilt=lthresh;
	int maxfiter=3;
	while((it<maxIter)&&(nend))
	{
		//printVector(tSize,tableNum);
		nend=false;
		if(maxIter>1)
			lthresh-=(orilt-0.5)/(maxIter-1.0);
		double lth=lthresh*avgOfArray(tSize,tableNum);
		double uth=uthresh*avgOfArray(tSize,tableNum);
		///*
		for (int i=0; i<tableNum; i++) 
		{
			if(tableArray[i].getTableSize()<lth)
			{
				nend=true;
				int iter=0;
				while((iter<maxfiter)&&(tableArray[i].getTableSize()<lth))
				{
					tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh);	
					iter++;
				}
			}
			if(tableArray[i].getTableSize()>uth)
			{
				nend=true;
				int iter=0;
				while((iter<maxfiter)&&(tableArray[i].getTableSize()>uth))
				{
					tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh);	
					iter++;
				}
			}
		}
		getTabSize(tSize);
		//*/
		it++;
	}
	return;
}