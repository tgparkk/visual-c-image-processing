// FileNewDlg.cpp : implementation file
//

#include "pch.h"
#include "ImageTool.h"
#include "afxdialogex.h"
#include "FileNewDlg.h"


// CFileNewDlg dialog

IMPLEMENT_DYNAMIC(CFileNewDlg, CDialogEx)

CFileNewDlg::CFileNewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_FILE_NEW, pParent)
	, m_nWidth(256)
	, m_nHeight(256)
	, m_nType(0)
{

}

CFileNewDlg::~CFileNewDlg()
{
}

void CFileNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 1, 4096);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 1, 4096);
	DDX_CBIndex(pDX, IDC_IMAGE_TYPE, m_nType);
}


BEGIN_MESSAGE_MAP(CFileNewDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileNewDlg message handlers
