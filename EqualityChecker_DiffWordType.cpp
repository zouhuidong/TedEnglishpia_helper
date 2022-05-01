#include "EqualityChecker_DiffWordType.h"
using namespace std;

namespace EChecker_DiffWT {

	vector<WordNames> vecWords;

	vector<string> prefix;
	vector<string> suffix;
	vector<string> affix;

	// 获取文件内容
	string GetFile(const char* filepath)
	{
		ifstream t(filepath);
		string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
		t.close();

		if (str.size() <= 0)
		{
			printf("\n\nopen %s error.", filepath);
			_getch();
			exit(-1);
		}

		return str;
	}

	// 是否为英文字母（或空格）
	bool isEnglishAlphabet(char c)
	{
		return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
	}

	void LoadWords(string res)
	{
		vector<string> list;
		string name;
		for (int i = 0; i < (int)res.size(); i++)
		{
			if (isEnglishAlphabet(res[i]))
			{
				name += res[i];
			}
			else if (name.size() > 0)
			{
				list.push_back(name);
				name = "";
			}
			if (res[i] == '\r' || res[i] == '\n')
			{
				vecWords.push_back(list);
				list = {};
			}
		}
	}

	vector<string> LoadAffix(string res)
	{
		stringstream s(res);
		string affix;
		vector<string> vec;
		while (getline(s, affix))
		{
			vec.push_back(affix);
		}
		return vec;
	}

	void InitChecker(
		const char* pszNounsDicPath,
		const char* pszVerbsDicPath,
		const char* pszPrefixDicPath,
		const char* pszSuffixDicPath
	)
	{
		string strNounsDic = GetFile(pszNounsDicPath);
		string strVerbsDic = GetFile(pszVerbsDicPath);
		string strPrefixDic = GetFile(pszPrefixDicPath);
		string strSuffixDic = GetFile(pszSuffixDicPath);

		LoadWords(strNounsDic);
		LoadWords(strVerbsDic);
		prefix = LoadAffix(strPrefixDic);
		suffix = LoadAffix(strSuffixDic);
		affix.insert(affix.end(), prefix.begin(), prefix.end());
		affix.insert(affix.end(), suffix.begin(), suffix.end());
	}

	// 获取两字符串的最大重合子串
	// s1, s2 原字符串
	// strCoincidence 重合字符串
	// begin1, end1 在 s1 上的重合位置
	// begin2, end2 在 s2 上的重合位置
	// 返回重合长度
	int StringCoincidence(string s1, string s2, string& strCoincidence, int& begin1, int& end1, int& begin2, int& end2)
	{
		begin1 = begin2 = end1 = end2 = 0;
		int l1 = (int)s1.size(), l2 = (int)s2.size();
		for (int i = 0; i < l1; i++)
		{
			for (int j = 0; j < l2; j++)
			{
				if (s2[j] == s1[i])
				{
					int k = 1;
					for (; i + k < l1 && j + k < l2; k++)
					{
						if (s2[j + k] != s1[i + k])
						{
							break;
						}
					}
					if (k > end1 - begin1)
					{
						begin1 = i;
						end1 = i + k;
						begin2 = j;
						end2 = j + k;
						j += k;
					}
				}
			}
		}
		strCoincidence = s1.substr(begin1, end1 - begin1);
		return end1 - begin1;
	}

	bool AutoCheck(string s1, string s2)
	{
		string strCoincidence;
		int begin[2], end[2], l1 = (int)s1.size(), l2 = (int)s2.size();
		int ll = l1 > l2 ? l1 : l2;
		int pl = (l1 + l2) / 2;
		int sum = StringCoincidence(s1, s2, strCoincidence, begin[0], end[0], begin[1], end[1]);
		int lbegin = l1 > l2 ? begin[0] : begin[1], lend = l1 > l2 ? end[0] : end[1];
		string diff[2], res = l1 > l2 ? s1 : s2;
		diff[0] = res.substr(0, lbegin);
		diff[1] = res.substr(lend, res.size());
		bool flag_affix = false;

		if (sum < pl * 0.68)
		{
			return false;
		}

		for (int i = 0; i < (int)affix.size(); i++)
		{
			string sc;

			if (diff[0] == affix[i] || diff[1] == affix[i])
			{
				flag_affix = true;
				break;
			}
			if (affix[i].size() > 2 && (diff[0].size() > 1 || diff[1].size() > 1))
			{
				if (StringCoincidence(diff[0], affix[i], sc, begin[0], end[0], begin[1], end[1]) >
					(int)(diff[0].size() > affix[i].size() ? diff[0] : affix[i]).size() - 1 ||
					StringCoincidence(diff[1], affix[i], sc, begin[0], end[0], begin[1], end[1]) >
					(int)(diff[1].size() > affix[i].size() ? diff[1] : affix[i]).size() - 1)
				{
					flag_affix = true;
					break;
				}
			}
		}

		return flag_affix;
	}

	bool CheckWordsEquality(string s1, string s2)
	{
		if (s1 == s2) return true;

		int l1 = -2, l2 = -2;
		for (int i = 0; i < (int)vecWords.size(); i++)
		{
			for (int j = 0; j < (int)vecWords[i].size(); j++)
			{
				if (s1 == vecWords[i][j])
				{
					l1 = i;
				}
				if (s2 == vecWords[i][j])
				{
					l2 = i;
				}
				if (l1 >= 0 && l2 >= 0)
				{
					i = (int)1e9;
					break;
				}
			}
		}

		if (l1 == l2 && l1 >= 0)
		{
			return true;
		}
		/*else if ((l1 + 1) * (l2 + 1) < 0)
		{
			return AutoCheck(s1, s2);
		}
		else
		{
			return false;
		}*/
		return AutoCheck(s1, s2);
	}

}
