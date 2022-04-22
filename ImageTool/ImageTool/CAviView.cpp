// CAviView.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "CAviView.h"


// CAviView

IMPLEMENT_DYNCREATE(CAviView, CScrollView)

CAviView::CAviView()
{

}

CAviView::~CAviView()
{
}


BEGIN_MESSAGE_MAP(CAviView, CScrollView)
END_MESSAGE_MAP()


// CAviView 그리기

void CAviView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CAviView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CAviView 진단

#ifdef _DEBUG
void CAviView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CAviView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAviView 메시지 처리기
