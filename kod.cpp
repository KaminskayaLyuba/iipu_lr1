#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
using namespace std;
#pragma comment (lib, "Setupapi.lib")
int main( int argc, char *argv[ ], char *envp[ ] )
{
    HDEVINFO hDevInfo;					
    SP_DEVINFO_DATA DeviceInfoData;		
    DWORD i;

    hDevInfo = SetupDiGetClassDevs			//��������� ������ ��������� ������������� ������������� ������ 
		(NULL,								//����� ��������� ��� ������������ ( 0 - ��� ����������)
        REGSTR_KEY_PCIENUM,					//�������� ��� ������� ��������� 
        0,									//���������� ���� �� �������� �������� ���������� ��� �������
        DIGCF_PRESENT | DIGCF_ALLCLASSES );	//����� (��� ����, ������������� ������ �������� � ���� �� ����� ������ ������������� � �������)
       
    if (hDevInfo == INVALID_HANDLE_VALUE)	
    {
        return 1;
    }
       
       
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i, &DeviceInfoData);i++)					//��������� ���������� ����������
    {
        DWORD DataT;
        LPTSTR buffer = NULL;
        DWORD buffersize = 0;
        
		
		while (!SetupDiGetDeviceRegistryProperty(				//��������� �� ������� ��������� ��������
            hDevInfo,											//���������� ������
            &DeviceInfoData,									//��������� �� ��������
			SPDRP_HARDWAREID,									//��� ���������� ������� ����� �������� ()
            &DataT,												
            (PBYTE)buffer,										
            buffersize,											
            &buffersize))										
        {
            if (GetLastError() ==							//���� ��������� ������� ������
                ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer) LocalFree(buffer);				//������������ ������
                buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);		//���������� ������ ������
            }
            else
            {
                break;
            }
        }
           
		int p=-1;
		printf("\nResult(ven/dev):");
		while (buffer[p++])
			printf("%c",buffer[p]);

        while (!SetupDiGetDeviceRegistryProperty(				//��������� �� ������� ��������� �������� (�������� ���������)
            hDevInfo,
            &DeviceInfoData,
			SPDRP_DEVICEDESC,
            &DataT,
            (PBYTE)buffer,
            buffersize,
            &buffersize))
        {
            if (GetLastError() ==							//���� ��������� ������� ������
                ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer) LocalFree(buffer);				//������������ ������
                buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);		//���������� ������ ������
            }
            else
            {
                break;
            }
        }
           
		p=-1;
		printf("\nResult(Device):");
		while (buffer[p++])
			printf("%c",buffer[p]);
        //printf("\n");
        //if (buffer) LocalFree(buffer);


		while (!SetupDiGetDeviceRegistryProperty(			//��������� �� ������� ��������� �������� (�������������)
            hDevInfo,
            &DeviceInfoData,
			SPDRP_MFG,
            &DataT,
            (PBYTE)buffer,
            buffersize,
            &buffersize))
        {
            if (GetLastError() == 
                ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer) LocalFree(buffer);
                buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);
            }
            else
            {
                break;
            }
        }
           
		p=-1;
		printf("\nResult(Vendor):");
		while (buffer[p++])
			printf("%c",buffer[p]);
        printf("\n");
        if (buffer) LocalFree(buffer);
    }

    if ( GetLastError()!=NO_ERROR &&
        GetLastError()!=ERROR_NO_MORE_ITEMS )
    {
        return 1;
    }
       
    SetupDiDestroyDeviceInfoList(hDevInfo);					//�������� ������ ���������
       
    return 0;
}