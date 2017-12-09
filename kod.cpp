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

    hDevInfo = SetupDiGetClassDevs			//получение списка устройств принадлежащих определенному классу 
		(NULL,								//класс устройств для перечисления ( 0 - все устройства)
        REGSTR_KEY_PCIENUM,					//параметр для выборки устройств 
        0,									//дескриптор окна из процесса которого вызывается эта функция
        DIGCF_PRESENT | DIGCF_ALLCLASSES );	//флаги (все устр, установленные сейчас всистеме и устр на даный момент присутсвующие в системе)
       
    if (hDevInfo == INVALID_HANDLE_VALUE)	
    {
        return 1;
    }
       
       
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i, &DeviceInfoData);i++)					//получение следующего устройства
    {
        DWORD DataT;
        LPTSTR buffer = NULL;
        DWORD buffersize = 0;
        
		
		while (!SetupDiGetDeviceRegistryProperty(				//получение из реестра указанное свойство
            hDevInfo,											//дескриптор списка
            &DeviceInfoData,									//указатель на структру
			SPDRP_HARDWAREID,									//тип информации которую нужно получить ()
            &DataT,												
            (PBYTE)buffer,										
            buffersize,											
            &buffersize))										
        {
            if (GetLastError() ==							//если нехватает размера буфера
                ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer) LocalFree(buffer);				//освобождение памяти
                buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);		//увеличение объема буфера
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

        while (!SetupDiGetDeviceRegistryProperty(				//получение из реестра указанное свойство (описание устойства)
            hDevInfo,
            &DeviceInfoData,
			SPDRP_DEVICEDESC,
            &DataT,
            (PBYTE)buffer,
            buffersize,
            &buffersize))
        {
            if (GetLastError() ==							//если нехватает размера буфера
                ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer) LocalFree(buffer);				//освобождение памяти
                buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);		//увеличение объема буфера
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


		while (!SetupDiGetDeviceRegistryProperty(			//получение из реестра указанное свойство (производитель)
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
       
    SetupDiDestroyDeviceInfoList(hDevInfo);					//очистить список устройств
       
    return 0;
}