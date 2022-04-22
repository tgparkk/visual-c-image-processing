#pragma once


// CAviChildFrame

class CAviChildFrame : public CMDIChildWndEx
{
	//DECLARE_DYNAMIC(CAviChildFrame)
	DECLARE_DYNCREATE(CAviChildFrame)

public:
	CAviChildFrame();
	virtual ~CAviChildFrame();

protected:
	DECLARE_MESSAGE_MAP()
};


