// DParticle.h : DParticle DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDParticleApp
// �йش���ʵ�ֵ���Ϣ������� DParticle.cpp
//

class CDParticleApp : public CWinApp
{
public:
	CDParticleApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
