
#include "stdafx.h"
#include "LoadImageLib.h"

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

	for(int m = 0; m < 5; m++)
	{
		int n = 0;
		ifstream fin(path + "data_batch_" + char(m + '1') +".bin", ios::binary);
		//使用构造函数创建矩阵
		while(fin.eof() != true)
		{
			if(n >= 10000)
				break;
			imgs[m * 10000 + n].create(width, height, type);
			uchar *buf=(uchar*)calloc(imgDataSize,sizeof(uchar));
			fin.read((char *)buf,(imgDataSize)*sizeof(uchar));
			imgs[m * 10000 + n].type = buf[0];
			imgs[m * 10000 + n].id = m * 10000 + n;
			for(int i = 0; i < height; i++) 
			{
				for(int j = 0; j < width; j++ ) {
					uchar* dataIJ = imgs[m * 10000 + n].data + i * imgs[m * 10000 + n].step + j * imgs[m * 10000 + n].elemSize();// img.at(i, j)
					for(int k = 0; k < nChannels; k++)
						dataIJ[k] = buf[1 + planeId[k] * imgSize + i * width + j];
				}
			}
			n++;
			free(buf);
		}
	}
	return imgs;
}