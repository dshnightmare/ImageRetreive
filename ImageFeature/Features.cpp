#include "stdafx.h"
#include "Features.h"
#include <fstream>
using namespace std;

double* ImageFeature::getGLCM()
{
	return GrayLevelCoocurrenceMatrix;
}
double* ImageFeature::getEH()
{
	return EdgeHist;
}
double* ImageFeature::getSIFT()
{
	return Sift;
}
int ImageFeature:: getlength(int FeatID)
{
	switch (FeatID)
	{
	case GLCM:
		return GLCM_length;
	case EH:
		return EH_length;
	case SIFT:
		return SIFT_length;
	}
}

double ImageFeature::Distance(ImageFeature imgFeat, int FeatID)
{
	double* Feat1, * Feat2;
	double diff = 0;
	int i, j, k;
	int length;
	switch (FeatID)
	{
	case GLCM:
		Feat1 = this->getGLCM();
		length = this->getlength(GLCM);
		Feat2 = imgFeat.getGLCM();
	case EH:
		Feat1 = this->getEH();
		length = this->getlength(EH);
		Feat2 = imgFeat.getEH();
	case SIFT:
		Feat1 = this->getSIFT();
		length = this->getlength(SIFT);
		Feat2 = imgFeat.getSIFT();

	}
	for (i = 0; i < length; i++)
	{
		diff += (Feat1[i] - Feat2[i])*(Feat1[i] - Feat2[i]);
	}
	return diff;
}


int result[GLCM_LEVEL* GLCM_LEVEL];
double mins[9998] = {0};
int min_ids[9998] = {0};
int min_type[9998] = {0};

int max(int* a, int l);
void accumarray(int* sub1, int* sub2, int l, int row, int col);

double* calculateFeature::calcGLCM(Mat image, int offset1, int offset2)
{
	int i, j, k, row, col, Index;
	int nl = 8;
	int ch = image.channels();
	row = image.rows;
	col = image.cols;
	//int* result = new int [GLCM_LEVEL*GLCM_LEVEL];
	int* level = new int [row*col];
	int* v1 = new int [row*col];
	int* v2 = new int [row*col];
	uchar* imageData = image.data;

	// for Gray
	k = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			int ind = 3*(i*col+j);
			int temp = (int)imageData[(i*col+j)*3];
			level[i*col+j] = ((int)imageData[(i*col+j)*3])*(GLCM_LEVEL-1)/255+1;
			v1[j*row+i] = ((int)imageData[(i*col+j)*3])*(GLCM_LEVEL-1)/255+1;
		}
	}

	//memset(result, 0, GLCM_LEVEL*GLCM_LEVEL);

	//for (i = 0; i < GLCM_LEVEL; i++)
	//	for (j = 0; j < GLCM_LEVEL; j++)
	//		result[i * GLCM_LEVEL + j] = 0;

	int r,c;

	k = 0;
	for (j = 0; j < col; j++)
	{
		for (i = 0; i < row; i++)
		{
			r = j+offset1;
			c = i+offset2;

			Index = r+c*row;
			v2[k] = level[Index];
			k++;
		}
	}

	accumarray(v1, v2, row*col, row, col);

	int t = 1;

	double entropy = 0,energy = 0,contrast = 0,homogenity = 0;

	for (i = 0;i < GLCM_LEVEL;i++)
	{
		for (j = 0;j < GLCM_LEVEL;j++)
		{
			//熵
			if(result[i * GLCM_LEVEL + j] > 0)
				entropy += result[i * GLCM_LEVEL + j] * log10(double(result[i * GLCM_LEVEL + j]));
			//能量
			energy += result[i * GLCM_LEVEL + j] * result[i * GLCM_LEVEL + j];
			//对比度
			contrast += (i - j) * (i - j) * result[i * GLCM_LEVEL + j];
			//一致性
			if (i != j)
				homogenity += result[i * GLCM_LEVEL + j] / ((i - j) * (i - j));
		}
	}
	//*/


	double a[4]={0};// = new double [4];
	a[0] = entropy;
	a[1] = energy;
	a[2] = contrast;
	a[3] = homogenity;
	//delete result;
	delete level;
	delete v1;
	delete v2;

	return a;
}

int max(int* a, int l)
{
	int i, t;
	int mmax = -999;
	for (i = 0; i < l; i++)
	{
		if (mmax < a[i])
		{
			mmax = a[i];
		}
	}
	return mmax;
}

void accumarray(int* sub1, int* sub2, int l, int row, int col)
{
	int i, j, k, idx;
	//int* result = new int [GLCM_LEVEL* GLCM_LEVEL];
	memset(result, 0, sizeof(int)*GLCM_LEVEL*GLCM_LEVEL);
	/*
	for (i = 0; i < GLCM_LEVEL; i++)
		for (j = 0; j < GLCM_LEVEL; j++)
			result[i * GLCM_LEVEL + j] = 0;
			*/

	for (i = 0; i < l; i++)
	{
		if (sub1[i] >= row || sub2[i] >= col || sub1[i] < 0 || sub2[i] < 0)
			continue;
		result[sub1[i]*GLCM_LEVEL+sub2[i]] += 1;
	}
	//return result;
}

/*
Mat ImageFeature::ReadImage(int _id)
{
	int offset = _id;
	uchar *buf=(uchar*)calloc(imgDataSize,sizeof(uchar));
	fin.seekg(offset*imgDataSize,ios::beg);
	fin.read((char*)buf,(imgDataSize)*sizeof(char));
	int origin_type = (int)buf[0];
	cout << (int)buf[0] << endl;
	
	int planeId[3]={2,1,0};

	for( int i=0; i<height; i++) 
	{
		for( int j=0; j<width; j++ ) {
			uchar* dataIJ = img.data + i * img.step + j * img.elemSize();// img.at(i, j)
			for(int k=0;k<nChannels;k++)
{
				int idx=1+planeId[k]*imgSize+i*width+j;
				dataIJ[k]=buf[idx];
			}
			
		}
	}
}*/


DLLEXPORT void CalFeatureForImages(string path)
{
	//TODO:根据path把数据读进来然后算特征,算完特征存起来？
}