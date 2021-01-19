// Mirage.cpp: implementation of the CMirage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Inputer.h"
#include "MDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMDevice::CMDevice()
{

}

CMDevice::~CMDevice()
{
	CloseHandle(m_hDevice);
	WorkFlag=FALSE;
	CloseHandle(hTHR);
}


BOOL CMDevice::Create(int DeviceNumber,HWND pParentWnd)
{
	inhwnd = pParentWnd;
  	CString DeviceName;
	CString strDeviceNumber;
	strDeviceNumber.Format("%d",DeviceNumber);
	DeviceName="//./MirageDevice";
	DeviceName+=strDeviceNumber;

	m_hDevice=CreateFile(DeviceName,GENERIC_READ|GENERIC_WRITE,
		                 0,NULL,
					     OPEN_EXISTING,
					     FILE_ATTRIBUTE_NORMAL,
					     NULL);
	if (m_hDevice==INVALID_HANDLE_VALUE) 
	{
		MessageBox(NULL,"Check the Mirage driver installation",NULL,MB_OK|MB_ICONERROR);
		exit(0);
	}

    if(!LoadLCA("yyy.bit")) {::MessageBox(NULL,"Устройство не отвечает на загрузку LCA","Очень плохая новость",MB_OK|MB_ICONERROR); return FALSE;}
	if(!LoadDSP("yyy.bin"))     {::MessageBox(NULL,"Устройство не отвечает на загрузку DSP","Очень плохая новость",MB_OK|MB_ICONERROR); return FALSE;}

    if(GetDataCount(0)==0) {
//		::MessageBox(NULL,"Нет сигнала на входе!!!",NULL,NULL);
		return FALSE;
	}

    return OpenTread(); // функция создания потока
	
  // return TRUE;    
}


BOOL CMDevice::LoadDSP(CString DumpFileName)
{
	// ~~~~~~~~~~~~ bit file mapping ~~~~~~~~~~~~~~~~~
	// Файл загрузки должен содержать правильный заголовок (смотри спецификацию драйвера)
	DWORD FileSize;
	PBYTE pBin=NULL;
	HANDLE hMapping=INVALID_HANDLE_VALUE;
	HANDLE hFile=CreateFile(LPCSTR(DumpFileName),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)	{::MessageBox(NULL,"Невозможно открыть файл загрузочного дампа DSP","Очень плохая новость",MB_OK|MB_ICONERROR);	return FALSE;}
	FileSize=GetFileSize(hFile,NULL);
	if(FileSize>20480){
		CloseHandle(hFile);
		return FALSE;
	}
	hMapping=CreateFileMapping(hFile,NULL,PAGE_WRITECOPY,0,0,NULL);
	CloseHandle(hFile);
	if(hMapping==INVALID_HANDLE_VALUE)	return FALSE;
	pBin=(PBYTE)MapViewOfFile(hMapping,FILE_MAP_COPY,0,0,0);
	if(!pBin){
		CloseHandle(hMapping);
		return FALSE;
	}
	//~~~~~~~~~~~~~~~~ bin file in memory ~~~~~~~~~~~~~
	DWORD Returned;
	return DeviceIoControl(m_hDevice,IOCTL_LoadUnit,
						  pBin,FileSize,
						  NULL,0,
						  &Returned,NULL);

}


BOOL CMDevice::LoadLCA(CString DumpFileName)
{
	// ~~~~~~~~~~~~ bit file mapping ~~~~~~~~~~~~~~~~~
	// Файл загрузки должен содержать правильный заголовок (смотри спецификацию драйвера)
	DWORD FileSize;
	PBYTE pBin=NULL;
	HANDLE hMapping=INVALID_HANDLE_VALUE;
	HANDLE hFile=CreateFile(LPCSTR(DumpFileName),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE) { 	::MessageBox(NULL,"Невозможно открыть файл загрузочного дампа LCA ","Очень плохая новость",MB_OK|MB_ICONERROR);	return FALSE;}
	FileSize=GetFileSize(hFile,NULL);
	if(FileSize>20480){
		CloseHandle(hFile);
		return FALSE;
	}
	hMapping=CreateFileMapping(hFile,NULL,PAGE_WRITECOPY,0,0,NULL);
	CloseHandle(hFile);
	if(hMapping==INVALID_HANDLE_VALUE)	return FALSE;
	pBin=(PBYTE)MapViewOfFile(hMapping,FILE_MAP_COPY,0,0,0);
	if(!pBin){
		CloseHandle(hMapping);
		return FALSE;
	}
	//~~~~~~~~~~~~~~~~ bit file in memory ~~~~~~~~~~~~~
	DWORD Returned;
	return DeviceIoControl(m_hDevice,IOCTL_LoadUnit,
					pBin,FileSize,
					NULL,0,
					&Returned,NULL);

}

DWORD CMDevice::GetData(BYTE *pBuf, DWORD BufSize)
{
	DWORD Returned=0;
	InBuf[0]=0;
	DeviceIoControl(m_hDevice,IOCTL_GetData,
					InBuf,4,
					pBuf,BufSize,
					&Returned,NULL);
	return Returned;
}

DWORD CMDevice::GetBlock(BYTE*pBuf, DWORD BufSize)
{
	DWORD Returned;
	InBuf[0]=0;
	DeviceIoControl(m_hDevice,IOCTL_GetBlock,InBuf,4,pBuf,BufSize,	&Returned,NULL);
	return Returned;
}

DWORD CMDevice::GetDataCount(int DataChanel)
{	
	ULONG Returned;
	InBuf[0]=DataChanel;
	Sleep(150);
	DeviceIoControl(m_hDevice,IOCTL_GetDataCount,InBuf,4,OutBuf,4,&Returned,NULL);
	return OutBuf[0];
}


BOOL CMDevice::OpenTread()
{
	Sleep(1); WorkFlag=TRUE;
	if(hTHR=CreateThread(NULL, 0,ThreadFunc,(void*)this, 0, NULL))
	{	SetThreadPriority( hTHR, THREAD_PRIORITY_TIME_CRITICAL);
//    SetBoardAttribute();
	return TRUE;
	}
	return FALSE;
}

UINT LL=7800;
DWORD WINAPI ThreadFunc( void* Mir)
{
	WPARAM re=0;
	LPARAM nn=0;
	pClass md = (pClass)Mir;
	nn=md->LengthUsBuf;
	md->FlushData(0);    Sleep(200);
	while(md->WorkFlag)
	{	re=md->GetDataCount(0);
		if	((UINT)re > LL)
		{	::SendMessage(md->inhwnd, MES_COUNT, re, (LPARAM)nn);
		Sleep(1); 
		}
		else	Sleep(1);
	}
	md->hTHR=0;	
	return 0;
}	

void CMDevice::FlushData(int DataChanel)
{
	ULONG Returned;
	InBuf[0]=DataChanel;
	DeviceIoControl(m_hDevice,IOCTL_FlushData,InBuf,4,NULL,0,&Returned,NULL);
}

void CMDevice::SetBoardAttribute()
{
	ULONG Written;
	static UCHAR ControlByte=0;
	typedef struct ParcelTag{
		ULONG Port 			:8;
		ULONG DataChannel	:5;
		ULONG Width			:3;
		ULONG Delay 		:15;
		ULONG ReadWrite		:1;
		ULONG Reserv;
		ULONG Value;
	}PortParcel;
	PortParcel Shablon[2];
	int i;

	ControlByte|=0x02;		
//	ControlByte|=0x08;		
	
	for(i=0;i<2;i++){
		Shablon[i].Port=4;
		Shablon[i].DataChannel=0;
		Shablon[i].Width=2;
		Shablon[i].Delay=0;
		Shablon[i].ReadWrite=1;
		Shablon[i].Reserv=0;
		Shablon[i].Value=ControlByte;
	}
	WriteFile(m_hDevice,Shablon,24,&Written,NULL);	
}




