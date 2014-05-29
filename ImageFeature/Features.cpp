#include "stdafx.h"
#include "Features.h"
#include <fstream>
using namespace std;


ImageFeature::ImageFeature()
{
	GrayLevelCoocurrenceMatrix = new double[5];
	EdgeHist = new double[5];
	Sift = new double[SIFT_VOCA_SIZE+1];
	Hu = new double[7];
	HSVFeat = new double[9];
	WaveFeat = new double[12];

	GLCM_length = 5;
	EH_length = 5;
	SIFT_length = SIFT_VOCA_SIZE;
	HU_length = 7;
	HSV_length = 9;
	WAVE_length = 12;
}

double* ImageFeature::getFeat(int FeatID)
{
	switch (FeatID)
{
	case GLCM:
		return GrayLevelCoocurrenceMatrix;
	case EH:
		return EdgeHist;
	case HU:
		return Hu;
	case HSV:
		return HSVFeat;
	case SIFT:
		return Sift;
	case WAVELET:
		return WaveFeat;
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
	case HU:
		return HU_length;
	case HSV:
		 return HSV_length;
	case SIFT:
		return SIFT_length;
	case WAVELET:
		return WAVE_length;
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
	calc.calcWaveFeat(img, WaveFeat);
	//calc.calcSIFT(img, Sift);
}
double EucDis(double* feat1, double* feat2, int l);
double EucDis2(double* feat1, double* feat2, int l);
double HistInter(double* feat1, double* feat2, int l);
double Dis1(double* feat1, double* feat2, int l);
double Dis2(double* feat1, double* feat2, int l);

double ImageFeature::Distance(ImageFeature &imgFeat, int FeatID)
{
	double* Feat1, * Feat2;
	double diff = 0;
	Feat1 = this->getFeat(FeatID);
	int FeatLength = this->getlength(FeatID);
	Feat2 = imgFeat.getFeat(FeatID);


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
		case SIFT:
			diff = EucDis(Feat1, Feat2, FeatLength);
		case WAVELET:
			diff = EucDis2(Feat1, Feat2, FeatLength);
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
double EucDis2(double* feat1, double* feat2, int l)
{
	int i;
	double dis1 = 0, dis2 = 0;
	for (i = 0; i < 4; i++)
	{
		dis1 += (feat1[i] - feat2[i]) * (feat1[i] - feat2[i]);
	}
	for (i = 4; i < 12; i++)
	{
		dis2 += (feat1[i] - feat2[i]) * (feat1[i] - feat2[i]);
	}
	return 0.5*sqrt(dis1)+0.5*sqrt(dis2);
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
	return 1.0 - M;
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
	return 1.0 - M/(sqrt(N1)*sqrt(N2));
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
	return (1.0 - N1/N2);
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
	double mean = 0, sd = 0;

	for (i = 0;i < GLCM_LEVEL*GLCM_LEVEL;i++)
	{
		mean += result[i];
	}
	mean /= (GLCM_LEVEL*GLCM_LEVEL);
	for (i = 0;i < GLCM_LEVEL*GLCM_LEVEL;i++)
	{
		sd += (result[i] - mean) * (result[i] - mean);
	}
	sd = sqrt(sd / ((double)(GLCM_LEVEL*GLCM_LEVEL)));
	double f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0, f7 = 0, f8 = 0, f9 = 0, f10 = 0, f11 = 0, f12 = 0, f13 = 0;
	for (i = 0;i < GLCM_LEVEL;i++)
	{
		for (j = 0;j < GLCM_LEVEL;j++)
		{
			// Angular Second Moment
			f1 += (double)result[i * GLCM_LEVEL + j]*(double)result[i * GLCM_LEVEL + j];

			// Variance
			f4 += ((double)result[i * GLCM_LEVEL + j] - mean)*((double)result[i * GLCM_LEVEL + j] - mean)*(double)result[i * GLCM_LEVEL + j];

			// Inverse Difference Moment
			f5 += (double)result[i * GLCM_LEVEL + j]/(double)(1+(i-j)*(i-j));

			// Entropy
			if (result[i * GLCM_LEVEL + j] > 0)
				f9 += (double)result[i * GLCM_LEVEL + j]*log((double)result[i * GLCM_LEVEL + j]);


		}
	}
	int n;
	for (n = 1;n < GLCM_LEVEL;n++)
	{
		for (i = 0;i < GLCM_LEVEL;i++)
		{
			for (j = 0;j < GLCM_LEVEL;j++)
			{
				if (abs(i-j) == n)
					f2 += n*n*(double)result[i * GLCM_LEVEL + j];
			}
		}
	}
	a[0] = f1;
	a[1] = f2;
	a[2] = f4;
	a[3] = f5;
	a[4] = f9;
	/*
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
	//a[0] = entropy;
	//a[1] = energy;
	//a[2] = contrast;
	//a[3] = homogenity;
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
	int l = img.rows*img.cols, count = 0;
	double TH;
	//int* edgehist = new int [5];
	TH = 0;
	int level = img.channels();
	for (i = 0; i < l*level; i++)
	{
		TH += (double)img.data[i];
	}
	TH /= l;
	for (i = 0; i < l*level; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[0] = count;
	filter2D(image, img, -1, kernel2);

	TH = 0;
	for (i = 0; i < l*level; i++)
	{
		TH += (double)img.data[i];
	}
	TH /= l;
	count = 0;
	for (i = 0; i < l*level; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[1] = count;
	filter2D(image, img, -1, kernel3);

	TH = 0;
	for (i = 0; i < l*level; i++)
	{
		TH += (double)img.data[i];
	}
	TH /= l;
	count = 0;
	for (i = 0; i < l*level; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[2] = count;
	filter2D(image, img, -1, kernel4);

	TH = 0;
	for (i = 0; i < l*level; i++)
	{
		TH += (double)img.data[i];
	}
	TH /= l;
	count = 0;
	for (i = 0; i < l*level; i++)
	{
		if (img.data[i] > TH)
			count++;
	}
	edgehist[3] = count;
	filter2D(image, img, -1, kernel5);

	TH = 0;
	for (i = 0; i < l*level; i++)
	{
		TH += (double)img.data[i];
	}
	TH /= l;
	count = 0;
	for (i = 0; i < l*level; i++)
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

	hu[0] = (t1);
	hu[1] = (t2*t2+4*y11*y11);
	hu[2] = (t3*t3+t4*t4);
	hu[3] = (t5*t5+t6*t6);
	hu[4] = (t3*t5*(t5*t5-3*t6*t6)+t4*t6*(3*t5*t5-t6*t6));
	hu[5] = (t2*(t5*t5-t6*t6)+4*u11*t5*t6);
	hu[6] = (t4*t5*(t5*t5-3*t6*t6)-t3*t6*(3*t5*t5-t6*t6));
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

void DWT(IplImage *pImage, int nLayer)
{
   // 执行条件
   if (pImage)
   {
      if (pImage->nChannels == 1 &&
         pImage->depth == IPL_DEPTH_32F &&
         ((pImage->width >> nLayer) << nLayer) == pImage->width &&
         ((pImage->height >> nLayer) << nLayer) == pImage->height)
      {
         int     i, x, y, n;
         float   fValue   = 0;
         float   fRadius  = sqrt(2.0f);
         int     nWidth   = pImage->width;
         int     nHeight  = pImage->height;
         int     nHalfW   = nWidth / 2;
         int     nHalfH   = nHeight / 2;
         float **pData    = new float*[pImage->height];
         float  *pRow     = new float[pImage->width];
         float  *pColumn  = new float[pImage->height];
         for (i = 0; i < pImage->height; i++)
         {
            pData[i] = (float*) (pImage->imageData + pImage->widthStep * i);
         }
         // 多层小波变换
         for (n = 0; n < nLayer; n++, nWidth /= 2, nHeight /= 2, nHalfW /= 2, nHalfH /= 2)
         {
            // 水平变换
            for (y = 0; y < nHeight; y++)
            {
               // 奇偶分离
               memcpy(pRow, pData[y], sizeof(float) * nWidth);
               for (i = 0; i < nHalfW; i++)
               {
                  x = i * 2;
                  pData[y][i] = pRow[x];
                  pData[y][nHalfW + i] = pRow[x + 1];
               }
               // 提升小波变换
               for (i = 0; i < nHalfW - 1; i++)
               {
                  fValue = (pData[y][i] + pData[y][i + 1]) / 2;
                  pData[y][nHalfW + i] -= fValue;
               }
               fValue = (pData[y][nHalfW - 1] + pData[y][nHalfW - 2]) / 2;
               pData[y][nWidth - 1] -= fValue;
               fValue = (pData[y][nHalfW] + pData[y][nHalfW + 1]) / 4;
               pData[y][0] += fValue;
               for (i = 1; i < nHalfW; i++)
               {
                  fValue = (pData[y][nHalfW + i] + pData[y][nHalfW + i - 1]) / 4;
                  pData[y][i] += fValue;
               }
               // 频带系数
               for (i = 0; i < nHalfW; i++)
               {
                  pData[y][i] *= fRadius;
                  pData[y][nHalfW + i] /= fRadius;
               }
            }
            // 垂直变换
            for (x = 0; x < nWidth; x++)
            {
               // 奇偶分离
               for (i = 0; i < nHalfH; i++)
               {
                  y = i * 2;
                  pColumn[i] = pData[y][x];
                  pColumn[nHalfH + i] = pData[y + 1][x];
               }
               for (i = 0; i < nHeight; i++)
               {
                  pData[i][x] = pColumn[i];
               }
               // 提升小波变换
               for (i = 0; i < nHalfH - 1; i++)
               {
                  fValue = (pData[i][x] + pData[i + 1][x]) / 2;
                  pData[nHalfH + i][x] -= fValue;
               }
               fValue = (pData[nHalfH - 1][x] + pData[nHalfH - 2][x]) / 2;
               pData[nHeight - 1][x] -= fValue;
               fValue = (pData[nHalfH][x] + pData[nHalfH + 1][x]) / 4;
               pData[0][x] += fValue;
               for (i = 1; i < nHalfH; i++)
               {
                  fValue = (pData[nHalfH + i][x] + pData[nHalfH + i - 1][x]) / 4;
                  pData[i][x] += fValue;
               }
               // 频带系数
               for (i = 0; i < nHalfH; i++)
               {
                  pData[i][x] *= fRadius;
                  pData[nHalfH + i][x] /= fRadius;
               }
            }
         }
         delete[] pData;
         delete[] pRow;
         delete[] pColumn;
      }
   }
}

int DEC2BIN(int num);

void calculateFeature::calcWaveFeat(Mat img, double* wave)
{
	int i, j, k;
	int row = img.rows;
	int col = img.cols;
	int imgSize = row*col;
	Mat img_Single(col,row,CV_8UC1,Scalar(0,0));


	Mat img_gray = RGB2GRAY(img);
	double bitPlane[8] = {0};
	int bin;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			int val = img_gray.data[i*col+j];
			bin = DEC2BIN(val);
			k = 0;
			while (bin > 0)
			{
				bitPlane[k] += (bin%10);
				bin /= 10;
				k++;
			}
		}
	}
	double total = bitPlane[7] + bitPlane[6] + bitPlane[5] + bitPlane[4];
	wave[0] = bitPlane[7]/total;
	wave[1] = bitPlane[6]/total;
	wave[2] = bitPlane[5]/total;
	wave[3] = bitPlane[4]/total;

	//imshow("123", img_gray);
	//waitKey();

	IplImage iplimg = img_gray;
	IplImage* pSrc = &iplimg;
	int nLayer = 3;

	CvSize size = cvGetSize(pSrc);
	if ((pSrc->width >> nLayer) << nLayer != pSrc->width)
	{
	   size.width = ((pSrc->width >> nLayer) + 1) << nLayer;
	}
	if ((pSrc->height >> nLayer) << nLayer != pSrc->height)
	{
	   size.height = ((pSrc->height >> nLayer) + 1) << nLayer;
	}

	IplImage *pWavelet = cvCreateImage(size, IPL_DEPTH_32F, pSrc->nChannels);
	if (pWavelet)
	{
	   // 小波图象赋值
	   cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
	   cvConvertScale(pSrc, pWavelet, 1, -128);
	   cvResetImageROI(pWavelet);
	   // 彩色图像小波变换
	   IplImage *pImage = cvCreateImage(cvGetSize(pWavelet), IPL_DEPTH_32F, 1);
	   if (pImage)
	   {
		  for (int i = 1; i <= pWavelet->nChannels; i++)
		  {
			 cvSetImageCOI(pWavelet, i);
			 //cvShowImage("test", pWavelet);
			//waitKey(0);

			 cvCopy(pWavelet, pImage, NULL);
			 // 二维离散小波变换
			 DWT(pImage, nLayer);
			 // 二维离散小波恢复
			 // IDWT(pImage, nLayer);
			 cvCopy(pImage, pWavelet, NULL);
		  }
		  cvSetImageCOI(pWavelet, 0);
		  cvReleaseImage(&pImage);
	   }
	   // 小波变换图象
	   cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
	   cvConvertScale(pWavelet, pSrc, 1, 128);
	   //cvCopy(pWavelet, pSrc, NULL);
	   cvResetImageROI(pWavelet);
	   cvReleaseImage(&pWavelet);
	}
	// 显示图像pSrc
	// ...
	Mat img_WL(pSrc);

	Rect LL(0, 0, col/pow(2,nLayer), row/pow(2,nLayer));
	Mat img_LL = img_WL(LL);
	int LL_row = row/pow(2,nLayer);
	int LL_col = col/pow(2,nLayer);
	double* InterestValue = new double[LL_row*LL_col];

	memset(InterestValue, 0, sizeof(double)*LL_row*LL_col);

	Mat img_HL, img_LH;
	//imshow("1", img_WL);

	for (int layer = 0; layer < nLayer; layer++)
	{
		int _width = col/pow(2,nLayer-layer);
		int _height = row/pow(2,nLayer-layer);

		int x = col/pow(2, nLayer-layer);
		int y = 0;
		Rect HL(x, y, _width, _height);
		img_HL = img_WL(HL);

		y = row/pow(2, nLayer-layer);
		x = 0;
		Rect LH(x, y, _width, _height);
		img_LH = img_WL(LH);

		for (i = 0; i < LL_row; i++)
		{
			for (j = 0; j < LL_col; j++)
			{
				y = i * pow(2,layer);
				x = j * pow(2,layer);
				int bSize_row = pow(2,layer);
				int bSize_col = pow(2,layer);
				int bSize = bSize_row*bSize_col;

				Rect blk(x, y, bSize_col, bSize_row);
				if (x+bSize_col > img_HL.cols)
					int iiii = 1;
				if (y+bSize_row > img_HL.rows)
					int iiii = 1;
				Mat img_blk = img_HL(blk);
				for (k = 0; k < bSize; k++)
					InterestValue[i*LL_col+j] += ((double) img_blk.data[k])*((double) img_blk.data[k]);
				
				img_blk = img_LH(blk);
				for (k = 0; k < bSize; k++)
					InterestValue[i*LL_col+j] += ((double) img_blk.data[k])*((double) img_blk.data[k]);
			}
		}
	}
	double mean = 0, sd = 0, th = 0;
	int* id = new int[LL_row*LL_col];
	memset(id, 0, sizeof(int)*LL_row*LL_col);

	for (i = 0; i < LL_row*LL_col; i++)
	{
		InterestValue[i] = sqrt(InterestValue[i]);
		mean += InterestValue[i];
		id[i] = i;
	}
	mean = mean / ((double)LL_row*LL_col);
	for (i = 0; i < LL_row*LL_col; i++)
	{
		sd += (InterestValue[i] - mean) * (InterestValue[i] - mean);
	}
	sd = sqrt(sd / ((double)LL_row*LL_col));

	th = mean + sd;

	for (i = LL_row*LL_col - 1; i >=0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (InterestValue[j] > InterestValue[i])
			{
				double temp = InterestValue[i];
				InterestValue[i] = InterestValue[j];
				InterestValue[j] = temp;

				int temp1 = id[i];
				id[i] = id[j];
				id[j] = temp1;
			}
		}
	}
	
	double FeatBitPlane[8] = {0};

	for (i = 0; i < LL_row*LL_col; i++)
	{
		if (InterestValue[i] > th)
			break;
		bin = DEC2BIN(img_LL.data[id[i]]);
		k = 0;
		while (bin > 0)
		{
			FeatBitPlane[k] += (bin%10);
			bin /= 10;
			k++;
		}
	}
	total = 0;
	for (i = 0; i < 8; i++)
		total += FeatBitPlane[i];
	for (i = 0; i < 8; i++)
		 FeatBitPlane[i] /= total;
	wave[4] = FeatBitPlane[7];
	wave[5] = FeatBitPlane[6];
	wave[6] = FeatBitPlane[5];
	wave[7] = FeatBitPlane[4];
	wave[8] = FeatBitPlane[3];
	wave[9] = FeatBitPlane[2];
	wave[10] = FeatBitPlane[1];
	wave[11] = FeatBitPlane[0];
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

int DEC2BIN(int num)
{
	int result = 0;
	int k = 0;
	while (num > 0)
	{
		result += (num%2)*pow(10,k);
		num /= 2;
		k++;
	}
	return result;
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


DLLEXPORT ImageFeature* CalFeatureForImages(MyMat *imgs, int num, BOOL isFromLib)
{
	//TODO:根据path把数据读进来然后算特征,算完特征存起来？
	ImageFeature* features = new ImageFeature[num];
	calculateFeature calc;
	//sift gen
	Mat bowDescriptors;
	float * row;
	if(isFromLib)
	{
		FileStorage fs2("E:\\bowDescriptors.yml", FileStorage::READ);
		fs2["bowDescriptors"] >> bowDescriptors;
		fs2.release();
		assert(bowDescriptors.cols == SIFT_VOCA_SIZE);
		assert(bowDescriptors.rows == num);
	}
	else{
		calc.loadVocFile();
	}
	//calc.siftBowPreprocess(imgs, num);
	for(int i = 0; i < num; i++)
	{
		features[i].id = imgs[i].id;
		features[i].genFeat(imgs[i], calc);
		//sift gen
		if(isFromLib)
		{
			row = (float*)bowDescriptors.row(i).data;
			for(int j=0; j<SIFT_VOCA_SIZE; j++) 
				features[i].Sift[j] = row[j];
		}
		else
		{
			calc.calcSIFT(imgs[i], features[i].Sift);
		}
	}
	return features;
}


DLLEXPORT double CalFeatureDistance(ImageFeature &ele1, ImageFeature &ele2, int FeatIDs[], int weight[], int num)
{
	double d = 0;
	for(int i = 0; i < num; i++)
		d += weight[i] * ele1.Distance(ele2, FeatIDs[i]);
	return d;
}


DLLEXPORT ImageFeature* Create(int num)
{
	return new ImageFeature[num];
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

void calculateFeature::loadVocFile() {
	FileStorage fs("E:\\localVocabulary.yml", FileStorage::READ);
    fs["vocabulary"] >> localVocabulary;
	fs.release();
}
//调用之前调一次loadVocFile()!!
void calculateFeature::calcSIFT(Mat img, double* sift) {

	vector<KeyPoint> keypoints;
	SiftDescriptorExtractor detector;
	//快速最近邻匹配
	Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
	Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
    BOWImgDescriptorExtractor bowDE(extractor, matcher);
    //设置词汇表
    bowDE.setVocabulary(localVocabulary);
	detector.detect(img,keypoints);

	//计算词汇表特征向量
	//Mat descriptors;
	//extractor->compute(img, keypoints, descriptors);
	//size of vocabulary must match
	assert(SIFT_VOCA_SIZE == localVocabulary.rows);
	memset(sift, 0, sizeof(double)*localVocabulary.rows);
	Mat bowDescriptor;
	bowDE.compute(img,keypoints,bowDescriptor);
	float * row = (float*)(bowDescriptor.row(0).data);
	for(int i=0; i<SIFT_VOCA_SIZE; i++) {
		sift[i] = row[i];
	}
}