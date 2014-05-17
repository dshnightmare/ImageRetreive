
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
	HINSTANCE m_hLoadImageLib;   //DLL¾ä±ú
	PLoadFromCIFAR10 m_pfnLoadFromCIFAR10;
	BOOL  LoadToolDll();
	//Feature
	HINSTANCE m_hFeatures;       //DLL¾ä±ú
	PCalFeatureForImages m_pfnCalFeatureForImages;
	PCalFeatureDistance m_pfnCalFeatureDistance;
	BOOL LoadFeaturesDll();

	//¿Ø¼þ
	CStatic *pLibImages[SHOWIMGCOL*SHOWIMGROW];
	CStatic *pRstImages[SHOWIMGCOL*SHOWIMGROW];
	CComboBox *pLibImagesType;
	CEdit *pQueryTime;
	CEdit *pMQueryTime;
	CEdit *pQueryAP;
	CEdit *pMQueryAP;

	//Àà±ðË÷Òý
	int indexOfType[TYPENUM][TOTALIMG / TYPENUM];
	//²éÑ¯Í¼Æ¬
	//IplImage *queryImg;
	MyMat* queryImg;
	
	//Í¼Æ¬¿âÊý¾Ý
	MyMat* imgs;
	ImageFeature* features;


	void ShowImage(IplImage * img, CWnd *p, UINT id);
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnImgClickedLib(UINT nid);
	afx_msg void OnCbnSelChage();
	afx_msg void OnBnClickedGo();
	afx_msg void OnBnClickedIndex();
private:
	void ResizeImage(IplImage * img, IplImage* o_img);
};
int featureCmp(const void *ele1, const void *ele2);

class CCMP
{
public:
	int id;
	double d;
};
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
