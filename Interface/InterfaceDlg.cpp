
// InterfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Interface.h"
#include "InterfaceDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInterfaceDlg dialog



CInterfaceDlg::CInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInterfaceDlg::IDD, pParent)
	, queryImg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pLibImagesType = NULL;
	queryImg = NULL;
	imgs = NULL;
	features = NULL;
}

CInterfaceDlg::~CInterfaceDlg()
{
	if(queryImg)
		delete queryImg;
	if(imgs)
		delete[] imgs;
	for(int i = 0; i < SHOWIMGROW * SHOWIMGCOL; i++)
		if(pLibImages[i])
			delete pLibImages[i];
}

void CInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInterfaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD, &CInterfaceDlg::OnBnClickedLoad)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_IMGLIB, IDC_IMGLIB+39, &CInterfaceDlg::OnImgClickedLib)
	//ON_STN_CLICKED(IDC_IMGLIB, &CInterfaceDlg::OnImgClickedLib)
	ON_BN_CLICKED(IDC_GO, &CInterfaceDlg::OnBnClickedGo)
	ON_CBN_SELCHANGE(IDC_TYPE, &CInterfaceDlg::OnCbnSelChage)
	ON_BN_CLICKED(IDC_INDEX, &CInterfaceDlg::OnBnClickedIndex)
END_MESSAGE_MAP()


// CInterfaceDlg message handlers

BOOL CInterfaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//查询结果
	pQueryAP = (CEdit *)GetDlgItem(IDC_QUERYAP);
	pQueryTime = (CEdit *)GetDlgItem(IDC_QUERYTIME);
	pMQueryAP = (CEdit *)GetDlgItem(IDC_MQUERYAP);
	pMQueryTime = (CEdit *)GetDlgItem(IDC_QUERYTIME);
	pCheckGLCM = (CButton *)GetDlgItem(IDC_GLCM);
	pCheckEH = (CButton *)GetDlgItem(IDC_EH);
	pCheckHU = (CButton *)GetDlgItem(IDC_HU);
	pCheckHSV = (CButton *)GetDlgItem(IDC_HSV);
	pCheckSIFT = (CButton *)GetDlgItem(IDC_SIFT);

	// initial comboBox
	pLibImagesType = (CComboBox *)GetDlgItem(IDC_TYPE);
	pLibImagesType->InsertString(0, L"airplane");
	pLibImagesType->InsertString(1, L"automobile");
	pLibImagesType->InsertString(2, L"bird");
	pLibImagesType->InsertString(3, L"cat");
	pLibImagesType->InsertString(4, L"deer");
	pLibImagesType->InsertString(5, L"dog");
	pLibImagesType->InsertString(6, L"frog");
	pLibImagesType->InsertString(7, L"horse");
	pLibImagesType->InsertString(8, L"ship");
	pLibImagesType->InsertString(9, L"truck");
	pLibImagesType->InsertString(10, L"ALL");

	// initial libGroup
	CRect lib_rect;
	GetDlgItem(IDC_LIBGROUP)->GetWindowRect(&lib_rect);
	int width = (lib_rect.Width() - 4) / SHOWIMGCOL - 2;
	int height = (lib_rect.Height() - 20)/ SHOWIMGROW - 2;
	int x = lib_rect.left - 70;
	int y = lib_rect.top - 15;
	for(int i = 0; i < SHOWIMGROW; i++)
	{
		x = lib_rect.left - 70;
		for(int j = 0; j < SHOWIMGCOL; j++)
		{
			int num = i * SHOWIMGCOL + j;
			pLibImages[num] = new CStatic;
			CRect rect(x, y, x + width, y + height);
			pLibImages[num]->Create(NULL, WS_CHILD | WS_VISIBLE | SS_NOTIFY, 
				rect, this, IDC_IMGLIB + num);
			//pLibImages[num]->SetParent(this);
			x += (width + 2);
		}
		y += (height + 2);
	}

	//initial rltgroup
	GetDlgItem(IDC_RESULTGROUP)->GetWindowRect(&lib_rect);
	width = (lib_rect.Width() - 4) / SHOWIMGCOL - 2;
	height = (lib_rect.Height() - 20)/ SHOWIMGROW - 2;
	x = lib_rect.left - 70;
	y = lib_rect.top - 15;
	for(int i = 0; i < SHOWIMGROW; i++)
	{
		x = lib_rect.left - 70;
		for(int j = 0; j < SHOWIMGCOL; j++)
		{
			int num = i * SHOWIMGCOL + j;
			pLibImages[num] = new CStatic;
			CRect rect(x, y, x + width, y + height);
			pLibImages[num]->Create(NULL, WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_BLACKFRAME, 
				rect, this, IDC_IMGRLT + num);
			//pLibImages[num]->SetParent(this);
			x += (width + 2);
		}
		y += (height + 2);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInterfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		/*CDialogEx::UpdateWindow();
		ShowImage(queryImg, IDC_queryImg);*/
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CInterfaceDlg:: LoadToolDll()
{
	BOOL bsuccess = TRUE;
	m_hLoadImageLib = LoadLibrary(_T("Tools.dll"));
	if (m_hLoadImageLib == NULL)
	{
		bsuccess = FALSE;
	}
	else
	{
		m_pfnLoadFromCIFAR10=
			(PLoadFromCIFAR10)GetProcAddress(m_hLoadImageLib,"LoadFromCIFAR10");
		bsuccess = TRUE;
	}
	return bsuccess;
}

BOOL CInterfaceDlg:: LoadFeaturesDll()
{
	BOOL bsuccess = TRUE;
	m_hFeatures = LoadLibrary(_T("ImageFeature.dll"));
	if (m_hFeatures == NULL)
	{
		bsuccess = FALSE;
	}
	else
	{
		m_pfnCalFeatureForImages=
			(PCalFeatureForImages)GetProcAddress(m_hFeatures,"CalFeatureForImages");
		m_pfnCalFeatureDistance=
			(PCalFeatureDistance)GetProcAddress(m_hFeatures, "CalFeatureDistance");
		m_pfnCreate = (PCreate)GetProcAddress(m_hFeatures, "Create");
		bsuccess = TRUE;
	}
	return bsuccess;
}

void CInterfaceDlg::ShowImage(IplImage *img, CWnd *p, UINT id)
{
	CDC* pDC = p->GetDlgItem(id)->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	CRect rect;
	p->GetDlgItem(id)->GetClientRect(&rect);
	// 求出图片控件的宽和高   
	int rw = rect.right - rect.left;        
	int rh = rect.bottom - rect.top;  
	// 读取图片的宽和高  
	int iw = img->width;    
	int ih = img->height;   
	// 使图片的显示位置正好在控件的正中   
	int tx = (int)(rw - iw)/2;   
	int ty = (int)(rh - ih)/2;   
	SetRect(rect, tx, ty, tx+iw, ty+ih);

	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	p->ReleaseDC(pDC);

}


void CInterfaceDlg::OnBnClickedLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!LoadToolDll())
	{
		MessageBox(L"error", L"DLL load error!", MB_OK);
		return;
	}
	
	imgs = (*m_pfnLoadFromCIFAR10)("E:\\");
	int times[10] = {0};
	for(int i = 0; i < TOTALIMG; i++)
	{
		int tmp = imgs[i].type;
		indexOfType[tmp][times[tmp]++] = imgs[i].id;
	}
	if(pLibImagesType)
		pLibImagesType->SetCurSel(10);
	OnCbnSelChage();
}

void CInterfaceDlg::OnImgClickedLib(UINT nid)
{
	if(imgs && MessageBox(L"是否将该图片设置为查询图片？", L"设置查询图片", MB_YESNO) == IDYES)
	{
		char type = pLibImagesType->GetCurSel();
		CvSize imgSize;
		imgSize.width = IMAGE_WIDTH;
		imgSize.height = IMAGE_HEIGHT;
		IplImage* tmp = cvCreateImage(imgSize, IPL_DEPTH_8U, IMAGE_CHANNEL);
		//TODO page的处理
		int num = nid - 2000;
		if(type == 10)
		{
			*tmp = imgs[num];
			queryImg = &imgs[num];
		}
		else
		{
			*tmp = imgs[indexOfType[type][num]];
			queryImg = &imgs[indexOfType[type][num]];
		}
		//ResizeImage();
		ShowImage(tmp, this, IDC_queryImg);            // 调用显示图片函数
	}
}

void CInterfaceDlg::OnCbnSelChage()
{
	char type = pLibImagesType->GetCurSel();
	if(NULL == imgs)
		return;
	for(int i = 0; i < SHOWIMGROW; i++)
	{
		for(int j = 0; j < SHOWIMGCOL; j++)
		{
			//TODO num可能需要根据页数改变
			int num = i * SHOWIMGCOL + j;
			Mat tmp;
			if(type != 10)
				tmp = imgs[indexOfType[type][num]].clone();
			else
				tmp = imgs[num].clone();
			IplImage xx = tmp;
			ShowImage(&xx, this, IDC_IMGLIB + num);
		}
	}
}


void CInterfaceDlg::ResizeImage(IplImage * img, IplImage* o_img)
{
	int w = img->width;
	int h = img->height;

	int max = (w > h)? w: h;

	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (float) ( (float) max / 256.0f );

	// 缩放后图片的宽和高
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > nh)? 0: (int)(256-nw)/2;
	int tly = (nw > nh)? (int)(256-nh)/2: 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI(o_img, cvRect( tlx, tly, nw, nh) );

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize(img, o_img);

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI(o_img);
}

void CInterfaceDlg::OnBnClickedGo()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileFind find;
	if(imgs == NULL)
		MessageBox(L"尚未载入图像库！", L"查询", MB_OK);
	else if(features == NULL)
	{
		if(find.FindFile(L"xxxx"))
		{

		}
		else
			MessageBox(L"尚未建立索引！", L"查询", MB_OK);
	}
	else if(queryImg == NULL)
		MessageBox(L"尚未设置查询图片！", L"查询", MB_OK);
	else
	{
		if (!LoadFeaturesDll())
		{
			MessageBox(L"error", L"DLL load error!", MB_OK);
			return;
		}
		CCMP* cc = new CCMP[TOTALIMG];
		int* method = new int[5];
		int num = 0;
		if(pCheckGLCM->GetCheck())
		{
			method[num] = GLCM;
			num++;
		}
		if(pCheckEH->GetCheck())
		{
			method[num] = EH;
			num++;
		}
		if(pCheckHU->GetCheck())
		{
			method[num] = HU;
			num++;
		}
		if(pCheckHSV->GetCheck())
		{
			method[num] = HSV;
			num++;
		}
		if(pCheckSIFT->GetCheck())
		{
			method[num] = SIFT;
			num++;
		}

		if(num == 0)
		{
			MessageBox(L"尚未选择检索特征", L"查询", MB_OK);
			return;
		}

		//TODO 通过索引获取1000-2000个个备选img
		for(int i = 0; i < TOTALIMG; i++)
		{
			cc[i].id = i;
			cc[i].d = (*m_pfnCalFeatureDistance)(features[i], features[queryImg->id], method, num);
			//features[i].d = features->Distance(features[queryImg->id], GLCM);
		}
		qsort(cc, TOTALIMG, sizeof(CCMP), featureCmp);

		//显示查询结果
		for(int i = 0; i < SHOWIMGROW; i++)
		{
			for(int j = 0; j < SHOWIMGCOL; j++)
			{
				//TODO num可能需要根据页数改变
				int num = i * SHOWIMGCOL + j;
				Mat tmp = imgs[cc[num].id].clone();
				IplImage xx = tmp;
				ShowImage(&xx, this, IDC_IMGRLT + num);
			}
		}

		//计算AP
		int cal = 0;
		double ap = 0;
		for(int i = 1; i <= 1000; i++)
		{
			if(imgs[cc[i].id].type == queryImg->type)
			{
				cal++;
				ap += (cal + 0.0) / i;
			}
		}
		ap /= 1000;
		CString str = L"";
		str.Format(L"%f", ap);
		pQueryAP->SetWindowTextW(str.GetBuffer(0));

		delete[] cc;
	}
}


void CInterfaceDlg::OnBnClickedIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileFind find;
	if(imgs == NULL)
		MessageBox(L"尚未载入图像库！", L"建立索引", MB_OK);
	else if(features)
	{
	}
	else if(find.FindFile(L"E:\\feat1.txt"))
	{
		if (!LoadFeaturesDll())
		{
			MessageBox(L"error", L"DLL load error!", MB_OK);
			return;
		}
		ifstream inf;
		int temp=0;
		features = (*m_pfnCreate)(TOTALIMG);
		int GLCM_length = features[0].GLCM_length, EH_length = features[0].EH_length, 
			HU_length = features[0].HU_length, HSV_length = features[0].HSV_length, SIFT_length = features[0].SIFT_length;
		inf.open("E:\\feat1.txt");
		inf>>temp;
		inf>>temp;
		for(int i = 0; i < TOTALIMG; i++)
			for(int j = 0; j < GLCM_length; j++)
				inf >> features[i].GrayLevelCoocurrenceMatrix[j];
		inf.close();

		inf.open("E:\\feat2.txt");
		inf>>temp;
		inf>>temp;
		for(int i = 0; i < TOTALIMG; i++)
			for(int j = 0; j < EH_length; j++)
				inf >> features[i].EdgeHist[j];
		inf.close();

		inf.open("E:\\feat3.txt");
		inf>>temp;
		inf>>temp;
		for(int i = 0; i < TOTALIMG; i++)
			for(int j = 0; j < HU_length; j++)
				inf >> features[i].Hu[j];
		inf.close();

		inf.open("E:\\feat4.txt");
		inf>>temp;
		inf>>temp;
		for(int i = 0; i < TOTALIMG; i++)
			for(int j = 0; j < HSV_length; j++)
				inf >> features[i].HSVFeat[j];
		inf.close();

		/*for(int i = 0; i < TOTALIMG; i++)
			for(int j = 0; j < SIFT_length; j++)
				inf >> features[i].Sift[j]*/
		inf.close();
	}
	else
	{
		if (!LoadFeaturesDll())
		{
			MessageBox(L"error", L"DLL load error!", MB_OK);
			return;
		}
		features = (*m_pfnCalFeatureForImages)(imgs, TOTALIMG);
		StoreFeatures();
	}
}

void CInterfaceDlg::StoreFeatures()
{
	if(features == NULL)
		return;
	//归一化
	double *pdGLCM, *pdEH, *pdHU, *pdHSV, *pdSIFT, *pdminHU, *pdminHSV;
	int GLCM_length = features[0].GLCM_length, EH_length = features[0].EH_length, 
		HU_length = features[0].HU_length, HSV_length = features[0].HSV_length, SIFT_length = features[0].SIFT_length;
	//pdGLCM = new double[GLCM_length];
	//pdEH = new double[EH_length];
	//pdHU = new double[HU_length];
	//pdHSV = new double[HSV_length];
	//pdSIFT = new double[SIFT_length];
	//pdminHU = new double[HU_length];
	//pdminHSV = new double[HSV_length];
	//memset(pdGLCM, 0, sizeof(double)*GLCM_length);
	//memset(pdEH, 0, sizeof(double)*EH_length);
	//memset(pdHU, 0, sizeof(double)*HU_length);
	//memset(pdHSV, 0, sizeof(double)*HSV_length);
	//memset(pdSIFT, 0, sizeof(double)*SIFT_length);
	//memset(pdminHU, 0, sizeof(double)*HU_length);
	//memset(pdminHSV, 0, sizeof(double)*HSV_length);
	//for(int i = 0; i < TOTALIMG; i++)
	//{
	//	for(int j = 0; j < GLCM_length; j++)
	//		pdGLCM[j] += features[i].GrayLevelCoocurrenceMatrix[j];
	//	for(int j = 0; j < EH_length; j++)
	//		pdEH[j] += features[i].EdgeHist[j];
	//	//特征值有可能出现负数
	//	for(int j = 0; j < HU_length; j++)
	//	{
	//		if(features[i].Hu[j] < pdminHU[j])
	//			pdminHU[j] = features[i].Hu[j];
	//		pdHU[j] += features[i].Hu[j];
	//	}
	//	//特征值有可能出现负数
	//	for(int j = 0; j < HSV_length; j++)
	//	{
	//		if(features[i].HSVFeat[j] < pdminHSV[j])
	//			pdminHSV[j] = features[i].HSVFeat[j];
	//		pdHSV[j] += features[i].HSVFeat[j];
	//	}
	//	/*for(int j = 0; j < SIFT_length; j++)
	//		pdSIFT[j] += features[i].Sift[j];*/
	//}
	//for(int i = 0; i < TOTALIMG; i++)
	//{
	//	for(int j = 0; j < GLCM_length; j++)
	//		features[i].GrayLevelCoocurrenceMatrix[j] *= (TOTALIMG / pdGLCM[j]);
	//	for(int j = 0; j < EH_length; j++)
	//		features[i].EdgeHist[j]  *= (TOTALIMG / pdEH[j]);
	//	//特征有可能出现负值
	//	for(int j = 0; j < HU_length; j++)
	//	{
	//		features[i].Hu[j] -= pdminHU[j];
	//		features[i].Hu[j]  *= (TOTALIMG / (pdHU[j] - TOTALIMG * pdminHU[j]));
	//	}
	//	//特征有可能出现负值
	//	for(int j = 0; j < HSV_length; j++)
	//	{
	//		features[i].HSVFeat[j] -= pdminHSV[j];
	//		features[i].HSVFeat[j]  *= (TOTALIMG / (pdHSV[j] - TOTALIMG * pdminHSV[j]));
	//	}
	//	/*for(int j = 0; j < SIFT_length; j++)
	//		features[i].Sift[j]  *= (TOTALIMG / pdSIFT[j]);*/
	//}
	
	//store
	ofstream of;
	of.open("E:\\feat1.txt");
	of<<TOTALIMG<<" "<<GLCM_length<<endl;
	for(int i = 0; i < TOTALIMG; i++)
	{
		for(int j = 0; j < GLCM_length; j++)
			of << features[i].GrayLevelCoocurrenceMatrix[j] << " ";
		of<<endl;
	}
	of.close();
	of.open("E:\\feat2.txt");
	of<<TOTALIMG<<" "<<EH_length<<endl;
	for(int i = 0; i < TOTALIMG; i++)
	{
		for(int j = 0; j < EH_length; j++)
			of << features[i].EdgeHist[j] << " ";
		of<<endl;
	}
	of.close();
	of.open("E:\\feat3.txt");
	of<<TOTALIMG<<" "<<HU_length<<endl;
	for(int i = 0; i < TOTALIMG; i++)
	{
		for(int j = 0; j < HU_length; j++)
			of << features[i].Hu[j] << " ";
		of << endl;
	}
	of.close();
	of.open("E:\\feat4.txt");
	of<<TOTALIMG<<" "<<HU_length<<endl;
	for(int i = 0; i < TOTALIMG; i++)
	{
		for(int j = 0; j < HSV_length; j++)
			of << features[i].HSVFeat[j] << " ";
		of << endl;
	}
	of.close();

	/*of.open("E:\\SIFTfeat.dat", ios::binary);
	for(int i = 0; i < TOTALIMG; i++)
		for(int j = 0; j < SIFT_length; j++)
			of << features[i].Sift[j] << " ";
	of.close();*/
}
int featureCmp(const void *ele1, const void *ele2)
{
	CCMP* e1 = (CCMP*)ele1;
	CCMP* e2 = (CCMP*)ele2;
	double s = e1->d - e2->d;
	if(s > 0)
		return 1;
	else if(s < 0)
		return -1;
	else
		return 0;
}

