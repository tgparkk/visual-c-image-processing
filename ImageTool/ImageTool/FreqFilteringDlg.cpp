// FreqFilteringDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "afxdialogex.h"
#include "FreqFilteringDlg.h"


// CFreqFilteringDlg 대화 상자

IMPLEMENT_DYNAMIC(CFreqFilteringDlg, CDialogEx)

CFreqFilteringDlg::CFreqFilteringDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FREQUENCY_FILTERING, pParent)
	, m_nFilterType(0)
	, m_nFilterShape(0)
	, m_nCutoff(32)
	, m_strRange(_T(""))
{

}

CFreqFilteringDlg::~CFreqFilteringDlg()
{
}

void CFreqFilteringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_FILTER_TYPE, m_nFilterType);
	DDX_CBIndex(pDX, IDC_FILTER_SHAPE, m_nFilterShape);
	DDX_Text(pDX, IDC_CUTOFF_FREQ, m_nCutoff);
	DDX_Text(pDX, IDC_RADIUS_RANGE, m_strRange);
}


BEGIN_MESSAGE_MAP(CFreqFilteringDlg, CDialogEx)
END_MESSAGE_MAP()


// CFreqFilteringDlg 메시지 처리기
