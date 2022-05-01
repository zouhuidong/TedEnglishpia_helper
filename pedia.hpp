////////////////////////////////
//
//	pedia.hpp
//	处理单词文档的生成
//

#pragma once

#include "web.hpp"
#include "file.hpp"

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

// 根据单词构建 HTML 格式的目录
// 也就是生成在 pedia 最前面的单词列表
string GetDirectory(vector<string> vecStrings)
{
	string strDirectory = "<div><span class=\"blue\">[ Word List ]</span></div>\n" DIV_RETURN "\n";
	for (int i = 0; i < (int)vecStrings.size(); i++)
	{
		char buf[12] = { 0 };
		_itoa_s(i + 1, buf, 10);
		strDirectory += "<div>[" + (string)buf + "] " + vecStrings[i] + "</div>";
		strDirectory += "\n" DIV_RETURN "\n";
	}
	strDirectory += "&nbsp;\n" DIV_RETURN "\n";

	return strDirectory;
}

// 传入单词列表，获取各个单词的详细信息
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

// 获取一个单词以 Ted Englishpia 格式排版的 HTML 内容
// eword	单词信息
// strModuleHTML	模版 HTML 页面内容
// count	当前单词在该词组中的索引
string GetWordHTML(EWORD eword, string strModuleHTML, int count)
{
	static int nTimeCost = 0;	// 该组词组的耗时累计（分钟）
	int spent_per_word = 4;		// 记录每个单词所需时间（分钟）
	int deviation_per_word = 1;	// 记录单词所需时间的随机偏差大小（分钟）

	// 抄录时间计算
	char bufTime[128] = { 0 };
	{
		// 如果是新的词组，则重新开始计算耗时累计
		if (count == 0)
		{
			nTimeCost = 0;
		}

		// 基准时间设置
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		sys.wMonth = g_month <= 0 ? sys.wMonth : g_month;
		sys.wDay = g_day <= 0 ? sys.wDay : g_day;
		sys.wHour = g_hour < 0 ? sys.wHour : g_hour;
		sys.wMinute = g_min < 0 ? sys.wMinute : g_min;

		// 计算抄录此单词的时间
		int spend = spent_per_word +
			/* offset */
			((rand() % 3 - 1) * rand() % (deviation_per_word + 1));

		// 时间累计
		sys.wMinute += nTimeCost;
		nTimeCost += spend;

		// 时间进位
		sys.wHour += sys.wMinute / 60;
		sys.wDay += sys.wHour / 24;
		sys.wMinute %= 60;
		sys.wHour %= 24;

		// 抄录时间转字符串
		sprintf_s(bufTime, 128, "%02d.%02d %02d:%02d",
			sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);
	}

	// HTML 内容
	string strHTML = strModuleHTML;

	// 单词
	string strWord = eword.name;
	if (eword.remark.size())
	{
		strWord += " (" + eword.remark + ")";
	}

	// 若选择生成目录，则将在每个单词前加上序号
	if (g_bDirectoryMode)
	{
		char chCount[6] = { 0 };
		_itoa_s(count + 1, chCount, 10);
		strWord = "<span class=\"black\">[" + (string)chCount + "]</span> " + strWord;
	}

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
		string low_word = eword.name;
		transform(low_en.begin(), low_en.end(), low_en.begin(), ::tolower);
		transform(low_word.begin(), low_word.end(), low_word.begin(), ::tolower);
		int pos = low_en.find(low_word);
		int len = eword.name.length();

		int _pos = pos - 1;
		int pos_ = pos + eword.name.length();
		while (_pos >= 0 && en[_pos] != ' ')				_pos--;
		while (pos_ < (int)en.size() && en[pos_] != ' ')	pos_++;
		pos = _pos + 1;
		len = pos_ - pos;

		if (pos != string::npos)
		{
			// 找到关键字之后仍使用原句中的字符串，因为用户输入的字符串大小写可能不正确
			char dest[256] = { 0 };
			strncpy_s(dest, en.c_str() + pos, len);
			en.replace(pos, len, "<span class=\"red\"><b>" + (string)dest + "</b></span>");
		}

		strSentences += buf + (string)". " + en + DIV_RETURN "&nbsp;&nbsp;" + eword.sentences[i].zh;
		if (i + 1 < (int)eword.sentences.size())
		{
			strSentences += DIV_RETURN;
		}
	}

	// 填写参数
	replace_all(strHTML, "{WORD}", strWord);
	replace_all(strHTML, "{TIME}", bufTime);
	replace_all(strHTML, "{PRONUNCIATION}", "/" + eword.pronunciation + "/");
	replace_all(strHTML, "{MEANING}", strMeaning);
	replace_all(strHTML, "{SENTENCES}", strSentences);
	replace_all(strHTML, "{DIV_ID}", "div_" + eword.name);
	replace_all(strHTML, "{IFRAME_SRC}", "http://dict.cn/" + eword.name);
	replace_all(strHTML, "{GOOGLE_URL}", "https://translate.google.cn/?sl=en&tl=zh-CN&text=" + eword.name);
	replace_all(strHTML, "{BAIDU_URL}", "https://fanyi.baidu.com/#en/zh/" + eword.name);
	replace_all(strHTML, "{MORE_SENTENCES_URL}", "http://www.jukuu.com/search.php?q=" + eword.name);

	return strHTML;
}

vector<string> GetStringList(vector<EWORD> vecEWords)
{
	vector<string> vecStrings;
	for (int i = 0; i < (int)vecEWords.size(); i++)
	{
		vecStrings.push_back(vecEWords[i].name);
	}
	return vecStrings;
}

// 【 主要函数 】
// 根据单词列表构建整个 Ted Englishpia 网页文档，返回构建出的 HTML 内容
string BuildEnglishPia(vector<EWORD> vecEWords, vector<string> vecStrings)
{
	// 生成单词目录
	string strDirectory;
	if (g_bDirectoryMode)
	{
		strDirectory = GetDirectory(vecStrings);
	}

	// 获取模版页面
	string strModuleIndex = GetModuleIndexHTML();
	string strModuleWord = GetModuleWordHTML();

	// 所有单词对应的 HTML 格式的内容
	string strWordsHTML;
	for (int i = 0; i < (int)vecEWords.size(); i++)
	{
		strWordsHTML += GetWordHTML(vecEWords[i], strModuleWord, i);
	}

	// 当前时间
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char path[128] = { 0 };
	sprintf_s(path, 128, "%04d.%02d.%02d %02d:%02d",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);

	// 单词数量
	char chNum[6] = { 0 };
	_itoa_s(vecEWords.size(), chNum, 10);

	// 填写参数
	replace_all(strModuleIndex, "{CSS}", GetCSSFileName());
	replace_all(strModuleIndex, "{TIME}", path);
	replace_all(strModuleIndex, "{NUM}", chNum);
	replace_all(strModuleIndex, "{CONTENT}", strDirectory + strWordsHTML);

	return strModuleIndex;
}

string BuildEnglishPia(vector<string> vecStrings)
{
	vector<EWORD> vecEWords = GetWordsInfo(vecStrings);
	return BuildEnglishPia(vecEWords, vecStrings);
}
