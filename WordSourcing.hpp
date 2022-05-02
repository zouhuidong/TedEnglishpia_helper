///////////////////////////////////////////////////
//
//	WordSourcing
//	�ı���Դ����ģ��
// 
//	�ó�����Խ�ָ���ĵ��ʱ��е�ÿ�����ʣ�
//	��ָ���������н�����Դ��
//	�ó������������Ӣ��̲�
// 
//	�ļ���ʽע�����
//	�����ļ�Ҫ�� ANSI ��ʽ���������Ļ����룻
//	�ӽ̲� pdf �ļ��и�������������Ĭ��Ϊ
//	Unicode ���룬��Ҫ�ֶ�ת ANSI��ת����
//	���ܻ���ֺܶ��ʺţ�������ᡣ
//	���Ƶ��ʱ�ʱ��ע�ⲻҪ�� "Unit 1" һ���
//	�ı������ϡ�����ʱ���ܻḴ�Ƶ� pdf �ļ���
//	���½ǵ�ˮӡ�����¸��Ƴ�һЩ�����һ��ҵ�
//	���֣���Ҫ�ֶ�ɾ������ο�ʾ���ļ��ĸ�ʽ��
//	������������ʱ��ֱ�Ӹ�����������Ԫ������
//	���ݼ��ɡ�
// 
//

#pragma once

#include "stdafx.hpp"
#include "file.hpp"
#include "string_func.hpp"
#include "EqualityChecker_DiffWordType.h"

// ����
struct Sentence
{
	string sentence;
	vector<string> words;
};

// �Ƿ�ΪӢ����ĸ����ո�
bool isEnglishAlphabet(char c)
{
	return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == ' ';
}

// ���з�
bool isReturn(char c)
{
	return c == '\r' || c == '\n';
}

// ���÷�
bool isUselessChar(char c)
{
	return isReturn(c) || c == '\t' || c == '?' /* ���� */;
}

// ����
bool isUnicodeChar(char c)
{
	return c < 0;
}

// �жϽ������������Ƿ�Ϊ����
bool isParaphrase(string str)
{
	for (int i = 0; i < (int)str.size(); i++)
	{
		if (!isEnglishAlphabet(str[i]))
		{
			if (str[i] == '.')
			{
				return true;
			}
			else if (isUselessChar(str[i]))
			{
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

// ��ȡ���ʱ�ע
// ����� '(' ��ʼ���ַ���
string GetRemark(string res, int& index)
{
	string str;
	int i = 1;
	for (; i < (int)res.size() && res[i] != ')'; i++)
	{
		str += res[i];
	}
	index += i;
	return str;
}

// ��������
// ����� '/' ��ʼ���ַ���
void SkipPhoneticSymbol(string res, int& index)
{
	int i = 1;
	for (; i < (int)res.size() && res[i] != '/'; i++);
	index += i;
}

// ��ȡ����
// ��������忪ʼ���ַ���
string GetParaphrase(string res, int& index)
{
	string str;
	int i = 0;
	for (; i < (int)res.size(); i++)
	{
		// �ڶ���һЩ�����ǰ���£����к�ֱ�ӳ���Ӣ��
		// �жϸ�Ӣ���Ƿ�Ϊ����Ŀ�ͷ�����������˵�������Ѿ�����
		if (isUselessChar(res[i]))
		{
			// �ո��������
			if (res[i] == '?')
			{
				str += ' ';
			}
			else if (str.size() > 2 && i + 1 < (int)res.size() &&
				isEnglishAlphabet(res[i + 1]) && res[i + 1] != ' ' && !isParaphrase(&res[i + 1]))
			{
				break;
			}
		}
		else if (res[i] == '/')
		{
			// ��������
			SkipPhoneticSymbol(&res[i], i);
		}
		else
		{
			str += res[i];
		}
	}
	index += i;
	return str;
}

// �޼��ַ�������Ŀհ�
string Trim(string str)
{
	bool flag = false;
	int size = (int)str.size(), end = size, k = 1;
	int i1 = 0, i2 = end;
	for (int i = 0;; i += k)
	{
		if ((k == 1 && i >= end) || (k == -1 && i < 0))
		{
			break;
		}
		if (str[i] == ' ')
		{
			if (k == 1)	i1 = i + 1;
			else		i2 = i;
			flag = true;
		}
		else if (flag || i == 0 || i == size - 1)
		{
			if (k == 1)
			{
				flag = false;
				i = end;
				end = 0;
				k = -1;
			}
			else
			{
				break;
			}
		}
	}
	return str.substr(i1, i2 - i1);
}

vector<EWORD> GetList(string strList)
{
	vector<EWORD> vecWords;
	EWORD word;
	bool end = false;
	int size = (int)strList.size();
	for (int i = 0; i < size; i++)
	{
		if (isEnglishAlphabet(strList[i]))
		{
			word.name += strList[i];
		}

		else if (isUselessChar(strList[i]))
		{

		}

		// ͻȻ�������ģ�˵��ֱ�ӽ�������
		else if (isUnicodeChar(strList[i]))
		{
			word.meaning.push_back(Trim(GetParaphrase(&strList[i], i)));
			end = true;
		}

		else
		{
			switch (strList[i])
			{
				// �����ڲ����Ա�ע
			case '(':
				word.remark = GetRemark(&strList[i], i);
				break;

			case '/':
				// ��������
				SkipPhoneticSymbol(&strList[i], i);

				// ���������δ���ţ��ҳ���Ӣ�ģ�����������壨Ӣ�Ĵ��ԣ���Ҳ���ܻ��ڵ�����
				if (!(i + 1 < size && isUselessChar(strList[i + 1])))
				{
					if ((i += 2) < size && isEnglishAlphabet(strList[i]))
					{
						// ���������������壬�ͼ���������
						if (!isParaphrase(&strList[i]))
						{
							break;
						}
					}
				}
				else
				{
					i += 2;
				}

				// �������������壬��ֱ�ӳ������ģ����ɶ�ȡ����
				word.meaning.push_back(Trim(GetParaphrase(&strList[i], i)));
				end = true;
				break;
			}
		}

		if (end)
		{
			word.name = Trim(word.name);
			vecWords.push_back(word);
			word = {};
			end = false;
		}
	}
	return vecWords;
}

//////// ���´���

// ����ƥ�䣨������ƥ�� Unicode �ַ���
bool isSymbol(string res, string sym)
{
	int s1 = (int)res.size(), s2 = (int)sym.size();
	if (s1 < s2)
	{
		return false;
	}
	for (int i = 0; i < s2; i++)
	{
		if (res[i] != sym[i])
		{
			return false;
		}
	}
	return true;
}

// ��ӿո񣬲���֤���ظ��ո�
void AddWhitespace(string& str)
{
	int size = (int)str.size();
	if (size > 0 && str[size - 1] != ' ')
	{
		str += ' ';
	}
}

// ���·ִ�
vector<Sentence> AnalyzeArticle(string strArticle)
{
	vector<Sentence> vecSentences;
	Sentence sentence;
	string strWord;
	bool flagS = false, flagW = false;
	const int nSymbolsNum = 4, nIllegalSymbolsNum = 6;
	string strSymbols[nSymbolsNum] = { "��","��","��","��" };
	string strIllegalSymbols[] = { "��","��","��","��","��","ͳ" };

	for (int i = 0; i < (int)strArticle.size(); i++)
	{
		if (!isReturn(strArticle[i]))
		{
			if (strArticle[i] == ' ')
			{
				AddWhitespace(sentence.sentence);
			}
			else
			{
				// �����Ƿ�����ʱֹͣ
				if (isUnicodeChar(strArticle[i]))
				{
					for (int j = 0; j < nIllegalSymbolsNum; j++)
					{
						if (isSymbol(&strArticle[i], strIllegalSymbols[j]))
						{
							flagS = true;
							goto add_end;
						}
					}
				}

				sentence.sentence += strArticle[i];
			}
		}
		else
		{
			AddWhitespace(sentence.sentence);
		}
	add_end:

		// Ъֹ���ж�
		for (int j = 0; j < nSymbolsNum; j++)
		{
			if (isSymbol(&strArticle[i], strSymbols[j]))
			{
				flagW = true;
				break;
			}
		}

		if (!flagW && !flagS)
		{
			switch (strArticle[i])
			{
			case '.': case '?': case '!':
				flagS = true;
			case ' ': case ',': case ':': case';':
			case '(': case ')': case '/': case '\'': case '\"':
			case '\r': case '\n': case '\t':
				flagW = true;
				break;
			default:
				strWord += strArticle[i];
				break;
			}
		}

		if (flagW)
		{
			if (strWord.size() > 0)
			{
				sentence.words.push_back(strWord);
				strWord = "";
			}
			flagW = false;
		}
		if (flagS)
		{
			if (sentence.sentence.size() > 1)
			{
				vecSentences.push_back(sentence);
				sentence = {};
			}
			flagS = false;
		}
	}
	return vecSentences;
}

// ƥ�����
void MatchSentence(vector<EWORD>& vecList, vector<Sentence> vecSentences)
{
	for (int i = 0; i < (int)vecList.size(); i++)
	{
		if (vecList[i].name.find(" ") != vecList[i].name.npos)
		{
			continue;
		}
		for (int j = 0; j < (int)vecSentences.size(); j++)
		{
			for (int k = 0; k < (int)vecSentences[j].words.size(); k++)
			{
				if (strlwr(vecSentences[j].words[k]) == strlwr(vecList[i].name))
				{
					ExampleSentence es = { vecSentences[j].sentence };
					vecList[i].sentences.push_back(es);
					j = (int)1e9;
					break;
				}
			}
		}
	}
	for (int i = 0; i < (int)vecList.size(); i++)
	{
		if (vecList[i].name.find(" ") != vecList[i].name.npos)
		{
			continue;
		}
		if (vecList[i].sentences.size() == 0)
		{
			for (int j = 0; j < (int)vecSentences.size(); j++)
			{
				for (int k = 0; k < (int)vecSentences[j].words.size(); k++)
				{
					if (EChecker_DiffWT::CheckWordsEquality(strlwr(vecSentences[j].words[k]), strlwr(vecList[i].name)))
					{
						ExampleSentence es = { vecSentences[j].sentence };
						vecList[i].sentences.push_back(es);
						printf("%s -> %s\n", vecList[i].name.c_str(), vecSentences[j].words[k].c_str());
						j = (int)1e9;
						break;
					}
				}
			}
		}
	}
}

// ������Դ
vector<EWORD> WordSourcing(const char* list_path, const char* article_path)
{
	static bool flag = false;
	if (!flag)
	{
		EChecker_DiffWT::InitChecker(
			"./dic/irregular_nouns.txt",
			"./dic/irregular_verbs.txt",
			"./dic/prefix.txt",
			"./dic/suffix.txt"
		);
		flag = true;
	}

	printf("\n\n--- WordSourcing ---\n");

	string strList = GetFile(list_path);
	string strArticle = GetFile(article_path);

	vector<EWORD> vecWords = GetList(strList);
	vector<Sentence> vecSentences = AnalyzeArticle(strArticle);
	MatchSentence(vecWords, vecSentences);
	printf("\n");

	return vecWords;
}
