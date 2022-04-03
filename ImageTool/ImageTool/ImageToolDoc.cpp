
// ImageToolDoc.cpp: CImageToolDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "ImageToolDoc.h"
#include "FileNewDlg.h"

#include "IppImage/IppImage.h"
#include "IppImage/IppConvert.h"
#include "IppImage/IppEnhance.h"
#include "BrightnessContrastDlg.h"
#include "GammaCorrectionDlg.h"
#include "ArithmeticLogicaDlg.h"

#include "IppImage/IppFilter.h"
#include "GaussianDlg.h"

#include "HistogramDlg.h"

#include "AddNoiseDlg.h"

#include "DiffusionDlg.h"

#include "IppImage/IppGeometry.h"
#include "TranslateDlg.h"

#include "ResizeDlg.h"

#include "RotateDlg.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "IppImage/IppFourier.h"
#include "FreqFilteringDlg.h"

#include "IppImage/IppFeature.h"

#include "CannyEdgeDlg.h"

#include "HarrisCornerDlg.h"

#include "IppImage/IppColor.h"

#include "ColorCombineDlg.h"

#include <algorithm>
#include <functional>

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageToolDoc

#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	IppByteImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img, dib) \
	IppDib dib; \
	IppImageToDib(img, dib);

#define CONVERT_DIB_TO_RGBIMAGE(m_dib, img) \
	IppRgbImage img; \
	IppDibToImage(m_dib, img);


#define SHOW_SPECTRUM_PHASE_IMAGE

IMPLEMENT_DYNCREATE(CImageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageToolDoc, CDocument)
	ON_COMMAND(ID_WINDOW_DUPLICATE, &CImageToolDoc::OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, &CImageToolDoc::OnEditCopy)
	ON_COMMAND(ID_IMAGE_INVERSE, &CImageToolDoc::OnImageInverse)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERSE, &CImageToolDoc::OnUpdateImageInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnBrightnessContrast)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CImageToolDoc::OnGammaCorrection)
	ON_COMMAND(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnViewHistogram)
	ON_COMMAND(ID_HISTO_STRETCHING, &CImageToolDoc::OnHistoStretching)
	ON_COMMAND(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnHistoEqualization)
	ON_COMMAND(ID_ARITHMETIC_LOGICAL, &CImageToolDoc::OnArithmeticLogical)
	ON_COMMAND(ID_BITPLANE_SLICING, &CImageToolDoc::OnBitplaneSlicing)
	ON_COMMAND(ID_FILTER_MEAN, &CImageToolDoc::OnFilterMean)
	ON_COMMAND(ID_FILTER_WEIGHTED_MEAN, &CImageToolDoc::OnFilterWeightedMean)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CImageToolDoc::OnFilterGaussian)
	ON_COMMAND(ID_FILTER_LAPLACIAN, &CImageToolDoc::OnFilterLaplacian)
	ON_COMMAND(ID_FILTER_UNSHARP_MASK, &CImageToolDoc::OnFilterUnsharpMask)
	ON_COMMAND(ID_FILTER_HIGHBOOST, &CImageToolDoc::OnFilterHighboost)
	ON_COMMAND(ID_ADD_NOISE, &CImageToolDoc::OnAddNoise)
	ON_COMMAND(ID_FILTER_MEDIAN, &CImageToolDoc::OnFilterMedian)
	ON_COMMAND(ID_FILTER_DIFFUSION, &CImageToolDoc::OnFilterDiffusion)
	ON_COMMAND(ID_IMAGE_TRANSLATION, &CImageToolDoc::OnImageTranslation)
	ON_COMMAND(ID_IMAGE_RESIZE, &CImageToolDoc::OnImageResize)
	ON_COMMAND(ID_IMAGE_ROTATE, &CImageToolDoc::OnImageRotate)
	ON_COMMAND(ID_IMAGE_MIRROR, &CImageToolDoc::OnImageMirror)
	ON_COMMAND(ID_IMAGE_FLIP, &CImageToolDoc::OnImageFlip)
	ON_COMMAND(ID_FOURIER_DFT, &CImageToolDoc::OnFourierDft)
	ON_COMMAND(ID_FOURIER_DFTRC, &CImageToolDoc::OnFourierDftrc)
	ON_COMMAND(ID_FOURIER_FFT, &CImageToolDoc::OnFourierFft)
	ON_COMMAND(ID_FREQ_FILTERING, &CImageToolDoc::OnFreqFiltering)
	ON_COMMAND(ID_EDGE_ROBERTS, &CImageToolDoc::OnEdgeRoberts)
	ON_COMMAND(ID_EDGE_PREWITT, &CImageToolDoc::OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_SOBEL, &CImageToolDoc::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_CANNY, &CImageToolDoc::OnEdgeCanny)
	ON_COMMAND(ID_HOUGH_LINE, &CImageToolDoc::OnHoughLine)
	ON_COMMAND(ID_HARRIS_CORNER, &CImageToolDoc::OnHarrisCorner)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnColorGrayscale)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnUpdateColorGrayscale)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnColorSplitRgb)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnUpdateColorSplitRgb)
	ON_COMMAND(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnColorSplitHsi)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnUpdateColorSplitHsi)
	ON_COMMAND(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnColorSplitYuv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnUpdateColorSplitYuv)
	ON_COMMAND(ID_COLOR_COMBINE_RGB, &CImageToolDoc::OnColorCombineRgb)
	ON_COMMAND(ID_COLOR_COMBINE_HSI, &CImageToolDoc::OnColorCombineHsi)
	ON_COMMAND(ID_COLOR_COMBINE_YUV, &CImageToolDoc::OnColorCombineYuv)
	ON_COMMAND(ID_COLOR_EDGE, &CImageToolDoc::OnColorEdge)
	ON_UPDATE_COMMAND_UI(ID_COLOR_EDGE, &CImageToolDoc::OnUpdateColorEdge)
	ON_UPDATE_COMMAND_UI(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnUpdateHistoEqualization)
END_MESSAGE_MAP()


// CImageToolDoc 생성/소멸

CImageToolDoc::CImageToolDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageToolDoc::~CImageToolDoc()
{
}

BOOL CImageToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	BOOL ret = TRUE;
	if (theApp.m_pNewDib == NULL)
	{
		CFileNewDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_nType == 0)
				ret = m_Dib.CreateGrayBitmap(dlg.m_nWidth, dlg.m_nHeight);
			else
				ret = m_Dib.CreateRgbBitmap(dlg.m_nWidth, dlg.m_nHeight);
		}
		else
		{
			ret = FALSE;
		}
	}
	else
	{
		m_Dib = *(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}
	

	return ret;
}




// CImageToolDoc serialization

void CImageToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageToolDoc 진단

#ifdef _DEBUG
void CImageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageToolDoc 명령


BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	//return m_Dib.Load(CT2A(lpszPathName));
	BOOL res = m_Dib.Load(CT2A(lpszPathName));
	if (res)
		AfxPrintInfo(_T("[파일 열기] 파일 경로: %s, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 색상수: %d"),
			lpszPathName, m_Dib.GetWidth(), m_Dib.GetHeight(), 0x01 << m_Dib.GetBitCount());

	return res;
}


BOOL CImageToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return m_Dib.Save(CT2A(lpszPathName));
}


void CImageToolDoc::OnWindowDuplicate()
{
	// TODO: Add your command handler code here
	AfxNewBitmap(m_Dib);
}


void CImageToolDoc::OnEditCopy()
{
	// TODO: Add your command handler code here
	if (m_Dib.IsValid())
		m_Dib.CopyToClipboard();
}


void CImageToolDoc::OnImageInverse()
{
	// TODO: Add your command handler code here
	if (m_Dib.GetBitCount() == 8)
	{
		//IppByteImage img;
		//IppDibToImage(m_Dib, img);
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

		IppInverse(img);

		//IppDib dib;
		//IppImageToDib(img, dib);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[반전] 입력 영상: %s"), GetTitle());
		
		AfxNewBitmap(dib); // 새로운 뷰에 그리기
		//UpdateAllViews(NULL); // 다시 그리기
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[반전] 입력 영상: %s"), GetTitle());
		
		AfxNewBitmap(dib); // 새로운 뷰에 그리기
		//UpdateAllViews(NULL); // 다시 그리기
	}
}


void CImageToolDoc::OnUpdateImageInverse(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


/*
#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	IppByteImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img, dib) \
	IppDib dib; \
	IppImageToDib(img, dib);
*/

void CImageToolDoc::OnBrightnessContrast()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppBrightness(img, dlg.m_nBrightness);
		IppContrast(img, dlg.m_nContrast);
		CONVERT_IMAGE_TO_DIB(img, dib);

		AfxPrintInfo(_T("[밝기/명암비 조절] 입력 영상 : %s, 밝기 : %d, 명암비 : %d%%"),
			GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
		AfxNewBitmap(dib);

	}

}


void CImageToolDoc::OnGammaCorrection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGammaCorrectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppGammaCorrection(img, dlg.m_fGamma);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[감마 보정] 입력 영상 : %s, 감마: %4.2f"), GetTitle(), dlg.m_fGamma);
		AfxNewBitmap(dib);
			
	}
}


void CImageToolDoc::OnViewHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHistogramDlg dlg;
	dlg.SetImage(&m_Dib);
	dlg.DoModal();
}


void CImageToolDoc::OnHistoStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppHistogramStretching(img);
	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[히스토그램 스트레칭] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnHistoEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppByteImage imgY, imgU, imgV;
		IppColorSplitYUV(img, imgY, imgU, imgV);
		IppHistogramEqualization(imgY);

		IppRgbImage imgRes;
		IppColorCombineYUV(imgY, imgU, imgV, imgRes);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}

void CImageToolDoc::OnUpdateHistoEqualization(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnArithmeticLogical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, img1)
			CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, img2)
			IppByteImage img3;

		bool ret = false;

		switch (dlg.m_nFunction)
		{
		case 0: ret = IppAdd(img1, img2, img3);  break;
		case 1: ret = IppSub(img1, img2, img3);  break;
		case 2: ret = IppAve(img1, img2, img3);  break;
		case 3: ret = IppDiff(img1, img2, img3); break;
		case 4: ret = IppAND(img1, img2, img3);  break;
		case 5: ret = IppOR(img1, img2, img3);   break;
		}

		if (ret)
		{
			CONVERT_IMAGE_TO_DIB(img3, dib)

				TCHAR* op[] = { _T("덧셈"), _T("뺄셈"), _T("평균"), _T("차이"), _T("논리 AND"), _T("논리 OR") };
			AfxPrintInfo(_T("[산술 및 논리 연산] [%s] 입력 영상 #1: %s, 입력 영상 #2: %s"),
				op[dlg.m_nFunction], pDoc1->GetTitle(), pDoc2->GetTitle());
			AfxNewBitmap(dib);
		}
		else
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
	}
}


void CImageToolDoc::OnBitplaneSlicing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgPlane;

	for (int i = 0; i < 8; i++)
	{
		IppBitPlane(img, imgPlane, i);
		CONVERT_IMAGE_TO_DIB(imgPlane, dib)
		AfxNewBitmap(dib);
	}

	AfxPrintInfo(_T("[비트 평면 분할] 입력 영상: %s"), GetTitle());
}


void CImageToolDoc::OnFilterMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[평균 값 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterWeightedMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterWeightedMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[가중 평균 값 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterGaussian(imgSrc, imgDst, dlg.m_fSigma);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[가우시안 필터] 입력 영상: %s, Sigma: %4.2f"), GetTitle(), dlg.m_fSigma);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterLaplacian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[라플라시안 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterUnsharpMask()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterUnsharpMask(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[언샤프 마스크 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterHighboost()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	float alpha = 1.2f;
	IppFilterHighboost(imgSrc, imgDst, alpha);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[하이부스트 필터] 입력 영상: %s, alpha = %4.2f"), GetTitle(), alpha);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnAddNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAddNoiseDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;

		if (dlg.m_nNoiseType == 0)
			IppNoiseGaussian(imgSrc, imgDst, dlg.m_nAmount);
		else
			IppNoiseSaltNPepper(imgSrc, imgDst, dlg.m_nAmount);

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* noise[] = { _T("가우시안"), _T("소금&후추") };
		AfxPrintInfo(_T("[잡음 추가] 입력 영상: %s, 잡음 종류: %s, 잡음 양: %d"),
			GetTitle(), noise[dlg.m_nNoiseType], dlg.m_nAmount);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMedian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[미디언 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterDiffusion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDiffusionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterDiffusion(imgSrc, imgDst, dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[비등방성 확산 필터] 입력 영상: %s, Lambda: %4.2f, K: %4.2f, 반복 횟수: %d"),
				GetTitle(), dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CTranslateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		IppTranslate(imgSrc, imgDst, dlg.m_nNewSX, dlg.m_nNewSY);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[이동 변환] 입력 영상: %s, 가로 이동: %d, 세로 이동: %d"),
				GetTitle(), dlg.m_nNewSX, dlg.m_nNewSY);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageResize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CResizeDlg dlg;
	dlg.m_nOldWidth = m_Dib.GetWidth();
	dlg.m_nOldHeight = m_Dib.GetHeight();
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		switch (dlg.m_nInterpolation)
		{
		case 0: IppResizeNearest(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		case 1: IppResizeBilinear(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		case 2: IppResizeCubic(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		}
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* interpolation[] = { _T("최근방 이웃 보간법"), _T("양선형 보간법"), _T("3차 회선 보간법") };
		AfxPrintInfo(_T("[크기 변환] 입력 영상: %s, , 새 가로 크기: %d, 새 세로 크기: %d, 보간법: %s"),
			GetTitle(), dlg.m_nNewWidth, dlg.m_nNewHeight, interpolation[dlg.m_nInterpolation]);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CRotateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		switch (dlg.m_nRotate)
		{
		case 0: IppRotate90(imgSrc, imgDst); break;
		case 1: IppRotate180(imgSrc, imgDst); break;
		case 2: IppRotate270(imgSrc, imgDst); break;
		case 3: IppRotate(imgSrc, imgDst, (double)dlg.m_fAngle); break;
		}

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* rotate[] = { _T("90도"), _T("180도"), _T("270도") };
		if (dlg.m_nRotate != 3)
			AfxPrintInfo(_T("[회전 변환] 입력 영상: %s, 회전 각도: %s"), GetTitle(), rotate[dlg.m_nRotate]);
		else
			AfxPrintInfo(_T("[회전 변환] 입력 영상: %s, 회전 각도: %4.2f도"), GetTitle(), dlg.m_fAngle);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageMirror()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppMirror(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[좌우 대칭] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnImageFlip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFlip(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[상하 대칭] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFourierDft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 128 * 128)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다.\n계속 하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.DFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[푸리에변환/DFT] 입력 영상: %s, 입력 영상 크기: %dx%d, 처리 시간: %dmsec"),
			GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFourierDftrc()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 256 * 256)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다.\n계속 하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFTRC(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.DFTRC(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[푸리에변환/DFTRC] 입력 영상: %s, 입력 영상 크기: %dx%d, 처리 시간: %dmsec"),
			GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFourierFft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.FFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.FFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[푸리에변환/FFT] 입력 영상: %s, 입력 영상 크기: %dx%d, 처리 시간: %dmsec"),
			GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFreqFiltering()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CFreqFilteringDlg dlg;
	dlg.m_strRange.Format(_T("(0 ~ %d)"), __min(w / 2, h / 2));
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

		IppFourier fourier;
		fourier.SetImage(img);
		fourier.FFT(1);

		if (dlg.m_nFilterType == 0)
		{
			if (dlg.m_nFilterShape == 0)
				fourier.LowPassIdeal(dlg.m_nCutoff);
			else
				fourier.LowPassGaussian(dlg.m_nCutoff);
		}
		else
		{
			if (dlg.m_nFilterShape == 0)
				fourier.HighPassIdeal(dlg.m_nCutoff);
			else
				fourier.HighPassGaussian(dlg.m_nCutoff);
		}

		fourier.FFT(-1);

		IppByteImage img2;
		fourier.GetImage(img2);
		CONVERT_IMAGE_TO_DIB(img2, dib)

		TCHAR* type[] = { _T("저역 통과 필터"), _T("고역 통과 필터") };
		TCHAR* shape[] = { _T("이상적(Ideal)"), _T("가우시안(Gaussian)") };
		AfxPrintInfo(_T("[주파수 공간 필터링] 입력 영상: %s, 필터 종류: %s, 필터 모양: %s, 차단 주파수: %d"),
			GetTitle(), type[dlg.m_nFilterType], shape[dlg.m_nFilterShape], dlg.m_nCutoff);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnEdgeRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeRoberts(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/로버츠] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnEdgePrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgePrewitt(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/프리윗] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnEdgeSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeSobel(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/소벨] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnEdgeCanny()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCannyEdgeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgEdge;
		IppEdgeCanny(img, imgEdge, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

		AfxPrintInfo(_T("[캐니 엣지 검출] 입력 영상: %s, sigma: %4.2f, Low Th: %4.2f, High Th: %4.2f"),
				GetTitle(), dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnHoughLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeCanny(img, imgEdge, 1.4f, 30.f, 60.f);

	std::vector<IppLineParam> lines;
	IppHoughLine(imgEdge, lines);

	if (lines.size() == 0)
	{
		AfxMessageBox(_T("검출된 직선이 없습니다."));
		return;
	}

	std::sort(lines.begin(), lines.end());

	// 최대 10개의 직선만 화면에 그려줌.
	int cnt = min(10, lines.size());
	for (int i = 0; i < cnt; i++)
		IppDrawLine(img, lines[i], 255);

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[허프 선 검출] 입력 영상: %s, 중요 직선: rho = %4.2f, angle = %4.2f, vote = %d"),
			GetTitle(), lines[0].rho, (lines[0].ang * 180 / 3.14f), lines[0].vote);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnHarrisCorner()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHarrisCornerDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		std::vector<IppPoint> corners;
		IppHarrisCorner(img, corners, dlg.m_nHarrisTh);

		BYTE** ptr = img.GetPixels2D();

		int x, y;
		for (IppPoint cp : corners)
		{
			x = cp.x;
			y = cp.y;

			ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
			ptr[y][x - 1] = ptr[y][x] = ptr[y][x + 1] = 0;
			ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;
		}

		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[해리스 코너 검출] 입력 영상: %s, Threshold: %d, 검출된 코너 갯수: %d"),
				GetTitle(), dlg.m_nHarrisTh, corners.size());
		AfxNewBitmap(dib);
	}
}

void CImageToolDoc::OnColorGrayscale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgGray;
	imgGray.Convert(imgColor);
	CONVERT_IMAGE_TO_DIB(imgGray, dib)

	AfxPrintInfo(_T("[그레이스케일 변환] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorGrayscale(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgR, imgG, imgB;
	IppColorSplitRGB(imgColor, imgR, imgG, imgB);
	CONVERT_IMAGE_TO_DIB(imgR, dibR)
	CONVERT_IMAGE_TO_DIB(imgG, dibG)
	CONVERT_IMAGE_TO_DIB(imgB, dibB)

	AfxPrintInfo(_T("[색상 평면 나누기/RGB] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibR);
	AfxNewBitmap(dibG);
	AfxNewBitmap(dibB);
}


void CImageToolDoc::OnUpdateColorSplitRgb(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgH, imgS, imgI;
	IppColorSplitHSI(imgColor, imgH, imgS, imgI);
	CONVERT_IMAGE_TO_DIB(imgH, dibH)
	CONVERT_IMAGE_TO_DIB(imgS, dibS)
	CONVERT_IMAGE_TO_DIB(imgI, dibI)

	AfxPrintInfo(_T("[색상 평면 나누기/HSI] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibH);
	AfxNewBitmap(dibS);
	AfxNewBitmap(dibI);
}


void CImageToolDoc::OnUpdateColorSplitHsi(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgY, imgU, imgV;
	IppColorSplitYUV(imgColor, imgY, imgU, imgV);
	CONVERT_IMAGE_TO_DIB(imgY, dibY)
	CONVERT_IMAGE_TO_DIB(imgU, dibU)
	CONVERT_IMAGE_TO_DIB(imgV, dibV)

	AfxPrintInfo(_T("[색상 평면 나누기/YUV] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibY);
	AfxNewBitmap(dibU);
	AfxNewBitmap(dibV);
}


void CImageToolDoc::OnUpdateColorSplitYuv(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorCombineRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgR)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgG)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgB)

		IppRgbImage imgColor;
		if (IppColorCombineRGB(imgR, imgG, imgB, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/RGB] 입력 영상: R: %s, G: %s, B: %s"),
				pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgH)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgS)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgI)

		IppRgbImage imgColor;
		if (IppColorCombineHSI(imgH, imgS, imgI, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/HSI] 입력 영상: H: %s, S: %s, I: %s"),
				pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgY)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgU)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgV)

		IppRgbImage imgColor;
		if (IppColorCombineYUV(imgY, imgU, imgV, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/YUV] 입력 영상: Y: %s, U: %s, V: %s"),
				pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppColorEdge(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[컬러 엣지 검출] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorEdge(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}