////////////////////////////////
//
//	stdafx.h
//	ȫ�ֱ�׼����
//

#pragma once

#include <stdio.h>
#include <conio.h>
#include <comutil.h>
#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <algorithm>
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "wininet.lib")
using namespace std;

////// �궨��

// HTML ���б�ǩ
#define DIV_RETURN "<div class=\"return\"></div>"


////// ���Ͷ���

// ����
struct ExampleSentence
{
	string en;	// Ӣ��
	string zh;	// ����
};

// Ӣ��ʵ䵥�ʣ�����һ������Ӧ�þ߱��Ļ�����Ϣ
typedef struct EnglishpiaWord
{
	string word;				// ����
	string pronunciation;		// ����
	vector<string> meaning;     // ��������
	vector<ExampleSentence> sentences;   // ��������

}EWORD;

// ��������ĵ�ģʽ
enum DocTypes
{
	DOC_standard, DOC_simple, DOC_print
};

// �����ĵ�ģʽ��
const int g_nDocTypesNum = 3;

////// ȫ�ֱ���

// �汾��Ϣ
char g_strInfo[] = "Ver 0.5 | 2021.12.18";

// ��׼����
int g_month = -1;
int g_day = -1;
int g_hour = -1;
int g_min = -1;

// ��������ĵ�ģʽ
int g_nDocMode = DOC_standard;

// �������Ŀ¼ģʽ
bool g_bDirectoryMode = false;


////// ȫ�ֺ���

// ���������Ϣ����������
// die ָ���Ƿ��������Ĭ��Ϊ��
void OutError(string error, bool die = true)
{
	printf(("\n\n" + error + "\n").c_str());

	if (die)
	{
		_getch();
		exit(0);
	}
}
