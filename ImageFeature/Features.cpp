#include "stdafx.h"
#include "Features.h"
#include <fstream>
using namespace std;

ImageFeature::ImageFeature()
{
	GrayLevelCoocurrenceMatrix = new double[4];
	EdgeHist = new double[5];
	Sift = new double[100];
	Hu = new double[7];
	HSVFeat = new double[9];

	GLCM_length = 4;
	EH_length = 5;
	SIFT_length = 100;
	HU_length = 7;
	HSV_length = 9;
}
ImageFeature::~ImageFeature()
{
	/*delete GrayLevelCoocurrenceMatrix;
	delete EdgeHist;
	delete Hu;
	delete HSVFeat;*/
}
double* ImageFeature::getFeat(int FeatID)
{
	switch (FeatID)
{
	case GLCM:
		return GrayLevelCoocurrenceMatrix;
	case EH:
	return EdgeHist;
	case SIFT:
	return Sift;
	case HU:
		return Hu;
	case HSV:
		return HSVFeat;
	default:
		return false;
}
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
	case HU:
		return HU_length;
	case HSV:
		 return HSV_length;
	default:
		return false;
	}
}
void ImageFeature::genFeat(Mat img, calculateFeature calc)
{
	calc.calcGLCM(img, 0, 1, GrayLevelCoocurrenceMatrix);
	calc.calcEH(img, EdgeHist);
	calc.calcHU(img, Hu);
	calc.calcHSV(img, HSVFeat);
	//calc.calcSIFT()
}
double EucDis(double* feat1, double* feat2, int l);
double HistInter(double* feat1, double* feat2, int l);
double Dis1(double* feat1, double* feat2, int l);
double Dis2(double* feat1, double* feat2, int l);

double ImageFeature::Distance(ImageFeature imgFeat, int FeatID)
{
	double* Feat1, * Feat2;
	double diff = 0;
	Feat1 = this->getFeat(GLCM);
	int FeatLength = this->getlength(GLCM);
	Feat2 = imgFeat.getFeat(GLCM);


	switch (FeatID)
	{
		case GLCM:
			diff = EucDis(Feat1, Feat2, FeatLength);
			break;
		case EH:
			diff = HistInter(Feat1, Feat2, FeatLength);
			break;
		case HU:
			diff = Dis1(Feat1, Feat2, FeatLength);
			break;
		case HSV:
			diff = EucDis(Feat1, Feat2, FeatLength);
			break;
	}

	return diff;
}

double EucDis(double* feat1, double* feat2, int l)
{
	int i;
	double dis = 0;
	for (i = 0; i < l; i++)
	{
		dis += (feat1[i] - feat2[i]) * (feat1[i] - feat2[i]);
	}
	return dis;
}
double HistInter(double* feat1, double* feat2, int l)
{
	int i;
	double N1 = 0, N2 = 0, p1, p2, M = 0, Np = 0;
	for (i = 0; i < l; i++)
	{
		N1 += feat1[i];
		N2 += feat2[i];
	}
	for (i = 0; i < l; i++)
	{
		p1 = feat1[i] / N1;
		p2 = feat2[i] / N2;
		M += min(p1, p2);
		Np += p1;
	}
	return M/Np;
}
double Dis1(double* feat1, double* feat2, int l)
{
	int i;
	double M = 0, N1 = 0, N2 = 0;
	for (i = 0; i < l; i++)
	{
		M += fabs(feat1[i]*feat2[i]);
		N1 += feat1[i]*feat1[i];
		N2 += feat2[i]*feat2[i];
	}
	return M/(sqrt(N1)*sqrt(N2));
}
double Dis2(double* feat1, double* feat2, int l)
{
	int i;
	double M = 0, N1 = 0, N2 = 0;
	for (i = 0; i < l; i++)
	{
		N1 += fabs(feat1[i] - feat2[i]);
		N2 += fabs(feat1[i] + feat2[i]);
	}
	return (1-N1/N2);
}


int result[GLCM_LEVEL* GLCM_LEVEL];
double mins[9998] = {0};
int min_ids[9998] = {0};
int min_type[9998] = {0};

int max(int* a, int l);
void accumarray(int* sub1, int* sub2, int l, int row, int col);
Mat RGB2GRAY(Mat img);
void RGB2HSV(Mat img, double * HSVdata);

void calculateFeature::calcGLCM(Mat image, int offset1, int offset2, double* a)
{
	image = RGB2GRAY(image);
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
	//*/


	//double *a = new double [4];
	a[0] = entropy;
	a[1] = energy;
	a[2] = contrast;
	a[3] = homogenity;
	//delete result;
	//delete level;
	//delete v1;
	//delete v2;

	//return a;
}

/*
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
*/

int max(int* a, int l)
{
	int i;
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
	int i;
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



void calculateFeature::calcEH(Mat image, double* edgehist)
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
	//int* edgehist = new int [5];
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

void calculateFeature::calcHU(Mat img, double *hu)
{
	double m10 = 0, m01 = 0, m00 = 0, u00 = 0, u11 = 0, u20 = 0, u02 = 0, u21 = 0, u12 = 0, u30 = 0, u03 = 0;
	double x0, y0, y11, y20, y02, y30, y03, y21, y12;
	int row = img.rows;
	int col = img.cols;
	int i, j;
	Mat gray = RGB2GRAY(img);
	//hu = new double [7];
	
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			m10 += (double)((i+1)*img.data[i*col+j]);
			m01 += (double)((j+1)*img.data[i*col+j]);
			m00 += (double)(img.data[i*col+j]);
		}
	}
	x0 = m10/m00;
	y0 = m01/m00;

	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			double x, y;
			x = i+1-x0;
			y = j+1-y0;

			u00 += (double)(img.data[i*col+j]);

			//二阶
			double a = (double)(img.data[i*col+j]);
			u11 += x*y*(double)(img.data[i*col+j]);
			u20 += x*x*(double)(img.data[i*col+j]);
			u02 += y*y*(double)(img.data[i*col+j]);

			//三阶
			u21 += x*x*y*(double)(img.data[i*col+j]);
			u12 += x*y*y*(double)(img.data[i*col+j]);
			u30 += x*x*x*(double)(img.data[i*col+j]);
			u03 += y*y*y*(double)(img.data[i*col+j]);
		}
	}
	
	//二阶
	y11 = u11/(u00*u00);
	y20 = u20/(u00*u00);
	y02 = u02/(u00*u00);

	//三阶
	y21 = u21/sqrt(u00*u00*u00*u00*u00);
	y12 = u12/sqrt(u00*u00*u00*u00*u00);
	y30 = u30/sqrt(u00*u00*u00*u00*u00);
	y03 = u03/sqrt(u00*u00*u00*u00*u00);

	double t1, t2, t3, t4, t5, t6;
	t1 = y20+y02;
	t2 = y20-y02;
	t3 = y30-3*y12;
	t4 = 3*y21-y03;
	t5 = y30+y12;
	t6 = y21+y03;

	hu[0] = log(t1);
	hu[1] = log(t2*t2+4*y11*y11);
	hu[2] = log(t3*t3+t4*t4);
	hu[3] = log(t5*t5+t6*t6);
	hu[4] = log(t3*t5*(t5*t5-3*t6*t6)+t4*t6*(3*t5*t5-t6*t6));
	hu[5] = log(t2*(t5*t5-t6*t6)+4*u11*t5*t6);
	hu[6] = log(t4*t5*(t5*t5-3*t6*t6)-t3*t6*(3*t5*t5-t6*t6));
}
void calculateFeature::calcHSV(Mat img, double *hsvfeat)
{
	int col = img.cols;
	int row = img.rows;
	int imgSize = row*col;
	double *HSVdata = new double[3*imgSize];
	double *Hdata = new double[imgSize];
	double *Sdata = new double[imgSize];
	double *Vdata = new double[imgSize];
	RGB2HSV(img, HSVdata);
	Mat img_H(col,row,CV_8UC1,Scalar(0,0)), img_S(col,row,CV_8UC1,Scalar(0,0)), img_V(col,row,CV_8UC1,Scalar(0,0)), img_HSV(col,row,CV_8UC3,Scalar(0,0,0));
	int k = 0;
	double mean_H = 0, mean_S = 0, mean_V = 0, var_H = 0, var_S = 0, var_V = 0, skew_H = 0, skew_S = 0, skew_V = 0;
	for (int i = 0; i < imgSize*3; i+= 3)
	{
		Hdata[k] = HSVdata[i];
		mean_H += (double)HSVdata[i];
		k++;
	}
	k = 0;
	for (int i = 1; i < imgSize*3; i+= 3)
	{
		Sdata[k] = HSVdata[i];
		mean_S += (double)HSVdata[i];
		k++;
	}
	k = 0;
	for (int i = 2; i < imgSize*3; i+= 3)
	{
		Vdata[k] = HSVdata[i];
		mean_V += (double)HSVdata[i];
		k++;
	}
	mean_H = mean_H / (double)imgSize;
	mean_S = mean_S / (double)imgSize;
	mean_V = mean_V / (double)imgSize;

	for (int i = 0; i < imgSize; i++)
	{
		var_H += (Hdata[i] - mean_H) * (Hdata[i] - mean_H);
		var_S += (Sdata[i] - mean_S) * (Sdata[i] - mean_S);
		var_V += (Vdata[i] - mean_V) * (Vdata[i] - mean_V);

		skew_H += (Hdata[i] - mean_H) * (Hdata[i] - mean_H) * (Hdata[i] - mean_H);
		skew_S += (Sdata[i] - mean_S) * (Sdata[i] - mean_S) * (Sdata[i] - mean_S);
		skew_V += (Vdata[i] - mean_V) * (Vdata[i] - mean_V) * (Vdata[i] - mean_V);
	}
	var_H = sqrt(var_H / (double)imgSize);
	var_S = sqrt(var_S / (double)imgSize);
	var_V = sqrt(var_V / (double)imgSize);

	double temp = skew_H / (double)imgSize;
	if (temp < 0)
		skew_H = -pow(-skew_H / (double)imgSize, 1.0/3.0);
	else
		skew_H = pow(skew_H / (double)imgSize, 1.0/3.0);
	
	temp = skew_S / (double)imgSize;
	if (temp < 0)
		skew_S = -pow(-skew_S / (double)imgSize, 1.0/3.0);
	else
		skew_S = pow(skew_S / (double)imgSize, 1.0/3.0);
	
	temp = skew_V / (double)imgSize;
	if (temp < 0)
		skew_V = -pow(-skew_V / (double)imgSize, 1.0/3.0);
	else
		skew_V = pow(skew_V / (double)imgSize, 1.0/3.0);
	//skew_S = pow(skew_S / (double)imgSize, 1.0/3.0);
	//skew_V = pow(skew_V / (double)imgSize, 1.0/3.0);

	hsvfeat[0] = mean_H;
	hsvfeat[1] = var_H;
	hsvfeat[2] = skew_H;
	hsvfeat[3] = mean_S;
	hsvfeat[4] = var_S;
	hsvfeat[5] = skew_S;
	hsvfeat[6] = mean_V;
	hsvfeat[7] = var_V;
	hsvfeat[8] = skew_V;
	delete HSVdata;
	delete Hdata;
	delete Sdata;
	delete Vdata;
}

Mat RGB2GRAY(Mat img)
{
	if (img.channels() == 1)
		return img;
	int col = img.cols;
	int row = img.rows;
	int imgSize = row*col;
	Mat img_R(col,row,CV_8UC1,Scalar(0,0)), img_G(col,row,CV_8UC1,Scalar(0,0)), img_B(col,row,CV_8UC1,Scalar(0,0)), img_gray(col,row,CV_8UC1,Scalar(0,0));
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
	for (int i = 0; i < imgSize; i++)
	{
		img_gray.data[i] = (uchar)(0.299*((double)img_R.data[i])+0.587*((double)img_G.data[i])+0.114*((double)img_B.data[i]));
	}
	return img_gray;

}
// 输出的排列顺序H,S,V
void RGB2HSV(Mat img, double * HSVdata)
{
	int col = img.cols;
	int row = img.rows;
	int imgSize = row*col;
	Mat img_R(col,row,CV_8UC1,Scalar(0,0)), img_G(col,row,CV_8UC1,Scalar(0,0)), img_B(col,row,CV_8UC1,Scalar(0,0)), img_HSV(col,row,CV_8UC3,Scalar(0,0,0));
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

	// calc
	k = 0;
	int r,g,b;
	for (int i = 0; i < imgSize*3; i+= 3)
	{
		// H
		r = img_R.data[k];
		g = img_G.data[k];
		b = img_B.data[k];
		int MAX = max(max(r,g),b);
		int MIN = min(min(r,g),b);
		if (MAX == MIN)
			HSVdata[i] = 0;
		else if (MAX == r && g >= b)
			HSVdata[i] = 60.0*((double)(g-b))/((double)MAX-(double)MIN);
		else if (MAX == r && g < b)
			HSVdata[i] = 60.0*((double)(g-b))/((double)MAX-(double)MIN)+360.0;
		else if (MAX == g)
			HSVdata[i] = 60.0*((double)(b-r))/((double)MAX-(double)MIN)+120.0;
		else if (MAX == b)
			HSVdata[i] = 60.0*((double)(r-g))/((double)MAX-(double)MIN)+240.0;
		k++;
	}
	k = 0;
	for (int i = 1; i < imgSize*3; i+= 3)
	{
		// S
		r = img_R.data[k];
		g = img_G.data[k];
		b = img_B.data[k];
		int MAX = max(max(r,g),b);
		int MIN = min(min(r,g),b);
		if (MAX == 0)
			HSVdata[i] = 0;
		else
			HSVdata[i] = 1.0-((double)MIN/(double)MAX);
		k++;
	}
	k = 0;
	for (int i = 2; i < imgSize*3; i+= 3)
	{
		// V
		r = img_R.data[k];
		g = img_G.data[k];
		b = img_B.data[k];
		HSVdata[i] = max(max(r,g),b);
		k++;
	}
	//return img_HSV;
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


DLLEXPORT ImageFeature* CalFeatureForImages(MyMat *imgs, int num)
{
	//TODO:根据path把数据读进来然后算特征,算完特征存起来？
	ImageFeature* features = new ImageFeature[num];
	calculateFeature calc;
	//calc.siftBowPreprocess(imgs, num);
	for(int i = 0; i < num; i++)
	{
		features[i].id = imgs[i].id;
		features[i].genFeat(imgs[i], calc);
	}
	return features;
}


DLLEXPORT double CalFeatureDistance(ImageFeature &ele1, ImageFeature &ele2, int FeatIDs[], int num)
{
	double d = 0;
	for(int i = 0; i < num; i++)
		d += ele1.Distance(ele2, FeatIDs[i]);
	return d;
}

//如果已经存在词汇表,就不用调用这个函数. 最好事先生成词汇表
void calculateFeature::siftBowPreprocess(MyMat *imgs, int num){
	//如果已经存在词汇表,直接返回
	FileStorage fs("E:\\localVocabulary.yml", FileStorage::READ);
	if(fs.isOpened())
		return;

	//sift特征点
	vector<KeyPoint> keypoints;
	//sift特征向量
	Mat descriptor, featuresUnclustered;
	SiftDescriptorExtractor detector;

	for(int f=0;f<num;f++){
		detector.detect(imgs[f], keypoints);
		detector.compute(imgs[f], keypoints,descriptor);
		featuresUnclustered.push_back(descriptor);
		printf("%f percent finished", ((double)f+1)/num);
	}


	int dictionarySize=500;
	//终止条件
	TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
	//最近邻
	int flags=KMEANS_PP_CENTERS;
	BOWKMeansTrainer bowTrainer(dictionarySize,tc,1,flags);
	//聚类
	localVocabulary=bowTrainer.cluster(featuresUnclustered);
	FileStorage fs1("E:\\localVocabulary.yml", FileStorage::WRITE);
	fs1 << "vocabulary" << localVocabulary;
	fs1.release();
}

double* calculateFeature::calcSIFT(Mat img, int dictSize = 500) {
	FileStorage fs("E:\\localVocabulary.yml", FileStorage::READ);
    fs["vocabulary"] >> localVocabulary;
	fs.release();

	vector<KeyPoint> keypoints;
	SiftDescriptorExtractor detector;
	//快速最近邻匹配
	FlannBasedMatcher matcher;
	SiftDescriptorExtractor extractor;
    BOWImgDescriptorExtractor bowDE(&extractor, &matcher);
    //设置词汇表
    bowDE.setVocabulary(localVocabulary);
	detector.detect(img,keypoints);

	//计算词汇表特征向量
	Mat descriptors;
	extractor.compute(img, keypoints, descriptors);
	double *Sift = new double[localVocabulary.rows];
	memset(Sift, 0, sizeof(double)*localVocabulary.rows);

	vector<DMatch> matches;
	matcher.match(descriptors, localVocabulary, matches);
	for(int i = 0; i < descriptors.rows; i++) {
		DMatch tmpMatch = matches[i];
		int tmpInt = tmpMatch.trainIdx;
		Sift[tmpInt]++;
	}

	return Sift;
}