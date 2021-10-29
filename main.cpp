//////////////////////////////////////////////////////////////////////////
//
//	TedEnglishpia_helper
//	
//	编译环境：
//		VisualStudio 2019
// 
//	项目简介：
//		我的高中英语老师 Ted 要求我们每天查至少 5 个生词，
//		并记录在 Englishpia 上，也就是自己制作英语词典。
// 
//		由于在线翻译网站的排版问题，通常查询一个单词的各项
//		信息比较耗费时间，查字典的效率则更低，所以特写此
//		工具快速生成 Englishpia。
// 
//		Ted 对 Englishpia 有格式要求，
//		一条合格的 Englishpia 词条记录应该符合以下要求：
//		1. 有记录时间
//		2. 音标
//		3. 释义
//		4. 例句，至少两个，例句中的对应单词要用红笔高亮
// 
//		此程序生成的 Englishpia 满足以上要求。
// 
//		由于时间紧迫，为了快速、方便地开发，生成的 Englishpia
//		以 HTML 的形式显示。
// 
//		P.S.英语单词的详细信息来自 dict.cn
// 
// 操作说明：
//		运行程序，在 cmd 中输入要查询的单词，输入每个单词之后
//		都要回车，若输入 ‘/’ 字符后再回车即可开始查询。
// 
// 作者：huidong <huidong_mail@163.com>
// 版本：Ver 0.4
// 最后修改： 2021.10.29
//
//

#include <stdio.h>
#include <conio.h>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <windows.h>
#include <wininet.h>
#include <comutil.h>
#include <algorithm>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "comsuppw.lib")
using namespace std;

////// 全局变量

// 版本信息
char g_strInfo[] = "Ver 0.4 | 2021.10.29";

// 基准日期
int m_month = -1;
int m_day	= -1;
int m_hour	= -1;
int m_min	= -1;

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
	string word;				// 单词
	string pronunciation;		// 音标
	vector<string> meaning;     // 释义数组
	vector<ExampleSentence> sentences;   // 例句数组

}EWORD;

////// 函数定义

string wtos(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

wstring stow(const string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

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

// 替换字符串中的所有指定字符串
string& replace_all(string& str, const  string& old_value, const  string& new_value)
{
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return   str;
}

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

// 获取文件内容
string GetFile(const char* filepath)
{
	ifstream t(filepath);
	string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	t.close();

	if (str.size() <= 0)
	{
		string error = "open " + (string)filepath + " error.";
		OutError(error);
	}

	return str;
}

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

// 显示帮助页面
void PrintHelpPage()
{
	printf(
		"\n\n"
		"-------------------------------------------------------------------------\n"
		"TedEnglishpia_helper 指令说明\n"
		"\n"
		"在 '/' 后可以附加如下指令：\n"
		"\n"
		"help            显示帮助页面\n"
		"file            选择单词列表文件\n"
		"\n"
		"ab-cd (ef:gh)   设置基准时间，也就是显示在 Englishpia 上的抄录日期\n"
		"                ab-cd 是基准月份和天数，\n"
		"                ef:gh 是可选添加的，用于指定基准小时和分钟\n"
		"                若数字是个位数，可以自由选择是否补 0 凑两位数。\n"
		"                示例：\n"
		"                /9-20\n"
		"                /12-1 11:3\n"
		"                /01-07 07:09\n"
		"\n"
		"直接在单词后面加 '/' 可以直接开始查询\n"
		"-------------------------------------------------------------------------\n"
		"\n"
	);
}

// 选择文件
// isSave标志着是否为保存模式
const WCHAR* SelectFile(bool isSave = false)
{
	OPENFILENAME ofn;
	static WCHAR szFile[256];
	static WCHAR szFileTitle[256];
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetConsoleWindow();
	ofn.lpstrFilter = L"要查询的单词文件 (*.txt)\0*.txt;\0All File(*.*)\0*.*;\0";
	ofn.lpstrDefExt = L"txt";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	wchar_t oldpath[1024] = { 0 };
	_wgetcwd(oldpath, 1024);

	if (isSave)
	{
		if (GetSaveFileName(&ofn))
		{
			SetCurrentDirectory(oldpath);
			return szFile;
		}
	}
	else
	{
		if (GetOpenFileName(&ofn))
		{
			SetCurrentDirectory(oldpath);
			return szFile;
		}
	}

	return L"";
}

// 从文件中获取要查询的单词
vector<string> GetWordsInFile(string path)
{
	vector<string> vecWords;
	FILE* fp;

	if (fopen_s(&fp, path.c_str(), "r") != 0)
	{
		OutError("打开文件失败", false);
		return vecWords;
	}

	const int size = 1024;
	char buf[size] = { 0 };
	while (fscanf_s(fp, "%s\n", buf, size) != -1)
	{
		vecWords.push_back(buf);
		memset(buf, 0, size);
	}

	fclose(fp);
	fp = NULL;

	return vecWords;
}

// 获取用户输入的所有单词
vector<string> GetInputWords()
{
	vector<string> vecStrings;
	while (true)
	{
		char buf[256] = { 0 };
		gets_s(buf, 256);

		// 结束
		if (buf[0] == '/')
		{
			// 含参数
			if (strlen(buf) > 1)
			{
				string strCommand = buf + 1;

				if (strCommand == "help")
				{
					PrintHelpPage();
				}
				
				else if (strCommand == "file")
				{
					vecStrings = GetWordsInFile(wtos(SelectFile()));
					if (vecStrings.size() > 0)
					{
						return vecStrings;
					}
				}

				// 设置基准日期
				else
				{
					int offset = 0;

					string strMonth, strDay, strHour, strMinute;

					// 读取日期

					// "月份-天数" 格式或 "月份.天数" 格式
					for (; offset < (int)strCommand.size() && strCommand[offset] != '-' && strCommand[offset] != '.'; strMonth += strCommand[offset], offset++);
					for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ' '; strDay += strCommand[offset], offset++);

					m_month = atoi(strMonth.c_str());
					m_day = atoi(strDay.c_str());

					printf("\n已设置基准时间为：%02d 月 %02d 日 ", m_month, m_day);

					// 读取时间
					if (offset < (int)strCommand.size())
					{
						// "小时:分钟" 格式
						for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ':'; strHour += strCommand[offset], offset++);
						for (offset++; offset < (int)strCommand.size(); strMinute += strCommand[offset], offset++);

						m_hour = atoi(strHour.c_str());
						m_min = atoi(strMinute.c_str());

						printf("%02d 点 %02d 分", m_hour, m_min);
					}

					// 设置日期却未设置时间，则不使用基准时间
					else
					{
						m_hour = -1;
						m_min = -1;
					}

					printf("\n");
				}
			}

			// 若没有输入单词，就不退出循环
			if (vecStrings.size() > 0)
			{
				break;
			}
		}

		else
		{
			if (strlen(buf) > 0)
			{
				// 直接结束
				if (buf[strlen(buf) - 1] == '/')
				{
					buf[strlen(buf) - 1] = '\0';
					vecStrings.push_back(buf);
					break;
				}
				// 正常输入
				else
				{
					vecStrings.push_back(buf);
				}
			}
		}
	}

	return vecStrings;
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
	sys.wMonth	= m_month	<= 0 ? sys.wMonth	: m_month;
	sys.wDay	= m_day		<= 0 ? sys.wDay		: m_day;
	sys.wHour	= m_hour	< 0 ? sys.wHour		: m_hour;
	sys.wMinute	= m_min		< 0 ? sys.wMinute	: m_min;

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
			en.replace(pos, eword.word.length(), "<font color=red>" + (string)dest + "</font>");
		}
		
		strSentences += buf + (string)". " + en + "<div class=\"return\"></div>&nbsp;&nbsp;" + eword.sentences[i].zh;
		if (i + 1 < (int)eword.sentences.size())
		{
			strSentences += "<div class=\"return\"></div>";
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

// 根据单词构建 Ted Englishpia，返回构建出的 HTML 内容
string BuildEnglishPia(vector<string> vecStrings)
{
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
	replace_all(strModuleIndex, "{TIME}", path);
	replace_all(strModuleIndex, "{NUM}", chNum);
	replace_all(strModuleIndex, "{CONTENT}", strWordsHTML);

	return strModuleIndex;
}

// 输出 HTML 内容为文件，返回文件路径
string OutputHTML(string html)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char path[256] = { 0 };
	sprintf_s(path, 256, "./res/temp/TedEnglishpia_%04d-%02d-%02d-%02d-%02d-%02d.html", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	ofstream out(path);
	out.write(html.c_str(), html.size());
	out.close();

	return path;
}

// 打开 HTML
void OpenHTML(string path)
{
	system(("start " + path).c_str());
}

int main()
{
	srand((UINT)time(0));

	printf(
		"TedEnglishpia_helper by huidong %s\n\n"
		"根据单词自动生成 Ted 的 Englishpia，提高作业效率\n\n"
		"输入待查询单词，每个单词结束后回车，最后输入 '/' 并回车开始查询。\n"
		"'/' 后可以带参数，请输入 '/help' 查看更多。\n\n"
		"在此输入待查询单词：\n\n", g_strInfo);

	while (true)
	{
		OpenHTML(
			OutputHTML(
				BuildEnglishPia(
					GetInputWords())));
		printf("\n done.\n\n");
	}

	return 0;
}

