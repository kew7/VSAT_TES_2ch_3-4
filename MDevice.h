// Mirage.h: interface for the CMirage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIRAGE_H__A6307BB7_0FB1_46BC_A1CD_1B222F4742E1__INCLUDED_)
#define AFX_MIRAGE_H__A6307BB7_0FB1_46BC_A1CD_1B222F4742E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEVICE_NOT_CREATED	1
#define DEVICE_NOT_LOADED	2
#define DEVICE_NOT_EVENT	3

// Стандартные IOCTL-Коды для драйверов (ввод/вывод по прерываниям)
#define IOCTL_GetData			0x00222043
#define IOCTL_GetBlock			0x00222083
#define IOCTL_GetDataCount  	0x002220c3
#define IOCTL_FlushData			0x00222103
#define IOCTL_LoadUnit			0x00222140
#define IOCTL_Reset				0x00222180
#define IOCTL_Check				0x002221c0
#define IOCTL_SetAckn			0x00222203
#define IOCTL_PIPE              0x00222243 

#define PIPE_INIT				10
#define PIPE_TEST				20
#define PIPE_GET_MESSAGE		30
#define PIPE_MESSAGE_NOMESSAGE	100
#define PIPE_MESSAGE_OVERFLOW	110
#define PIPE_MESSAGE_OK			120
#define PIPE_MESSAGE_CANCEL		130
#define PIPE_MESSAGE_DATA_READY	140

#define MES_COUNT (WM_USER+1)

DWORD WINAPI ThreadFunc( void* Mir);

class CMDevice  
{
public:
	void SetBoardAttribute();
	int LengthUsBuf;
	BOOL WorkFlag;
	BOOL OpenTread();
	DWORD GetDataCount(int);
	BOOL Create(int DeviceNumber,HWND pParentWnd);

	HWND	inhwnd;
	HANDLE m_hEvent;
	HANDLE m_hDevice;
	HANDLE hTHR;

	DWORD GetBlock(BYTE*pBuf,DWORD BufSize);
	DWORD GetData(BYTE* pBuf,DWORD BufSize);
	BOOL LoadLCA(CString DumpFileName);
	BOOL LoadDSP(CString DumpFileName);
	void FlushData(int DataChanel);
	CMDevice();
	~CMDevice();

private:
	ULONG InBuf[2],OutBuf[2];

};

typedef CMDevice*pClass;

#endif // !defined(AFX_MIRAGE_H__A6307BB7_0FB1_46BC_A1CD_1B222F4742E1__INCLUDED_)
