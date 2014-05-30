#include "stdafx.h"
#include "LSHTBuilder.h"
#include "mytest.h"

LSHTBuilder::LSHTBuilder()
{
}

LSHTBuilder::LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,double itcptRate)
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

LSHTBuilder::LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,double itcptRate,ofstream &fout)
{//API2	
	tableNum=tabNum;
	tableArray = new LSHTable[tableNum];
	for (int i=0; i<tableNum; i++) 
		tableArray[i] = LSHTable(dataVecList,dataNumber,vecLength,keyLength,thresh,itcptRate);
	//**ajusting
	double lthresh=0.8;
	double uthresh=3;
	int maxIter=5;
	ajustTowardsAvg(dataVecList,dataNumber,vecLength,keyLength,thresh,itcptRate,lthresh,uthresh,maxIter);
	writeToFile(fout);
}

LSHTBuilder::LSHTBuilder(ifstream &fin)
{
	//fin.seekg(0,ios::beg);
	fin.read((char *)&tableNum,sizeof(int));
	tableArray = new LSHTable[tableNum];
	for (int i=0; i<tableNum; i++) 
	{
		tableArray[i] = LSHTable(fin);
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
	fout.write((char *)&tableNum, sizeof(int));
	for (int i=0; i<tableNum; i++) 
		tableArray[i].writeToFile(fout);

}


void LSHTBuilder::getTabSize(int* tSize)
{
	for (int i=0; i<tableNum; i++) 
		tSize[i]=tableArray[i].getTableSize();
	return;
}

void LSHTBuilder::ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double itcptRate,double lthresh,double uthresh,int maxIter)
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
	delete[] tSize;
	return;
}


DLLEXPORT LSHTBuilder* Builder(ImageFeature *imgFeatArray,int dataNum)//main();
{
	/*
	//--------------test--------------//
	AllocConsole();
	freopen( "CONOUT$","w",stdout);
	//printf("ºÃ°ÉÑ½%d\n", 1);
	//cout<<"ºÃ°ÉÑ½"<<endl;
	//system("Pause");
	//FreeConsole();
	time_t start,end,time;
	//start=clock();

	mytestmain();
	
	system("Pause");
	FreeConsole();
	//--------------test--------------//
	*/
	int keyLen[MAX_FEAT_ID]={45,35,25,20,9,30,15};
	int tabNum[MAX_FEAT_ID]={8,8,8,8,8,8,8};
	double thresh[MAX_FEAT_ID]={0.015,0.015,0.015,0.015,0.015,0.015,0.015};
	double itcptRate[MAX_FEAT_ID]={0.1,0.15,0.2,0.05,0.02,0.08,0.02};
	LSHTBuilder* bldArray=new LSHTBuilder[MAX_FEAT_ID];
	for(int fid=1;fid<=MAX_FEAT_ID;fid++)
	{
		string lshFName=getlshFName(fid);//"E:/lshTable(fid).txt";
		ifstream fin(lshFName, ios::binary);
		if(fin)//cur index file exists
		{
			bldArray[fid-1]=LSHTBuilder(fin);
			fin.close();
		}
		else
		{
			ofstream fout(lshFName, ios::binary);
			int vecLen;
			switch (fid)
			{
			case GLCM:{vecLen=GLCM_length;break;}
			case EH:{vecLen=EH_length;break;}
			case HU:{vecLen=HU_length;break;}
			case HSV:{vecLen=HSV_length;break;}
			case SIFT:{vecLen=SIFT_length;break;}
			case WAVELET:{vecLen=WAVE_length;break;}
			case LBP:{vecLen=LBP_length;break;}
			default:
				break;
			}
			double** vecList=allocDoubleVecList(dataNum,vecLen,0);
			for(int i=0;i<dataNum;i++)
			{
				switch (fid)
				{
				case GLCM:{vecList[i]=imgFeatArray[i].GrayLevelCoocurrenceMatrix;break;}
				case EH:{vecList[i]=imgFeatArray[i].EdgeHist;break;}
				case HU:{vecList[i]=imgFeatArray[i].Hu;break;}
				case HSV:{vecList[i]=imgFeatArray[i].HSVFeat;break;}
				case SIFT:{vecList[i]=imgFeatArray[i].Sift;break;}
				case WAVELET:{vecList[i]=imgFeatArray[i].WaveFeat;break;}
				case LBP:{vecList[i]=imgFeatArray[i].Lbp;break;}
				default:
					break;
				}
			}
			bldArray[fid-1]=LSHTBuilder(vecList,dataNum,vecLen,keyLen[fid-1],tabNum[fid-1],thresh[fid-1],itcptRate[fid-1],fout);
			fout.close();
		}
	}
	//*/
	

	return bldArray;
}