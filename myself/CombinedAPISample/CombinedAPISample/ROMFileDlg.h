#if !defined(AFX_ROMFILEDLG_H__E16420E6_1431_4FEF_A72B_07C29CED8C71__INCLUDED_)
#define AFX_ROMFILEDLG_H__E16420E6_1431_4FEF_A72B_07C29CED8C71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ROMFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CROMFileDlg dialog

class CROMFileDlg : public CDialog
{
// Construction
public:
	int m_nTypeofSystem;
	CROMFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CROMFileDlg)
	enum { IDD = IDD_ROM_FILES };
	CEdit	m_ctlROMFile;
	CListCtrl	m_ctlROMFileList;
	CString	m_szPortID;
	CString	m_szROMFile;
	int m_nNoActivePorts;
	int m_nNoPassivePorts;
	int m_nNoMagneticPorts;
	BOOL	m_bChangesSaved;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROMFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CROMFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBrowse();
	afx_msg void OnCloseButton();
	afx_msg void OnClickPortListing(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownPortListing(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAccept();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void FillROMFileTable( );
	void AddItemToList( char * pszPortID, char * pszROMFileName );
	void SaveItemToINI( char * pszPortID, char * pszROMFileName );

	int
		m_nROMSelection;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROMFILEDLG_H__E16420E6_1431_4FEF_A72B_07C29CED8C71__INCLUDED_)
