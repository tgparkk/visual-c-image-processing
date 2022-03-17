// TranslateDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "afxdialogex.h"
#include "TranslateDlg.h"


// CTranslateDlg 대화 상자

IMPLEMENT_DYNAMIC(CTranslateDlg, CDialogEx)

CTranslateDlg::CTranslateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRANSLATE, pParent)
	, m_nNewSX(0)
	, m_nNewSY(0)
{

}

CTranslateDlg::~CTranslateDlg()
{
}

void CTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEW_SX, m_nNewSX);
	DDX_Text(pDX, IDC_NEW_SY, m_nNewSY);
}


BEGIN_MESSAGE_MAP(CTranslateDlg, CDialogEx)
END_MESSAGE_MAP()


// CTranslateDlg 메시지 처리기
