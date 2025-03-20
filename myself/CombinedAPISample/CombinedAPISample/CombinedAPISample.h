// CombinedAPISample.h : main header file for the COMBINEDAPISAMPLE application
//

#if !defined(AFX_COMBINEDAPISAMPLE_H__E981B3FF_8849_4695_9922_F260EA4E3FAE__INCLUDED_)
#define AFX_COMBINEDAPISAMPLE_H__E981B3FF_8849_4695_9922_F260EA4E3FAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCombinedAPISampleApp:
// See CombinedAPISample.cpp for the implementation of this class
//

class CCombinedAPISampleApp : public CWinApp
{
public:
	CCombinedAPISampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCombinedAPISampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCombinedAPISampleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBINEDAPISAMPLE_H__E981B3FF_8849_4695_9922_F260EA4E3FAE__INCLUDED_)
