#pragma once
#include "afxdialogex.h"


// CFileNewDlg dialog

class CFileNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileNewDlg)

public:
	CFileNewDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFileNewDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_FILE_NEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// 이미지 가로, 세로 크기
	int m_nWidth;
	int m_nHeight;
	// 이미지 색상 타입(0: 그레이스케일, 1: 트루컬러)
	int m_nType;
};
