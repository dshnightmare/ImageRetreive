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

LSHTExtractor::LSHTExtractor(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,double itcptRate)
{
	tableNum=tabNum;
	tableArray = new LSHTable[tableNum];
	for (int i=0; i<tableNum; i++) 
		tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh,itcptRate);

	//**ajusting
	double lthresh=0.8;
	double uthresh=3;
	int maxIter=5;
	ajustTowardsAvg(dataVecList,dataNumber,vecLength,keyLength,thresh,itcptRate,lthresh,uthresh,maxIter);
}

LSHTExtractor::LSHTExtractor(LSHTable *tabArray,int tabNum)
{
	tableArray=tabArray;
	tableNum=tabNum;
}

LSHTExtractor::LSHTExtractor(LSHTBuilder bld)//API
{
	int tabNum=0;
	tableArray=bld.getTable(tabNum);
	tableNum=tabNum;
}

LSHTExtractor::~LSHTExtractor()
{
	delete[] tableArray;
	//cout<<"LSHTExtractor delete"<<endl;
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
		vector<int> curCandIds=tableArray[i].getCandIDset(dataVec,maxEDist);
		//vector<int> curCandIds=tableArray[i].getCandIDss(dataVec,maxEDist);
		unionSortedIntVec(curCandIds,candIds,candIds);
	}
	return candIds;
}

vector<int> LSHTExtractor::getCandIDsetf(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	vector<int> *candSetList=new vector<int>[tableNum];
	double lth=0.25;
	double uth=2.5;
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDset(dataVec,maxEDist);
		//candSetList[i]=tableArray[i].getCandIDss(dataVec,maxEDist);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	candIds=resultFilter(candSetList,lth,uth);
	delete[] candSetList;
	return candIds;
}

vector<int> LSHTExtractor::getCandIDseta(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	vector<int> *candSetList=new vector<int>[tableNum];
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDset(dataVec,maxEDist);
		//candSetList[i]=tableArray[i].getCandIDss(dataVec,maxEDist);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	candIds=resultAssembler(candSetList);
	delete[] candSetList;
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

vector<int> LSHTExtractor::getCandIDsetf(double *dataVec,int maxEDist,bool useThresh)
{
	vector<int> candIds;
	vector<int> *candSetList=new vector<int>[tableNum];
	double lth=0.25;
	double uth=2.5;
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDset(dataVec,maxEDist,useThresh);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	candIds=resultFilter(candSetList,lth,uth);
	delete[] candSetList;
	return candIds;
}

vector<int> LSHTExtractor::getCandIDseta(double *dataVec,int maxEDist,bool useThresh)
{
	vector<int> candIds;
	vector<int> *candSetList=new vector<int>[tableNum];
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDset(dataVec,maxEDist,useThresh);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	candIds=resultAssembler(candSetList);
	delete[] candSetList;
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
	double lth=0.25;
	double uth=2.5;
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDs(dataVec,maxEDist);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	candIds=resultFilter(candSetList,lth,uth);
	delete[] candSetList;
	return candIds;
}

vector<int> LSHTExtractor::getCandIDss(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	for (int i=0; i<tableNum; i++) 
	{
		vector<int> curCandIds=tableArray[i].getCandIDss(dataVec,maxEDist);
		unionSortedIntVec(curCandIds,candIds,candIds);
	}
	return candIds;
}

vector<int> LSHTExtractor::getCandIDssf(double *dataVec,int maxEDist)
{
	vector<int> candIds;
	vector<int> *candSetList=new vector<int>[tableNum];
	double lth=0.25;
	double uth=2.5;
	for (int i=0; i<tableNum; i++) 
	{
		candSetList[i]=tableArray[i].getCandIDss(dataVec,maxEDist);
		//unionSortedIntVec(curCandIds,candIds,candIds);
	}
	candIds=resultFilter(candSetList,lth,uth);
	delete[] candSetList;
	return candIds;
}

vector<int> LSHTExtractor::getCandID(double *dataVec,int maxEDist,int minSize)
{
	vector<int> candIds=getCandIDssf(dataVec,0);
	if(candIds.size()>=minSize)
		return candIds;
	else
	{
		cout<<"candIds"<<candIds.size()<<endl;
		vector<int> ccandIds=getCandIDsetf(dataVec,1,1);
		if(candIds.size()>=minSize)
			return ccandIds;
		else
		{
			cout<<"ccandIds"<<ccandIds.size()<<endl;
			vector<int> cccandIds=getCandIDsetf(dataVec,2,1);
			return cccandIds;
		}
	}
}

void LSHTExtractor::getTabSize(int* tSize)
{
	for (int i=0; i<tableNum; i++) 
		tSize[i]=tableArray[i].getTableSize();
	return;
}

void LSHTExtractor::ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double itcptRate,double lthresh,double uthresh,int maxIter)
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
					tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh,itcptRate);	
					iter++;
				}
			}
			if(tableArray[i].getTableSize()>uth)
			{
				nend=true;
				int iter=0;
				while((iter<maxfiter)&&(tableArray[i].getTableSize()>uth))
				{
					tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh,itcptRate);	
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

vector<int> LSHTExtractor::resultFilter(vector<int>* candSetList,double lth,double uth)
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
	//printVector(tSize,tableNum);//debug
	for(int i=0;i<tableNum;i++)
	{
		if(tSize[i]>int(uth*avgSize))
		{
			sum-=tSize[i];
			count--;
			//cout<<tSize[i]<<" ";
		}
		if(tSize[i]<int(lth*avgSize))
		{
			sum-=tSize[i];
			count--;
			//cout<<tSize[i]<<" ";
		}
	}
	//cout<<endl;
	avgSize = (0.0+sum)/(0.0+count);
	//cout<<"****1*****_"<<avgSize<<endl;
	vector<int>* filteredList=new vector<int>[tableNum];
	for(int i=0;i<tableNum;i++)
	{
		if((tSize[i]>int(lth*avgSize))&&(tSize[i]<int(uth*avgSize)))
			filteredList[i]=candSetList[i];
		else
		{
			vector<int> emptySet;
			filteredList[i]=emptySet;
		}
	}
	//cout<<endl;
	candIds=resultAssembler(filteredList);
	delete[] filteredList;
	//cout<<candIds.size()<<endl;//debug
	return candIds;
}

vector<int> LSHTExtractor::resultAssembler(vector<int>* candSetList)
{
	vector<int> candIds;
	int *tSize=new int[tableNum];
	for(int i=0;i<tableNum;i++)
		tSize[i]=candSetList[i].size();
	//printVector(tSize,tableNum);//debug
	delete[] tSize;
	if(tableNum==1)
	{
		unionSortedIntVec(candSetList[0],candIds,candIds);
		return candIds;
	}	
	vector<int>* serialUnionList=new vector<int>[tableNum];
	vector<int>* tempCrossList=new vector<int>[tableNum];
	vector<int> emptySet;
	serialUnionList[tableNum-1]=emptySet;
	for(int i=tableNum-2;i>=0;i--)
		unionSortedIntVec(candSetList[i+1],serialUnionList[i+1],serialUnionList[i]);
	for(int i=0;i<tableNum;i++)
		intersectSortedIntVec(candSetList[i],serialUnionList[i],tempCrossList[i]);
	for(int i=0;i<tableNum;i++)
		unionSortedIntVec(tempCrossList[i],candIds,candIds);
	delete[] serialUnionList;
	delete[] tempCrossList;
	//cout<<candIds.size()<<endl;//debug
	return candIds;
}

DLLEXPORT vector<int> Extractor(LSHTBuilder bld,int fid,ImageFeature &imgfeat)
{	
	vector<int> res;	
	LSHTExtractor exct=LSHTExtractor(bld);
	int vecLength;
	switch (fid)
	{
	case GLCM:{vecLength=GLCM_length;break;}
	case EH:{vecLength=EH_length;break;}
	case HU:{vecLength=HU_length;break;}
	case HSV:{vecLength=HSV_length;break;}
	case SIFT:{vecLength=SIFT_length;break;}
	case WAVELET:{vecLength=WAVE_length;break;}
	case LBP:{vecLength=LBP_length;break;}
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
	case LBP:{dataVec=imgfeat.Lbp;break;}
	default:
		break;
	}
	int maxEDist[MAX_FEAT_ID]={0,0,0,0,0,0};
	bool useThresh[MAX_FEAT_ID]={0,0,0,0,0,0};
	int minResSize=1000;
	res=exct.getCandID(dataVec,maxEDist[fid],minResSize);
	return res;
}
