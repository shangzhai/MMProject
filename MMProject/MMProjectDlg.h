
// MMProjectDlg.h : header file
//

#pragma once
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "CvvImage.h"
#include "afxwin.h"


// CMMProjectDlg dialog
class CMMProjectDlg : public CDialogEx
{
// Construction
public:
	CMMProjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MMPROJECT_DIALOG };

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
public:
	afx_msg void OnBnClickedButton1();
	void DrawPicToHDC(IplImage *img, UINT ID);
	int imageretrieval();
	int objectdetection();
	CComboBox retrievallist;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
