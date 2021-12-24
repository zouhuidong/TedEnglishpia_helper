////////////////////////////////
//
//	pedia.h
//	�������ĵ�������
//

#pragma once

#include "web.h"
#include "file.h"

// ��ȡ index ҳ��ģ�� HTML ����
string GetModuleIndexHTML()
{
	return GetFile("./res/module/index.html");
}

// ��ȡ���ʵ�ģ�� HTML ����
string GetModuleWordHTML()
{
	return GetFile("./res/module/word.html");
}

// ��ȡ��Ҫʹ�õ� CSS ��ʽ����ļ���
string GetCSSFileName()
{
	switch (g_nDocMode)
	{
	case DOC_standard:	return "standard.css";
	case DOC_simple:	return "simple.css";
	case DOC_print:		return "print.css";
	default:			return "standard.css";	// δ֪���
	}
}

// ���ݵ��ʹ��� HTML ��ʽ��Ŀ¼
// Ҳ���������� pedia ��ǰ��ĵ����б�
string GetDirectory(vector<string> vecStrings)
{
	string strDirectory = "<span class=\"blue\">[ Word List ]</span>\n" DIV_RETURN "\n";
	for (int i = 0; i < (int)vecStrings.size(); i++)
	{
		char buf[12] = { 0 };
		_itoa_s(i + 1, buf, 10);
		strDirectory += "[" + (string)buf + "] " + vecStrings[i];
		strDirectory += "\n" DIV_RETURN "\n";
	}
	strDirectory += "&nbsp;\n" DIV_RETURN "\n";

	return strDirectory;
}

// ���뵥���б���ȡ�������ʵ���ϸ��Ϣ
vector<EWORD> GetWordsInfo(vector<string> vecStrings)
{
	vector<EWORD> words;
	for (int i = 0; i < (int)vecStrings.size(); i++)
	{
		if (vecStrings[i].size() > 0)
		{
			words.push_back(SearchWord(vecStrings[i]));
		}
	}
	return words;
}

// ��ȡһ�������� Ted Englishpia ��ʽ�Ű�� HTML ����
// eword	������Ϣ
// strModuleHTML	ģ�� HTML ҳ������
// count	��ǰ�����ڸô����е�����
string GetWordHTML(EWORD eword, string strModuleHTML, int count)
{
	static int nTimeCost = 0;	// �������ĺ�ʱ�ۼƣ����ӣ�
	int spent_per_word = 4;		// ��¼ÿ����������ʱ�䣨���ӣ�
	int deviation_per_word = 1;	// ��¼��������ʱ������ƫ���С�����ӣ�

	// ��¼ʱ�����
	char bufTime[128] = { 0 };
	{
		// ������µĴ��飬�����¿�ʼ�����ʱ�ۼ�
		if (count == 0)
		{
			nTimeCost = 0;
		}

		// ��׼ʱ������
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		sys.wMonth = g_month <= 0 ? sys.wMonth : g_month;
		sys.wDay = g_day <= 0 ? sys.wDay : g_day;
		sys.wHour = g_hour < 0 ? sys.wHour : g_hour;
		sys.wMinute = g_min < 0 ? sys.wMinute : g_min;

		// ���㳭¼�˵��ʵ�ʱ��
		int spend = spent_per_word +
			/* offset */
			((rand() % 3 - 1) * rand() % (deviation_per_word + 1));

		// ʱ���ۼ�
		sys.wMinute += nTimeCost;
		nTimeCost += spend;

		// ʱ���λ
		sys.wHour += sys.wMinute / 60;
		sys.wDay += sys.wHour / 24;
		sys.wMinute %= 60;
		sys.wHour %= 24;

		// ��¼ʱ��ת�ַ���
		sprintf_s(bufTime, 128, "%02d.%02d %02d:%02d",
			sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);
	}
	
	// HTML ����
	string strHTML = strModuleHTML;

	// ����
	string strWord = eword.word;

	// ��ѡ������Ŀ¼������ÿ������ǰ�������
	if (g_bDirectoryMode)
	{
		char chCount[6] = { 0 };
		_itoa_s(count + 1, chCount, 10);
		strWord = "<span class=\"black\">[" + (string)chCount + "]</span> " + strWord;
	}

	// �������������һ���ַ�����
	string strMeaning;
	for (int i = 0; i < (int)eword.meaning.size(); i++)
	{
		char buf[6] = { 0 };
		_itoa_s(i + 1, buf, 6, 10);
		strMeaning += buf + (string)". " + eword.meaning[i] + "&nbsp;&nbsp;";
	}

	// �������������һ���ַ�����
	string strSentences;
	for (int i = 0; i < (int)eword.sentences.size(); i++)
	{
		char buf[6] = { 0 };
		_itoa_s(i + 1, buf, 6, 10);

		// ���䵥�ʸ���
		// ȫ��תСд�ٲ��ң��������ڴ�Сд��һ�µ��¸���ʧ�ܵ�����
		string en = eword.sentences[i].en;
		string low_en = en;
		string low_word = eword.word;
		transform(low_en.begin(), low_en.end(), low_en.begin(), ::tolower);
		transform(low_word.begin(), low_word.end(), low_word.begin(), ::tolower);
		int pos = low_en.find(low_word);
		if (pos >= 0)
		{
			// �ҵ��ؼ���֮����ʹ��ԭ���е��ַ�������Ϊ�û�������ַ�����Сд���ܲ���ȷ
			char dest[256] = { 0 };
			strncpy_s(dest, en.c_str() + pos, eword.word.size());
			en.replace(pos, eword.word.length(), "<span class=\"red\">" + (string)dest + "</span>");
		}

		strSentences += buf + (string)". " + en + DIV_RETURN "&nbsp;&nbsp;" + eword.sentences[i].zh;
		if (i + 1 < (int)eword.sentences.size())
		{
			strSentences += DIV_RETURN;
		}
	}

	// ��д����
	replace_all(strHTML, "{WORD}", strWord);
	replace_all(strHTML, "{TIME}", bufTime);
	replace_all(strHTML, "{PRONUNCIATION}", "/" + eword.pronunciation + "/");
	replace_all(strHTML, "{MEANING}", strMeaning);
	replace_all(strHTML, "{SENTENCES}", strSentences);
	replace_all(strHTML, "{DIV_ID}", "div_" + eword.word);
	replace_all(strHTML, "{IFRAME_SRC}", "http://dict.cn/" + eword.word);
	replace_all(strHTML, "{GOOGLE_URL}", "https://translate.google.cn/?sl=en&tl=zh-CN&text=" + eword.word);
	replace_all(strHTML, "{BAIDU_URL}", "https://fanyi.baidu.com/#en/zh/" + eword.word);
	replace_all(strHTML, "{MORE_SENTENCES_URL}", "http://www.jukuu.com/search.php?q=" + eword.word);

	return strHTML;
}


// �� ��Ҫ���� ��
// ���ݵ����б������� Ted Englishpia ��ҳ�ĵ������ع������� HTML ����
string BuildEnglishPia(vector<string> vecStrings)
{
	// ���ɵ���Ŀ¼
	string strDirectory;
	if (g_bDirectoryMode)
	{
		strDirectory = GetDirectory(vecStrings);
	}

	// ��ȡ���е��ʵ���ϸ��Ϣ
	vector<EWORD> words = GetWordsInfo(vecStrings);

	// ��ȡģ��ҳ��
	string strModuleIndex = GetModuleIndexHTML();
	string strModuleWord = GetModuleWordHTML();

	// ���е��ʶ�Ӧ�� HTML ��ʽ������
	string strWordsHTML;
	for (int i = 0; i < (int)words.size(); i++)
	{
		strWordsHTML += GetWordHTML(words[i], strModuleWord, i);
	}

	// ��ǰʱ��
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char path[128] = { 0 };
	sprintf_s(path, 128, "%04d.%02d.%02d %02d:%02d",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);

	// ��������
	char chNum[6] = { 0 };
	_itoa_s(words.size(), chNum, 10);

	// ��д����
	replace_all(strModuleIndex, "{CSS}", GetCSSFileName());
	replace_all(strModuleIndex, "{TIME}", path);
	replace_all(strModuleIndex, "{NUM}", chNum);
	replace_all(strModuleIndex, "{CONTENT}", strDirectory + strWordsHTML);

	return strModuleIndex;
}
