//////////////////////////////////////////////////////////////////////////
//
//	TedEnglishpia_helper
//	
//	���뻷����
//		VisualStudio 2019
// 
//	��Ŀ��飺
//		�ҵĸ���Ӣ����ʦ Ted Ҫ������ÿ������� 5 �����ʣ�
//		����¼�� Englishpia �ϣ�Ҳ�����Լ�����Ӣ��ʵ䡣
// 
//		�������߷�����վ���Ű����⣬ͨ����ѯһ�����ʵĸ���
//		��Ϣ�ȽϺķ�ʱ�䣬���ֵ��Ч������ͣ�������д��
//		���߿������� Englishpia��
// 
//		Ted �� Englishpia �и�ʽҪ��
//		һ���ϸ�� Englishpia ������¼Ӧ�÷�������Ҫ��
//		1. �м�¼ʱ��
//		2. ����
//		3. ����
//		4. ���䣬���������������еĶ�Ӧ����Ҫ�ú�ʸ���
// 
//		�˳������ɵ� Englishpia ��������Ҫ��
// 
//		����ʱ����ȣ�Ϊ�˿��١�����ؿ��������ɵ� Englishpia
//		�� HTML ����ʽ��ʾ��
// 
//		P.S.Ӣ�ﵥ�ʵ���ϸ��Ϣ���� dict.cn
// 
// ����˵����
//		���г����� cmd ������Ҫ��ѯ�ĵ��ʣ�����ÿ������֮��
//		��Ҫ�س��������� ��/�� �ַ����ٻس����ɿ�ʼ��ѯ��
// 
// ���ߣ�huidong <huidong_mail@163.com>
// �汾��Ver 0.4
// ����޸ģ� 2021.10.29
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

////// ȫ�ֱ���

// �汾��Ϣ
char g_strInfo[] = "Ver 0.4 | 2021.10.29";

// ��׼����
int m_month = -1;
int m_day	= -1;
int m_hour	= -1;
int m_min	= -1;

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

////// ��������

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

// �滻�ַ����е�����ָ���ַ���
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

// ��ȡ�ļ�����
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

// ��ʾ����ҳ��
void PrintHelpPage()
{
	printf(
		"\n\n"
		"-------------------------------------------------------------------------\n"
		"TedEnglishpia_helper ָ��˵��\n"
		"\n"
		"�� '/' ����Ը�������ָ�\n"
		"\n"
		"help            ��ʾ����ҳ��\n"
		"file            ѡ�񵥴��б��ļ�\n"
		"\n"
		"ab-cd (ef:gh)   ���û�׼ʱ�䣬Ҳ������ʾ�� Englishpia �ϵĳ�¼����\n"
		"                ab-cd �ǻ�׼�·ݺ�������\n"
		"                ef:gh �ǿ�ѡ��ӵģ�����ָ����׼Сʱ�ͷ���\n"
		"                �������Ǹ�λ������������ѡ���Ƿ� 0 ����λ����\n"
		"                ʾ����\n"
		"                /9-20\n"
		"                /12-1 11:3\n"
		"                /01-07 07:09\n"
		"\n"
		"ֱ���ڵ��ʺ���� '/' ����ֱ�ӿ�ʼ��ѯ\n"
		"-------------------------------------------------------------------------\n"
		"\n"
	);
}

// ѡ���ļ�
// isSave��־���Ƿ�Ϊ����ģʽ
const WCHAR* SelectFile(bool isSave = false)
{
	OPENFILENAME ofn;
	static WCHAR szFile[256];
	static WCHAR szFileTitle[256];
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetConsoleWindow();
	ofn.lpstrFilter = L"Ҫ��ѯ�ĵ����ļ� (*.txt)\0*.txt;\0All File(*.*)\0*.*;\0";
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

// ���ļ��л�ȡҪ��ѯ�ĵ���
vector<string> GetWordsInFile(string path)
{
	vector<string> vecWords;
	FILE* fp;

	if (fopen_s(&fp, path.c_str(), "r") != 0)
	{
		OutError("���ļ�ʧ��", false);
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

// ��ȡ�û���������е���
vector<string> GetInputWords()
{
	vector<string> vecStrings;
	while (true)
	{
		char buf[256] = { 0 };
		gets_s(buf, 256);

		// ����
		if (buf[0] == '/')
		{
			// ������
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

				// ���û�׼����
				else
				{
					int offset = 0;

					string strMonth, strDay, strHour, strMinute;

					// ��ȡ����

					// "�·�-����" ��ʽ�� "�·�.����" ��ʽ
					for (; offset < (int)strCommand.size() && strCommand[offset] != '-' && strCommand[offset] != '.'; strMonth += strCommand[offset], offset++);
					for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ' '; strDay += strCommand[offset], offset++);

					m_month = atoi(strMonth.c_str());
					m_day = atoi(strDay.c_str());

					printf("\n�����û�׼ʱ��Ϊ��%02d �� %02d �� ", m_month, m_day);

					// ��ȡʱ��
					if (offset < (int)strCommand.size())
					{
						// "Сʱ:����" ��ʽ
						for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ':'; strHour += strCommand[offset], offset++);
						for (offset++; offset < (int)strCommand.size(); strMinute += strCommand[offset], offset++);

						m_hour = atoi(strHour.c_str());
						m_min = atoi(strMinute.c_str());

						printf("%02d �� %02d ��", m_hour, m_min);
					}

					// ��������ȴδ����ʱ�䣬��ʹ�û�׼ʱ��
					else
					{
						m_hour = -1;
						m_min = -1;
					}

					printf("\n");
				}
			}

			// ��û�����뵥�ʣ��Ͳ��˳�ѭ��
			if (vecStrings.size() > 0)
			{
				break;
			}
		}

		else
		{
			if (strlen(buf) > 0)
			{
				// ֱ�ӽ���
				if (buf[strlen(buf) - 1] == '/')
				{
					buf[strlen(buf) - 1] = '\0';
					vecStrings.push_back(buf);
					break;
				}
				// ��������
				else
				{
					vecStrings.push_back(buf);
				}
			}
		}
	}

	return vecStrings;
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

// ��ȡһ�������� Ted Englishpia ��ʽ�Ű�� HTML ����
// eword	������Ϣ
// strModuleHTML	ģ�� HTML ҳ������
// count	��ǰ�����ڸô����е�����
string GetWordHTML(EWORD eword, string strModuleHTML, int count)
{
	// �������ĺ�ʱ�ۼƣ����ӣ�
	static int nTimeCost = 0;

	// ��¼ÿ����������ʱ�䣨���ӣ�
	int spent_per_word = 4;

	// ��¼��������ʱ������ƫ���С�����ӣ�
	int deviation_per_word = 1;

	// HTML ����
	string strHTML = strModuleHTML;

	// ������µĴ��飬�����¿�ʼ�����ʱ�ۼ�
	if (count == 0)
	{
		nTimeCost = 0;
	}

	// time
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	// ��׼ʱ������
	sys.wMonth	= m_month	<= 0 ? sys.wMonth	: m_month;
	sys.wDay	= m_day		<= 0 ? sys.wDay		: m_day;
	sys.wHour	= m_hour	< 0 ? sys.wHour		: m_hour;
	sys.wMinute	= m_min		< 0 ? sys.wMinute	: m_min;

	// ���㳭¼�˵��ʵ�ʱ��
	int spend = spent_per_word + 
				/* offset */
				((rand() % 3 - 1) * rand() % (deviation_per_word + 1));

	sys.wMinute += nTimeCost;
	nTimeCost += spend;			// ʱ���ۼ�

	// ʱ���λ
	sys.wHour += sys.wMinute / 60;
	sys.wDay += sys.wHour / 24;
	sys.wMinute %= 60;
	sys.wHour %= 24;

	char bufTime[128] = { 0 };
	sprintf_s(bufTime, 128, "%02d.%02d %02d:%02d", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute);

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
			en.replace(pos, eword.word.length(), "<font color=red>" + (string)dest + "</font>");
		}
		
		strSentences += buf + (string)". " + en + "<div class=\"return\"></div>&nbsp;&nbsp;" + eword.sentences[i].zh;
		if (i + 1 < (int)eword.sentences.size())
		{
			strSentences += "<div class=\"return\"></div>";
		}
	}

	// ��д����
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

// ���ݵ��ʹ��� Ted Englishpia�����ع������� HTML ����
string BuildEnglishPia(vector<string> vecStrings)
{
	// ��ȡ���е��ʵ���ϸ��Ϣ
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

	// ��д����
	replace_all(strModuleIndex, "{TIME}", path);
	replace_all(strModuleIndex, "{NUM}", chNum);
	replace_all(strModuleIndex, "{CONTENT}", strWordsHTML);

	return strModuleIndex;
}

// ��� HTML ����Ϊ�ļ��������ļ�·��
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

// �� HTML
void OpenHTML(string path)
{
	system(("start " + path).c_str());
}

int main()
{
	srand((UINT)time(0));

	printf(
		"TedEnglishpia_helper by huidong %s\n\n"
		"���ݵ����Զ����� Ted �� Englishpia�������ҵЧ��\n\n"
		"�������ѯ���ʣ�ÿ�����ʽ�����س���������� '/' ���س���ʼ��ѯ��\n"
		"'/' ����Դ������������� '/help' �鿴���ࡣ\n\n"
		"�ڴ��������ѯ���ʣ�\n\n", g_strInfo);

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

