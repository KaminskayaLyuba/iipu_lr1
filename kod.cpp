#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
using namespace std;
#pragma comment (lib, "Setupapi.lib")
void printInfoFromHID(FILE* in, LPTSTR HID)
{
	char* input = (char*)calloc(150, sizeof(char));
	while (!feof(in))
	{
		fgets(input, 150, in);
		notDevice:
		
		if (input[0] == '#') continue;		
		if (HID[8] == input[0] && HID[9] == input[1] && HID[10] == input[2] && HID[11] == input[3])
		{
			printf("\n");
			puts(input);
			do
			{
				fgets(input,150,in);
				notSubsys:
				if (input[0] == '\t')
				{
					if (HID[17] == input[1] && HID[18] == input[2] && HID[19] == input[3] && HID[20] == input[4])
					{
						printf("\n");
						puts(input);
						do
						{
							fgets(input, 150, in);
							if (input[0] == '\t'&&input[1] == '\t')
							{
								if (HID[29] == input[2] && HID[30] == input[3] && HID[31] == input[4] && HID[32] == input[5] &&
									HID[33] == input[7] && HID[34] == input[8] && HID[35] == input[9] && HID[36] == input[10])
								{
									printf("\n");
									puts(input);
								}
							}
							else goto notSubsys;
						}
						while (input[0] == '\t'&&input[1] == '\t');
					}
				}
				else goto notDevice;

			} while (input[0]=='\t');
		}
	}
}
int main( int argc, char *argv[ ], char *envp[ ] )
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;
	char* input = (char*)calloc(150, sizeof(char));
	
//	while (!feof(in))
//	{
//		fgets(input, 150, in);
//	}
	

    hDevInfo = SetupDiGetClassDevs(NULL,
        REGSTR_KEY_PCIENUM,
        0,
        DIGCF_PRESENT | DIGCF_ALLCLASSES );
       
    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        return 1;
    }
       
       
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,
        &DeviceInfoData);i++)
    {
		FILE* in = fopen("pci.ids", "r");
        DWORD DataT;
        LPTSTR buffer = NULL;
        DWORD buffersize = 0;
           
        while (!SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &DeviceInfoData,
			SPDRP_DEVICEDESC,
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
           
		int p=-1;
		printf("\nResult:");
		while (buffer[p++])
			printf("%c",buffer[p]);
        //printf("\n");
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_HARDWAREID,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			if (GetLastError() ==
				ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer) LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
			}
			else
			{
				break;
			}
		}

		p = -1;
		printf("\nResultID:");
		while (buffer[p++])
			printf("%c", buffer[p]);

		printInfoFromHID(in, buffer);

		printf("\n");
        //if (buffer) LocalFree(buffer);

		while (!SetupDiGetDeviceRegistryProperty(
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
		printf("Result(Vendor):");
		while (buffer[p++])
			printf("%c",buffer[p]);
        printf("\n");
        if (buffer) LocalFree(buffer);
		fclose(in);
    }
	/*
	int p=-1;
	while (envp[p++]) printf("%s\n",envp[p]);
	*/
    if ( GetLastError()!=NO_ERROR &&
        GetLastError()!=ERROR_NO_MORE_ITEMS )
    {
        return 1;
    }
       
    SetupDiDestroyDeviceInfoList(hDevInfo);
       
    return 0;
}