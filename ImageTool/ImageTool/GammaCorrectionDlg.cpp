// GammaCorrectionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "afxdialogex.h"
#include "GammaCorrectionDlg.h"


// CGammaCorrectionDlg 대화 상자

IMPLEMENT_DYNAMIC(CGammaCorrectionDlg, CDialogEx)

CGammaCorrectionDlg::CGammaCorrectionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAMMA_CORRECTION, pParent)
	, m_fGamma(0)
{

}

CGammaCorrectionDlg::~CGammaCorrectionDlg()
{
}

void CGammaCorrectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAMMA_SLIDER, m_sliderGamma);
	DDX_Text(pDX, IDC_GAMMA_EDIT, m_fGamma);
	DDV_MinMaxFloat(pDX, m_fGamma, 0.20, 5.00);
}


BEGIN_MESSAGE_MAP(CGammaCorrectionDlg, CDialogEx)
END_MESSAGE_MAP()


// CGammaCorrectionDlg 메시지 처리기
