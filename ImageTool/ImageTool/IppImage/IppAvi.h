#pragma once

#include <Vfw.h>

#include "IppDib.h"

class IppAvi
{
public:
	IppAvi();
	~IppAvi();

public:
	BOOL    Open(LPCTSTR lpszPathName);
	void    Close();

	// �׸��� �Լ�
	void    DrawFrame(HDC hDC, int nFrame);
	void    DrawFrame(HDC hDC, int nFrame, int dx, int dy);
	void    DrawFrame(HDC hDC, int nFrame, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);

	// ���� ������ ĸ�� �Լ�
	BOOL    GetFrame(int nFrame, IppDib& dib);

	// AVI ���� ���� ��ȯ �Լ�
	int     GetFrameRate() { return m_nFrameRate; }
	int     GetTotalFrame() { return m_nTotalFrame; }
	int     GetHeight() { return m_nHeight; }
	int     GetWidth() { return m_nWidth; }
	BOOL    IsValid() { return (m_pAviFile != NULL); };

protected:
	PAVIFILE    m_pAviFile;         // AVI ���� �������̽� ������
	PAVISTREAM  m_pVideoStream;     // ���� ��Ʈ�� ������
	PGETFRAME   m_pVideoFrame;      // ���� ������ ������

	int         m_nWidth;           // ���� ������(����) ���� ũ��
	int         m_nHeight;          // ���� ������(����) ���� ũ��

	int         m_nTotalFrame;      // ��ü ������ ����
	int         m_nFrameRate;       // �ʴ� ������ ����
};
