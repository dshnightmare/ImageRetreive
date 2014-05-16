
#include "stdafx.h"
#include "LoadImageLib.h"

DLLEXPORT  MyMat* LoadFromCIFAR10(string path)
{
	MyMat* imgs = new MyMat[10000];
	ifstream fin(path, ios::binary);

	int width=32;
	int height=32;
	int type=CV_8UC3;
	int imgSize=width*height;
	int nChannels=3;
	int imgDataSize = 1+imgSize*nChannels;
	int n = 0;

	//ʹ�ù��캯����������
	//CV_8UC3 ��ʾʹ��8λ�� unsigned char �ͣ�ÿ������������Ԫ�������ͨ��
	int planeId[3]={2,1,0};
	while(fin.eof() != true)
	{
		if(n >= 10000)
			break;
		imgs[n].create(width, height, type);
		uchar *buf=(uchar*)calloc(imgDataSize,sizeof(uchar));
		fin.read((char *)buf,(imgDataSize)*sizeof(uchar));
		imgs[n].type = buf[0];
		for(int i = 0; i < height; i++) 
		{
			for(int j = 0; j < width; j++ ) {
				uchar* dataIJ = imgs[n].data + i * imgs[n].step + j * imgs[n].elemSize();// img.at(i, j)
				for(int k = 0; k < nChannels; k++)
					dataIJ[k] = buf[1 + planeId[k] * imgSize + i * width + j];
			}
		}
		n++;
		free(buf);
	}
	return imgs;
}