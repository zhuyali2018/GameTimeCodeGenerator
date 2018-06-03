// GameTimeCodeGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <time.h>
#include <string>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
using namespace std;
string GetPassCode(char * buf);
string decode(const char * buf);
int main(int argc, char **argv)
{
	if (argc < 3) {
		cout << "version 1.0 Usage: " << endl;
		cout << "      " << argv[0] << " " << "<number_of_min_in_2 digits>" << " " << "<SerialNo_today>" << " [YYMMDD]" << endl;
		return 1;
	}
	
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime	// for more information about date/time format
	strftime(buf, sizeof(buf), "%y%m%d", &tstruct);
	if (argc == 4) {
		strcpy(buf, argv[3]);
	}
	cout << "date: " << buf << endl;
	if (strlen(buf)>6) {
		cout << "Error: 3rd argument too long" << endl;
		return 1;
	}
	if (strlen(argv[1])!=2 ) {
		cout << "Error: 1st argument has to be 2 digits" << endl;
		return 1;
	}
	if (strlen(argv[2]) != 1) {
		cout << "Error: 2nd argument has to be 1 digits, 0-9, A-Z, a-e" << endl;
		return 1;
	}
	char buf1[10];
	sprintf(buf1, "%s%s%s", argv[1], buf, argv[2]);
	buf1[9] = 0;
	cout << "string: " << buf1 << endl;
	string pwd = GetPassCode(buf1);
	cout << "passcode: " << pwd << " 20" << buf << " for " << argv[1] << " minutes" << endl ;
	string decoded=decode(pwd.c_str());
	cout << "recovered string=" << decoded << endl;
    return 0;
}
char table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
string GetPassCode(char * buf) {
	char converted[10];
	converted[9] = 0;
	int tablesize = (size_t)strlen(table);
	//cout << "tablesize=" << tablesize << endl;
	int bdd = *(buf + 8);
	int b = bdd - 0x30;
	//cout << "b=" << b << endl;
	int i = 0;
	for (i = 0; i < 8;i++){
		int add = *(buf+i);
		int a = add - 0x30;
		//cout << i << ":" << a << endl;
		converted[i] = table[a+b];
	}
	converted[i] = table[b];
	return converted;
}

int getIndex(char c) {
	int len = strlen(table);
	for (int i = 0; i < len; i++) {
		if (table[i] == c) {
			return i;
		}
	}
	return -1;
}
string decode(const char * buf) {
	int len = strlen(table);
	char converted[10];
	char b = buf[8];     //last char was base index
	//cout << "b=" << b << endl;
	int index = getIndex(b);   //get index which is base + index
	//cout << "index=" << index << endl;
	int i = 0;
	for (i = 0; i < 8; i++) {
		char c=buf[i];
		//cout << "char" << i << ": " << c << endl;
		int bindex = getIndex(c);
		int asc = bindex - index;
		char ch = asc + 0x30;
		converted[i] = ch;
	}
	converted[i] = 0;
	return converted;
}
