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


DLLEXPORT void Builder(ImageFeature *imgFeatArray,int dataNum)//main();
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
			int vecLen;
			switch (fid)
			{
			case GLCM:{vecLen=imgFeatArray[0].GLCM_length;break;}
			case EH:{vecLen=imgFeatArray[0].EH_length;break;}
			case HU:{vecLen=imgFeatArray[0].HU_length;break;}
			case HSV:{vecLen=imgFeatArray[0].HSV_length;break;}
			case SIFT:{vecLen=imgFeatArray[0].SIFT_length;break;}
			case WAVELET:{vecLen=imgFeatArray[0].WAVE_length;break;}
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
			//LSHTBuilder bld=LSHTBuilder(fin,keyLen[fid],tabNum[fid],thresh[fid]);
			//LSHTBuilder bld=LSHTBuilder(fin,keyLen[fid],tabNum[fid],thresh[fid],"E:/"+to_string(fid)+".txt");
			LSHTBuilder bld=LSHTBuilder(vecList,dataNum,vecLen,keyLen[fid],tabNum[fid],thresh[fid],fout);
			fout.close();
		}
	}
	return;
}