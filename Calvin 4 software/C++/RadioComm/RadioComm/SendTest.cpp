// // ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "windows.h"
#include <iostream>

using namespace std;

int main()
{
	HANDLE hSerial;
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);
	char output[] = "";
	hSerial = CreateFile(TEXT("COM7"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			//serial port does not exist. Inform user.
			cout << "error 4" << endl;
		}
		//some other error occurred. Inform user.
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		cout << "error 1" << endl;
		//error getting state
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams)) {
		//error setting serial port state
		cout << "error 2" << endl;
	}

	bool abContinue = true;
	while (abContinue)
	{
		DWORD read, write = 0;
		WriteFile(hSerial, , read, &write, NULL);

	}

	CloseHandle(hSerial);	CloseHandle(keyboard);
}

