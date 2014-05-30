#include "stdafx.h"
#include "mytest.h"


void mytestmain()
{	
	time_t start,end,time,time1,time2;
	start=clock();
	string fName="E:/feat2.dat";
	int dataNum=0;
	int vecLen=0;
	double** vecList=readSingleData(fName,dataNum,vecLen);
	//printVectorList(vecList,dataNum,vecLen);
	
	end = clock();
	time = end-start;
	//cout<<dataNum<<"_"<<vecLen<<endl;
	cout<<"Reading time: "<<time<<endl;
	start = clock();	
	
	int testCasesNum=20;
	for (int tcn=0; tcn<testCasesNum; tcn++) 
	{		
		random_device rd;
		default_random_engine generator(rd());
		uniform_int_distribution<> u(0,dataNum-1);
		int gIdx=u(generator);
		vector<int> sortedNearIdx=distOrderToOthers(vecList,dataNum,vecLen,gIdx,0.01);
		//printVector(sortedNearIdx);
	
		///*
		int testTimes=1;
		int repTimes=5;
		int bestKeyLen1=0;
		int bestKeyLen2=0;
		int bestTabNum1=0;
		int bestTabNum2=0;
		double bestAvgSize1=0;
		double bestAvgSize2=0;
		double maxRecall1=-1;
		double maxRecall2=-1;
		double* avgSizeArray1=new double[testTimes];
		double* avgSizeArray2=new double[testTimes];
		double* avgTimeArray1=new double[testTimes];
		double* avgTimeArray2=new double[testTimes];
		double* avgRecallArray1=new double[testTimes];
		double* avgRecallArray2=new double[testTimes];
		for (int i=0; i<testTimes; i++) 
		{		
			double avgSize1=0;
			double avgSize2=0;
			double avgTime1=0;
			double avgTime2=0;
			double avgRecall1=0;
			double avgRecall2=0;

			int keyLen=45+5*i;
			double itcptRate=0.2-0*i;
			int tabNum=8;
			for (int j=0; j<repTimes; j++) 
			{
				double sig=1.0/6.0;
				double thrRatio=0.1;
				double thresh=thrRatio*sig;
				start = clock();
				LSHTExtractor exct=LSHTExtractor(vecList,dataNum,vecLen,keyLen,tabNum,thresh,itcptRate);
				end = clock();
				time1 = end-start;
				cout<<"time__"<<time1<<endl;
	
				vector<int> result,result0,result1,result2;
				int maxThDist=0;
				int maxNThDist1=0;
				int maxNThDist2=1;
				
				start = clock();
				//result=exct.getCandIDset(vecList[gIdx],maxThDist,1);
				result=exct.getCandIDsf(vecList[gIdx],maxNThDist1);
				end = clock();
				time1 = end-start;
				vector<int> itsct(dataNum);
				intersectSortedIntVec(sortedNearIdx,result,itsct);
				double recall1=(itsct.size()+0.0)/(sortedNearIdx.size()+0.0);
				//cout<<sortedNearIdx.size()<<"_"<<result.size()<<"_"<<itsct.size()<<"__"<<recall1<<"__Search time1: "<<time1<<endl;
				
				start = clock();
				//result0=exct.getCandIDsetf(vecList[gIdx],maxThDist,1);
				result0=exct.getCandIDssf(vecList[gIdx],maxNThDist1);
				end = clock();
				time2 = end-start;
				vector<int> itsct0(dataNum);
				intersectSortedIntVec(sortedNearIdx,result0,itsct0);
				double recall2=(itsct0.size()+0.0)/(sortedNearIdx.size()+0.0);
				//cout<<sortedNearIdx.size()<<"_"<<result0.size()<<"_"<<itsct0.size()<<"__"<<recall2<<"__Search time2: "<<time2<<endl;
				
				avgSize1+=(double)result.size();
				avgSize2+=(double)result0.size();
				avgTime1+=(double)time1;
				avgTime2+=(double)time2;	
				avgRecall1+=(double)recall1;
				avgRecall2+=(double)recall2;			
			}
			avgSize1/=(double)repTimes;
			avgSize2/=(double)repTimes;
			avgTime1/=(double)repTimes;
			avgTime2/=(double)repTimes;
			avgRecall1/=(double)repTimes;
			avgRecall2/=(double)repTimes;
			if(maxRecall1<avgRecall1/avgSize1)
				{
					maxRecall1=avgRecall1/avgSize1;
					bestKeyLen1=keyLen;
					bestTabNum1=tabNum;
					bestAvgSize1=avgSize1;
				}
				if(maxRecall2<avgRecall2/avgSize2)
				{
					maxRecall2=avgRecall2/avgSize2;
					bestKeyLen2=keyLen;
					bestTabNum2=tabNum;
					bestAvgSize2=avgSize2;
				}
		
			avgSizeArray1[i]=avgSize1;
			avgSizeArray2[i]=avgSize2;
			avgTimeArray1[i]=avgTime1;
			avgTimeArray2[i]=avgTime2;
			avgRecallArray1[i]=avgRecall1;
			avgRecallArray2[i]=avgRecall2;
		}
		printVector(avgSizeArray1,testTimes);
		printVector(avgSizeArray2,testTimes);
		printVector(avgTimeArray1,testTimes);
		printVector(avgTimeArray2,testTimes);
		printVector(avgRecallArray1,testTimes);
		printVector(avgRecallArray2,testTimes);
		cout<<maxRecall1<<"__"<<bestKeyLen1<<"__"<<bestAvgSize1<<endl;
		cout<<maxRecall2<<"__"<<bestKeyLen2<<"__"<<bestAvgSize2<<endl;
		delete[] avgSizeArray1;
		delete[] avgSizeArray2;
		delete[] avgRecallArray1;
		delete[] avgRecallArray2;
	}
    return;
}


double *** readData(string fName,int *dataNumArray,int *vecLenArray)
{
	double ***dataMatrixArray=new double**[MAX_FEAT_ID];
	ifstream fin(fName);
	if(!fin)
	{
		cout<<"File open failed!"<<endl;
		return NULL;
	}
	for(int i=0;i<MAX_FEAT_ID;i++)
	{
		dataMatrixArray[i]=NULL;
		readDArrayListFromFile(dataMatrixArray[i],dataNumArray[i],vecLenArray[i],fin);
		cout<<i+1<<"th matrix read!"<<endl;
	}
	fin.close();
	return dataMatrixArray;
}

double ** readSingleData(string fName,int& dataNum,int& vecLen)
{
	double **dataMatrix=NULL;
	ifstream fin(fName);
	if(!fin)
	{
		cout<<"File open failed!"<<endl;
		return NULL;
	}
	readDArrayListFromFile(dataMatrix,dataNum,vecLen,fin);

	fin.close();
	return dataMatrix;
}

void rAndWData(string fName,int *dataNumArray,int *vecLenArray)
{
	double ***dataMatrixArray=new double**[MAX_FEAT_ID];
	ifstream fin(fName);
	if(!fin)
	{
		cout<<"File open failed!"<<endl;
		return;
	}
	for(int i=0;i<MAX_FEAT_ID;i++)
	{
		string cfname;
		string head="E:/feat";
		string tail=".dat";
		string id=to_string(i);
		cfname=head+id+tail;
		dataMatrixArray[i]=NULL;
		
		time_t start,end,time;
		start=clock();

		readDArrayListFromFile(dataMatrixArray[i],dataNumArray[i],vecLenArray[i],fin);
		cout<<i+1<<"th matrix read!"<<endl;

		end = clock();
		time = end-start;
		cout<<"time"<<i<<": "<<time<<endl;

		ofstream fout(cfname);
		writeDArrayListToFile(dataMatrixArray[i],dataNumArray[i],vecLenArray[i],fout);
		fout.close();
		cout<<i+1<<"th matrix written!"<<endl;
	}
	fin.close();
	return;
}




