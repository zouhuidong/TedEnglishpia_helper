////////////////////////////////
//
//	stdafx.hpp
//	全局标准定义
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
#include <locale>
using namespace std;

////// 宏定义

// HTML 换行标签
#define DIV_RETURN "<div class=\"return\"></div>"


////// 类型定义

// 例句
struct ExampleSentence
{
	string en;	// 英文
	string zh;	// 中文
};

// 英语词典单词，包含一个单词应该具备的基本信息
typedef struct EnglishpiaWord
{
	string name;				// 单词
	string remark;              // 备注
	string pronunciation;		// 音标
	vector<string> meaning;     // 释义数组
	vector<ExampleSentence> sentences;   // 例句数组

}EWORD;

// 输出单词文档模式
enum DocTypes
{
	DOC_standard, DOC_simple, DOC_print
};

// 单词文档模式数
const int g_nDocTypesNum = 3;

////// 全局变量

// 版本信息
char g_strInfo[] = "Ver 0.6.2 | 2022.05.04";

// 基准日期
int g_month = -1;
int g_day = -1;
int g_hour = -1;
int g_min = -1;

// 输出单词文档模式
int g_nDocMode = DOC_standard;

// 输出单词目录模式
bool g_bDirectoryMode = false;


////// 全局函数

// 输出错误信息并结束程序
// die 指定是否结束程序，默认为真
void OutError(string error, bool die = true)
{
	printf(("\n\n" + error + "\n").c_str());

	if (die)
	{
		_getch();
		exit(0);
	}
}
