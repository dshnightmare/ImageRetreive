#include "stdafx.h"
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
{//API2	
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

LSHTBuilder::LSHTBuilder(ifstream &fin)
{
	fin>>tableNum;
	tableArray = new LSHTable[tableNum];
	for (int i=0; i<tableNum; i++) 
	{
		tableArray[i] = LSHTable(fin);
		//tableArray[i].printTable();
	}
}

LSHTBuilder::LSHTBuilder(LSHTable *tabArray,int tabNum)
{
	tableArray=tabArray;
	tableNum=tabNum;
}

LSHTable *LSHTBuilder::getTable(int& tabNum)
{
	tabNum=tableNum;
	return tableArray;
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


DLLEXPORT LSHTBuilder* Builder(ImageFeature *imgFeatArray,int dataNum)//main();
{

	int keyLen[MAX_FEAT_ID]={3,3,3,3,3,3};
	int tabNum[MAX_FEAT_ID]={2,2,2,2,2,2};
	double thresh[MAX_FEAT_ID]={0.5,0.5,0.5,0.5,0.5,0.5};
	LSHTBuilder* bldArray=new LSHTBuilder[MAX_FEAT_ID];
	for(int fid=1;fid<=MAX_FEAT_ID;fid++)
	{
		string lshFName=getlshFName(fid);//"E:/lshTable(fid).txt";
		ifstream fin(lshFName);
		if(fin)//cur index file exists
		{
			bldArray[fid-1]=LSHTBuilder(fin);
			fin.close();
		}
		else
		{
			ofstream fout(lshFName);
			int vecLen;
			switch (fid)
			{
			case GLCM:{vecLen=GLCM_length;break;}
			case EH:{vecLen=EH_length;break;}
			case HU:{vecLen=HU_length;break;}
			case HSV:{vecLen=HSV_length;break;}
			case SIFT:{vecLen=SIFT_length;break;}
			case WAVELET:{vecLen=WAVE_length;break;}
			default:
				break;
			}
			double** vecList=allocDoubleVecList(dataNum,vecLen,0);
			for(int i=0;i<dataNum;i++)
			{
				switch (fid)
				{
				case GLCM:{vecList[dataNum]=imgFeatArray[dataNum].GrayLevelCoocurrenceMatrix;break;}
				case EH:{vecList[dataNum]=imgFeatArray[dataNum].EdgeHist;break;}
				case HU:{vecList[dataNum]=imgFeatArray[dataNum].Hu;break;}
				case HSV:{vecList[dataNum]=imgFeatArray[dataNum].HSVFeat;break;}
				case SIFT:{vecList[dataNum]=imgFeatArray[dataNum].Sift;break;}
				case WAVELET:{vecList[dataNum]=imgFeatArray[dataNum].WaveFeat;break;}
				default:
					break;
				}
			}
			bldArray[fid-1]=LSHTBuilder(vecList,dataNum,vecLen,keyLen[fid-1],tabNum[fid-1],thresh[fid-1],fout);
			fout.close();
		}
	}
	return bldArray;
}