// // ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <istream>
#include <fstream>
#include "windows.h"
#include <iostream>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <array>

using namespace std;


void parseString(string data, string values[])
{
	int i = 0;
	while (data.length() > 0)
	{
		string dataPiece = "";
		int comma = data.find(",", 0);
		if (comma > -1)
		{
			int comma = data.find(",", 0);
			dataPiece = data.substr(0, comma);
			data = data.substr(comma + 1, data.length());
			values[i] = dataPiece;
		}
		else
		{
			dataPiece = data;
			data = "";
			values[i] = dataPiece.substr(0, dataPiece.length() - 2);
		}
		i++;
	}
}

void createHeader()
{
	printf("%13s %13s %13s %13s %14s %13s %14s %14s %15s %15s %15s %15s %12s %12s %16s %12s \n", "|Pressure 0|", "|Pressure 1|", "|Pressure 2|", "|Pressure 3|", "|Baro Temp 1|", "|Accel Temp|", "|  RH Temp  |", "|  RH Temp  |", "|  Humidity  |", "|  Humidity  |", "|Altitude (m)|", "|  Latitude  |", "|  Longitude  |", "|Velocity|", "|Heading (deg)|", "|Satellite|");
	printf(" %9s %13s %13s %13s %14s %13s %14s %14s %15s %15s %15s %15s %15s %12s %16s %12s \n", "|  (mbar)  |", "|  (mbar)  |", "|  (mbar)  |", "|  (mbar)  |", "|    (F)    |", "|    (F)   |", "| Inside (F)|", "|Outside (F)|", "| Outside (%)|", "| Inside (%) |", "|            |", "|            |","|             |", "|  (mph) |", "|             |", "|  Count  |");
	cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void outputData(string values[])
{
	printf("%11s %13s %13s %13s %12s %13s %14s %14s %15s %15s %15s %18s %15s %11s %13s %15s \n", values[0].c_str(), values[1].c_str(), values[2].c_str(), values[3].c_str(), values[4].c_str(), values[5].c_str(), values[6].c_str(), values[7].c_str(), values[8].c_str(), values[9].c_str(), values[10].c_str(), values[11].c_str(), values[12].c_str(), values[13].c_str(), values[14].c_str(), values[15].c_str());
}


int main()
{
	createHeader();
	string data[16];
	HANDLE hSerial;
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	hSerial = CreateFile(TEXT("\\\\.\\COM10"),
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
				cout << "error 4" << endl;
			}
		}
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) 
	{
		cout << "error 1" << endl;
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams)) 
	{
		cout << "error 2" << endl;
	}


	//Grab data
	int dataSets = 0;
	bool abContinue = true;
	while (abContinue)
	{
		DWORD read, write = 0;
		char szBuff[129] = { 0 };
		bool isRead = ReadFile(hSerial, szBuff, sizeof(szBuff), &read, NULL);
		if (!isRead)
		{
			abContinue = false;
			break;
		}
		//Make string from input
		string output = string(szBuff);
		output = output.substr(0, 129);
		parseString(output, data);
		dataSets++;
		if (dataSets == 35)
		{
			system("CLS");
			createHeader();
			dataSets = 0;
		}
		outputData(data);	
	}
	CloseHandle(hSerial);
}

