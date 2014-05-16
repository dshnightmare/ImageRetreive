
// InterfaceDlg.h : header file
//
#include "../Tools/LoadImageLib.h"
#include <cv.h>
#include <highgui.h>
#pragma once


#define IMAGE_WIDTH 32
#define IMAGE_HEIGHT 32
#define IMAGE_CHANNEL 3
#define  SHOWIMGROW 4
#define  SHOWIMGCOL 10

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
	BOOL  LoadQRBinaryDataDll();

	//¿Ø¼þ
	CStatic *pLibImages[SHOWIMGCOL*SHOWIMGROW];
	CStatic *pRstImages[SHOWIMGCOL*SHOWIMGROW];
	CComboBox *pLibImagesType;

	//²éÑ¯Í¼Æ¬
	IplImage *queryImg;
	
	//Í¼Æ¬¿âÊý¾Ý
	MyMat* imgs;

	void ShowImage(IplImage * img, CWnd *p, UINT id);
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnImgClickedLib(UINT nid);
	afx_msg void OnCbnSelChage();
private:
	void ResizeImage(IplImage * img);
public:
	afx_msg void OnBnClickedGo();
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
