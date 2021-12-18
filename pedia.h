////////////////////////////////
//
//	pedia.h
//	处理单词文档的生成
//

#pragma once

#include "web.h"
#include "file.h"

// 获取 index 页的模版 HTML 内容
string GetModuleIndexHTML()
{
	return GetFile("./res/module/index.html");
}

// 获取单词的模版 HTML 内容
string GetModuleWordHTML()
{
	return GetFile("./res/module/word.html");
}

// 获取需要使用的 CSS 样式表的文件名
string GetCSSFileName()
{
	switch (g_nDocMode)
	{
	case DOC_standard:	return "standard.css";
	case DOC_simple:	return "simple.css";
	case DOC_print:		return "print.css";
	default:			return "standard.css";	// 未知情况
	}
}

// 获取一个单词以 Ted Englishpia 格式排版的 HTML 内容
// eword	单词信息
// strModuleHTML	模版 HTML 页面内容
// count	当前单词在该词组中的索引
string GetWordHTML(EWORD eword, string strModuleHTML, int count)
{
	// 该组词组的耗时累计（分钟）
	static int nTimeCost = 0;

	// 记录每个单词所需时间（分钟）
	int spent_per_word = 4;

	// 记录单词所需时间的随机偏差大小（分钟）
	int deviation_per_word = 1;

	// HTML 内容
	string strHTML = strModuleHTML;

	// 如果是新的词组，则重新开始计算耗时累计
	if (count == 0)
	{
		nTimeCost = 0;
	}

	// time
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	// 基准时间设置
	sys.wMonth = g_month <= 0 ? sys.wMonth : g_month;
	sys.wDay = g_day <= 0 ? sys.wDay : g_day;
	sys.wHour = g_hour < 0 ? sys.wHour : g_hour;
	sys.wMinute = g_min < 0 ? sys.wMinute : g_min;

	// 计算抄录此单词的时间
	int spend = spent_per_word +
		/* offset */
		((rand() % 3 - 1) * rand() % (deviation_per_word + 1));

	sys.wMinute += nTimeCost;
	nTimeCost += spend;			// 时间累计

	// 时间进位
	sys.wHour += sys.wMinute / 60;
	sys.wDay += sys.wHour / 24;
	sys.wMinute %= 60;
	sys.wHour %= 24;

	char bufTime[128] = { 0 };
	sprintf_s(bufTime, 128, "%02d.%02d %02d:%02d", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);

	// 将所有释义加入一个字符串中
	string strMeaning;
	for (int i = 0; i < (int)eword.meaning.size(); i++)
	{
		char buf[6] = { 0 };
		_itoa_s(i + 1, buf, 6, 10);
		strMeaning += buf + (string)". " + eword.meaning[i] + "&nbsp;&nbsp;";
	}

	// 将所有例句加入一个字符串中
	string strSentences;
	for (int i = 0; i < (int)eword.sentences.size(); i++)
	{
		char buf[6] = { 0 };
		_itoa_s(i + 1, buf, 6, 10);

		// 例句单词高亮
		// 全部转小写再查找，避免由于大小写不一致导致高亮失败的问题
		string en = eword.sentences[i].en;
		string low_en = en;
		string low_word = eword.word;
		transform(low_en.begin(), low_en.end(), low_en.begin(), ::tolower);
		transform(low_word.begin(), low_word.end(), low_word.begin(), ::tolower);
		int pos = low_en.find(low_word);
		if (pos >= 0)
		{
			// 找到关键字之后仍使用原句中的字符串，因为用户输入的字符串大小写可能不正确
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

	// 填写参数
	replace_all(strHTML, "{WORD}", eword.word);
	replace_all(strHTML, "{TIME}", bufTime);
	replace_all(strHTML, "{PRONUNCIATION}", "/" + eword.pronunciation + "/");
	replace_all(strHTML, "{MEANING}", strMeaning);
	replace_all(strHTML, "{SENTENCES}", strSentences);
	replace_all(strHTML, "{DIV_ID}", "div_" + eword.word);
	replace_all(strHTML, "{IFRAME_SRC}", "http://dict.cn/" + eword.word);
	replace_all(strHTML, "{GOOGLE_URL}", "https://translate.google.cn/?sl=en&tl=zh-CN&text=" + eword.word);
	replace_all(strHTML, "{BAIDU_URL}", "https://fanyi.baidu.com/#en/zh/" + eword.word);
	replace_all(strHTML, "{MORE_SENTENCES_URL}", "http://www.jukuu.com/search.php?q=" + eword.word);

	count++;

	return strHTML;
}

// 根据单词构建 HTML 格式的目录
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

// 根据单词构建 Ted Englishpia，返回构建出的 HTML 内容
string BuildEnglishPia(vector<string> vecStrings)
{
	// 单词目录
	string strDirectory;
	if (g_bDirectoryMode)
	{
		strDirectory = GetDirectory(vecStrings);
	}

	// 获取所有单词的详细信息
	vector<EWORD> words;
	for (int i = 0; i < (int)vecStrings.size(); i++)
	{
		if (vecStrings[i].size() > 0)
		{
			words.push_back(SearchWord(vecStrings[i]));
		}
	}

	string strModuleIndex = GetModuleIndexHTML();
	string strModuleWord = GetModuleWordHTML();

	string strWordsHTML;
	for (int i = 0; i < (int)words.size(); i++)
	{
		strWordsHTML += GetWordHTML(words[i], strModuleWord, i);
	}

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char path[128] = { 0 };
	sprintf_s(path, 128, "%04d.%02d.%02d %02d:%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);

	char chNum[6] = { 0 };
	_itoa_s(words.size(), chNum, 10);

	// 填写参数
	replace_all(strModuleIndex, "{CSS}", GetCSSFileName());
	replace_all(strModuleIndex, "{TIME}", path);
	replace_all(strModuleIndex, "{NUM}", chNum);
	replace_all(strModuleIndex, "{CONTENT}", strDirectory + strWordsHTML);

	return strModuleIndex;
}

// 输出 HTML 内容为文件，返回文件路径
string OutputHTML(string html)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char path[256] = { 0 };
	sprintf_s(path, 256, "./res/temp/TedEnglishpia_%04d-%02d-%02d-%02d-%02d-%02d.html",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	ofstream out(path);
	out.write(html.c_str(), html.size());
	out.close();

	return path;
}
