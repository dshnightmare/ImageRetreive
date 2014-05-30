
#include "stdafx.h"
#include "LoadImageLib.h"
#include<cv.h>
#include <cxcore.h>
#include<highgui.h>
#include<iostream>
#include "opencv2/nonfree/nonfree.hpp"
using namespace cv;
using namespace std;

DLLEXPORT  MyMat* LoadFromCIFAR10(string path)
{
	MyMat* imgs = new MyMat[50000];
	int planeId[3]={2,1,0};
	int width=32;
	int height=32;
	int type=CV_8UC3;
	int imgSize=width*height;
	int nChannels=3;
	int imgDataSize = 1+imgSize*nChannels;
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	Mat img;
	Mat rs;
	for(int m = 0; m < 5; m++)
	{
		int n = 0;
		ifstream fin(path + "data_batch_" + char(m + '1') +".bin", ios::binary);
		//使用构造函数创建矩阵
		while(fin.eof() != true)
		{
			if(n >= 10000)
				break;
			img.create(width, height, type);
			rs.create(64, 64, type);
			imgs[m * 10000 + n].create(width, height, type);
			uchar *buf=(uchar*)calloc(imgDataSize,sizeof(uchar));
			fin.read((char *)buf,(imgDataSize)*sizeof(uchar));
			imgs[m * 10000 + n].type = buf[0];
			imgs[m * 10000 + n].id = m * 10000 + n;
			for(int i = 0; i < height; i++) 
			{
				for(int j = 0; j < width; j++ ) {
					uchar* dataIJ = img.data + i * img.step + j * img.elemSize();// img.at(i, j)
					for(int k = 0; k < nChannels; k++)
						dataIJ[k] = buf[1 + planeId[k] * imgSize + i * width + j];
				}
			}
			//拉普拉斯锐化
			resize(img, rs, rs.size(), 0, 0, INTER_CUBIC);
			filter2D(rs, imgs[m * 10000 + n], rs.depth(), kernel );
			n++;
			free(buf);
		}
		fin.close();
	}
	return imgs;
}

DLLEXPORT MyMat* LoadFromCIFAR10Test(string path)
{
	MyMat* imgs = new MyMat[10000];
	MyMat* imgs200 = new MyMat[200];
	int id[10000];
	int indexOfType[10][1000];
	int countOfType[10] = {0};
	int planeId[3]={2,1,0};
	int width=32;
	int height=32;
	int type=CV_8UC3;
	int imgSize=width*height;
	int nChannels=3;
	int imgDataSize = 1+imgSize*nChannels;
	int n = 0;
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	Mat img;
	Mat rs;
	ifstream fin(path + "test_batch.bin", ios::binary);
	//使用构造函数创建矩阵
	while(fin.eof() != true)
	{
		if(n >= 10000)
			break;
		img.create(width, height, type);
		rs.create(64, 64, type);
		imgs[n].create(64, 64, type);
		uchar *buf=(uchar*)calloc(imgDataSize,sizeof(uchar));
		fin.read((char *)buf,(imgDataSize)*sizeof(uchar));
		imgs[n].type = buf[0];
		indexOfType[buf[0]][countOfType[buf[0]]++] = n;
		imgs[n].id = n;
		for(int i = 0; i < height; i++) 
		{
			for(int j = 0; j < width; j++ ) {
				uchar* dataIJ = img.data + i * img.step + j * img.elemSize();// img.at(i, j)
				for(int k = 0; k < nChannels; k++)
					dataIJ[k] = buf[1 + planeId[k] * imgSize + i * width + j];
			}
		}
		id[n] = n;
		//拉普拉斯锐化
		resize(img, rs, rs.size(), 0, 0, INTER_CUBIC);
		filter2D(rs, imgs[n], rs.depth(), kernel );
		n++;
		free(buf);
	}
	fin.close();
	srand(time(NULL));
	int modnum = 10000;
	for(int i = 0; i < 200; i++)
	{
		//int r = rand() % modnum;
		int j = id[i];
		imgs200[i] = imgs[j];
		//id[r] = id[modnum - 1];
		modnum--;
	}
	delete[] imgs;
	return imgs200;
}

DLLEXPORT  MyMat* LoadFromCorel1000(string path)
{
	int offset;
	string image_path;
	char a[6];
	MyMat* imgs = new MyMat[1000];
	MyMat* imgs200 = new MyMat[200];
	Mat temp;
	int id[1000];
	
	for (offset = 0; offset <= 999; offset++)
	{
		_itoa(offset, a, 10);
		image_path = path+a+".jpg";
		temp = imread(image_path);
		imgs[offset].id = offset;
		imgs[offset].data = temp.data;
		imgs[offset].type = offset/100;
	}
	int modnum = 1000;
	for(int i = 0; i < 200; i++)
	{
		int r = rand() % modnum;
		int j = id[r];
		imgs200[i] = imgs[j];
		id[r] = id[modnum - 1];
		modnum--;
	}
	delete[] imgs;
	return imgs200;
}

/*
DLLEXPORT  MyMat* LoadFromCarel10000(string path)
{
	int offset;
	string image_path;
	char a[6];
	MyMat* imgs = new MyMat[10000];
	MyMat* imgs200 = new MyMat[200];
	Mat temp;
	int id[10000];
	
	for (offset = 0; offset <= 9999; offset++)
	{
		_itoa(offset, a, 10);
		image_path = path+a+".jpg";
		temp = imread(image_path);
		imgs[offset].id = offset;
		imgs[offset].data = temp.data;
		imgs[offset].type = offset/100;
	}
	int modnum = 10000;
	for(int i = 0; i < 200; i++)
	{
		int r = rand() % modnum;
		int j = id[r];
		imgs200[i] = imgs[j];
		id[r] = id[modnum - 1];
		modnum--;
	}
	delete[] imgs;
	return imgs200;
}
*/