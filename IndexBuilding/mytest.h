#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

#include "stdafx.h"

#include<iostream>
#include <fstream>
#include <random>
#include "LSHTable.h"
#include "LSHTExtractor.h"
#include "LSHTBuilder.h"
//#define MAX_FEAT_ID 6

using namespace std;

double *** readData(string fName,int *dataNumArray,int *vecLenArray);
double ** readSingleData(string fName,int& dataNum,int& vecLen);
void rAndWData(string fName,int *dataNumArray,int *vecLenArray);
void mytestmain();