#include "commons.h"


double * allocDoubleVector(int col,double initValue)
{
	double *tVec = new double[col];
	for (int j=0; j<col; j++) 
		tVec[j]=initValue;
	return tVec;
}

double ** allocDoubleVecList(int row,int col,double initValue)
{
	double **VectorList = new double*[row];
	for (int i=0; i<row; i++) 
		VectorList[i] = allocDoubleVector(col,initValue);
	return VectorList;
}

void getNDVector(double *vec,int vecLen,double *muVec,double *sigVec)
{
	random_device rd;
	default_random_engine generator(rd());
	for (int i=0; i<vecLen; i++) 
	{	  
		normal_distribution<double> distribution(muVec[i],sigVec[i]);
		vec[i] = distribution(generator);
	}
	return;
}

void getStdNDVector(double *vec,int vecLen)
{
	double *muVec=allocDoubleVector(vecLen,0.0);
	double *sigVec=allocDoubleVector(vecLen,1.0);
	getNDVector(vec,vecLen,muVec,sigVec);
	return;
}

void getNDVectorList(double **vecList,int row,int vecLen,double *muVec,double *sigVec)
{
//ith dimension of each vector follows the same distribution N(mu[i],sig[i])
	for (int i=0; i<row; i++) 
	{
		getNDVector(vecList[i],vecLen,muVec,sigVec);
	}
}

double getBasicLSH(double *dataVec,double *stdndVec,int vecLen)
{
	double ret=0.0;
	for (int i=0; i<vecLen; i++) 
		ret+=stdndVec[i]*dataVec[i];
	return ret;
}

int maxOfArray(int *vec,int vecLen)
{
	int max=INT_MIN;
	for (int i=0; i<vecLen; i++) 
		if(max<vec[i])
			max=vec[i];
	return max;
}

int sumOfArray(int *vec,int vecLen)
{
	int sum=0;
	for (int i=0; i<vecLen; i++) 
		sum+=vec[i];
	return sum;
}

double sumOfArray(double *vec,int vecLen)
{
	double sum=0;
	for (int i=0; i<vecLen; i++) 
		sum+=vec[i];
	return sum;
}

double avgOfArray(int *vec,int vecLen)
{
	if(vecLen==0)
		return -1;
	double sum=(double)sumOfArray(vec,vecLen);
	return sum/vecLen;
}

double avgOfArray(double *vec,int vecLen)
{
	if(vecLen==0)
		return -1;
	double sum=sumOfArray(vec,vecLen);
	return sum/vecLen;
}

void ajustTowardsAvg(int *vec,int vecLen,double lthresh,double uthresh,int maxIter)
{
	int it=0;
	int max=maxOfArray(vec,vecLen);
	bool nend=true;
	double orilt=lthresh;
	int maxfiter=1;
	while((it<maxIter)&&(nend))
	{
		nend=false;
		if(maxIter>1)
			lthresh-=(orilt-0.5)/(maxIter-1.0);
		double lth=lthresh*avgOfArray(vec,vecLen);
		double uth=uthresh*avgOfArray(vec,vecLen);
		for (int i=0; i<vecLen; i++) 
		{
			if(vec[i]<lth)
			{
				nend=true;
				int iter=0;
				while((iter<maxfiter)&&(vec[i]<lth))
				{
					random_device rd;
					default_random_engine generator(rd());
					uniform_int_distribution<> u(1,max) ;
					vec[i]=u(generator);		
					iter++;
				}
				if(iter==maxfiter)
					cout<<"lr!"<<endl;
			}
			if(vec[i]>uth)
			{
				nend=true;
				int iter=0;
				while((iter<maxfiter)&&(vec[i]>uth))
				{
					random_device rd;
					default_random_engine generator(rd());
					uniform_int_distribution<> u(1,max) ;
					vec[i]=u(generator);		
					iter++;					
				}
				if(iter==maxfiter)
					cout<<"ur!"<<endl;
			}
		}
		printVector(vec,vecLen);
		it++;
	}
}

void unionSortedIntVec(vector<int> vec1,vector<int> vec2,vector<int>& gVec)
{
	int size1=vec1.size();
	int size2=vec2.size();
	int maxSize=size1+size2;
	vector<int> tempSet(maxSize);
	vector<int>::iterator tempIt;
	tempIt = set_union (vec1.begin(),vec1.end(),vec2.begin(),vec2.end(),tempSet.begin());
	gVec.assign(tempSet.begin(),tempIt);
}

void intersectSortedIntVec(vector<int> vec1,vector<int> vec2,vector<int>& gVec)
{
	int size1=vec1.size();
	int size2=vec2.size();
	int maxSize=max(size1,size2);
	sort (vec1.begin ( ) ,vec1.end ( ) , less<int>() );
	sort (vec2.begin ( ) ,vec2.end ( ) , less<int>() );
	vector<int> tempSet(maxSize);
	vector<int>::iterator tempIt;
	tempIt = set_intersection (vec1.begin(),vec1.end(),vec2.begin(),vec2.end(),tempSet.begin());
	gVec.assign(tempSet.begin(),tempIt);
}

int cmp(const void* it1, const void* it2)
{
    return (int)(*(double*)it1 - *(double*)it2);
}

int dAndICmp(const void* it1, const void* it2)
{
	return (int)( ((dAndI*)it1)->value - ((dAndI*)it2)->value);
}

void sortDataAndIdx(double *vec,int *sortedIdx,int vecLen)
{
	dAndI *temp=new dAndI[vecLen];
	for(int i=0;i<vecLen;i++)
	{
		temp[i].idx=i;
		temp[i].value=vec[i];
	}
	qsort(temp,vecLen,sizeof(dAndI),dAndICmp);
	for(int i=0;i<vecLen;i++)
		sortedIdx[i]=temp[i].idx;
	return;
}

int countBeyondThresh(double *vec,int vecLen,double thresh)
{
	int count=0;
	for (int i=0; i<vecLen; i++) 
		if(abs(vec[i])>thresh)
			count++;
	return count;
}

void get0DIgnStringSet(string bkey,int *ignTag,int ignNum,string *res)
{
	uint64_t num=(uint64_t)pow(ignNum,2.0);
	int len=bkey.size();
	for(uint64_t i=0;i<num;i++)
	{
		string tkey=bkey;
		int cbit=ignNum;
		for(int j=0;j<len;j++)
		{
			if(ignTag[j]==1)
			{
				uint64_t temp=i;
				uint64_t pos=(temp<<(64-cbit))>>63;
				tkey.replace(j,1,1,char('0'+pos));
				cbit--;
			}
		}
		res[i]=tkey;
	}
	return;
}

void get1DIgnStringSet(string bkey,int *ignTag,int ignNum,string *res)
{
	uint64_t num=(uint64_t)pow(ignNum,2.0);
	int len=bkey.size();
	int cur=0;
	
	for(int k=0;k<len;k++)
	{
		if(ignTag[k]==0)
		{
			string ttkey=bkey;
			char curChar=ttkey.at(k);
			int curValue = atoi(&curChar);
			int revValue = 1-curValue;
			char revChar = '0'+revValue;
			ttkey.replace(k,1,1,revChar);
			for(uint64_t i=0;i<num;i++)
			{
				string tkey=ttkey;
				int cbit=ignNum;
				for(int j=0;j<len;j++)
				{
					if(ignTag[j]==1)
					{
						uint64_t temp=i;
						uint64_t pos=(temp<<(64-cbit))>>63;
						tkey.replace(j,1,1,char('0'+pos));
						cbit--;
					}
				}
				res[cur]=tkey;
				cur++;
			}
		}
	}
	return;
}

void get2DIgnStringSet(string bkey,int *ignTag,int ignNum,string *res)
{
	int num=(int)pow(ignNum,2.0);
	int len=bkey.size();
	int cur=0;	
	for(int k=0;k<len;k++)
	{
		if(ignTag[k]==0)
		{
			string ttkey=bkey;
			char curChar=ttkey.at(k);
			char revChar = '0'+1-atoi(&curChar);
			ttkey.replace(k,1,1,revChar);
			for(int l=k+1;l<len;l++)
			{
				if(ignTag[l]==0)
				{
					string tttkey=ttkey;
					char curChar=tttkey.at(l);
					char revChar = '0'+1-atoi(&curChar);
					tttkey.replace(l,1,1,revChar);
					get0DIgnStringSet(tttkey,ignTag,ignNum,res+cur);
					cur+=num;
				}
			}
		}
	}
	return;
}


//***************** distance-calculating functions **********************//
int hammingDist(string str1,string str2)
{
	int len1=str1.size();
	int len2=str2.size();
	if(len1!=len2)
		return -1;
	int dist=0;
	for (int i=0; i<len1; i++) 
	{
		if(str1.compare(i,1,str2,i,1)!=0)
			dist++;
	}
	return dist;
}

int hammingDist(string str1,string str2,int* ignTag,int keyLen)
{
	int len1=str1.size();
	int len2=str2.size();
	if(len1!=len2)
		return -1;
	if(len1!=keyLen)
		return -1;
	int dist=0;
	for (int i=0; i<len1; i++) 
	{
		if(ignTag[i]==1)
			continue;
		if(str1.compare(i,1,str2,i,1)!=0)
			dist++;
	}
	return dist;
}

double dist(double *vec1,double *vec2,int vecLen)
{
	double ret=0.0;
	for (int i=0; i<vecLen; i++) 
		ret+=pow((vec1[i]-vec2[i]),2.0);
	ret = sqrt(ret);
	return ret;
}

vector<int> distOrderToOthers(double **vecList,int row,int col,int id,double pcnt)
{
	if((row<=1)||(col<1))
	{		
		vector<int> sortedNearIdx;
		return sortedNearIdx;
	}
	double *d = new double[row-1];
	double *curVec = vecList[id];
	for (int i=0; i<row; i++) 
		d[i] = dist(curVec,vecList[i],col);
	int *sortedIdx=new int[row];
	sortDataAndIdx(d,sortedIdx,row);
	int last=(int)(((double)row)*pcnt);
	vector<int> sortedNearIdx(sortedIdx,sortedIdx+last);
	return sortedNearIdx;
}

double minDistToOthers(double **vecList,int row,int col,int id,int &nnId)
{//the (id)th point's min dist to other points in this Set.
	if((row<=1)||(col<1))
		return -1;
	double min = DBL_MAX;
	double *curVec = vecList[id];
	for (int i=0; i<row; i++) 
	{
		if(i==id)
			continue;
		double *tVec = vecList[i];
		double curD = dist(curVec,tVec,col);
		if(min>curD)
		{
			min=curD;
			nnId=i;
		}
	}
	return min;
}

double minorAvgDistToOthers(double **vecList,int row,int col,int id,double thresh)
{//the (id)th point's avg dist to other points in this Set.
	int goal=(int)(row*thresh);
	if((goal<1)||(col<1))
		return -1;
	int nnId=0;
	if(goal==1)
		return minDistToOthers(vecList,row,col,id,nnId);
	double *d = new double[row-1];
	double *curVec = vecList[id];
	for (int i=0,j=0; i<row; i++) 
	{
		if(i==id)
			continue;
		double *tVec = vecList[i];
		d[j] = dist(curVec,tVec,col);
		j++;
	}
	qsort(d,row-1,sizeof(double),cmp);
	
	return avgOfArray(d,goal+1);
}

double qntDistToOthers(double **vecList,int row,int col,int id,double thresh)
{//the (id)th point's avg dist to other points in this Set.
	int goal=(int)(row*thresh);
	if((goal<1)||(col<1))
		return -1;
	int nnId=0;
	if(goal==1)
		return minDistToOthers(vecList,row,col,id,nnId);
	double *d = new double[row-1];
	double *curVec = vecList[id];
	for (int i=0,j=0; i<row; i++) 
	{
		if(i==id)
			continue;
		double *tVec = vecList[i];
		d[j] = dist(curVec,tVec,col);
		j++;
	}
	qsort(d,row-1,sizeof(double),cmp);

	return d[goal];
}

double avgDistToOthers(double **vecList,int row,int col,int id)
{//the (id)th point's avg dist to other points in this Set.
	if((row<=1)||(col<1))
		return -1;
	double avg = 0;
	double *curVec = vecList[id];
	for (int i=0; i<row; i++) 
	{
		if(i==id)
			continue;
		double *tVec = vecList[i];
		avg+=dist(curVec,tVec,col);
	}
	avg/=(row-1);
	return avg;
}
//***************** printing functions **********************//

void readVectorFromFile(vector<int> &vec,ifstream &fin)
{
	istringstream instr;
	string s;
	while((fin.peek()==10)&&(fin.peek()!=EOF))
		getline(fin,s);
	getline(fin,s);
	if(s.compare(s.length()-1,1," ")==0)
		s.erase(s.length()-1,1);
	instr.str(s);
	int t=0;
	while(!instr.eof())
	{
		instr>>t;		
		vec.push_back(t);
	}
	return;
}

void writeVectorToFile(vector<int> vec,ofstream &fout)
{
	for (int i=0; i<vec.size(); i++) 
		  fout<<vec[i]<<" ";
	fout<<endl;
	return;
}

void readDArrayFromFile(double *vec,int vecLen,ifstream &fin)
{
	for (int i=0; i<vecLen; i++) 
		fin>>vec[i];
	return ;
}

void writeDArrayToFile(double *vec,int vecLen,ofstream &fout)
{
	for (int i=0; i<vecLen; i++) 
		  fout<<vec[i]<<" ";
	fout<<endl;
	return ;
}

void readDArrayListFromFile(double** &vecList,int &row,int &vecLen,ifstream &fin)
{
	//fin.seekg(0,ios::beg);
	fin>>row;
	fin>>vecLen;
	vecList=new double*[row];
	for (int i=0; i<row; i++) 
	{
		vecList[i]=new double[vecLen];
		double *vec=vecList[i];
		for (int j=0; j<vecLen; j++) 
			fin>>vec[j];
	}
	return;
}

void writeDArrayListToFile(double **vecList,int row,int vecLen,ofstream &fout)
{
	fout.seekp(0,ios::beg);
	fout<<row<<" "<<vecLen<<endl;
	for (int i=0; i<row; i++) 
	{
		double *vec=vecList[i];
		for (int j=0; j<vecLen; j++) 
			fout<<vec[j]<<" ";
		fout<<endl;
	}
	return ;
}

void printInt(int i)
{
	cout<<i<<" ";
}

void printlnVector(string *vec,int vecLen)
{
	for (int i=0; i<vecLen; i++) 
		  cout<<vec[i]<<endl;
	return;
}

void printVector(vector<int> vec)
{
	if(vec.size()==0)
		cout<<"Empty vector!";
	for(int i=0;i<vec.size();i++)
		cout<<vec[i]<<" ";
	cout<<endl;
}

void printVector(double *vec,int vecLen)
{
	  for (int i=0; i<vecLen; i++) 
		  cout<<vec[i]<<" ";
	  cout<<endl;
	  return;
}

void printVector(int *vec,int vecLen)
{
	  for (int i=0; i<vecLen; i++) 
		  cout<<vec[i]<<" ";
	  cout<<endl;
	  return;
}

void printVectorList(double **VectorList,int row,int col)
{
	for (int i=0; i<row; i++) 
		printVector(VectorList[i],col);
}

//***************** other functions **********************//
string getlshFName(int fid)
{
	string fname;
	string head="E:/lshTable";
	string tail=".txt";
	string id=to_string(fid);
	fname=head+id+tail;
	return fname;
}


