// DiffusionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "afxdialogex.h"
#include "DiffusionDlg.h"


// CDiffusionDlg 대화 상자

IMPLEMENT_DYNAMIC(CDiffusionDlg, CDialogEx)

CDiffusionDlg::CDiffusionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIFFUSION, pParent)
	, m_fLambda(0.25f)
	, m_fK(4.f)
	, m_nIteration(10)
{

}

CDiffusionDlg::~CDiffusionDlg()
{
}

void CDiffusionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DIFFUSION_LAMBDA, m_fLambda);
	DDV_MinMaxFloat(pDX, m_fLambda, 0, 0.25);
	DDX_Text(pDX, IDC_DIFFUSION_K, m_fK);
	DDX_Text(pDX, IDC_DIFFUSION_ITER, m_nIteration);
	DDV_MinMaxInt(pDX, m_nIteration, 0, 100);
}


BEGIN_MESSAGE_MAP(CDiffusionDlg, CDialogEx)
END_MESSAGE_MAP()


// CDiffusionDlg 메시지 처리기
