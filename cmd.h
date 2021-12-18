////////////////////////////////
//
//	cmd.h
//	处理用户指令
//

#pragma once

#include "stdafx.h"
#include "file.h"

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
	case DOC_print:		printf(	"\n已设置输出单词文档模式为：打印模式\n"
								"\n如需打印，建议将网页内容无格式复制到 word 文档（建议使用 chrome 内核浏览器），使用双栏布局进行打印，以提高纸张利用率\n");
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
		printf(	"\n启用 输出单词目录功能\n");
	}
	else
	{
		printf("\n禁用 输出单词目录功能\n");
	}
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

	return vecStrings;
}

