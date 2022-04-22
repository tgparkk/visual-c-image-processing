﻿#pragma once

// CAviDoc 문서

class CAviDoc : public CDocument
{
	DECLARE_DYNCREATE(CAviDoc)

public:
	CAviDoc();
	virtual ~CAviDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
