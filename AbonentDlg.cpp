// Mirag_VvodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mirag_Vvod.h"
#include "AbonentDlg.h"
#include "MDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMirag_VvodDlg dialog

CAbonentDlg::CAbonentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAbonentDlg::IDD, pParent)
{
//	memset((LPVOID)&CTR, 0,sizeof(SCTRL));
	//{{AFX_DATA_INIT(CMirag_VvodDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAbonentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMirag_VvodDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAbonentDlg, CDialog)
	//{{AFX_MSG_MAP(CMirag_VvodDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_B_Pusk, OnBPusk)
	ON_MESSAGE(MES_COUNT, OnMesCount)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_B_Stop, OnBStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMirag_VvodDlg message handlers

BOOL CAbonentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAbonentDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAbonentDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


LRESULT CAbonentDlg::OnMesCount(WPARAM def, LPARAM Block ) 
{
if(flWork==TRUE)
{
BYTE buf[8192];
int len_block,i;

static BOOL SynFlag1=FALSE,SynFlag2=FALSE;

while(1)
{
len_block=Mirag.GetBlock(buf,sizeof(buf));   //вызываем функцию GetBlock,чтобы
                                            // размер буфера канала был кратен 8-ми
                                            // в этом случае не нужно запоминать состояние декодера
if(len_block==sizeof(buf)) break; else Sleep(1);        //задержка для заполнения буфера

}

Ocean.buf_in1=new BYTE[len_block>>2];
Ocean.buf_in2=new BYTE[len_block>>2];
Ocean.buf_out=new BYTE[len_block>>2];
Ocean.buf_temp=new BYTE[len_block>>2];
Ocean.buf_deper=new BYTE[len_block>>2];
Ocean.buf_dec=new BYTE[len_block>>2];
Ocean.buf_dif=new BYTE[len_block>>2];
Ocean.buf_dskr=new BYTE[len_block>>2];

//  Ocean.fout.Write(buf,len_block);
  SetDlgItemInt(IDC_SIZ_BBUF,len_block);

  if(Ocean.Demultiplex(buf,len_block))
  if(Ocean.SizeBuf_kan>400)
  {
    SetDlgItemInt(IDC_SIZE_BUF,Ocean.SizeBuf_kan);

    if(SynFlag1==FALSE)  // если нет синхронизации в первом канале
	{
	    for(i=0;i<48;i++) 
        if((Ocean.massiv[i]=new BYTE[400])==NULL) 
		{
    	  ::MessageBox(NULL,"Не хватает памяти!!!",NULL,MB_OK);
		    OnBStop();
			return 1;
		}	

	   if(Ocean.synchro_search(Ocean.buf_in1,400,0)==TRUE)
	   {
          
          for(i=0;i<48;i++) delete [] Ocean.massiv[i];

		  SynFlag1=TRUE;                  //устанавливаем флаг синхронизации в единицу

          if(Ocean.Obrabotka(Ocean.buf_in1,Ocean.SizeBuf_kan,0)) BytesDone1+=Ocean.SizeBuf_kan*3/4;		
		  SetDlgItemText(IDC_SYN1,"OK"); 
		  switch (Ocean.turn_faza)
		  {
		    case 0:{ SetDlgItemText(IDC_PHASA1,"0"); break; }
		    case 1:{ SetDlgItemText(IDC_PHASA1,"90"); break; }
		    case 2:{ SetDlgItemText(IDC_PHASA1,"180"); break; }           
		    case 3:{ SetDlgItemText(IDC_PHASA1,"270"); break; }
		  }
          SetDlgItemInt(IDC_SDWIG1, Ocean.sdwig);
		  SetDlgItemInt(IDC_PQ1, Ocean.reg_decode);
		  SetDlgItemInt(IDC_S_DYBYT1, Ocean.prev_shift);
	   } 
       else
	   {
//		  Ocean.Num_kan=2;
		  for(i=0;i<48;i++) delete [] Ocean.massiv[i];
          SetDlgItemText(IDC_SYN1,"NO"); 
	   }

	}
	else                                  // если есть синхронизация
	{
		if(Ocean.Obrabotka(Ocean.buf_in1,Ocean.SizeBuf_kan,0)==TRUE)
		{
           SetDlgItemText(IDC_SYN1,"OK"); 
		   BytesDone1+=Ocean.SizeBuf_kan*3/4;		   
        }
		else 
		{
//        Ocean.Num_kan=2;
          SetDlgItemText(IDC_SYN1,"NO");  
    	  SynFlag1=FALSE;
		}
	}
    
	if(SynFlag2==FALSE)  // если нет синхронизации в первом канале
	{
	    for(i=0;i<48;i++) 
        if((Ocean.massiv[i]=new BYTE[400])==NULL) 
		{
    	  ::MessageBox(NULL,"Не хватает памяти!!!",NULL,MB_OK);
		    OnBStop();
			return 1;
		}	

	   if(Ocean.synchro_search(Ocean.buf_in2,400,1)==TRUE)
	   {
          
          for(i=0;i<48;i++) delete [] Ocean.massiv[i];

		  SynFlag2=TRUE;                  //устанавливаем флаг синхронизации в единицу

          if(Ocean.Obrabotka(Ocean.buf_in2,Ocean.SizeBuf_kan,1)) BytesDone2+=Ocean.SizeBuf_kan*3/4;		;		
		  SetDlgItemText(IDC_SYN2,"OK"); 
		  switch (Ocean.turn_faza)
		  {
		    case 0:{ SetDlgItemText(IDC_PHASA2,"0"); break; }
		    case 1:{ SetDlgItemText(IDC_PHASA2,"90"); break; }
		    case 2:{ SetDlgItemText(IDC_PHASA2,"180"); break; }           
		    case 3:{ SetDlgItemText(IDC_PHASA2,"270"); break; }
		  }
          SetDlgItemInt(IDC_SDWIG2, Ocean.sdwig);
		  SetDlgItemInt(IDC_PQ2, Ocean.reg_decode);
		  SetDlgItemInt(IDC_S_DYBYT2, Ocean.prev_shift);
	   } 
       else
	   {
//		  Ocean.Num_kan=2;
		  for(i=0;i<48;i++) delete [] Ocean.massiv[i];
          SetDlgItemText(IDC_SYN2,"NO"); 
	   }

	}
	else                                  // если есть синхронизация
	{
		if(Ocean.Obrabotka(Ocean.buf_in2,Ocean.SizeBuf_kan,1)==TRUE)
		{
           SetDlgItemText(IDC_SYN2,"OK"); 
		   BytesDone2+=Ocean.SizeBuf_kan*3/4;		
		}
		else 
		{
  //        Ocean.Num_kan=2;
          SetDlgItemText(IDC_SYN2,"NO");  
          SynFlag2=FALSE;
		}
	}

  }

delete [] Ocean.buf_in1;
delete [] Ocean.buf_in2;
delete [] Ocean.buf_out;
delete [] Ocean.buf_temp;
delete [] Ocean.buf_deper;
delete [] Ocean.buf_dec;
delete [] Ocean.buf_dif;
delete [] Ocean.buf_dskr;
//delete [] buf;
}
return 0;
}


void CAbonentDlg::OnBPusk() 
{
	
	// TODO: Add your control notification handler code here

    flWork=TRUE;    
    UpdateData(TRUE);
    BytesDone1=0 ;
	BytesDone2=0 ;
	SetTimer(1, 100, NULL);
 	GetDlgItem(ID_B_Pusk)->EnableWindow(FALSE);
	GetDlgItem(ID_B_Stop)->EnableWindow(TRUE);
	UpdateData(TRUE);

    Ocean.fout.Open("fileout.dat",CFile::modeCreate|CFile::modeWrite);
	Ocean.fout1.Open("file1.728",CFile::modeCreate|CFile::modeWrite);
    Ocean.fout2.Open("file2.728",CFile::modeCreate|CFile::modeWrite);

  if(!Mirag.Create(0, this->m_hWnd))
		{	
		OnBStop();
	    return;
		}
	
}





void CAbonentDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    SetDlgItemInt(IDC_ISDONE1, BytesDone1/1024);
	SetDlgItemInt(IDC_ISDONE2, BytesDone2/1024);

	static int t=0;     
	switch(t%10)
	{
	case 0:{	SetDlgItemText(IDC_IND, "|"); break;}
	case 1:{    SetDlgItemText(IDC_IND, "||||"); break;}
	case 2:{    SetDlgItemText(IDC_IND, "||||||| "); break;}
	case 3:{    SetDlgItemText(IDC_IND, "||||||||||"); break;}
	case 4:{    SetDlgItemText(IDC_IND, "|||||||||||||"); break;}
    case 5:{	SetDlgItemText(IDC_IND, "||||||||||||||||"); break;}
	case 6:{    SetDlgItemText(IDC_IND, "|||||||||||||||||||"); break;}
	case 7:{    SetDlgItemText(IDC_IND, "||||||||||||||||||||||"); break;}
	case 8:{    SetDlgItemText(IDC_IND, "|||||||||||||||||||||||||"); break;}
	case 9:{    SetDlgItemText(IDC_IND, "||||||||||||||||||||||||||||"); break;}
	}	t++;
	
	CDialog::OnTimer(nIDEvent);
}



void CAbonentDlg::OnBStop() 
{
	// TODO: Add your control notification handler code here
	flWork=FALSE;
    KillTimer(1);
    Mirag.~CMDevice();
    Ocean.fout1.Close();
	Ocean.fout2.Close();
	Ocean.fout.Close();
	Ocean.Num_kan=2;
	GetDlgItem(ID_B_Pusk)->EnableWindow(TRUE);
	GetDlgItem(ID_B_Stop)->EnableWindow(FALSE);
	SetDlgItemText(IDC_SYN1,""); 
	SetDlgItemText(IDC_SYN2,""); 
	
}


