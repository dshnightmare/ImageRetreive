#include "stdafx.h"
#include "LSHTable.h"

LSHTable::LSHTable()
{
}

LSHTable::LSHTable(ifstream &fin)
{
	readFromFile(fin);
}

LSHTable::LSHTable(ifstream &fin,int keyLength)
{
	keyLen= keyLength;
	double **dataVecList=NULL;
	readDArrayListFromFile(dataVecList,dataNum,vecLen,fin);

	mu=0.5;
	sigma=0.15;
	threshRatio=0.5;
	ignThresh=threshRatio*sqrt(vecLen*(pow(mu,2.0)+pow(sigma,2.0)));
	Table lshT;
	stdndVecList = new double*[keyLen];

	for (int bitId=0; bitId<keyLen; bitId++) 
	{
		double *stdndVec = new double[vecLen];
		getStdNDVector(stdndVec,vecLen);
		stdndVecList[bitId] = stdndVec;
	}
	
	for (int dataId=0; dataId<dataNum; dataId++) 
	{
		double *dataVec = dataVecList[dataId];
		string key = getLSHKey(dataVec);
		TableIter it =  lshT.find(key);
		if(it != lshT.end())
		{
			vector<int> &idList = it->second;
			idList.push_back(dataId);
		}
		else
		{
			vector<int> idList;
			idList.push_back(dataId);
			lshT.insert(TableTerm(key,idList));
		}
	}
	tab=lshT;
}

LSHTable::LSHTable(double **dataVecList,int dataNumber,int vecLength,int keyLength)
{		
	dataNum= dataNumber;
	vecLen= vecLength;
	keyLen= keyLength;
	mu=0.5;
	sigma=0.15;
	threshRatio=0.5;
	ignThresh=threshRatio*sqrt(vecLen*(pow(mu,2.0)+pow(sigma,2.0)));
	Table lshT;
	stdndVecList = new double*[keyLen];

	for (int bitId=0; bitId<keyLen; bitId++) 
	{
		double *stdndVec = new double[vecLen];
		getStdNDVector(stdndVec,vecLen);
		stdndVecList[bitId] = stdndVec;
	}
	
	for (int dataId=0; dataId<dataNum; dataId++) 
	{
		double *dataVec = dataVecList[dataId];
		string key = getLSHKey(dataVec);
		TableIter it =  lshT.find(key);
		if(it != lshT.end())
		{
			vector<int> &idList = it->second;
			idList.push_back(dataId);
		}
		else
		{
			vector<int> idList;
			idList.push_back(dataId);
			lshT.insert(TableTerm(key,idList));
		}
	}
	tab=lshT;
}

LSHTable::LSHTable(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh)
{		
	dataNum= dataNumber;
	vecLen= vecLength;
	keyLen= keyLength;
	mu=0.5;
	sigma=0.15;
	threshRatio=0.5;
	ignThresh=thresh*sqrt(keyLen);
	Table lshT;
	stdndVecList = new double*[keyLen];

	for (int bitId=0; bitId<keyLen; bitId++) 
	{
		double *stdndVec = new double[vecLen];
		getStdNDVector(stdndVec,vecLen);
		stdndVecList[bitId] = stdndVec;
	}
	
	for (int dataId=0; dataId<dataNum; dataId++) 
	{
		double *dataVec = dataVecList[dataId];
		string key = getLSHKey(dataVec);
		TableIter it =  lshT.find(key);
		if(it != lshT.end())
		{
			vector<int> &idList = it->second;
			idList.push_back(dataId);
		}
		else
		{
			vector<int> idList;
			idList.push_back(dataId);
			lshT.insert(TableTerm(key,idList));
		}
	}
	tab=lshT;
}

LSHTable::LSHTable(double **dataVecList,int dataNumber,int vecLength,double u,double sig,double thrRatio,int keyLength)
{	
	mu=u;
	sigma=sig;
	threshRatio=thrRatio;
	ignThresh=threshRatio*sqrt(vecLen*(pow(mu,2.0)+pow(sigma,2.0)));
	Table lshT;
	dataNum= dataNumber;
	vecLen= vecLength;
	keyLen= keyLength;
	stdndVecList = new double*[keyLen];

	for (int bitId=0; bitId<keyLen; bitId++) 
	{
		double *stdndVec = new double[vecLen];
		getStdNDVector(stdndVec,vecLen);
		stdndVecList[bitId] = stdndVec;
	}
	
	for (int dataId=0; dataId<dataNum; dataId++) 
	{
		double *dataVec = dataVecList[dataId];
		string key = getLSHKey(dataVec);
		TableIter it =  lshT.find(key);
		if(it != lshT.end())
		{
			vector<int> &idList = it->second;
			idList.push_back(dataId);
		}
		else
		{
			vector<int> idList;
			idList.push_back(dataId);
			lshT.insert(TableTerm(key,idList));
		}
	}
	tab=lshT;
}
/*
LSHTable::~LSHTable()
{
	for (int bitId=0; bitId<keyLen; bitId++) 
		delete[] stdndVecList[bitId];
	delete[] stdndVecList;
	//cout<<"LSHTable delete"<<endl;
}
*/

Table LSHTable::getTable()
{
	return tab;
}

int LSHTable::getTableSize()
{
	return tab.size();
}
//**********************data fomat in file**********************//
//		(One LSHTable)
//		keyLen vecLen 
//		a(i)(1*vecLen vector)
//		(i=0~keyLen) (keyLen*vecLen matrix)
//		keyNum
//		key(i) 
//		idList(i) 
//		(i=0~keyNum)
//**********************data fomat in file**********************//

void LSHTable::writeToFile(ofstream &fout)
{
	fout<<keyLen<<" "<<vecLen<<endl;
	for (int bitId=0; bitId<keyLen; bitId++) 
		writeDArrayToFile(stdndVecList[bitId],vecLen,fout);
	int keyNum=tab.size();
	fout<<keyNum<<" "<<ignThresh<<endl;
	for(TableIter it = tab.begin(); it != tab.end(); it++)
	{
		fout<<it->first<<endl;
		vector<int> idList=it->second;
		writeVectorToFile(idList,fout);
	}
	return ;
}

void LSHTable::readFromFile(ifstream &fin)
{
	fin>>keyLen;
	fin>>vecLen;
	stdndVecList = allocDoubleVecList(keyLen,vecLen,0);
	for (int bitId=0; bitId<keyLen; bitId++) 
		readDArrayFromFile(stdndVecList[bitId],vecLen,fin);
	int keyNum=0;
	fin>>keyNum;
	fin>>ignThresh;
	//cout<<keyNum<<endl;
	for(int i=0;i<keyNum;i++)
	{
		string key;
		fin>>key;
		//cout<<key<<endl;
		vector<int> idList;
		readVectorFromFile(idList,fin);
		//printVector(idList);
		tab.insert(TableTerm(key,idList));
	}
	return ;
}

void LSHTable::setThreshRatio(double thrRatio)
{
	threshRatio=thrRatio;
}

void LSHTable::getAV(double *dataVec,double* av)
{
	for (int bitId=0; bitId<keyLen; bitId++) 
	{
		av[bitId]=getBasicLSH(dataVec,stdndVecList[bitId],vecLen);
	}
	return;
}

string LSHTable::getLSHKey(double *dataVec)
{
	string key = string(keyLen,'0');
	for (int bitId=0; bitId<keyLen; bitId++) 
	{
		if(getBasicLSH(dataVec,stdndVecList[bitId],vecLen)>=0)
			key.replace(bitId,1,1,'1');
		else
			key.replace(bitId,1,1,'0');
	}
	return key;
}

void LSHTable::printTable()
{
	for(TableIter it = tab.begin(); it != tab.end(); it++)
	{
		cout << it->first << "-> " ;
		vector<int> idList=it->second;
		int size = (int)idList.size();
		for (int i=0; i<size; i++) 
			cout<<idList[i]<<" ";
		cout<<endl;
	}
}

int LSHTable::getIgnTag(double *dataVec,int* ignTag)
{
	int ignNum=0;
	double pcnt=0;
	for (int bitId=0; bitId<keyLen; bitId++) 
	{
		if(abs(getBasicLSH(dataVec,stdndVecList[bitId],vecLen))<ignThresh)
		{
			ignTag[bitId]=1;
			ignNum++;
		}
		else
			ignTag[bitId]=0;
	}
	pcnt=((double)ignNum)/keyLen;
	//cout<<"pcnt"<<pcnt<<endl;
	int maxIgnNum=min(64,int(keyLen*0.2));
	if(ignNum>maxIgnNum)
	{
		int k=1;
		for (int i=0;i<keyLen; i++)
		{
			if(ignTag[i]==1)
			{
				if(k>maxIgnNum)
					ignTag[i]=0;
				k++;
			}
		}
		ignNum=maxIgnNum;
	}
	return ignNum;
}

vector<int> LSHTable::getCandIDset(double *dataVec,int maxEDist)
{
	time_t start,end,time;
	start=clock();
	vector<int> candIDs;
	string gKey = getLSHKey(dataVec);
	for(TableIter it = tab.begin(); it != tab.end(); it++)
	{
		string tKey = it->first;
		int d=hammingDist(gKey,tKey);
		if((d!=-1)&&(d<=maxEDist))
			unionSortedIntVec(it->second,candIDs,candIDs);
	}
	end=clock();
	time=end-start;
	//cout<<"time______"<<time<<endl;
	return candIDs;
}

vector<int> LSHTable::getCandIDset(double *dataVec,int maxEDist,bool useThresh)
{
	if(!useThresh)
		return getCandIDset(dataVec,maxEDist);
	vector<int> candIDs;
	int* ignTag=new int[keyLen];
	int ignNum=getIgnTag(dataVec,ignTag);
	string gKey = getLSHKey(dataVec);
	for(TableIter it = tab.begin(); it != tab.end(); it++)
	{
		string tKey = it->first;
		int d=hammingDist(gKey,tKey,ignTag,keyLen);
		if((d!=-1)&&(d<=maxEDist))
			unionSortedIntVec(it->second,candIDs,candIDs);
	}
	return candIDs;
}

//use thresh and 0<=maxEDist<=2
vector<int> LSHTable::getCandIDss(double *dataVec,int maxEDist)
{
	vector<int> candIDs;
	int* ignTag=new int[keyLen];
	int ignNum=getIgnTag(dataVec,ignTag);
	string gKey = getLSHKey(dataVec);
	string *candKeys=NULL;
	if((ignNum==0)||(ignNum>=6))
	{
		return getCandIDs(dataVec,maxEDist);
	}
	else
	{		
		int num=(int)pow(2.0,ignNum);
		//cout<<"num__"<<num<<endl;
		int onedNum=num*(keyLen-ignNum);
		int twodNum=num*(keyLen-ignNum)*(keyLen-ignNum)/2;
		candKeys=new string[num+onedNum+twodNum];
		get0DIgnStringSet(gKey,ignTag,ignNum,candKeys);	
		if(maxEDist==0)
		{
			candIDs=getId4KeySet(candKeys,num);
			delete[] candKeys;
			return candIDs;
		}
		else 
		{
			get1DIgnStringSet(gKey,ignTag,ignNum,candKeys+num);	
			if(maxEDist==1)
			{
				candIDs=getId4KeySet(candKeys,num+onedNum);
				delete[] candKeys;
				return candIDs;
			}
			else//(maxEDist==2)
			{
				get2DIgnStringSet(gKey,ignTag,ignNum,candKeys+num+onedNum);	
				candIDs=getId4KeySet(candKeys,num+onedNum+twodNum);
				delete[] candKeys;
				return candIDs;
			}
		}
	}
}

vector<int> LSHTable::getCandIDs(double *dataVec,int maxEDist)
{
	vector<int> candIDs;
	if((maxEDist<0)||(maxEDist>2))
		return candIDs;
	int maxKeyNum=1+keyLen+(keyLen)*(keyLen-1)/2;
	string *candKeys= new string[maxKeyNum];

	candKeys[0]=getLSHKey(dataVec);
	if(maxEDist==0)
	{
		candIDs=getId4KeySet(candKeys,1);
		delete[] candKeys;
		return candIDs;
	}
	
	get1EDistKeys(dataVec,candKeys+1);
	if(maxEDist==1) 
	{
		candIDs=getId4KeySet(candKeys,1+keyLen);
		delete[] candKeys;
		return candIDs;
	}

	get2EDistKeys(dataVec,candKeys+1+keyLen);
	if(maxEDist==2) 
	{
		candIDs=getId4KeySet(candKeys,1+keyLen+(keyLen)*(keyLen-1)/2);
		delete[] candKeys;
		return candIDs;
	}
}

vector<int> LSHTable::get0EDistIDs(double *dataVec)
{
	vector<int> candIDs;
	string gKey = getLSHKey(dataVec);
	
	TableIter it =  tab.find(gKey);
	if(it != tab.end())
		candIDs = it->second;
	return candIDs;
}

vector<int> LSHTable::getId4KeySet(string *candKeys,int keyNum)
{
	vector<int> candIDs;

	for (int i=0; i<keyNum; i++) 
	{
		TableIter it =  tab.find(candKeys[i]);
		if(it != tab.end())
			unionSortedIntVec(it->second,candIDs,candIDs);
	}	
	return candIDs;
}

vector<int> LSHTable::get1EDistIDs(double *dataVec)
{
	vector<int> candIDs;
	string gKey = getLSHKey(dataVec);
	string *candKeys= new string[keyLen];
	get1EDistKeys(dataVec,candKeys);

	for (int i=0; i<keyLen; i++) 
	{
		TableIter it =  tab.find(candKeys[i]);
		if(it != tab.end())
			unionSortedIntVec(it->second,candIDs,candIDs);
	}	
	return candIDs;
}

void LSHTable::get1EDistKeys(double *dataVec,string *ED1Keys)
{
	string gKey = getLSHKey(dataVec);
	for (int i=0; i<keyLen; i++) 
	{
		string tKey=gKey;
		char curChar=tKey.at(i);
		int curValue = atoi(&curChar);
		int revValue = 1-curValue;
		char revChar = '0'+revValue;
		tKey.replace(i,1,1,revChar);
		ED1Keys[i]=tKey;
	}
	return;
}

void LSHTable::get2EDistKeys(double *dataVec,string *ED2Keys)
{
	string gKey = getLSHKey(dataVec);
	int cur=0;
	for (int i=0; i<keyLen-1; i++) 
	{
		string tKey=gKey;
		char curChar=tKey.at(i);
		int curValue = atoi(&curChar);
		int revValue = 1-curValue;
		char revChar = '0'+revValue;
		tKey.replace(i,1,1,revChar);
		for (int j=i+1; j<keyLen; j++) 
		{
			string ttKey=tKey;
			char curChar=ttKey.at(j);
			int curValue = atoi(&curChar);
			int revValue = 1-curValue;
			char revChar = '0'+revValue;
			ttKey.replace(j,1,1,revChar);
			ED2Keys[cur]=ttKey;
			cur++;
		}
	}
	return;
}

void LSHTable::get3EDistKeys(double *dataVec,string *ED3Keys)
{
	string gKey = getLSHKey(dataVec);
	int cur=0;
	for (int i=0; i<keyLen-2; i++) 
	{
		string tKey=gKey;
		char curChar=tKey.at(i);
		int curValue = atoi(&curChar);
		int revValue = 1-curValue;
		char revChar = '0'+revValue;
		tKey.replace(i,1,1,revChar);
		for (int j=i+1; j<keyLen-1; j++) 
		{
			string ttKey=tKey;
			char curChar=ttKey.at(j);
			int curValue = atoi(&curChar);
			int revValue = 1-curValue;
			char revChar = '0'+revValue;
			ttKey.replace(j,1,1,revChar);
			for (int k=j+1; k<keyLen; k++) 
			{
				string tttKey=ttKey;
				char curChar=tttKey.at(k);
				int curValue = atoi(&curChar);
				int revValue = 1-curValue;
				char revChar = '0'+revValue;
				tttKey.replace(k,1,1,revChar);
				ED3Keys[cur]=tttKey;
				cur++;
			}
		}
	}
	return;
}

int LSHTable::getKeysEDist(double *vec1,double *vec2,int vecLength)
{
	if(vecLength!=vecLen)
		return -1;
	return hammingDist(getLSHKey(vec1),getLSHKey(vec2));
}

void LSHTable::getAvgStdndVec(double *avgVec)
{
	for (int i=0; i<keyLen; i++) 
	{
		double sum=0;
		for (int j=0; j<vecLen; j++) 
			sum+=stdndVecList[i][j];
		avgVec[i]=sum/vecLen;
	}
	return;
}

void LSHTable::getItermSize(int* tSize,int &tNum)//only for debugging
{
	tNum = tab.size();
	int i=0;
	for(TableIter it = tab.begin(); it != tab.end(); it++)
	{
		tSize[i]=it->second.size();
		i++;
	}
	return;
}