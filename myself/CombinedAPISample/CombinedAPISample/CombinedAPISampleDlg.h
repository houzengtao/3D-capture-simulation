// CombinedAPISampleDlg.h : header file
//

#if !defined(AFX_COMBINEDAPISAMPLEDLG_H__02B67C8C_DED3_416E_A5C1_06207345A96C__INCLUDED_)
#define AFX_COMBINEDAPISAMPLEDLG_H__02B67C8C_DED3_416E_A5C1_06207345A96C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_FILL_LIST		WM_USER+1
#define WM_COM_PORT_TO		WM_USER+2
#define MODE_PRE_INIT		0x01
#define MODE_INIT			0x02
#define MODE_TRACKING		0x03
#define MODE_ACTIVATED		0x04

#include "CommandHandling.h"	/* included the command handling header */
/////////////////////////////////////////////////////////////////////////////
// CCombinedAPISampleDlg dialog

class CCombinedAPISampleDlg : public CDialog
{
// Construction
public:
	int control;
	CDatabase m_database;
	CString strExcelFile;
	CString strSQL;
	HANDLE hDevice;
	int    ChannelCount;
	CCombinedAPISampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCombinedAPISampleDlg)
	enum { IDD = IDD_COMBINEDAPISAMPLE_DIALOG };
		CEdit	m_ctlFrameNo;
	CComboBox	m_ctlPortHandleCB;
	CComboBox	m_ctlRefHandleCB;
	CListCtrl	m_ctlTrackingList;
	CString	m_szFrameNumber;
	BOOL	m_bInterference;
	CString	m_szSystemMode;
	BOOL	m_bUse0x0800Option;
	BOOL	m_bUseEulerAngles;
	int		m_nTrackingMode;
	CString	m_szManufID;
	BOOL	m_bPortEnabled;
	BOOL	m_bPortInitialized;
	CString	m_szSerialNo;
	CString	m_szToolRev;
	CString	m_szToolType;
	CString	m_szPartNumber;
	float	m_displacing;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCombinedAPISampleDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCombinedAPISampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnResetSystem();
	afx_msg void OnInitializeSystem();
	afx_msg void OnActivatePorts();
	afx_msg void OnTrackingBut();
	afx_msg LONG nGetSystemTransformData( UINT wParam, LONG lParam );
	afx_msg LONG nComPortTimeout( UINT wParam, LONG lParam );
	afx_msg void OnFileExit();
	afx_msg void OnSettingsComportsettings();
	afx_msg void OnSettingsRomfilesettings();
	afx_msg void OnSystemActivateports();
	afx_msg void OnSystemInitializesystem();
	afx_msg void OnSystemStarttracking();
	afx_msg void OnSystemStoptracking();
	afx_msg void OnViewSystemproperties();
	afx_msg void OnViewAlertFlags();
	afx_msg void OnSelchangePortHandles();
	afx_msg void OnSelchangeRefPortHandle();
	afx_msg void OnOptionsIlluminatorfiringrate();
	afx_msg void OnOptionsProgramoptions();
	afx_msg void OnHelpAboutcombinedapisample();
	afx_msg void OnPortEnabled();
	afx_msg void OnDiagnosticsPending();
	afx_msg void OnEulerAngles();
	afx_msg void OnButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/*****************************************************************
Variables
*****************************************************************/
	bool
		m_bResetHardware,		/* reset hardware variable */
		m_bWireless,			/* uses the wireless compatible settings */
		m_bSystemInitialized,	/* is the system initialized */
		m_bPortsActivated;		/* are ports activated */
	int
		m_nCOMPort;				/* the current com port number */

	CCommandHandling
		*pCommandHandling;		/* point to the command handling class */

	HACCEL 
		m_hAccel;				/* the main dialogs accelerator */

	CMenu
		*pMainMenu;				/* point to the main menu for the form */

/*****************************************************************
Routine Definitions
*****************************************************************/
	int nActivatePorts();		/* Activate all ports */
	int nStartTracking();		/* start tracking */
	int nStopTracking();		/* stop tracking */
	void SetMode( int nMode );	/* set the mode of the dialog */
	CString GetExcelDriver();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBINEDAPISAMPLEDLG_H__02B67C8C_DED3_416E_A5C1_06207345A96C__INCLUDED_)
