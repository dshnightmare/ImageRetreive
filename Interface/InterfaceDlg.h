
// InterfaceDlg.h : header file
//
#include "../Tools/LoadImageLib.h"
#include "../ImageFeature/Features.h"
#include "../IndexBuilding/LSHTBuilder.h"
#include "../IndexBuilding/LSHTExtractor.h"
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

//����ͳ��ͶƱ
class Vote
{
public:
	int id;
	int votes;
	Vote():id(0),votes(0){}
};

//���ڽ������
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
	HINSTANCE m_hLoadImageLib;   //DLL���
	PLoadFromCIFAR10 m_pfnLoadFromCIFAR10;
	PLoadFromCIFAR10Test m_pfnLoadFromCIFAR10Test;
	BOOL  LoadToolDll();
	//Feature
	HINSTANCE m_hFeatures;       //DLL���
	PCalFeatureForImages m_pfnCalFeatureForImages;
	PCalFeatureDistance m_pfnCalFeatureDistance;
	PCreate m_pfnCreate;
	BOOL LoadFeaturesDll();
	//index
	HINSTANCE m_hIndexBuilde;   //DLL���
	PBuilder m_pfnBuilder;
	PExtractor m_pfnExtrator;
	BOOL  LoadIndexBuildDll();

	//�ؼ�
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
	CEdit *pMQueryP;
	CButton *pCheckGLCM;
	CButton *pCheckEH;
	CButton *pCheckHU;
	CButton *pCheckHSV;
	CButton *pCheckSIFT;
	CButton *pCheckWAVE;
	CButton *pCheckLBP;
	CButton *pCheckVote;
	CEdit *pWGLCM;
	CEdit *pWEH;
	CEdit *pWHU;
	CEdit *pWHSV;
	CEdit *pWSIFT;
	CEdit *pWWAVE;
	CEdit *pWLBP;

	//�������
	int indexOfType[TYPENUM][TOTALIMG / TYPENUM];

	//��ѯͼƬ
	//IplImage *queryImg;
	MyMat* queryImg;
	
	//ͼƬ������
	MyMat* imgs;
	ImageFeature* features;

	//����
	LSHTBuilder *builders;

	//��ʾҳ��
	int LibPageNum;
	int RltPageNum;

	//�Ƿ����ͶƱ
	bool useVote;
	Vote votes[TOTALIMG];

	//��ѯ���id����
	CCMP *RltImages;

	//��һ������
	double *pdmaxGLCM, *pdmaxEH, *pdmaxHUp, *pdmaxHUn, *pdmaxHSV, *pdmaxSIFT, *pdmaxWAVE, *pdmaxLBP,
		*pdminGLCM, *pdminEH, *pdminHUp, *pdminHUn, *pdminHSV, *pdminSIFT, *pdminWAVE, *pdminLBP;

	void ShowImage(IplImage * img, CWnd *p, UINT id);
	void ShowLibImages();
	void ShowRltImages();
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnImgClickedLib(UINT nid);
	afx_msg void OnCbnSelChage();
	afx_msg void OnBnClickedGo();
	afx_msg void OnBnClickedIndex();
	afx_msg void OnBnClickedLibPre();
	afx_msg void OnBnClickedLibNext();
	afx_msg void OnBnClickedRltPre();
	afx_msg void OnBnClickedRltNext();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRand200();
	afx_msg void OnBnClickedWeight(UINT nid);
private:
	void ResizeImage(IplImage * img, IplImage* o_img);
	void StoreFeatures();
	void Normalization();
};
int featureCmp(const void *ele1, const void *ele2);
int voteCmp(const void *e1, const void *e2);

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
