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
		wchar_t temp8 = towlower(HID[8]);
		wchar_t temp9 = towlower(HID[9]);
		wchar_t temp10 = towlower(HID[10]);
		wchar_t temp11 = towlower(HID[11]);
		if ( temp8 == input[0] && temp9 == input[1] && temp10 == input[2] && temp11 == input[3])
		{
			printf("\n");
			puts(input);
			do
			{
				fgets(input,150,in);
			notSubsys:
				if (input[0] == '#') 
				{
					input[0] = '\t';
					continue;
				}

				if (input[0] == '\t')
				{			
					wchar_t temp17 = towlower(HID[17]);
					wchar_t temp18 = towlower(HID[18]);
					wchar_t temp19 = towlower(HID[19]);
					wchar_t temp20 = towlower(HID[20]);

					if (temp17 == input[1] && temp18 == input[2] && temp19 == input[3] && temp20 == input[4])
					{
						printf("\n");
						puts(input);
						do
						{
							fgets(input, 150, in);
							if (input[0] == '#')
							{
								input[0] = '\t'; 
								input[1] = '\t';
								continue;
							}
							if (input[0] == '\t'&&input[1] == '\t')
							{
								if (towlower(HID[29]) == input[2] && towlower(HID[30]) == input[3] && towlower(HID[31]) == input[4] && towlower(HID[32]) == input[5] &&
									towlower(HID[33]) == input[7] && towlower(HID[34]) == input[8] && towlower(HID[35]) == input[9] && towlower(HID[36]) == input[10])
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

		int p = -1;
		printf("\nResultID:");
		while (buffer[p++])
			printf("%c", buffer[p]);

		printInfoFromHID(in, buffer);

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