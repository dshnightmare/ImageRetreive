#include "stdafx.h"
#include "LSHTExtractor.h"

LSHTExtractor::LSHTExtractor(ifstream &fin)
{
	//fin.seekg(0,ios::beg);
	fin>>tableNum;
	tableArray = new LSHTable[tableNum];
	for (int i=0; i<tableNum; i++) 
	{
		tableArray[i] = LSHTable(fin);
		//tableArray[i].printTable();
	}
}

LSHTExtractor::LSHTExtractor(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh)
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

LSHTExtractor::LSHTExtractor(LSHTable *tabArray,int tabNum)
{
	tableArray=tabArray;
	tableNum=tabNum;
}

void LSHTExtractor::printTables()
{
	cout<<tableNum<<endl;
	for (int i=0; i<tableNum; i++) 
		tableArray[i].printTable();
}

vector<int> LSHTExtractor::getCandIDset(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	for (int i=0; i<tableNum; i++) 
	{
		//vector<int> curCandIds=tableArray[i].getCandIDset(dataVec,maxEDist);
		vector<int> curCandIds=tableArray[i].getCandIDss(dataVec,maxEDist);
		unionSortedIntVec(curCandIds,candIds,candIds);
	}
	return candIds;
}

vector<int> LSHTExtractor::getCandIDsetf(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	vector<int> *candSetList=new vector<int>[tableNum];
	double uth=3.0;
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDss(dataVec,maxEDist);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	
	candIds=resultFilter(candSetList,uth);
	return candIds;
}

vector<int> LSHTExtractor::getCandIDset(double *dataVec,int maxEDist,bool useThresh)
{
	vector<int> candIds;
	for (int i=0; i<tableNum; i++) 
	{
		vector<int> curCandIds=tableArray[i].getCandIDset(dataVec,maxEDist,useThresh);
		unionSortedIntVec(curCandIds,candIds,candIds);
	}
	return candIds;
}

vector<int> LSHTExtractor::getCandIDs(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	for (int i=0; i<tableNum; i++) 
	{
		vector<int> curCandIds=tableArray[i].getCandIDs(dataVec,maxEDist);
		unionSortedIntVec(curCandIds,candIds,candIds);
	}
	return candIds;
}

vector<int> LSHTExtractor::getCandIDsf(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	vector<int> *candSetList=new vector<int>[tableNum];
	double uth=3.0;
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDs(dataVec,maxEDist);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	
	candIds=resultFilter(candSetList,uth);
	return candIds;
}

void LSHTExtractor::getTabSize(int* tSize)
{
	for (int i=0; i<tableNum; i++) 
		tSize[i]=tableArray[i].getTableSize();
	return;
}

void LSHTExtractor::ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double lthresh,double uthresh,int maxIter)
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

vector<int> LSHTExtractor::resultFilter(vector<int>* candSetList,double uth)
{
	vector<int> candIds;
	int *tSize=new int[tableNum];
	for(int i=0;i<tableNum;i++)
		tSize[i]=candSetList[i].size();
	double avgSize=avgOfArray(tSize,tableNum);
	//bool *tag=new bool[tableNum];
	int sum=sumOfArray(tSize,tableNum);
	int count=tableNum;
	//cout<<"****1*****_"<<avgSize<<endl;
	printVector(tSize,tableNum);
	for(int i=0;i<tableNum;i++)
	{
		if(tSize[i]>int(uth*avgSize))
		{
			sum-=tSize[i];
			count--;
			//cout<<tSize[i]<<" ";
		}
	}
	//cout<<endl;
	avgSize = (0.0+sum)/(0.0+count);
	//cout<<"****1*****_"<<avgSize<<endl;
	for(int i=0;i<tableNum;i++)
	{
		if(tSize[i]<int(uth*avgSize))
			unionSortedIntVec(candSetList[i],candIds,candIds);
		else
			;//cout<<tSize[i]<<" ";
	}
	//cout<<endl;

	return candIds;
}

DLLEXPORT vector<int> Extractor(int fid,ImageFeature &imgfeat)
{
	string lshFName=getlshFName(fid);
	ifstream fin(lshFName);
	vector<int> res;
	if(!fin)
		return res;//cout<<"File open failed!"<<endl;
	LSHTExtractor exct=LSHTExtractor(fin);
	fin.close();
	int vecLength;
	switch (fid)
	{
	case GLCM:{vecLength=imgfeat.GLCM_length;break;}
	case EH:{vecLength=imgfeat.EH_length;break;}
	case HU:{vecLength=imgfeat.HU_length;break;}
	case HSV:{vecLength=imgfeat.HSV_length;break;}
	case SIFT:{vecLength=imgfeat.SIFT_length;break;}
	case WAVELET:{vecLength=imgfeat.WAVE_length;break;}
	default:
		break;
	}
	double *dataVec;//problem: already alloc ?
	switch (fid)
	{
	case GLCM:{dataVec=imgfeat.GrayLevelCoocurrenceMatrix;break;}
	case EH:{dataVec=imgfeat.EdgeHist;break;}
	case HU:{dataVec=imgfeat.Hu;break;}
	case HSV:{dataVec=imgfeat.HSVFeat;break;}
	case SIFT:{dataVec=imgfeat.Sift;break;}
	case WAVELET:{dataVec=imgfeat.WaveFeat;break;}
	default:
		break;
	}
	int maxEDist[6]={1,1,1,1,1,1};
	bool useThresh[6]={0,0,0,0,0,0};
	res=exct.getCandIDs(dataVec,maxEDist[fid]);
	return res;
}
