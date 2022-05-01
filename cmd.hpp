////////////////////////////////
//
//	cmd.hpp
//	处理用户指令
//

#pragma once

#include "stdafx.hpp"
#include "file.hpp"
#include "WordSourcing.hpp"
#include "translator.hpp"
#include "EncodingConvert.hpp"

// 判断一命令的命令根是否符合指定字符串
// 例如命令 "light:on"，命令根是 "light"，子命令是 "on"
bool isCommandRoot(string pCmd, string root)
{
	if (pCmd.size() < root.size())
	{
		return false;
	}

	char buf[1024] = { 0 };
	memcpy_s(buf, 1024, pCmd.c_str(), root.size());
	return buf == root;
}

// 获得一命令的子命令
string GetChildCommand(string pCmd)
{
	for (int i = 0; i < (int)pCmd.size(); i++)
	{
		if (pCmd[i] == ':' && i + 1 < (int)pCmd.size())
		{
			return &pCmd[i + 1];
		}
	}
	return "";
}

// 显示帮助页面
void PrintHelpPage()
{
	printf(
		"\n\n"
		"-------------------------------------------------------------------------\n"
		"TedEnglishpia_helper 指令说明\n"
		"\n"
		"直接在单词后面加 '/' 可以直接开始查询\n"
		"\n"
		"如果以 '/' 开头，其后可以附加如下指令：\n"
		"\n"
		"help            显示帮助页面\n"
		"file            根据单词列表文件（txt）进行生成\n"
		"\n"
		"ab-cd (ef:gh)   设置基准时间，也就是显示在 Englishpia 上的抄录日期\n"
		"                ab-cd 基准月份和天数\n"
		"                ef:gh 基准小时和分钟（可选）\n"
		"                示例：\n"
		"                /9-20\n"
		"                /01-07 07:09\n"
		"\n"
		"mode            更改当前单词文档页面模式（默认为标准模式）\n"
		"mode:standard   设置单词文档页面为标准模式\n"
		"mode:simple     设置单词文档页面为简易模式\n"
		"mode:print      设置单词文档页面为打印模式\n"
		"\n"
		"directory       更改当前输出单词目录模式（默认不输出）（建议在打印模式下使用）\n"
		"directory:on    启用输出单词目录功能\n"
		"directory:off   禁用输出单词目录功能\n"
		"\n"
		"WordSourcing    查找文本词源\n"
		"\n"
		"-------------------------------------------------------------------------\n"
		"\n"
	);
}

void SetStandardTime(string strCommand)
{
	int offset = 0;

	string strMonth, strDay, strHour, strMinute;

	// 读取日期

	// "月份-天数" 格式或 "月份.天数" 格式
	for (; offset < (int)strCommand.size() && strCommand[offset] != '-' && strCommand[offset] != '.'; strMonth += strCommand[offset], offset++);
	for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ' '; strDay += strCommand[offset], offset++);

	g_month = atoi(strMonth.c_str());
	g_day = atoi(strDay.c_str());

	printf("\n已设置基准时间为：%02d 月 %02d 日 ", g_month, g_day);

	// 读取时间
	if (offset < (int)strCommand.size())
	{
		// "小时:分钟" 格式
		for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ':'; strHour += strCommand[offset], offset++);
		for (offset++; offset < (int)strCommand.size(); strMinute += strCommand[offset], offset++);

		g_hour = atoi(strHour.c_str());
		g_min = atoi(strMinute.c_str());

		printf("%02d 点 %02d 分", g_hour, g_min);
	}

	// 设置日期却未设置时间，则不使用基准时间
	else
	{
		g_hour = -1;
		g_min = -1;
	}

	printf("\n");
}

void SetDocMode(string strCommand)
{
	string strChildCmd = GetChildCommand(strCommand);
	if (strChildCmd == "")
	{
		if (g_nDocMode < g_nDocTypesNum - 1)
		{
			g_nDocMode++;
		}
		else
		{
			g_nDocMode = 0;
		}
	}
	else if (strChildCmd == "standard")
	{
		g_nDocMode = DOC_standard;
	}
	else if (strChildCmd == "simple")
	{
		g_nDocMode = DOC_simple;
	}
	else if (strChildCmd == "print")
	{
		g_nDocMode = DOC_print;
	}
	else
	{
		OutError("未知指令", false);
		return;
	}

	switch (g_nDocMode)
	{
	case DOC_standard:	printf("\n已设置输出单词文档模式为：标准模式\n");	break;
	case DOC_simple:	printf("\n已设置输出单词文档模式为：简易模式\n");	break;
	case DOC_print:		printf("\n已设置输出单词文档模式为：打印模式\n"
		"\n如需打印，建议将网页内容无格式地复制到 Word 文档，使用双栏布局进行打印，以提高纸张利用率\n");
		break;
	}
}

void SetDirectoryMode(string strCommand)
{
	string strChildCmd = GetChildCommand(strCommand);
	if (strChildCmd == "")
	{
		g_bDirectoryMode = !g_bDirectoryMode;
	}
	else if (strChildCmd == "on")
	{
		g_bDirectoryMode = true;
	}
	else if (strChildCmd == "off")
	{
		g_bDirectoryMode = false;
	}
	else
	{
		OutError("未知指令", false);
		return;
	}

	if (g_bDirectoryMode)
	{
		printf("\n启用 输出单词目录功能\n");
	}
	else
	{
		printf("\n禁用 输出单词目录功能\n");
	}
}

// 将溯源得来的单词转换编码
void ConvertEncoding(vector<EWORD>& vecWords)
{
	for (int i = 0; i < (int)vecWords.size(); i++)
	{
		vecWords[i].remark = EncodingConvert::ToUTF8(stow(vecWords[i].remark));
		vecWords[i].meaning[0] = EncodingConvert::ToUTF8(stow(vecWords[i].meaning[0]));
		if (vecWords[i].sentences.size())
		{
			vecWords[i].sentences[0].en = EncodingConvert::ToUTF8(stow(vecWords[i].sentences[0].en));
		}
	}
}

// 将溯源得来的单词信息补全
void FillWords(vector<EWORD>& vecWords)
{
	for (int i = 0; i < (int)vecWords.size(); i++)
	{
		EWORD word = SearchWord(vecWords[i].name);
		vecWords[i].pronunciation = word.pronunciation;
		if (vecWords[i].sentences.size() == 0)
		{
			if (word.sentences.size())
			{
				vecWords[i].sentences.push_back(word.sentences[0]);
				vecWords[i].sentences[0].en = EncodingConvert::ToUTF8(stow("（网络）")) + vecWords[i].sentences[0].en;
			}
		}
		else
		{
			vecWords[i].sentences[0].zh = Translate(vecWords[i].sentences[0].en);
		}
	}
}

// 展示溯源单词
void ShowWords(vector<EWORD> vecWords)
{
	for (int i = 0; i < (int)vecWords.size(); i++)
	{
		printf("%d [%ws (%ws)]\n[%ws]\n{%ws}\n---\n\n", i,
			EncodingConvert::ToANSI(vecWords[i].name).c_str(),
			EncodingConvert::ToANSI(vecWords[i].remark).c_str(),
			EncodingConvert::ToANSI(vecWords[i].meaning[0]).c_str(),
			vecWords[i].sentences.size() ? EncodingConvert::ToANSI(vecWords[i].sentences[0].en).c_str() : L"");
	}
}

// 获取用户输入的所有单词
// 返回 0：用户输入了单词
// 返回 1：用户选择了单词溯源
int GetInputWords(vector<string>& pvecWords, vector<EWORD>& pvecEWords)
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
						pvecWords = vecStrings;
						return 0;
					}
				}

				else if (strlwr(strCommand) == "wordsourcing")
				{
					MessageBox(GetConsoleWindow(), L"接下来请选择单词表文件", L"TedEngpia", MB_OK);
					string pathList = wtos(SelectFile());
					MessageBox(GetConsoleWindow(), L"接下来请选择原文文本文件", L"TedEngpia", MB_OK);
					string pathArticle = wtos(SelectFile());
					if (pathList.empty() || pathArticle.empty())
					{
						MessageBox(GetConsoleWindow(), L"已取消", L"TedEngpia", MB_OK);
						continue;
					}

					pvecEWords = WordSourcing(pathList.c_str(), pathArticle.c_str());
					ConvertEncoding(pvecEWords);
					FillWords(pvecEWords);
					ShowWords(pvecEWords);
					return 1;
				}

				else if (isCommandRoot(strCommand, "mode"))
				{
					SetDocMode(strCommand);
				}

				else if (isCommandRoot(strCommand, "directory"))
				{
					SetDirectoryMode(strCommand);
				}

				// 设置基准日期
				else
				{
					SetStandardTime(strCommand);
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

	pvecWords = vecStrings;
	return 0;
}

