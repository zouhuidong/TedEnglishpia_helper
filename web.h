////////////////////////////////
//
//	web.h
//	���������������
//

#pragma once

#include "stdafx.h"
#include "string_func.h"

/**
 * @brief        UTF-8 �����ַ���ת GBK �����ַ���
 * @param[in]    lpUTF8Str: ԭ utf-8 �ַ���
 * @param[out]    lpGBKStr: ת���� isGBK �ַ���
 * @param[in]    nGBKStrLen: isGBK �ַ�������󳤶�
 * @return        ����ת�����ַ����ĳ���
 * @note        �������� https://www.cnblogs.com/zhongbin/p/3160641.html
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
 * @brief    �ж�һ���ַ����Ƿ�Ϊ UTF-8 ����
 * @note    ���� https://blog.csdn.net/jiankekejian/article/details/106720432 ����ɾ�ģ�
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
 * @brief        ��ȡ��ҳԴ��
 * @param[in]    Url ��ҳ����
 * @param[in]    �Ƿ�ǿ��ת��Ϊ GBK ����
 * @return       ������ҳԴ��
 * @note    �������� https://www.cnblogs.com/croot/p/3391003.html ����ɾ�ģ�
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
		strGBK.resize(strlen(strGBK.c_str()));    // ɾ������ \0
		return strGBK;
	}
	else
	{
		return strHTML;
	}
}


// �� HTML ҳ���в�������
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

// �� HTML ҳ���в�������
bool FindMeaning(EWORD* eword, string strHTML)
{
	// ������
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

		// ������������
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

// �� HTML ҳ���в�������
bool FindSentences(EWORD* eword, string strHTML)
{
	// ����ѡȡ�������䣬���ж����������ÿ������ѡȡһ������

	string search_key = "<div class=\"layout sort\">";
	int offset = strHTML.find(search_key);
	if (offset < 0)
	{
		return false;
	}

	// �����������ݿ�Ľ���λ��
	int end_sentences = strHTML.find("<div class=\"dict-chart\" id=\"dict-chart-examples\"");
	if (end_sentences < 0)
	{
		// �п��ܴ˵��ʲ����� [���Գ��öȷֲ�ͼ]�����Բ�����һ������λ��
		end_sentences = strHTML.find("<h3", offset);

		if (end_sentences < 0)
		{
			return false;
		}
	}

	// �������Ե����䣨���ܺ�������䣩����ʼ����ֹλ��
	typedef struct SentencesDivArea
	{
		int start = -1;
		int end = -1;
	}SDArea;

	SDArea index_category[16];

	// ��������
	int category_num = 0;

	// ����ȷ���������Ե������λ��
	for (;;)
	{
		offset = strHTML.find("<ol", offset);

		// ��������
		if (offset < 0 || offset > end_sentences)
		{
			break;
		}

		int end_ol = strHTML.find("</ol>", offset);

		// ��¼
		index_category[category_num] = { offset,end_ol };
		category_num++;

		offset = end_ol;
	}

	// û�ҵ�����
	if (category_num <= 0)
	{
		return false;
	}

	// ��ÿ��������Ѱ��һ������

	// ����Ƿ������������䣬����ֻ��һ�����Ե�ʱ��˱���������;��
	bool already_two = false;
	offset = index_category[0].start;
	for (int i = 0; i < category_num; i++)
	{
		search_key = "<li>";
		offset = strHTML.find(search_key, offset);

		// �˴������޶�Ӧ����
		if (offset < 0 || offset > index_category[i].end)
		{
			continue;
		}

		offset += search_key.size();

		// ��ǰ����Ľ���λ��
		int end_sentence = strHTML.find("</li>", offset);

		// ��ȡһ������
		ExampleSentence sentence;
		string* pStr = &sentence.en;
		for (int j = offset; j < end_sentence; j++)
		{
			// �������Ӣ���� "<br>" �ָ�
			if (strHTML[j] == '<')
			{
				for (; strHTML[j] != '>'; j++);
				pStr = &sentence.zh;
				continue;
			}

			*pStr += strHTML[j];
		}

		// ��¼����
		eword->sentences.push_back(sentence);

		// ������һ�����ԣ����´�ѭ�����ڵ�ǰ�����ĩβ��������һ�����䣬
		// ����ʵ���ڴ˴����������������䡣
		if (category_num == 1)
		{
			// �������Ӽ�������Ҫ��
			if (already_two)
			{
				break;
			}

			offset = end_sentence;
			already_two = true;
			i--;
		}
		// ���ж�����ԣ����´�ѭ������ת���¸����Ե����俪ͷ������һ�����䡣
		else
		{
			offset = index_category[i + 1].start;;
		}
	}

	return true;
}

// �鵥��
EWORD SearchWord(string strWord)
{
	// ��������վ
	string strHTML = GetWebSrcCode(stow("http://dict.cn/" + strWord).c_str(), false);
	EWORD eword = { strWord };

	// ������
	if (!FindPronunciation(&eword, strHTML))
	{
		OutError("���� " + strWord + " ������ʧ��", false);
	}

	// ������
	if (!FindMeaning(&eword, strHTML))
	{
		OutError("���� " + strWord + " ������ʧ��", false);
	}

	// Ѱ������
	if (!FindSentences(&eword, strHTML))
	{
		OutError("���� " + strWord + " ������ʧ��", false);
	}

	return eword;
}
