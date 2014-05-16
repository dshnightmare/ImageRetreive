
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
	// initial queryImg
	CvSize imgSize;
	imgSize.width = IMAGE_WIDTH;
	imgSize.height = IMAGE_HEIGHT;
	queryImg = cvCreateImage(imgSize, IPL_DEPTH_8U, IMAGE_CHANNEL);

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


BOOL CInterfaceDlg:: LoadQRBinaryDataDll()
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
	if (!LoadQRBinaryDataDll())
	{
		MessageBox(L"error", L"DLL load error!", MB_OK);
		return;
	}
	
	imgs = (*m_pfnLoadFromCIFAR10)("G:\\Users File\\Documents\\Visual Studio 2012\\Projects\\cbir\\cbir\\data_batch_5.bin");
	if(pLibImagesType)
		pLibImagesType->SetCurSel(10);
	for(int i = 0; i < SHOWIMGROW; i++)
	{
		for(int j = 0; j < SHOWIMGCOL; j++)
		{
			int num = i * SHOWIMGCOL + j;
			Mat tmp = imgs[num].clone();
			IplImage xx = tmp;
			ShowImage(&xx, this, IDC_IMGLIB + num);
			//ShowImage(&xx, this, IDC_IMGLIB + num);
		}
	}
}

void CInterfaceDlg::OnImgClickedLib(UINT nid)
{
	if(imgs && MessageBox(L"是否将该图片设置为查询图片？", L"设置查询图片", MB_YESNO) == IDYES)
	{
		*queryImg = imgs[nid - 2000];
		//ResizeImage();
		ShowImage(queryImg, this, IDC_queryImg);            // 调用显示图片函数
	}
}

void CInterfaceDlg::OnCbnSelChage()
{
	char type = pLibImagesType->GetCurSel();
	int n = -1;
	if(NULL == imgs)
		return;
	for(int i = 0; i < SHOWIMGROW; i++)
	{
		for(int j = 0; j < SHOWIMGCOL; j++)
		{
			int num = i * SHOWIMGCOL + j;
			while(imgs[++n].type != type)
			{
				if(type == 10)
					break;
			}
			if(n >= 9999)
				return;
			Mat tmp = imgs[n].clone();
			IplImage xx = tmp;
			ShowImage(&xx, this, IDC_IMGLIB + num);
			//ShowImage(&xx, this, IDC_IMGLIB + num);
		}
	}
}
void CInterfaceDlg::ResizeImage(IplImage * img)
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
	cvSetImageROI(queryImg, cvRect( tlx, tly, nw, nh) );

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize(img, queryImg);

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI(queryImg);
}

void CInterfaceDlg::OnBnClickedGo()
{
	// TODO: 在此添加控件通知处理程序代码
}
