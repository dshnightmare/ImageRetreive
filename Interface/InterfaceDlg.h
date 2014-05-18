
// InterfaceDlg.h : header file
//
#include "../Tools/LoadImageLib.h"
#include "../ImageFeature/Features.h"
#include <cv.h>
#include <highgui.h>
#pragma once


#define IMAGE_WIDTH 32
#define IMAGE_HEIGHT 32
#define IMAGE_CHANNEL 3
#define  SHOWIMGROW 4
#define  SHOWIMGCOL 10
#define  TYPENUM 10
#define  TOTALIMG 50000
#define  MAXRLTSHOW 1000

//用于结果排序
class CCMP
{
public:
	int id;
	char type;
	double d;
};

// CInterfaceDlg dialog
class CInterfaceDlg : public CDialogEx
{
// Construction
public:
	CInterfaceDlg(CWnd* pParent = NULL);	// standard constructor
	~CInterfaceDlg();

// Dialog Data
	enum { IDD = IDD_INTERFACE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	//LoadImageLib
	HINSTANCE m_hLoadImageLib;   //DLL句柄
	PLoadFromCIFAR10 m_pfnLoadFromCIFAR10;
	BOOL  LoadToolDll();
	//Feature
	HINSTANCE m_hFeatures;       //DLL句柄
	PCalFeatureForImages m_pfnCalFeatureForImages;
	PCalFeatureDistance m_pfnCalFeatureDistance;
	PCreate m_pfnCreate;
	BOOL LoadFeaturesDll();

	//控件
	CStatic *pLibImages[SHOWIMGCOL*SHOWIMGROW];
	CStatic *pRstImages[SHOWIMGCOL*SHOWIMGROW];
	CComboBox *pLibImagesType;
	CButton *pLibPre;
	CButton *pLibNext;
	CButton *pRltPre;
	CButton *pRlsNext;
	CEdit *pQueryTime;
	CEdit *pMQueryTime;
	CEdit *pQueryAP;
	CEdit *pQueryP;
	CEdit *pMQueryAP;
	CButton *pCheckGLCM;
	CButton *pCheckEH;
	CButton *pCheckHU;
	CButton *pCheckHSV;
	CButton *pCheckSIFT;
	CButton *pCheckWAVE;
	CEdit *pWGLCM;
	CEdit *pWEH;
	CEdit *pWHU;
	CEdit *pWHSV;
	CEdit *pWSIFT;
	CEdit *pWWAVE;

	//类别索引
	int indexOfType[TYPENUM][TOTALIMG / TYPENUM];

	//查询图片
	//IplImage *queryImg;
	MyMat* queryImg;
	
	//图片库数据
	MyMat* imgs;
	ImageFeature* features;

	//显示页数
	int LibPageNum;
	int RltPageNum;

	//查询结果id排序
	CCMP *RltImages;

	void ShowImage(IplImage * img, CWnd *p, UINT id);
	void ShowLibImages();
	void ShowRltImages();
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnImgClickedLib(UINT nid);
	afx_msg void OnCbnSelChage();
	afx_msg void OnBnClickedGo();
	afx_msg void OnBnClickedIndex();
private:
	void ResizeImage(IplImage * img, IplImage* o_img);
	void StoreFeatures();
	void Normalization();
public:
	afx_msg void OnBnClickedLibPre();
	afx_msg void OnBnClickedLibNext();
	afx_msg void OnBnClickedRltPre();
	afx_msg void OnBnClickedRltNext();
};
int featureCmp(const void *ele1, const void *ele2);

//class CPanel: public CStatic
//{
//	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo){
//		if (CStatic::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//			return TRUE;
//		CWnd* pParent = GetParent();
//		if (pParent != NULL)
//			return pParent->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//		return FALSE;
//	}
//};
