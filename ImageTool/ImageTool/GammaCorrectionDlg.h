#pragma once
#include "afxdialogex.h"


// CGammaCorrectionDlg 대화 상자

class CGammaCorrectionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGammaCorrectionDlg)

public:
	CGammaCorrectionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGammaCorrectionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMMA_CORRECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderGamma;
	float m_fGamma;
};
