
// face_compare.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cface_compareApp: 
// �йش����ʵ�֣������ face_compare.cpp
//

class Cface_compareApp : public CWinApp
{
public:
	Cface_compareApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cface_compareApp theApp;