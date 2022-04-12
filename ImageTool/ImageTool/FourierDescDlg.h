#pragma once
#include "afxdialogex.h"


// CFourierDescDlg 대화 상자

class CFourierDescDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFourierDescDlg)

public:
	CFourierDescDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFourierDescDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOURIER_DESCRIPTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nPercent;
};
