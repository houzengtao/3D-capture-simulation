// CombinedAPISampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CombinedAPISample.h"
#include "CombinedAPISampleDlg.h"
#include "COMPortSettings.h"
#include "ROMFileDlg.h"
#include "SystemFeaturesDlg.h"
#include "IlluminatorFiringRate.h"
#include "ProgramOptions.h"
#include "NewAlertFlagsDlg.h"
#include "INIFileRW.h"
#include "Conversions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*****************************************************************
Global Variables
*****************************************************************/
bool
	m_bStopTracking, /* flag that tells the thread to stop tracking */
	m_bIsTracking;   /* flag that specifies if we are tracking */
CDC m_dc;
UINT FillTrackingTable(LPVOID pParam); /* FillTrackingTable thread declaration */
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCombinedAPISampleDlg dialog

CCombinedAPISampleDlg::CCombinedAPISampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCombinedAPISampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCombinedAPISampleDlg)
	m_szFrameNumber = _T("");
	m_bInterference = FALSE;
	m_szSystemMode = _T("");
	m_bUse0x0800Option = FALSE;
	m_bUseEulerAngles = FALSE;
	m_nTrackingMode = 0;
	m_szManufID = _T("");
	m_bPortEnabled = FALSE;
	m_bPortInitialized = FALSE;
	m_szSerialNo = _T("");
	m_szToolRev = _T("");
	m_szToolType = _T("");
	m_szPartNumber = _T("");
	m_displacing = 0.0f;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCombinedAPISampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCombinedAPISampleDlg)
	DDX_Control(pDX, IDC_FRAME_NO, m_ctlFrameNo);
	DDX_Control(pDX, IDC_PORT_HANDLES, m_ctlPortHandleCB);
	DDX_Control(pDX, IDC_REFERENCE_HANDLE_CMB, m_ctlRefHandleCB);
	DDX_Control(pDX, IDC_TRACKING_LIST, m_ctlTrackingList);
	DDX_Text(pDX, IDC_FRAME_NO, m_szFrameNumber);
	DDX_Text(pDX, IDC_SYSTEMMODE, m_szSystemMode);
	DDX_Check(pDX, IDC_TRACKING_REPORT_OOV, m_bUse0x0800Option);
	DDX_Check(pDX, IDC_EULER_ANGLES, m_bUseEulerAngles);
	DDX_Radio(pDX, IDC_TRACKING_TX, m_nTrackingMode);
	DDX_Text(pDX, IDC_MANUF_ID, m_szManufID);
	DDX_Check(pDX, IDC_PORT_ENABLED, m_bPortEnabled);
	DDX_Check(pDX, IDC_PORT_INIT, m_bPortInitialized);
	DDX_Text(pDX, IDC_SERIAL_NO, m_szSerialNo);
	DDX_Text(pDX, IDC_TOOL_REV, m_szToolRev);
	DDX_Text(pDX, IDC_TOOL_TYPE, m_szToolType);
	DDX_Text(pDX, IDC_PARTNUMBER, m_szPartNumber);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCombinedAPISampleDlg, CDialog)
	//{{AFX_MSG_MAP(CCombinedAPISampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RESET_SYSTEM, OnResetSystem)
	ON_BN_CLICKED(IDC_INITIALIZE_SYSTEM, OnInitializeSystem)
	ON_BN_CLICKED(IDC_ACTIVATE_PORTS, OnActivatePorts)
	ON_BN_CLICKED(IDC_TRACKING_BUT, OnTrackingBut)
	ON_MESSAGE(WM_FILL_LIST, nGetSystemTransformData)
	ON_MESSAGE(WM_COM_PORT_TO, nComPortTimeout)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_SETTINGS_COMPORTSETTINGS, OnSettingsComportsettings)
	ON_COMMAND(ID_SETTINGS_ROMFILESETTINGS, OnSettingsRomfilesettings)
	ON_COMMAND(ID_SYSTEM_ACTIVATEPORTS, OnSystemActivateports)
	ON_COMMAND(ID_SYSTEM_INITIALIZESYSTEM, OnSystemInitializesystem)
	ON_COMMAND(ID_SYSTEM_STARTTRACKING, OnSystemStarttracking)
	ON_COMMAND(ID_SYSTEM_STOPTRACKING, OnSystemStoptracking)
	ON_COMMAND(ID_VIEW_SYSTEMPROPERTIES, OnViewSystemproperties)
	ON_COMMAND(ID_VIEW_DIAGNOSTICALERTS, OnViewAlertFlags)
	ON_CBN_SELCHANGE(IDC_PORT_HANDLES, OnSelchangePortHandles)
	ON_CBN_SELCHANGE(IDC_REFERENCE_HANDLE_CMB, OnSelchangeRefPortHandle)
	ON_COMMAND(ID_OPTIONS_ILLUMINATORFIRINGRATE, OnOptionsIlluminatorfiringrate)
	ON_COMMAND(ID_OPTIONS_PROGRAMOPTIONS, OnOptionsProgramoptions)
	ON_COMMAND(ID_HELP_ABOUTCOMBINEDAPISAMPLE, OnHelpAboutcombinedapisample)
	ON_BN_CLICKED(IDC_PORT_ENABLED, OnPortEnabled)
	ON_BN_CLICKED(IDC_DIAGNOSTICS_PENDING, OnDiagnosticsPending)
	ON_BN_CLICKED(IDC_EULER_ANGLES, OnEulerAngles)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCombinedAPISampleDlg message handlers

BOOL CCombinedAPISampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
//ShowWindow(3);//窗口最大化


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	pCommandHandling = new CCommandHandling;

	m_hAccel = LoadAccelerators(NULL, 
								MAKEINTRESOURCE(IDR_MAIN_MENU));

	/*
	 * setup the table headings
	 */
	m_ctlTrackingList.InsertColumn( 0, "Handle", LVCFMT_LEFT, 49, -1 );
	m_ctlTrackingList.InsertColumn( 1, "Port", LVCFMT_LEFT, 0, -1 );
	m_ctlTrackingList.InsertColumn( 2, "Tx", LVCFMT_LEFT, 65, -1 );
	m_ctlTrackingList.InsertColumn( 3, "Ty", LVCFMT_LEFT, 65, -1 );
	m_ctlTrackingList.InsertColumn( 4, "Tz", LVCFMT_LEFT, 65, -1 );
	m_ctlTrackingList.InsertColumn( 5, "Qo", LVCFMT_LEFT, 65, -1 );
	m_ctlTrackingList.InsertColumn( 6, "Qx", LVCFMT_LEFT, 65, -1 );
	m_ctlTrackingList.InsertColumn( 7, "Qy", LVCFMT_LEFT, 65, -1 );
	m_ctlTrackingList.InsertColumn( 8, "Qz", LVCFMT_LEFT, 65, -1 );
	m_ctlTrackingList.InsertColumn( 9, "Error", LVCFMT_LEFT, 60, -1 );
	m_ctlTrackingList.InsertColumn( 10, "Status", LVCFMT_LEFT, 42, -1 );

	m_ctlTrackingList.SetExtendedStyle(LVS_EX_FULLROWSELECT );

	/*
	 * set initial dialog mode
	 */
	m_szSystemMode = "Setup Mode";
	SetMode( MODE_PRE_INIT );
	UpdateData(false);

	/* variable initialization */
	m_bStopTracking = FALSE;
	m_bIsTracking = FALSE;
	m_bResetHardware = FALSE;
	m_bWireless = FALSE;
	m_bSystemInitialized = FALSE;
	m_bPortsActivated = FALSE;
	m_nCOMPort = 0;	
	//以下数据库初始化
		CString pathname,strexten;
	CString strDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel安装驱动

	strexten=".xls";
	CFileDialog dlg(FALSE);//<-这里用TRUE与FALSE有什么不同？


    if(IDOK==dlg.DoModal())
	{
	 pathname=dlg.GetPathName()+strexten; 
	}

	strExcelFile =pathname;
	TRY
	{
	
		strSQL.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
			strDriver,strExcelFile,strExcelFile);	
		if( m_database.OpenEx(strSQL,CDatabase::noOdbcDialog) )
		{			
		//	strSQL = "CREATE TABLE Table1 (Handle TEXT,Tx TEXT,Ty TEXT,Tz TEXT,Q0 TEXT,QX TEXT,QY TEXT,QZ TEXT,Error TEXT,Status TEXT)";	
			strSQL = "CREATE TABLE Table1 (Handle TEXT,Status TEXT,Tx TEXT,Ty TEXT,Tz TEXT,Error TEXT)";	
			m_database.ExecuteSQL(strSQL);
			strSQL = "CREATE TABLE Table2 (Pressure NUMBER,distance NUMBER,angle NUMBER,Pressureup NUMBER)";	
			m_database.ExecuteSQL(strSQL);
		}
		
	}
	CATCH_ALL(e)
	{
		TRACE1("Excel驱动没有安装: %s",strDriver);
	}
	END_CATCH_ALL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCombinedAPISampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCombinedAPISampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{

			int m,w;
			int i;
			CRect rect;
			CString str;
			//char line[10][5]={"L1","L2","L3","L4","L5","L6","L7","L8","L9","L10"};
			CClientDC dc(this); //创建客户区设备环境
					
			GetClientRect(rect);//得到整个客户区的大小
			m=rect.Height()/12;
			w=rect.Width()/30;
			//for(i=0;i<10;i++)
			//	dc.TextOut(100,(10+1)*m,line[i]);//显示数字


/*	     CRect rect1(24,9*m,24+19*w,11*m);	
	     CBrush bkbrush(HS_CROSS,RGB(0,128,0));
	     dc.SetBkColor(RGB(0,0,0));
	//CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
         dc.FillRect(rect1,&bkbrush);
*/
         dc.SetBkColor(RGB(255,0,0));//设定对话框的背景颜色

			dc.MoveTo(24,11*m);
			dc.LineTo(24+19*w,11*m);//X轴
			dc.MoveTo(24,11*m);
			dc.LineTo(24,9*m);//Y轴
			dc.TextOut(240,(10+1)*m,"X轴");//显示数字
            for(i=0;i<6;i++)
			{
				//dc.MoveTo(24+i*w,11*m);
				//dc.LineTo(24+i*w,9*m);//画竖线的两句
				dc.MoveTo(24,(9+0.4*i)*m);
				dc.LineTo(24+19*w,(9+0.4*i)*m);//画横线的两句
				str.Format("%d",i*100);
				dc.TextOut(4,(11-0.4*(i))*m-8,str);
			}
	}
	
	CDialog::OnPaint();
	
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCombinedAPISampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
BOOL CCombinedAPISampleDlg::DestroyWindow() 
{
	/*
	 * if we are tracking, stop. Then close com port and do clean up
	 */
	m_database.Close();
	if (m_bIsTracking)
		nStopTracking();

	pCommandHandling->nCloseComPorts();

	if ( pCommandHandling )
		delete ( pCommandHandling );
	if(hDevice)
	{	
	  USB5935_ReleaseDeviceAD(hDevice);
	  USB5935_ReleaseDevice(hDevice); 
	}
	return CDialog::DestroyWindow();
}
/*****************************************************************
				PreTranslateMessage
			VISUAL C++ FUNCTION CALL
*****************************************************************/
BOOL CCombinedAPISampleDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (m_hAccel && TranslateAccelerator(GetSafeHwnd(), m_hAccel, pMsg))
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}
/*****************************************************************
Name:				OnResetSystem

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles what happens when the user
				presses the Reset System button.  It resets the
				system they are connected to.
*****************************************************************/
void CCombinedAPISampleDlg::OnResetSystem() 
{
	CWaitCursor
		wait;

	int
		nRet = 0;

	/*
	 * read the reset variable from the ini file, this tells us
	 * if you want to reset the system.
	 */

	/* 
	 * This feature is useful for debugging only
	 */
	ReadINIParm( "Communication", "Reset Hardware", "0", &m_bResetHardware );
	ReadINIParm( "Communication", "COM Port", "0", &m_nCOMPort );
	ReadINIParm( "Communication", "Wireless", "0", &m_bWireless );

	pCommandHandling->nCloseComPorts();

	if (!pCommandHandling->nOpenComPort( m_nCOMPort ))
	{
		MessageBox( "COM Port could not be opened.  "
		"Check your COM Port settings and\n"
		"make sure you system is turned on.",
		"COM ERROR", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
		return;
	} /* if */

	if ( m_bResetHardware )
	{
		if( !pCommandHandling->nHardWareReset(m_bWireless) )
			return;		

		if ( !pCommandHandling->nSetSystemComParms( 0, 0, 0, 0, 0))
			return;
	} /* if */

	/*
	 * set the new mode
	 */
	m_szSystemMode = "Setup Mode";
	SetMode( MODE_PRE_INIT );
	MessageBox( "System reset successful", "Reset", MB_ICONINFORMATION|MB_SYSTEMMODAL|MB_SETFOREGROUND );
	UpdateData(false);
} /* OnResetSystem */
/*****************************************************************
Name:				OnInitializeSystem

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles what happen when the user
				presses the Intialize System button.  We initialize
				the System.
*****************************************************************/
void CCombinedAPISampleDlg::OnInitializeSystem() 
{
	int
		nBaudRate = 0,
		nStopBits = 0,
		nParity   = 0,
		nDataBits = 0,
		nHardware = 0,
		nWireless = 0;

	CWaitCursor
		wait;

	/*
	 * read the COM port parameters from the ini file
	 */
	ReadINIParm( "Communication", "Baud Rate", "0", &nBaudRate );
	ReadINIParm( "Communication", "Stop Bits", "0", &nStopBits );
	ReadINIParm( "Communication", "Parity", "0", &nParity );
	ReadINIParm( "Communication", "Data Bits", "0", &nDataBits );
	ReadINIParm( "Communication", "Hardware", "0", &nHardware );
	ReadINIParm( "Communication", "COM Port", "0", &m_nCOMPort );
	ReadINIParm( "Communication", "Wireless", "0", &m_bWireless );

	/* 
	 * This feature is useful for debugging only, m_bResetHardware is set to 
	 * TRUE to disable it. 
	 */
	ReadINIParm( "Communication", "Reset Hardware", "0", &m_bResetHardware );

	/*
	 * close, then open the port
	 */
	pCommandHandling->nCloseComPorts();
	if (!pCommandHandling->nOpenComPort( m_nCOMPort ))
	{
		MessageBox( "COM Port could not be opened.  "
		"Check your COM Port settings and\n"
		"make sure you system is turned on.",
		"COM ERROR", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
		return;
	} /* if */

	/*
	 * if we are supposed to reset, call the reset now
	 */
	if ( m_bResetHardware )
	{
		if (!pCommandHandling->nHardWareReset(m_bWireless))
			return;
	}/* if */

	/*
	 * get the timeout values for the commands
	 * this will return an error with all other systems, other than Vicra
	 */
	pCommandHandling->CreateTimeoutTable();

	/*
	 * set the System COM Port parameters, then the computers COM Port parameters.
	 * if that is successful, initialize the system
	 */
	if(pCommandHandling->nSetSystemComParms( nBaudRate, nDataBits, nParity, nStopBits, nHardware ))
	{
		if(pCommandHandling->nSetCompCommParms( nBaudRate, nDataBits, nParity, nStopBits, nHardware ))
		{
			if(pCommandHandling->nInitializeSystem())
			{
				/*
				 * get the system information
				 */
				if (!pCommandHandling->nGetSystemInfo())
				{
					/*
					 * Check system type: Polaris, Polaris Accedo, and Aurora
					 */
					MessageBox( "Could not determine type of system\n"
					"(Polaris, Polaris Accedo, Polaris Vicra or Aurora)",
					"INIT ERROR", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
					return;
				} /* if */

				/* 
				 * Set firing rate if system type is Polaris or Polaris Accedo.
				 */
				if( pCommandHandling->m_dtSystemInformation.nTypeofSystem != AURORA_SYSTEM )
				{
					pCommandHandling->nSetFiringRate(); 
				}/* if */
				m_szSystemMode = "System Initialized";
				SetMode( MODE_INIT );
				UpdateData(false);
				m_bSystemInitialized = TRUE;

				MessageBox( "System successfully intialized", "Initialization", MB_ICONINFORMATION|MB_SYSTEMMODAL|MB_SETFOREGROUND );
				return;
			} /* if */
			else
			{
				MessageBox( "System could not be initialized. "
				"Check your COM Port settings, make sure your\n"
				"system is turned on and the system components are compatible.",
				"INIT ERROR", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
			}/* else */
		}/* if */
	}/* if */
} /* OnInitializeSystem */


/*****************************************************************
Name:				OnActivePorts

Inputs:
	None.

Return Value:
	None.

Description:   This routine handles what happens when the user presses
			   the active ports button.
*****************************************************************/
void CCombinedAPISampleDlg::OnActivatePorts() 
{
	nActivatePorts();
} /* OnActivatePorts */
/*****************************************************************
Name:				OnTrackingBut

Inputs:
	None.

Return Value:
	None.

Description:   This routine handles what happens when the user 
			   presses the Tracking Button.
*****************************************************************/
void CCombinedAPISampleDlg::OnTrackingBut() 
{
	CWaitCursor	wait;

	CString	szButtonText;

	GetDlgItem( IDC_TRACKING_BUT )->GetWindowText( szButtonText );

	if (  szButtonText == "&Start Tracking" )
	{
			//	CDatabase m_database;
		nStartTracking();
	}
	else
	{
		nStopTracking();
	}
} /* OnTrackingBut */
/*****************************************************************
Name:				nActivePorts

Inputs:
	None.

Return Value:
	int - 1 if successful, 0 otherwise

Description:   This routine actives the ports plugged into the system
*****************************************************************/
int CCombinedAPISampleDlg::nActivatePorts() 
{
	CWaitCursor
		wait;

	char
		pszPortID[9];
	int
		i = 0;

	/*
	 * if we can active the ports, we then fill the port information
	 * on the main dialog
	 */
	if (pCommandHandling->nActivateAllPorts())
	{
		m_bPortsActivated = TRUE;

		m_ctlPortHandleCB.ResetContent();
		m_ctlRefHandleCB.ResetContent();
		m_ctlTrackingList.DeleteAllItems();

		m_szManufID = _T("");
		m_bPortEnabled = FALSE;
		m_bPortInitialized = FALSE;
		m_szSerialNo = _T("");
		m_szToolRev = _T("");
		m_szToolType = _T("");
		m_szPartNumber = _T("");
		UpdateData(false);

		for ( int i = NO_HANDLES; i > 0; i-- )
		{
			if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bInitialized == 1 ) 
			{
				sprintf( pszPortID, "%02X", i );
				if( pCommandHandling->m_dtHandleInformation[i].szToolType[1] != '8' )
				{
					m_ctlTrackingList.InsertItem( 0, NULL );
					m_ctlTrackingList.SetItemText( 0, 1, 
						pCommandHandling->m_dtHandleInformation[i].szPhysicalPort );

					m_ctlTrackingList.SetItemText( 0, 0, pszPortID );
				}/* if */
				m_ctlPortHandleCB.AddString( pszPortID );
				m_ctlRefHandleCB.AddString( pszPortID );
			} /* if */
		} /* for */

		m_ctlRefHandleCB.InsertString( 0, "None" );

		GetDlgItem( IDC_PORT_HANDLES )->EnableWindow( TRUE );
		GetDlgItem( IDC_REFERENCE_HANDLE_CMB )->EnableWindow( TRUE );					
		m_ctlRefHandleCB.SetCurSel(0);
		SetMode( MODE_ACTIVATED );

		if (!m_bIsTracking)
			MessageBox( "Ports successfully activated", "Port Activation", 
						MB_ICONINFORMATION|MB_SYSTEMMODAL|MB_SETFOREGROUND );
		return 1;
	} /* if */

	m_bPortsActivated = FALSE;

	MessageBox( "Ports could not be activated.\n"
	"Check your SROM image file settings and\n"
	"make sure your system is turned on and initialized.",
	"PORT ACTIVATION ERROR", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );
	SetMode( MODE_PRE_INIT );
	return 0;
} /* nActivatePorts */
/*****************************************************************
Name:				nStartTracking

Inputs:
	None.

Return Value:
	int - 1 if successful, 0 otherwise

Description:   This routine starts the System tracking
*****************************************************************/
int CCombinedAPISampleDlg::nStartTracking() 
{
	if ( pCommandHandling->nStartTracking() )
	{
		/*
		 * if we can start tracking, set the appropriate window text,
		 * start the tracking thread and set the mode.
		 */
		GetDlgItem( IDC_TRACKING_BUT )->SetWindowText("S&top Tracking");
        SetTimer(1,1000,NULL);
		AfxBeginThread( FillTrackingTable,
					  m_hWnd,
					  THREAD_PRIORITY_NORMAL,
					  0,
					  0 );

		m_bIsTracking = TRUE;
		m_bStopTracking = FALSE;

		m_szSystemMode = "System Tracking";
		GetDlgItem( IDC_TRACKING_BUT )->SetWindowText("S&top Tracking");
		GetDlgItem( IDC_PORT_ENABLED )->EnableWindow( FALSE );
		SetMode( MODE_TRACKING );
		/* EC-03-0071 */
		if (m_ctlPortHandleCB.GetCount() > 0)
		{
			m_ctlPortHandleCB.SetCurSel( 0 );
			OnSelchangePortHandles();
		}
		UpdateData(false);

		return 1;
	} /* if */

	return 0;
} /* nStartTracking */


/*****************************************************************
Name:				nStopTracking

Inputs:
	None.

Return Value:
	int - 1 if successful, 0 otherwise

Description:   
	This routine stops the tracking procedure.
*****************************************************************/
int CCombinedAPISampleDlg::nStopTracking() 
{
	if ( pCommandHandling->nStopTracking() )
	{
		/*
		 * set the variable that will stop the thread.
		 * set the text on the dialog and put program in proper mode.
		 */
		KillTimer(1);
		m_bIsTracking = FALSE;
		m_bStopTracking = TRUE;
		GetDlgItem( IDC_TRACKING_BUT )->SetWindowText("&Start Tracking");

		m_szSystemMode = "System Initialized";
		SetMode( MODE_ACTIVATED );
		GetDlgItem( IDC_TRACKING_BUT )->SetWindowText("&Start Tracking");
		if ( m_ctlPortHandleCB.GetCurSel() > -1 )
			GetDlgItem( IDC_PORT_ENABLED )->EnableWindow( TRUE );
		UpdateData(false);

		return 1;
	} /* if */

	return 0;
} /* nStopTracking */


/*****************************************************************
Name:				FillTrackingTable

Inputs:
	input LPVOID pParam - normal thread input

Return Value:
	UINT - normal thread return

Description:  
	This is the thread call that controls the collection of data

	This thread allows the user to perform other tasks
	within the program while data is being collected
*****************************************************************/
UINT FillTrackingTable( LPVOID pParam)
{
	HWND hWnd = (HWND)pParam;

	while ( !m_bStopTracking )
	{
		/*
		 * while tracking, post messages to fill the list
		 */
		if ( m_bIsTracking )
			::SendMessage( hWnd, WM_FILL_LIST, 0, 0 );
		else
			m_bStopTracking = TRUE;
	}/* while */

	/* when tracking stopped, kill the thread */
	m_bStopTracking = FALSE;
	AfxEndThread( 0, TRUE );
	return 0;
} /* FillTrackingTable */


/*****************************************************************
Name:				nGetSystemTransformData

Inputs:
	UNIT wParam and LONG lParam are normal message handling inputs

Return Value:
	LONG is the normal return for a message handling routine

Description:   
	This routine is the message handler for a WM_FILL_LIST posting.
	This routine gets the next set of transformation data and displays
	it in the main dialog.
*****************************************************************/
LONG CCombinedAPISampleDlg::nGetSystemTransformData( UINT wParam, LONG lParam )
{
	char
		pszTemp[256];		
	CString
		szCBHandle,
		szPortNo;
	int
		nRow = -1;

	Rotation 
		dtEulerRot;

	if (!m_bIsTracking)
		return 0;

	UpdateData(true);

	/* 
	 * if tracking mode is 0, we are asking for TX data, else we are
	 * asking for BX data.
	 */
	if ( m_nTrackingMode == 0 )
	{
		if ( !pCommandHandling->nGetTXTransforms( m_bUse0x0800Option ? true : false ) )
			return 0;
	} /* if */
	else if ( m_nTrackingMode == 1 )
	{
		if ( !pCommandHandling->nGetBXTransforms( m_bUse0x0800Option ? true : false ) )
			return 0;
	} /* else if */

	/* check for system flags */
	GetDlgItem(IDC_TEMPERATURE_NOTE)->SetWindowText(pCommandHandling->m_dtSystemInformation.bTemperatureOutOfRange ?
											     	"Temperature out of range" : "Temperature within range" );
	GetDlgItem(IDC_DIAGNOSTICS_PENDING)->EnableWindow(pCommandHandling->m_dtSystemInformation.bDiagnosticsPending);

	UpdateData(false);

	/* 
	 * if a new port has become occupied we do the following:
	 * 1) Stop tracking
	 * 2) Activate Ports
	 * 3) Start Tracking
	 */
	if ( pCommandHandling->m_dtSystemInformation.bPortOccupied )
	{
		if ( pCommandHandling->nStopTracking() &&
			 nActivatePorts() && 
			 pCommandHandling->nStartTracking() )
		{
			return 1;
		}/* if */

		/*
		 * We don't want the tracking thread to track if 
		 * activating the ports failed!
		 */
		m_bStopTracking = true;
		m_bIsTracking = FALSE;
		return 0;
	} /* if */

	for ( int i = 0; i < NO_HANDLES; i ++ )
	{
		if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bInitialized > 0 &&
			 pCommandHandling->m_dtHandleInformation[i].szToolType[1] != '8' )
		{
			/* only update the frame if the handle isn't disabled*/
			if ( pCommandHandling->m_dtHandleInformation[i].Xfrms.ulFlags == TRANSFORM_VALID ||
				 pCommandHandling->m_dtHandleInformation[i].Xfrms.ulFlags == TRANSFORM_MISSING )
			{
				m_szFrameNumber.Format( "%d", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.ulFrameNumber );
				UpdateData(false);
				m_ctlFrameNo.RedrawWindow();
			}/* if */

			if( i == pCommandHandling->m_nRefHandle )
				sprintf( pszTemp, "R%02X", i ); 
			else
				sprintf( pszTemp, "%02X", i ); 

			for ( int j = nRow+1; j < m_ctlTrackingList.GetItemCount(); j++ )
			{
				szPortNo = m_ctlTrackingList.GetItemText( j, 1 );
				if ( pCommandHandling->m_dtHandleInformation[i].szPhysicalPort == szPortNo )
				{
						nRow = j;
						break;
				} /* if */
				//nRow = -1;
			} /* for */

			/* fill the table */
			m_ctlTrackingList.SetItemText( nRow, 0, pszTemp );
			if ( pCommandHandling->m_dtHandleInformation[i].Xfrms.ulFlags == TRANSFORM_VALID )
			{
				sprintf( pszTemp, "%.2f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.x );
				m_ctlTrackingList.SetItemText( nRow, 2, pszTemp );
				sprintf( pszTemp, "%.2f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.y );
				m_ctlTrackingList.SetItemText( nRow, 3, pszTemp );
				sprintf( pszTemp, "%.2f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.z );
				m_ctlTrackingList.SetItemText( nRow, 4, pszTemp );
				if( !m_bUseEulerAngles )
				{
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.q0 );
					m_ctlTrackingList.SetItemText( nRow, 5, pszTemp );
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qx );
					m_ctlTrackingList.SetItemText( nRow, 6, pszTemp );
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qy );
					m_ctlTrackingList.SetItemText( nRow, 7, pszTemp );
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qz );
					m_ctlTrackingList.SetItemText( nRow, 8, pszTemp );
				}
				else
				{
					CvtQuatToEulerRotation( &pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation,
											&dtEulerRot );
					sprintf( pszTemp, "%.4f", dtEulerRot.fYaw );
					m_ctlTrackingList.SetItemText( nRow, 5, pszTemp );
					sprintf( pszTemp, "%.4f", dtEulerRot.fPitch );
					m_ctlTrackingList.SetItemText( nRow, 6, pszTemp );
					sprintf( pszTemp, "%.4f", dtEulerRot.fRoll );
					m_ctlTrackingList.SetItemText( nRow, 7, pszTemp );
					sprintf( pszTemp, "" );
					m_ctlTrackingList.SetItemText( nRow, 8, pszTemp );
				}
				sprintf( pszTemp, "%.4f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.fError );
				m_ctlTrackingList.SetItemText( nRow, 9, pszTemp );

				if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bPartiallyOutOfVolume )
					m_ctlTrackingList.SetItemText( nRow, 10, "POOV" );
				else if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bOutOfVolume )
					m_ctlTrackingList.SetItemText( nRow, 10, "OOV" );
				else
					m_ctlTrackingList.SetItemText( nRow, 10, "OK" );
			}/* if */
			else 
			{
				if ( pCommandHandling->m_dtHandleInformation[i].Xfrms.ulFlags == TRANSFORM_MISSING )
					m_ctlTrackingList.SetItemText( nRow, 2, "MISSING" );
				else
				{
					m_ctlTrackingList.SetItemText( nRow, 2, "DISABLED" );

					/* EC-03-0071 */
					if (m_ctlPortHandleCB.GetCurSel() >= 0)
					{
						m_ctlPortHandleCB.GetLBText( m_ctlPortHandleCB.GetCurSel(), szCBHandle );
						if ( !(strncmp( (LPSTR) (LPCSTR)szCBHandle, pszTemp, 2 )) &&
							 pCommandHandling->m_dtHandleInformation[i].HandleInfo.bEnabled )
						{
							/* if port has become DISABLED show it in the check box */
							m_szManufID = _T("");
							m_bPortEnabled = FALSE;
							m_bPortInitialized = FALSE;
							m_szSerialNo = _T("");
							m_szToolRev = _T("");
							m_szToolType = _T("");
							m_szPartNumber = _T("");
							UpdateData(false);
						} /* if */
					}
				} /* else */
				m_ctlTrackingList.SetItemText( nRow, 3, "---" );
				m_ctlTrackingList.SetItemText( nRow, 4, "---" );
				m_ctlTrackingList.SetItemText( nRow, 5, "---" );
				m_ctlTrackingList.SetItemText( nRow, 6, "---" );
				m_ctlTrackingList.SetItemText( nRow, 7, "---" );
				if( !m_bUseEulerAngles )
					m_ctlTrackingList.SetItemText( nRow, 8, "---" );
				else
					m_ctlTrackingList.SetItemText( nRow, 8, "" );
				m_ctlTrackingList.SetItemText( nRow, 9, "---" );
				if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bPartiallyOutOfVolume )
					m_ctlTrackingList.SetItemText( nRow, 10, "POOV" );
				else if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bOutOfVolume )
					m_ctlTrackingList.SetItemText( nRow, 10, "OOV" );
				else
					m_ctlTrackingList.SetItemText( nRow, 10, "---" );
			}/* else */					
		}/* if */
	}/* for */

	m_ctlTrackingList.RedrawItems(0, m_ctlTrackingList.GetItemCount()-1);
	m_ctlTrackingList.UpdateWindow();

	return 1;
} /* nGetSystemTransformData */


/*****************************************************************
Name:				nComPortTimeout

Inputs:
	UNIT wParam and LONG lParam are normal message handling inputs

Return Value:
	LONG is the normal return for a message handling routine

Description:   
	This routine is the message handler for a WM_COM_TIMEOUT posting.
	This routine handles what happens when a COM port timeout occurs.
*****************************************************************/
LONG CCombinedAPISampleDlg::nComPortTimeout( UINT wParam, LONG lParam )
{

	switch (wParam)
	{
		/* if close application was choosen */
		case ERROR_TIMEOUT_CLOSE:
		{
			m_bIsTracking = FALSE;
			PostMessage( WM_CLOSE, 0, 0l );
			break;
		}
		/* if restart application was selected */
		case ERROR_TIMEOUT_RESTART:
		{
			m_bIsTracking = FALSE;
			pCommandHandling->nCloseComPorts();
			m_szSystemMode = "Setup Mode";
			SetMode( MODE_PRE_INIT );
			UpdateData(false);
			break;
		}
		/* if retry was choosen */
		case ERROR_TIMEOUT_CONT:
			break;
	}; /* switch */

	return 0;
} /* nComPortTimeout */
/*****************************************************************
Name:				OnSelchangePortHandles

Inputs:
	None.

Return Value:
	None.

Description:   This routine handles the selection of port handles
			   from the port handle combo box.  It fills the form
			   with the information that pertains to the select
			   port handle.
*****************************************************************/
void CCombinedAPISampleDlg::OnSelchangePortHandles() 
{
	int
		nPortHandle = 0,
		nSelection = 0;
	CString
		szPortHandle = "";

	nSelection = m_ctlPortHandleCB.GetCurSel();
	m_ctlPortHandleCB.GetLBText( nSelection, szPortHandle );

	nPortHandle = uASCIIToHex( (LPSTR) (LPCSTR)szPortHandle, 2 );

	/* fill the form with the info that pertains to the selected handle */
	m_szManufID = pCommandHandling->m_dtHandleInformation[nPortHandle].szManufact;
	m_bPortEnabled = pCommandHandling->m_dtHandleInformation[nPortHandle].HandleInfo.bEnabled;
	m_bPortInitialized = pCommandHandling->m_dtHandleInformation[nPortHandle].HandleInfo.bInitialized;
	m_szSerialNo = pCommandHandling->m_dtHandleInformation[nPortHandle].szSerialNo;
	m_szToolRev = pCommandHandling->m_dtHandleInformation[nPortHandle].szRev;
	m_szToolType = pCommandHandling->m_dtHandleInformation[nPortHandle].szToolType;
	m_szPartNumber = pCommandHandling->m_dtHandleInformation[nPortHandle].szPartNumber;

	if (!m_bIsTracking)
		GetDlgItem( IDC_PORT_ENABLED )->EnableWindow( TRUE );

	UpdateData( false );

} /* OnSelchangePortHandles */

/*****************************************************************
Name:				OnSelchangePortHandles

Inputs:
	None.

Return Value:
	None.

Description:   This routine handles the selection of port handles
			   from the port handle combo box.  It fills the form
			   with the information that pertains to the select
			   port handle.
*****************************************************************/
void CCombinedAPISampleDlg::OnSelchangeRefPortHandle() 
{
	int
		nPortHandle = 0,
		nSelection = 0;
	CString
		szPortHandle = "";

	nSelection = m_ctlRefHandleCB.GetCurSel();
	m_ctlRefHandleCB.GetLBText( nSelection, szPortHandle );

	if( szPortHandle != "None" )
		pCommandHandling->m_nRefHandle = uASCIIToHex( (LPSTR) (LPCSTR)szPortHandle, 2 );
	else
		pCommandHandling->m_nRefHandle = -1;
} /* OnSelchangePortHandles */
/*****************************************************************
Name:				OnPortEnabled

Inputs:
	None.

Return Value:
	None.

Description:   This routine enables or disables the select port
			   handle depending on the status of the check box
*****************************************************************/
void CCombinedAPISampleDlg::OnPortEnabled() 
{
	int
		nPortHandle = 0,
		nSelection = 0;
	CString
		szPortHandle = "";

	nSelection = m_ctlPortHandleCB.GetCurSel();
	m_ctlPortHandleCB.GetLBText( nSelection, szPortHandle );

	/* get the port handle */
	nPortHandle = uASCIIToHex( (LPSTR) (LPCSTR)szPortHandle, 2 );

	UpdateData( true );

	/* 
	 * enable or disable the port depending on the status of
	 * the check box
	 */
		
	if ( !m_bPortEnabled )
			pCommandHandling->nDisablePort( nPortHandle );
	else
			pCommandHandling->nEnableOnePorts( nPortHandle );
} /* OnPortEnabled */

/*****************************************************************
Name:			OnFileExit	

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the File Menu's Exit call
*****************************************************************/
void CCombinedAPISampleDlg::OnFileExit() 
{

	CDialog::OnCancel();	
} /* OnFileExit */
/*****************************************************************
Name:			OnSettingsComportsettings	

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the Setting Menu's COM Port 
				Settings call
*****************************************************************/
void CCombinedAPISampleDlg::OnSettingsComportsettings() 
{
	CComPortSettings
		*COMPortSettings = NULL;

	COMPortSettings = new CComPortSettings;

	if ( COMPortSettings )
	{
		COMPortSettings->DoModal();
		delete( COMPortSettings );
	} /* if */
	else
	{
		MessageBox( "COM Port Settings Dialog could not be opened. "
		"\nPlease contact Northern Digital Inc. for assistance.",
		"COM Settings Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );		
	} /* else */
} /* OnSettingsComportsettings */
/*****************************************************************
Name:			OnSettingsRomfilesettings

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the Setting Menu's ROM File 
				Settings call
*****************************************************************/
void CCombinedAPISampleDlg::OnSettingsRomfilesettings() 
{
	CROMFileDlg
		*ROMFiles = NULL;

	ROMFiles = new CROMFileDlg;

	if ( ROMFiles )
	{
		/* 
		 * we need to know what system we are connect to so we need to
		 * initialize the system if not already done.
		 */
		if ( !m_bSystemInitialized )
			OnInitializeSystem();
		ROMFiles->m_nNoActivePorts = pCommandHandling->m_dtSystemInformation.nNoActivePorts;
		ROMFiles->m_nNoPassivePorts = pCommandHandling->m_dtSystemInformation.nNoPassivePorts;
		ROMFiles->m_nNoMagneticPorts = pCommandHandling->m_dtSystemInformation.nNoMagneticPorts;
		/* EC-03-0071 */
		ROMFiles->m_nTypeofSystem = pCommandHandling->m_dtSystemInformation.nTypeofSystem;
		ROMFiles->DoModal();
		delete( ROMFiles );
	} /* if */
	else
	{
		MessageBox( "SROM Image Files Dialog could not be opened. "
		"\nPlease contact Northern Digital Inc. for assistance.",
		"SROM Image Files Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );		
	} /* else */
} /* OnSettingsRomfilesettings */
/*****************************************************************
Name:			OnSystemActivateports

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the System Menu's Activate
				Ports call
*****************************************************************/
void CCombinedAPISampleDlg::OnSystemActivateports() 
{
	OnActivatePorts();	
} /* OnSystemActivateports */
/*****************************************************************
Name:			OnSystemInitializesystem

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the Systems Menu's Initialize
				System call
*****************************************************************/
void CCombinedAPISampleDlg::OnSystemInitializesystem() 
{
	OnInitializeSystem();	
} /* OnSystemInitializesystem */
/*****************************************************************
Name:			OnSystemStarttracking

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the System Menu's Start
				Tracking call
*****************************************************************/
void CCombinedAPISampleDlg::OnSystemStarttracking() 
{
	nStartTracking();
} /* OnSystemStarttracking */
/*****************************************************************
Name:			OnSystemStoptracking

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the System Menu's Stop
				Tracking call
*****************************************************************/
void CCombinedAPISampleDlg::OnSystemStoptracking() 
{
	nStopTracking();
} /* OnSystemStoptracking */
/*****************************************************************
Name:			OnViewSystemproperties

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the View Menu's System 
				Properties call
*****************************************************************/
void CCombinedAPISampleDlg::OnViewSystemproperties() 
{
	CSystemFeaturesDlg
		*SystemFeatures = NULL;

	SystemFeatures = new CSystemFeaturesDlg;

	if ( SystemFeatures )
	{
		/* if we are connected to a POLARIS set the POLARIS variables */
		pCommandHandling->nGetSystemInfo();
		if(pCommandHandling->m_dtSystemInformation.nTypeofSystem != AURORA_SYSTEM)
		{
			SystemFeatures->SetVariables( pCommandHandling->m_dtSystemInformation.szVersionInfo,
										  pCommandHandling->m_dtSystemInformation.nNoActivePorts,
										  pCommandHandling->m_dtSystemInformation.nNoPassivePorts,
										  pCommandHandling->m_dtSystemInformation.nNoActWirelessPorts,
										  pCommandHandling->m_dtSystemInformation.nNoActTIPPorts,
										  pCommandHandling->m_dtSystemInformation.nTypeofSystem );
		} /* if */
		else
		{
			/* if we are connected to a AURORA set the AURORA variables */
			SystemFeatures->SetVariables( pCommandHandling->m_dtSystemInformation.szVersionInfo,
										  pCommandHandling->m_dtSystemInformation.nNoMagneticPorts,
										  pCommandHandling->m_dtSystemInformation.nNoFGs,
										  pCommandHandling->m_dtSystemInformation.nNoFGCards,
										  pCommandHandling->m_dtSystemInformation.nTypeofSystem );
		} /* else */
		SystemFeatures->DoModal();
		delete( SystemFeatures );
	} /* if */
	else
	{
		MessageBox( "System Features Dialog could not be opened. "
		"\nPlease contact Northern Digital Inc. for assistance.",
		"System Features Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );		
	} /* else */
} /* OnViewSytemproperties */

/*****************************************************************
Name:			OnViewAlertFlags

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the View Menu's System 
				Properties call
*****************************************************************/
void CCombinedAPISampleDlg::OnViewAlertFlags() 
{
	CNewAlertFlagsDlg
		NewAlertsDlg;

	if(	pCommandHandling->nGetAlerts( FALSE ) )
	{
		NewAlertsDlg.m_dtNewAlerts = pCommandHandling->m_dtNewAlerts;
		NewAlertsDlg.DoModal();
	} /* if */
} /* OnViewAlertFlags */
/*****************************************************************
Name:			OnOptionsIlluminatorfiringrate

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the Options Menu's 
				Illuminator Activation Rate call
*****************************************************************/
void CCombinedAPISampleDlg::OnOptionsIlluminatorfiringrate() 
{
	CIlluminatorFiringRate
		*FiringRate = NULL;

	if(pCommandHandling->m_dtSystemInformation.nTypeofSystem != AURORA_SYSTEM)
	{
		FiringRate = new CIlluminatorFiringRate;

		if ( FiringRate )
		{
			FiringRate->m_nTypeOfSystem = pCommandHandling->m_dtSystemInformation.nTypeofSystem;
			FiringRate->DoModal();
			pCommandHandling->nSetFiringRate();
			delete( FiringRate );
		} /* if */
		else
		{
			MessageBox( "Illuminator Activation Rate Dialog could not be opened. "
			"\nPlease contact Northern Digital Inc. for assistance.",
			"Activation Rate Settings Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );		
		} /* else */
	}/* if */
	
} /* OnOptionsIlluminatorfiringrate */

/*****************************************************************
Name:			OnOptionsProgramoptions

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the Options Menu's Program 
				Options call
*****************************************************************/
void CCombinedAPISampleDlg::OnOptionsProgramoptions() 
{
	CProgramOptions
		*ProgramOptions = NULL;

	ProgramOptions = new CProgramOptions;

	if ( ProgramOptions )
	{
		ProgramOptions->DoModal();
		ReadINIParm( "Logging Options", "Log File Name", "", MAX_PATH, pCommandHandling->m_szLogFile );
		ReadINIParm( "Logging Options", "Log To File", "", &pCommandHandling->m_bLogToFile );
		ReadINIParm( "Logging Options", "Date Time Stamp", "", &pCommandHandling->m_bDateTimeStampFile );
		delete( ProgramOptions );
	} /* if */
	else
	{
		MessageBox( "Program Options Dialog could not be opened. "
		"\nPlease contact Northern Digital Inc. for assistance.",
		"Program Options Error", MB_ICONERROR|MB_SYSTEMMODAL|MB_SETFOREGROUND );		
	} /* else */	
} /* OnOptionsProgramoptions */
/*****************************************************************
Name:			OnHelpAboutcombinedapisample

Inputs:
	None.

Return Value:
	None.

Description:    This routine handles the Help Menu's About
				NDI CombinedAPI Sample call
*****************************************************************/
void CCombinedAPISampleDlg::OnHelpAboutcombinedapisample() 
{
	CAboutDlg
		*About = NULL;
	About = new CAboutDlg;
	About->DoModal();
	delete( About );
} /* OnHelpAboutcombinedapisample */

/*****************************************************************
Name:				SetMode

Inputs:
	int nMode - the mode to be set in to

Return Value:
	none

Description:   This routine handles the setting of the dialog
			   items depending on the mode to be placed into.
			   There are four modes:
			   MODE_PRE_INIT - the preinitialization mode, first time
			   program is opened or a reset has been called
			   MODE_INIT - the initialization mode, the system is
			   intialized
			   MODE_ACTIVATED - the ports have been activated or 
			   tracking has been stopped.
			   MODE_TRACKING - we are tracking
*****************************************************************/
void CCombinedAPISampleDlg::SetMode( int nMode ) 
{
	bool
		bTrackingMode = FALSE,
		bPreInitialization = FALSE,
		bInitialized = FALSE;

	pMainMenu = GetMenu();

	switch (nMode)
	{
		case MODE_PRE_INIT:
			{
				bPreInitialization = TRUE;
				break;
			} /* case */
		case MODE_INIT:
			{
				bInitialized = TRUE;
				bPreInitialization = FALSE;
				m_bPortsActivated = FALSE;
				break;
			} /* case */
		case MODE_ACTIVATED:
			{
				bInitialized = TRUE;
				break;
			} /* case */
		case MODE_TRACKING:
			{
				bTrackingMode = TRUE;
				break;
			} /* case */		
	} /* switch */

	if ( nMode == MODE_PRE_INIT || nMode == MODE_INIT )
	{
		m_ctlPortHandleCB.ResetContent();
		m_ctlTrackingList.DeleteAllItems();
		GetDlgItem( IDC_PORT_HANDLES )->EnableWindow( FALSE );
		GetDlgItem( IDC_REFERENCE_HANDLE_CMB )->EnableWindow( FALSE );
		GetDlgItem( IDC_PORT_ENABLED )->EnableWindow( FALSE );
		GetDlgItem( IDC_TRACKING_BUT )->SetWindowText("&Start Tracking");
		bInitialized = TRUE;				
		m_szManufID = _T("");
		m_bPortEnabled = FALSE;
		m_bPortInitialized = FALSE;
		m_szSerialNo = _T("");
		m_szToolRev = _T("");
		m_szToolType = _T("");
		m_szPartNumber = _T("");
		UpdateData(false);
	} /* if */

/* buttons */
	GetDlgItem( IDC_RESET_SYSTEM )->EnableWindow( !bTrackingMode );
	GetDlgItem( IDC_INITIALIZE_SYSTEM )->EnableWindow( !bTrackingMode );
	GetDlgItem( IDC_ACTIVATE_PORTS )->EnableWindow( !bPreInitialization && !bTrackingMode );
	GetDlgItem( IDC_TRACKING_BUT )->EnableWindow( bInitialized && !bPreInitialization && m_bPortsActivated || bTrackingMode );

/* fields */
	GetDlgItem( IDC_TRACKING_TX )->EnableWindow( bTrackingMode );
	GetDlgItem( IDC_TRACKING_BX )->EnableWindow( bTrackingMode );
	GetDlgItem( IDC_DIAGNOSTICS_PENDING )->EnableWindow( bTrackingMode );
	GetDlgItem( IDC_TRACKING_REPORT_OOV )->EnableWindow( bTrackingMode );
	GetDlgItem( IDC_EULER_ANGLES )->EnableWindow( bTrackingMode );
	
	GetDlgItem( IDC_TEMPERATURE_NOTE)->ShowWindow( bTrackingMode &&
												   (pCommandHandling->m_dtSystemInformation.nTypeofSystem == VICRA_SYSTEM ||
												    pCommandHandling->m_dtSystemInformation.nTypeofSystem == SPECTRA_SYSTEM ));
	GetDlgItem( IDC_DIAGNOSTICS_PENDING)->ShowWindow( pCommandHandling->m_dtSystemInformation.nTypeofSystem == VICRA_SYSTEM ||
												      pCommandHandling->m_dtSystemInformation.nTypeofSystem == SPECTRA_SYSTEM );

/* menu options */
	pMainMenu->EnableMenuItem( ID_SYSTEM_INITIALIZESYSTEM, bTrackingMode );
	pMainMenu->EnableMenuItem( ID_SYSTEM_ACTIVATEPORTS, bPreInitialization || bTrackingMode );
	pMainMenu->EnableMenuItem( ID_SYSTEM_STARTTRACKING, bPreInitialization || bTrackingMode || !m_bPortsActivated);
	pMainMenu->EnableMenuItem( ID_SYSTEM_STOPTRACKING, !bTrackingMode );
	pMainMenu->EnableMenuItem( ID_CSVFILELOGGING_OPTIONS, bTrackingMode );
	pMainMenu->EnableMenuItem( ID_VIEW_SYSTEMPROPERTIES, bPreInitialization || bTrackingMode );
	pMainMenu->EnableMenuItem( ID_VIEW_DIAGNOSTICALERTS, bPreInitialization || bTrackingMode ||
							   (pCommandHandling->m_dtSystemInformation.nTypeofSystem != VICRA_SYSTEM &&
							    pCommandHandling->m_dtSystemInformation.nTypeofSystem != SPECTRA_SYSTEM));
	pMainMenu->EnableMenuItem( ID_OPTIONS_ILLUMINATORFIRINGRATE, 
							   bPreInitialization || bTrackingMode ||
							   pCommandHandling->m_dtSystemInformation.nTypeofSystem == AURORA_SYSTEM );
	pMainMenu->EnableMenuItem( ID_SETTINGS_COMPORTSETTINGS, bTrackingMode );
	pMainMenu->EnableMenuItem( ID_SETTINGS_ROMFILESETTINGS, bTrackingMode );
	pMainMenu->EnableMenuItem( ID_OPTIONS_PROGRAMOPTIONS, bTrackingMode );

} /* SetMode */

/*****************************************************************
Name:			OnDiagnosticsPending

Inputs:
	None.

Return Value:
	None.

Description:    This button will bring up the diagnostics flags
				dialog to help you debug a potential problem.
*****************************************************************/
void CCombinedAPISampleDlg::OnDiagnosticsPending() 
{
	CNewAlertFlagsDlg
		NewAlertsDlg;

	if(	pCommandHandling->nGetAlerts( TRUE ) )
	{
		NewAlertsDlg.m_dtNewAlerts = pCommandHandling->m_dtNewAlerts;
		NewAlertsDlg.DoModal();
	} /* if */
}

/*****************************************************************
Name:			OnEulerAngles

Inputs:
	None.

Return Value:
	None.

Description:    This check box button will change the display from
				Euler to Quaternions and vise versa
*****************************************************************/
void CCombinedAPISampleDlg::OnEulerAngles() 
{
	LV_COLUMN
		ColumnAttribs;
	ColumnAttribs.mask = LVCF_TEXT;

	UpdateData( true );
	
	if( m_bUseEulerAngles )
	{
		ColumnAttribs.pszText = "Rx";
		m_ctlTrackingList.SetColumn( 5, &ColumnAttribs );
		ColumnAttribs.pszText = "Ry";
		m_ctlTrackingList.SetColumn( 6, &ColumnAttribs );
		ColumnAttribs.pszText = "Rz";
		m_ctlTrackingList.SetColumn( 7, &ColumnAttribs );
		ColumnAttribs.pszText = "";
		m_ctlTrackingList.SetColumn( 8, &ColumnAttribs );
	}
	else
	{
		ColumnAttribs.pszText = "Q0";
		m_ctlTrackingList.SetColumn( 5, &ColumnAttribs );
		ColumnAttribs.pszText = "Qx";
		m_ctlTrackingList.SetColumn( 6, &ColumnAttribs );
		ColumnAttribs.pszText = "Qy";
		m_ctlTrackingList.SetColumn( 7, &ColumnAttribs );
		ColumnAttribs.pszText = "Qz";
		m_ctlTrackingList.SetColumn( 8, &ColumnAttribs );
	}
}

void CCombinedAPISampleDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	int k,m,w;
    CRect rect;
	CClientDC dc(this); 
	GetClientRect(rect);//创建虚拟设备环境 
	m=rect.Height()/12;
	w=rect.Width()/30;
	k=m*11;
	CRect rect1(24,9*m,24+19*w,11*m);	
	CBrush bkbrush(HS_CROSS,RGB(0,128,0));
	 dc.SetBkColor(RGB(0,0,0));
	//CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
    dc.FillRect(rect1,&bkbrush);

	int DeviceLgcID;
	DWORD dwErrorCode;
	char strErrorMsg[256];
	USB5935_PARA_AD ADPara; // 硬件参数
 	memset(&ADPara, 0, sizeof(ADPara)); // 将参数结构初始化为确定值0(强烈建议)	
	// 预置硬件参数
	ADPara.FirstChannel		= 10; // 首通道
	ADPara.LastChannel		= 13; // 末通道
	ADPara.InputRange		= USB5935_INPUT_N10000_P10000mV;    // 模拟量输入量程范围
	ADPara.GroundingMode	= USB5935_GNDMODE_SE; // 选择接地方式为单端	
	ADPara.Gains			= USB5935_1MULT_GAINS; // 程控增益

	DeviceLgcID = 0;
	hDevice = USB5935_CreateDevice(DeviceLgcID); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		dwErrorCode = USB5935_GetLastErrorEx("USB5935_CreateDevice", strErrorMsg);
		MessageBox(strErrorMsg);
	    USB5935_ReleaseDeviceAD(hDevice);
	    USB5935_ReleaseDevice(hDevice); 
		return ;
	}
	 ChannelCount = ADPara.LastChannel - ADPara.FirstChannel +1;
	if(!USB5935_InitDeviceAD(hDevice, &ADPara))
	{
		dwErrorCode = USB5935_GetLastErrorEx("USB5935_InitDeviceAD", strErrorMsg);
		MessageBox(strErrorMsg);
		USB5935_ReleaseDeviceAD(hDevice);
	    USB5935_ReleaseDevice(hDevice); 
		return ;

	}    
	SetTimer(2,100,NULL);

	
}

void CCombinedAPISampleDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
 if(nIDEvent==1)
 {
	 char pszTemp[256];
	CString strhandle,strtx,strty,strtz,strtx1,strty1,strtz1,strq0,strqx,strqy,strqz,strerror,strstatus;
	float tx,ty,tz,tx1,ty1,tz1,tx0,ty0,tz0,tx10,ty10,tz10;
//	 RotationMatrix dtRotMatrix; 
      QuatRotation pdtQuatRot;
	Position3d   OriginalPositionPtr,RotatedPositionPtr;

	for ( int i = 1; i < NO_HANDLES; i ++ )
	{
		if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bInitialized > 0 &&
			 pCommandHandling->m_dtHandleInformation[i].szToolType[1] != '8' )
		{


			if( i == pCommandHandling->m_nRefHandle )
				sprintf( pszTemp, "R%02X", i ); 
			else
				sprintf( pszTemp, "%02X", i ); 
             strhandle=pszTemp;
			/* fill the DataBase */
		
			if ( pCommandHandling->m_dtHandleInformation[i].Xfrms.ulFlags == TRANSFORM_VALID )
			{
				sprintf( pszTemp, "%.2f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.x );
				strtx=pszTemp;
				tx=pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.x;

				sprintf( pszTemp, "%.2f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.y );
				strty=pszTemp;
				ty=pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.y;

				sprintf( pszTemp, "%.2f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.z );
				strtz=pszTemp;
				tz=pCommandHandling->m_dtHandleInformation[i].Xfrms.translation.z;
				if( !m_bUseEulerAngles )
				{
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.q0 );
					strq0=pszTemp;
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qx );
					strqx=pszTemp;
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qy );
					strqy=pszTemp;
					sprintf( pszTemp, "%.4f", 
						pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qz );
					strqz=pszTemp;

 		            pdtQuatRot.q0=(pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.q0);
                    pdtQuatRot.qx=(pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qx);
                    pdtQuatRot.qy=(pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qy);
                    pdtQuatRot.qz=(pCommandHandling->m_dtHandleInformation[i].Xfrms.rotation.qz);

                    //CvtQuatToRotationMatrix( &pdtQuatRot, dtRotMatrix );//四元素得到旋转矩阵
		              if(i==1)
					  {
/*			           tx1=tx*dtRotMatrix[0][0]+(ty+85)*dtRotMatrix[1][0]+tz*dtRotMatrix[2][0];
		               ty1=tx*dtRotMatrix[0][1]+(ty+85)*dtRotMatrix[1][1]+tz*dtRotMatrix[2][1];
		                tz1=tx*dtRotMatrix[0][2]+(ty+85)*dtRotMatrix[1][2]+tz*dtRotMatrix[2][2];*/
/*			             tx10=-0.63;ty10=13.59;tz10=-2.3;
			             tx0=-5.45;ty0=-50;tz0=10.66;
						 OriginalPositionPtr.x=tx10;
						 OriginalPositionPtr.y=ty10;
						 OriginalPositionPtr.z=tz10;
			             tx1=tx10*dtRotMatrix[0][0]+(ty10)*dtRotMatrix[1][0]+tz10*dtRotMatrix[2][0]+tx-tx0;
		                 ty1=tx10*dtRotMatrix[0][1]+(ty10)*dtRotMatrix[1][1]+tz10*dtRotMatrix[2][1]+ty-ty0;
		                 tz1=tx10*dtRotMatrix[0][2]+(ty10)*dtRotMatrix[1][2]+tz10*dtRotMatrix[2][2]+tz-tz0;*/
			             tx10=-0.63;ty10=13.59;tz10=-2.3;
			             tx0=-5.45;ty0=-50;tz0=10.66;
						 OriginalPositionPtr.x=tx10;
						 OriginalPositionPtr.y=ty10;
						 OriginalPositionPtr.z=tz10;
		                 QuatRotatePoint( &pdtQuatRot,&OriginalPositionPtr,&RotatedPositionPtr);//原始点在四元素下的旋转输出点
			             tx1=RotatedPositionPtr.x+tx+tx0;
		                 ty1=RotatedPositionPtr.y+ty+ty0;
		                 tz1=RotatedPositionPtr.z+tz+tz0;

					  }
		              else if(i==2)
					  {
/*			         tx1=tx*dtRotMatrix[0][0]+(ty+65)*dtRotMatrix[1][0]+tz*dtRotMatrix[2][0];
		             ty1=tx*dtRotMatrix[0][1]+(ty+65)*dtRotMatrix[1][1]+tz*dtRotMatrix[2][1];
		             tz1=tx*dtRotMatrix[0][2]+(ty+65)*dtRotMatrix[1][2]+tz*dtRotMatrix[2][2];*/
/*			         tx10=-3.81;ty10=22.15;tz10=-1.56;
			         tx0=-0.96;ty0=-62.47;tz0=4.13;
			         tx1=tx10*dtRotMatrix[0][0]+(ty10)*dtRotMatrix[1][0]+tz10*dtRotMatrix[2][0]+tx-tx0;
		             ty1=tx10*dtRotMatrix[0][1]+(ty10)*dtRotMatrix[1][1]+tz10*dtRotMatrix[2][1]+ty-ty0;
		             tz1=tx10*dtRotMatrix[0][2]+(ty10)*dtRotMatrix[1][2]+tz10*dtRotMatrix[2][2]+tz-tz0;*/
			        
					     tx10=-3.81;ty10=22.15;tz10=-1.56;
			             tx0=-0.96;ty0=-62.47;tz0=4.13;
						 OriginalPositionPtr.x=tx10;
						 OriginalPositionPtr.y=ty10;
						 OriginalPositionPtr.z=tz10;
		                 QuatRotatePoint( &pdtQuatRot,&OriginalPositionPtr,&RotatedPositionPtr);//原始点在四元素下的旋转输出点
			             tx1=RotatedPositionPtr.x+tx+tx0;
		                 ty1=RotatedPositionPtr.y+ty+ty0;
		                 tz1=RotatedPositionPtr.z+tz+tz0;

					  }
                     else
					 {
			          tx1=0;
		              ty1=0;
		              tz1=0;
					 }
					sprintf( pszTemp, "%.4f",tx1);
					strtx1=pszTemp;
					sprintf( pszTemp, "%.4f",ty1);
					strty1=pszTemp;
					sprintf( pszTemp, "%.4f",tz1);
					strtz1=pszTemp;

				}
				else
				{
				}
				sprintf( pszTemp, "%.4f", 
					pCommandHandling->m_dtHandleInformation[i].Xfrms.fError );
				strerror=pszTemp;

				if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bPartiallyOutOfVolume )
					
				strstatus="POOV";
				else if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bOutOfVolume )		
				strstatus="OOV";
				else
				strstatus="OK";
 	           strSQL.Format("INSERT INTO Table1(Handle,Status,Tx,Ty,Tz,Error) VALUES('%s','%s','%s','%s','%s','%s')",strhandle+'0',strstatus,strtx,strty,strtz,strerror);
               m_database.ExecuteSQL(strSQL);//第一个点	（为了跟MATLAB程序要读的数一致）  

	           strSQL.Format("INSERT INTO Table1(Handle,Status,Tx,Ty,Tz,Error) VALUES('%s','%s','%s','%s','%s','%s')",strhandle+'1',strstatus,strtx1,strty1,strtz1,strerror);

	           m_database.ExecuteSQL(strSQL);//同一个TOOL上的第二个点（为了跟MATLAB程序要读的数一致）

			}/* if */
			else 
			{
							if( i == pCommandHandling->m_nRefHandle )
				sprintf( pszTemp, "R%02X", i ); 
			 else
				sprintf( pszTemp, "%02X", i ); 
             strhandle=pszTemp;
				if ( pCommandHandling->m_dtHandleInformation[i].Xfrms.ulFlags == TRANSFORM_MISSING )
					strtx= "MISSING" ;
				else
				{
					strtx="DISABLED" ;

				} /* else */
				strty= "---";
				strtz= "---";
				strq0="---";
				strqx="---";
				strqy="---";
				if( !m_bUseEulerAngles )
					strqz= "---";
				else
					strqz= "";
				strerror="---";
				if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bPartiallyOutOfVolume )
					strstatus="POOV";
				else if ( pCommandHandling->m_dtHandleInformation[i].HandleInfo.bOutOfVolume )
					strstatus="OOV";
				else
					strstatus="---";
			}/* else */					
		}/* if */


	}/* for */
 }
 if(nIDEvent==2)                                                    
 {
	int k,m,w,i=1;
    static int x=1;
	 static float y=0.0;
    float y1=0,y2=0,y3=0,y4=0;
    char buffer[34];
    CRect rect;
	CClientDC dc(this); 
	GetClientRect(rect);//创建虚拟设备环境 
	m=rect.Height()/12;
	w=rect.Width()/30;
	k=m*11;
	CRect rect1(24,9*m,24+19*w,11*m);
	CRect rect2(0,0,19*w,2*m);
	CBrush bkbrush(HS_CROSS,RGB(0,128,0));
    //CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
     if(!m_dc.m_hDC)
	 {
       m_dc.CreateCompatibleDC ( &dc ) ;	 
		CBitmap bitmap;		      
	   bitmap.CreateCompatibleBitmap(&dc,18*w,2*m);
	   m_dc.SelectObject(&bitmap);
	   m_dc.SetBkColor(RGB(0,0,0));
      m_dc.FillRect(rect2,&bkbrush);	  
	 }	
CPen pen1(PS_SOLID,0,RGB(255,0,8));

CPen *oldpen1=NULL;

oldpen1=dc.SelectObject(&pen1);
									//移动画笔到点(24,k)
	MSG msg;
    USHORT ADBuffer[8192]; // 分配缓冲区(存储原始数据)
	
	
	DWORD dwErrorCode;
	char strErrorMsg[256];

	LONG nReadSizeWords;	// 每次读取AD数据的长度(字)
	LONG nRetSizeWords;
//	LONG InputRange=0;
//	int nADChannel = 0;
	USHORT ADData=0,ADData2=0,ADData3=0,ADData4=0;	
	float fVolt=0,fVolt2=0,fVolt3=0,fVolt4=0;
    CString showstr,showstr2,showstr3,showstr4;
	nReadSizeWords = 4096 - 4096 %( ChannelCount * 256 ); // 将数据长度字转换为双字

	  	if(!USB5935_ReadDeviceAD(hDevice, ADBuffer, nReadSizeWords, &nRetSizeWords))
		{
			dwErrorCode = USB5935_GetLastErrorEx("USB5935_ReadDeviceAD", strErrorMsg);
			MessageBox(strErrorMsg);
	     	USB5935_ReleaseDeviceAD(hDevice);
        	USB5935_ReleaseDevice(hDevice); 
		}

    	ADData = ADBuffer[0]&0x1FFF;//位移数据
	    fVolt = (float)((20000.00/8192) * ADData)-10000;
	     	y1=0.5+0.04*fVolt-0.79;
		      

		ADData2 = ADBuffer[1]&0x1FFF;//下压力传感器数据
	    fVolt2 = (float)((20000.00/8192) * ADData2)-10000;
            //y2=-3.82+0.14*fVolt2+3.82;
		    // y2=-3.3+0.146*fVolt2;
		       y2=0.47+1.262*(int(fVolt2/10))+0.0011*(int(fVolt2/10))*(int(fVolt2/10));//误差5N以内
			//y2=fVolt2;
	
		ADData3 = ADBuffer[2]&0x1FFF;//角度数据
	    fVolt3 = (float)((20000.00/8192) * ADData3)-10000;
		    //  if(fVolt3<=550)
            y3=38-0.07*fVolt3;//逆时针为正；顺时针为负；垂直时为0此时电压550mv
		//	  else
           // y3=-38+0.07*fVolt3;
		ADData4 = ADBuffer[3]&0x1FFF;//上压力传感器数据
	    fVolt4 = (float)((20000.00/8192) * ADData4)-10000;
		     y4=0.47+1.262*(int(fVolt4/10))+0.0011*(int(fVolt4/10))*(int(fVolt4/10));//误差5N以内
	
			showstr.Format("%fmm",y1); //位移mm格式化为字符串
			showstr2.Format("%fN",y2); //压力格式化为字符串
			showstr3.Format("%f度",y3); //角度格式化为字符串
			showstr4.Format("%fN",y4); //角度格式化为字符串
			GetDlgItem(IDC_displacingSTATIC)->SetWindowText(showstr); //显示位移瞬时值
			GetDlgItem(IDC_StressSTATIC2)->SetWindowText(showstr2); //显示压力瞬时值
			GetDlgItem(IDC_STATIC_angle)->SetWindowText(showstr3); //显示角度瞬时值
			GetDlgItem(IDC_StressSTATICup)->SetWindowText(showstr4); //显示上压力瞬时值

		strSQL.Format("INSERT INTO Table2(Pressure,distance,angle,Pressureup) VALUES('%f','%f','%f','%f')",y2,y1,y3,y4);//保存到表2
        m_database.ExecuteSQL(strSQL);	  
	 
		//	GetDlgItem(IDC_EDIT1)->SetWindowText(showstr);//这句不行，虽然有updatedata，但只在最后变值
				//	UpdateData(FALSE);
	//	dc.TextOut(24+20*w,(10+1)*m,showstr);//可以
		if(x<=19)
		{
	       
		  dc.MoveTo(24+(x-1)*w,k-(y/500)*2*m);
          y=y2;
          dc.LineTo(24+x*w,k-(y/500)*2*m); 

		}
		else
		{
	

          m_dc.BitBlt(0,0,18*w,2*m,&dc,24+w,9*m,SRCCOPY);
          dc.SetBkColor(RGB(0,0,0));
          dc.FillRect(rect1,&bkbrush);
		  dc.BitBlt(24,9*m,18*w,2*m,&m_dc,0,0,SRCCOPY);
          dc.MoveTo(24+18*w,k-(y/500)*2*m);
           y=y2;
          dc.LineTo(24+19*w,k-(y/500)*2*m);
		}
		x++;
     dc.SelectObject(oldpen1);
     m_dc.DeleteDC();

 }

	CDialog::OnTimer(nIDEvent);
}

void CCombinedAPISampleDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here

}
CString CCombinedAPISampleDlg::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;
	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";
	// 检索已安装的驱动是否有Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//发现 !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');
	return sDriver;
}

