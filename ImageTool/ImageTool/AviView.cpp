// CAviView.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "AviView.h"


// CAviView

IMPLEMENT_DYNCREATE(CAviView, CScrollView)

CAviView::CAviView()
	:m_nCurrentFrame(0), m_bPlay(FALSE)
{

}

CAviView::~CAviView()
{
}


BEGIN_MESSAGE_MAP(CAviView, CScrollView)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIDEO_PLAY, &CAviView::OnVideoPlay)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PLAY, &CAviView::OnUpdateVideoPlay)
	ON_COMMAND(ID_VIDEO_STOP, &CAviView::OnVideoStop)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_STOP, &CAviView::OnUpdateVideoStop)
	ON_COMMAND(ID_VIDEO_PAUSE, &CAviView::OnVideoPause)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PAUSE, &CAviView::OnUpdateVideoPause)
	ON_COMMAND(ID_VIDEO_START, &CAviView::OnVideoStart)
	ON_COMMAND(ID_VIDEO_PREV, &CAviView::OnVideoPrev)
	ON_COMMAND(ID_VIDEO_NEXT, &CAviView::OnVideoNext)
	ON_COMMAND(ID_VIDEO_END, &CAviView::OnVideoEnd)
	ON_COMMAND(ID_VIDEO_CAPTURE, &CAviView::OnVideoCapture)
END_MESSAGE_MAP()


// CAviView 그리기

void CAviView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	if (m_Avi.IsValid())
	{
		sizeTotal.cx = m_Avi.GetWidth();
		sizeTotal.cy = m_Avi.GetHeight();
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);

	ResizeParentToFit(TRUE);

	//sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
}

void CAviView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	if (m_Avi.IsValid())
	{
		m_Avi.DrawFrame(pDC->m_hDC, m_nCurrentFrame);
	}
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

BOOL CAviView::AviFileOpen(LPCTSTR lpszPathName)
{
	// AVI 파일을 열지 못한 경우, 메시지 출력하고 그냥 리턴
	if (!m_Avi.Open(lpszPathName))
	{
		AfxMessageBox(_T("Avi 파일을 열 수 없습니다."));
		return FALSE;
	}

	m_nCurrentFrame = 0;

	AfxPrintInfo(_T("[AVI 파일 열기] 파일 경로: %s, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 전체 프레임 수: %d"),
		lpszPathName, m_Avi.GetWidth(), m_Avi.GetHeight(), m_Avi.GetTotalFrame());

	return TRUE;
}



void CAviView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		m_nCurrentFrame++;

		if (m_nCurrentFrame >= m_Avi.GetTotalFrame())
		{
			m_nCurrentFrame = m_Avi.GetTotalFrame() - 1;
			KillTimer(1);
			m_bPlay = FALSE;
		}

		Invalidate(FALSE);
	}

	CScrollView::OnTimer(nIDEvent);
}


void CAviView::OnVideoPlay()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Avi.IsValid())
	{
		m_bPlay = TRUE;
		SetTimer(1, 1000 / m_Avi.GetFrameRate(), NULL);
	}
}


void CAviView::OnUpdateVideoPlay(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Avi.IsValid());
	pCmdUI->SetCheck(m_bPlay);
}


void CAviView::OnVideoStop()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bPlay = FALSE;
	KillTimer(1);
	m_nCurrentFrame = 0;
	Invalidate(FALSE);
}


void CAviView::OnUpdateVideoStop(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(!m_bPlay && m_nCurrentFrame == 0);
}


void CAviView::OnVideoPause()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bPlay = FALSE;
	KillTimer(1);
}


void CAviView::OnUpdateVideoPause(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(!m_bPlay && m_nCurrentFrame != 0);
}


void CAviView::OnVideoStart()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nCurrentFrame = 0;
	Invalidate(FALSE);
}


void CAviView::OnVideoPrev()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nCurrentFrame > 0)
		m_nCurrentFrame--;
	Invalidate(FALSE);
}


void CAviView::OnVideoNext()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nCurrentFrame < m_Avi.GetTotalFrame() - 1)
		m_nCurrentFrame++;
	Invalidate(FALSE);
}


void CAviView::OnVideoEnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nCurrentFrame = m_Avi.GetTotalFrame() - 1;
	Invalidate(FALSE);
}


void CAviView::OnVideoCapture()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	IppDib dib;
	m_Avi.GetFrame(m_nCurrentFrame, dib);
	AfxPrintInfo(_T("[프레임 캡쳐] 프레임 번호: %d, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 색상수: %d"),
		m_nCurrentFrame, dib.GetWidth(), dib.GetHeight(), 0x01 << dib.GetBitCount());
	AfxNewBitmap(dib);
}

BOOL CAviView::OnEraseBkgnd(CDC* pDC)
{
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);

	return TRUE;       // Erased
}