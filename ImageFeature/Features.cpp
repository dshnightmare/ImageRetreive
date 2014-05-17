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

void calculateFeature::calcGLCM(Mat image, int offset1, int offset2, double* a)
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

	/*
	for (i = 0; i < row*col*ch; i++)
	{
		if (i%3 == 0 && i!= 0)
			cout << endl;
		cout << (int) imageData[i] << ' ';
	}*/
	// for Gray
	
	k = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			if (j == 255)
			{
				int ab = 1;
			}
			if (j+offset2>= col)// || j+offset1 >= row)
			{
				continue;
			}
			int ind3 = 3*(i*col+j);
			int ind = i*col+j;
			int temp = (int)imageData[(i*col+j)];
			if (k == 65269)
			{
				int abc = 1;
			}
			v1[k] = (int)(((double)((int)imageData[(i*col+j)])*(GLCM_LEVEL-1)) /(double)255 + 1 + 0.5);
			//v1[k] = (int)imageData[(i*col+j)*3];
			k++;
		}
	}
	
	k = 0;
	for (j = 0; j < col; j++)
	{
		for (i = 0; i < row; i++)
		{
			//level[i*row+j] = (int)(((double)((int)imageData[(j*row+i)*3])*(GLCM_LEVEL-1)) /(double)255 + 1 + 0.5);
			if (i*row+j == 482)
			{
				int afff = 1;
			}
			//level[j*row+i] = (int)imageData[(i*col+j)*3]; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			level[j*row+i] = (int)(((double)((int)imageData[(i*col+j)])*(GLCM_LEVEL-1)) /(double)255 + 1 + 0.5);
			k++;
		}
	}
	//for (i = 307188;i <= 307199; i++)
	//	cout << level[i] << ' ';
	//cout << endl;


	/*
	// for RGB
	for (k = 0; k < ch; k++)
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				int temp = (int)imageData[k*row*col+i*col+j];
				level[i*col+j] = ((int)imageData[k*row*col+i*col+j])*(GLCM_LEVEL-1)/255+1;
				v1[j*row+i] = ((int)imageData[k*row*col+i*col+j])*(GLCM_LEVEL-1)/255+1;
			}
		}
	}
	*/
	


	//memset(result, 0, GLCM_LEVEL*GLCM_LEVEL);

	//for (i = 0; i < GLCM_LEVEL; i++)
	//	for (j = 0; j < GLCM_LEVEL; j++)
	//		result[i * GLCM_LEVEL + j] = 0;

	int r,c;

	k = 0;
	int t = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			r = i+offset1;
			c = j+offset2;
			if (i == 254)
			{
				int aa = 1;
			}
			if (c>= col)
			{
				t++;
				continue;
			}
			
			int ind3 = 3*(i*col+j);
			int ind = i*col+j;
			int temp = (int)imageData[(i*col+j)];
			int ind2 = i*col+j-t;
			//v1[i*col+j-t] = ((int)imageData[(i*col+j)*3]);//*(GLCM_LEVEL-1)/255+1;
			//v1[k] = ((int)imageData[(i*col+j)*3]);//*(GLCM_LEVEL-1)/255+1;
			Index = r+c*row;
			if (Index == 481)
			{
				int agggg = 1;
			}
			v2[k] = level[Index];
			if ((k+1)%470 == 0)
			{
				int agg = 1;
			}
			k++;
		}
	}
	
	/*
	cout << "v1:" << endl;
	//for (i = 65269;i <= 65280; i++)
	//for (i = 306710;i <= 306719; i++)
	//for (i = 149999;i <= 150009; i++)
	for (i = 0;i <= 10; i++)
		cout << v1[i] << ' ';
	cout << endl;
	
	cout << "v2:" << endl;
	//for (i = 65269;i <= 65280; i++)
	//for (i = 306710;i <= 306719; i++)
	//for (i = 149999;i <= 150009; i++)
	for (i = 0;i <= 10; i++)
		cout << v2[i] << ' ';
	cout << endl;
	*/

	accumarray(v1, v2, row*col, row, col);

	/*
	for (i = 0; i < GLCM_LEVEL; i++)
	{
		for (j = 0; j < GLCM_LEVEL; j++)
		{
			printf("%8d ", result[i*GLCM_LEVEL+j]);
		}
		cout << endl;
	}
	*/

	//int t = 1;

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
}

double* calculateFeature::calcCLCM(Mat img, int offset1, int offset2)
{
	int width = img.cols;
	int height = img.rows;
	int imgSize = width*height;
	Mat img_R(width,height,CV_8UC1,Scalar(0,0)), img_G(width,height,CV_8UC1,Scalar(0,0)), img_B(width,height,CV_8UC1,Scalar(0,0));
	double* glcm = new double[4];
	if (img.channels() == 3)
	{
		int k = 0;
		for (int i = 0; i < imgSize*3; i+= 3)
		{
			img_B.data[k] = img.data[i];
			k++;
		}
		k = 0;
		for (int i = 1; i < imgSize*3; i+= 3)
		{
			img_G.data[k] = img.data[i];
			k++;
		}
		k = 0;
		for (int i = 2; i < imgSize*3; i+= 3)
		{
			img_R.data[k] = img.data[i];
			k++;
		}
		double* R_GLCM = new double[4], *G_GLCM = new double[4], *B_GLCM = new double[4];
		calcGLCM(img_R, offset1, offset2, R_GLCM);
		calcGLCM(img_G, offset1, offset2, G_GLCM);
		calcGLCM(img_B, offset1, offset2, B_GLCM);
		for (int i = 0; i < 4; i++)
		{
			glcm[i] = (R_GLCM[i] + G_GLCM[i] + B_GLCM[i])/3;
		}
		delete R_GLCM;
		delete G_GLCM;
		delete B_GLCM;
	}
	else
	{
		calcGLCM(img, offset1, offset2, glcm);
	}
	return glcm;
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



void calculateFeature::calcEH(Mat &image, int* edgehist)
{
	int i;
	Mat kernel1 = (Mat_<char>(2,2) <<  1, -1,
									1, -1);
	Mat kernel2 = (Mat_<char>(2,2) <<  1, 1,
									-1, -1);
	Mat kernel3 = (Mat_<char>(2,2) <<  0, sqrt(2),
									-sqrt(2), 0);
	Mat kernel4 = (Mat_<char>(2,2) <<  sqrt(2), 0,
									0, -sqrt(2));
	Mat kernel5 = (Mat_<char>(2,2) <<  2, -2,
									-2, 2);

	//double _data[4] = {2, -2, -2, 2};
	//Mat kernel(2,2, type, _data);
	Mat img;
	filter2D(image, img, -1, kernel1);
	int l = img.rows*img.cols, count = 0, TH = 100;
	edgehist = new int [5];
	for (i = 0; i < l; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[0] = count;
	filter2D(image, img, -1, kernel2);
	count = 0;
	for (i = 0; i < l; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[1] = count;
	filter2D(image, img, -1, kernel3);
	count = 0;
	for (i = 0; i < l; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[2] = count;
	filter2D(image, img, -1, kernel4);
	count = 0;
	for (i = 0; i < l; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[3] = count;
	filter2D(image, img, -1, kernel5);
	count = 0;
	for (i = 0; i < l; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[4] = count;
	//return _data;
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


void calculateFeature::siftBowPreprocess(){
	//sift特征点
	vector<KeyPoint> keypoints;
	//sift特征向量
	Mat descriptor, featuresUnclustered;
	SiftDescriptorExtractor detector;    

	//for(int f=0;f<1000;f++){
	//	detector.detect(input, keypoints);
	//	detector.compute(input, keypoints,descriptor);
	//	featuresUnclustered.push_back(descriptor);
	//}


	int dictionarySize=200;
	//define Term Criteria
	TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
	//retries number
	int retries=1;
	//necessary flags
	int flags=KMEANS_PP_CENTERS;
	//Create the BoW (or BoF) trainer
	BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
	//cluster the feature vectors
	localVocabulary=bowTrainer.cluster(featuresUnclustered);
}

double* calculateFeature::calcSIFT(Mat img, int dictSize = 500) {
	vector<KeyPoint> keypoints;
	SiftDescriptorExtractor detector;
	//create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    //create Sift descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);    
    //create BoF (or BoW) descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor,matcher);
    //设置词汇表
    bowDE.setVocabulary(localVocabulary);
	detector.detect(img,keypoints);
    //计算bow特征向量
    Mat bowDescriptor;
    bowDE.compute(img,keypoints,bowDescriptor);

	return (double*)(bowDescriptor.data);
}