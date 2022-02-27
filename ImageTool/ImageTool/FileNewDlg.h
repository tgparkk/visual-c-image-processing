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
	// �̹��� ����, ���� ũ��
	int m_nWidth;
	int m_nHeight;
	// �̹��� ���� Ÿ��(0: �׷��̽�����, 1: Ʈ���÷�)
	int m_nType;
};
