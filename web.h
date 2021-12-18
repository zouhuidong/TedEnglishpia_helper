////////////////////////////////
//
//	web.h
//	处理网络相关内容
//

#pragma once

#include "stdafx.h"
#include "string_func.h"

/**
 * @brief        UTF-8 编码字符串转 GBK 编码字符串
 * @param[in]    lpUTF8Str: 原 utf-8 字符串
 * @param[out]    lpGBKStr: 转码后的 isGBK 字符串
 * @param[in]    nGBKStrLen: isGBK 字符串的最大长度
 * @return        返回转换后字符串的长度
 * @note        代码来自 https://www.cnblogs.com/zhongbin/p/3160641.html
*/
int UTF8ToGBK(char* lpUTF8Str, char* lpGBKStr, int nGBKStrLen)
{
	wchar_t* lpUnicodeStr = NULL;
	int nRetLen = 0;
	if (!lpUTF8Str) return 0;
	nRetLen = ::MultiByteToWideChar(CP_UTF8, 0, (char*)lpUTF8Str, -1, NULL, NULL);
	lpUnicodeStr = new WCHAR[nRetLen + 1];
	nRetLen = ::MultiByteToWideChar(CP_UTF8, 0, (char*)lpUTF8Str, -1, lpUnicodeStr, nRetLen);
	if (!nRetLen) return 0;
	nRetLen = ::WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, NULL, NULL, NULL, NULL);
	if (!lpGBKStr)
	{
		if (lpUnicodeStr) delete[] lpUnicodeStr;
		return nRetLen;
	}
	if (nGBKStrLen < nRetLen)
	{
		if (lpUnicodeStr) delete[] lpUnicodeStr;
		return 0;
	}
	nRetLen = ::WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, (char*)lpGBKStr, nRetLen, NULL, NULL);
	if (lpUnicodeStr) delete[] lpUnicodeStr;
	return nRetLen;
}

/**
 * @brief    判断一个字符串是否为 UTF-8 编码
 * @note    来自 https://blog.csdn.net/jiankekejian/article/details/106720432 （有删改）
*/
bool isUTF8(const char* str)
{
	int length = strlen(str);
	int check_sub = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i < length; i++)
	{
		if (check_sub == 0)
		{
			if ((str[i] >> 7) == 0)
			{
				continue;
			}
			struct
			{
				int cal;
				int cmp;
			} Utf8NumMap[] = { {0xE0,0xC0},{0xF0,0xE0},{0xF8,0xF0},{0xFC,0xF8},{0xFE,0xFC}, };
			for (j = 0; j < (sizeof(Utf8NumMap) / sizeof(Utf8NumMap[0])); j++)
			{
				if ((str[i] & Utf8NumMap[j].cal) == Utf8NumMap[j].cmp)
				{
					check_sub = j + 1;
					break;
				}
			}
			if (0 == check_sub)
			{
				return false;
			}
		}
		else
		{
			if ((str[i] & 0xC0) != 0x80)
			{
				return false;
			}
			check_sub--;
		}
	}
	return true;
}

/**
 * @brief        获取网页源码
 * @param[in]    Url 网页链接
 * @param[in]    是否强制转换为 GBK 编码
 * @return       返回网页源码
 * @note    代码来自 https://www.cnblogs.com/croot/p/3391003.html （有删改）
*/
string GetWebSrcCode(LPCTSTR Url, bool isGBK = true)
{
	string strHTML;
	HINTERNET hSession = InternetOpen(L"IE6.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hURL = InternetOpenUrl(hSession, Url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hURL != NULL)
		{
			const int nBlockSize = 1024;
			char Temp[nBlockSize] = { 0 };
			ULONG Number = 1;

			while (Number > 0)
			{
				InternetReadFile(hURL, Temp, nBlockSize - 1, &Number);
				for (int i = 0; i < (int)Number; i++)
					strHTML += Temp[i];
			}

			InternetCloseHandle(hURL);
			hURL = NULL;
		}

		InternetCloseHandle(hSession);
		hSession = NULL;
	}

	if (isGBK && isUTF8(strHTML.c_str()))
	{
		string strGBK;
		strGBK.resize(strHTML.size() * 2);
		UTF8ToGBK(&strHTML[0], &strGBK[0], strHTML.size() * 2);
		strGBK.resize(strlen(strGBK.c_str()));    // 删除多余 \0
		return strGBK;
	}
	else
	{
		return strHTML;
	}
}


// 在 HTML 页面中查找音标
bool FindPronunciation(EWORD* eword, string strHTML)
{
	string search_key = "<bdo lang=\"EN-US\">[";
	int offset = strHTML.find(search_key);
	if (offset < 0)
	{
		return false;
	}
	for (
		int i = search_key.size();
		strHTML[offset + i] != ']';
		eword->pronunciation += strHTML[offset + i], i++
		);

	return true;
}

// 在 HTML 页面中查找释义
bool FindMeaning(EWORD* eword, string strHTML)
{
	// 找释义
	string search_key = "<ul class=\"dict-basic-ul\">";
	int offset = strHTML.find(search_key);
	if (offset < 0)
	{
		return false;
	}

	int end_ul = strHTML.find("</ul>");
	for (;;)
	{
		search_key = "<li>";
		offset = strHTML.find(search_key, offset);

		// 找完所有释义
		if (offset < 0 || offset > end_ul)
		{
			break;
		}

		string meaning;
		offset += search_key.size();
		int end = strHTML.find("</li>", offset);
		for (int j = offset; j < end; j++)
		{
			if (strHTML[j] != '\t' && strHTML[j] != '\n')
			{
				meaning += strHTML[j];
			}
		}
		eword->meaning.push_back(meaning);

		offset = end;
	}

	return true;
}

// 在 HTML 页面中查找例句
bool FindSentences(EWORD* eword, string strHTML)
{
	// 至少选取两个例句，若有多个词性则尽量每个词性选取一个例句

	string search_key = "<div class=\"layout sort\">";
	int offset = strHTML.find(search_key);
	if (offset < 0)
	{
		return false;
	}

	// 整个例句内容块的结束位置
	int end_sentences = strHTML.find("<div class=\"dict-chart\" id=\"dict-chart-examples\"");
	if (end_sentences < 0)
	{
		// 有可能此单词不存在 [词性常用度分布图]，所以查找另一个结束位置
		end_sentences = strHTML.find("<h3", offset);

		if (end_sentences < 0)
		{
			return false;
		}
	}

	// 各个词性的例句（可能含多个例句）的起始和终止位置
	typedef struct SentencesDivArea
	{
		int start = -1;
		int end = -1;
	}SDArea;

	SDArea index_category[16];

	// 词性数量
	int category_num = 0;

	// 首先确定各个词性的例句的位置
	for (;;)
	{
		offset = strHTML.find("<ol", offset);

		// 找完所有
		if (offset < 0 || offset > end_sentences)
		{
			break;
		}

		int end_ol = strHTML.find("</ol>", offset);

		// 记录
		index_category[category_num] = { offset,end_ol };
		category_num++;

		offset = end_ol;
	}

	// 没找到例句
	if (category_num <= 0)
	{
		return false;
	}

	// 在每个词性中寻找一个例句

	// 标记是否已有两个例句，仅在只有一个词性的时候此变量存在用途。
	bool already_two = false;
	offset = index_category[0].start;
	for (int i = 0; i < category_num; i++)
	{
		search_key = "<li>";
		offset = strHTML.find(search_key, offset);

		// 此词性下无对应例句
		if (offset < 0 || offset > index_category[i].end)
		{
			continue;
		}

		offset += search_key.size();

		// 当前例句的结束位置
		int end_sentence = strHTML.find("</li>", offset);

		// 读取一个例句
		ExampleSentence sentence;
		string* pStr = &sentence.en;
		for (int j = offset; j < end_sentence; j++)
		{
			// 例句的中英文以 "<br>" 分割
			if (strHTML[j] == '<')
			{
				for (; strHTML[j] != '>'; j++);
				pStr = &sentence.zh;
				continue;
			}

			*pStr += strHTML[j];
		}

		// 记录例句
		eword->sentences.push_back(sentence);

		// 若仅有一个词性，则下次循环中在当前例句的末尾继续搜索一个例句，
		// 即可实现在此词性下搜索两个例句。
		if (category_num == 1)
		{
			// 两个句子即可满足要求
			if (already_two)
			{
				break;
			}

			offset = end_sentence;
			already_two = true;
			i--;
		}
		// 若有多个词性，则下次循环中跳转到下个词性的例句开头处搜索一个例句。
		else
		{
			offset = index_category[i + 1].start;;
		}
	}

	return true;
}

// 查单词
EWORD SearchWord(string strWord)
{
	// 爬单词网站
	string strHTML = GetWebSrcCode(stow("http://dict.cn/" + strWord).c_str(), false);
	EWORD eword = { strWord };

	// 找音标
	if (!FindPronunciation(&eword, strHTML))
	{
		OutError("查找 " + strWord + " 的音标失败", false);
	}

	// 找释义
	if (!FindMeaning(&eword, strHTML))
	{
		OutError("查找 " + strWord + " 的释义失败", false);
	}

	// 寻找例句
	if (!FindSentences(&eword, strHTML))
	{
		OutError("查找 " + strWord + " 的例句失败", false);
	}

	return eword;
}
