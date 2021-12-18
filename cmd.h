////////////////////////////////
//
//	cmd.h
//	�����û�ָ��
//

#pragma once

#include "stdafx.h"
#include "file.h"

// �ж�һ�����������Ƿ����ָ���ַ���
// �������� "light:on"��������� "light"���������� "on"
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

// ���һ�����������
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

// ��ʾ����ҳ��
void PrintHelpPage()
{
	printf(
		"\n\n"
		"-------------------------------------------------------------------------\n"
		"TedEnglishpia_helper ָ��˵��\n"
		"\n"
		"ֱ���ڵ��ʺ���� '/' ����ֱ�ӿ�ʼ��ѯ\n"
		"\n"
		"����� '/' ��ͷ�������Ը�������ָ�\n"
		"\n"
		"help            ��ʾ����ҳ��\n"
		"file            ���ݵ����б��ļ���txt����������\n"
		"\n"
		"ab-cd (ef:gh)   ���û�׼ʱ�䣬Ҳ������ʾ�� Englishpia �ϵĳ�¼����\n"
		"                ab-cd ��׼�·ݺ�����\n"
		"                ef:gh ��׼Сʱ�ͷ��ӣ���ѡ��\n"
		"                ʾ����\n"
		"                /9-20\n"
		"                /01-07 07:09\n"
		"\n"
		"mode            ���ĵ�ǰ�����ĵ�ҳ��ģʽ��Ĭ��Ϊ��׼ģʽ��\n"
		"mode:standard   ���õ����ĵ�ҳ��Ϊ��׼ģʽ\n"
		"mode:simple     ���õ����ĵ�ҳ��Ϊ����ģʽ\n"
		"mode:print      ���õ����ĵ�ҳ��Ϊ��ӡģʽ\n"
		"\n"
		"directory       ���ĵ�ǰ�������Ŀ¼ģʽ��Ĭ�ϲ�������������ڴ�ӡģʽ��ʹ�ã�\n"
		"directory:on    �����������Ŀ¼����\n"
		"directory:off   �����������Ŀ¼����\n"
		"\n"
		"-------------------------------------------------------------------------\n"
		"\n"
	);
}

void SetStandardTime(string strCommand)
{
	int offset = 0;

	string strMonth, strDay, strHour, strMinute;

	// ��ȡ����

	// "�·�-����" ��ʽ�� "�·�.����" ��ʽ
	for (; offset < (int)strCommand.size() && strCommand[offset] != '-' && strCommand[offset] != '.'; strMonth += strCommand[offset], offset++);
	for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ' '; strDay += strCommand[offset], offset++);

	g_month = atoi(strMonth.c_str());
	g_day = atoi(strDay.c_str());

	printf("\n�����û�׼ʱ��Ϊ��%02d �� %02d �� ", g_month, g_day);

	// ��ȡʱ��
	if (offset < (int)strCommand.size())
	{
		// "Сʱ:����" ��ʽ
		for (offset++; offset < (int)strCommand.size() && strCommand[offset] != ':'; strHour += strCommand[offset], offset++);
		for (offset++; offset < (int)strCommand.size(); strMinute += strCommand[offset], offset++);

		g_hour = atoi(strHour.c_str());
		g_min = atoi(strMinute.c_str());

		printf("%02d �� %02d ��", g_hour, g_min);
	}

	// ��������ȴδ����ʱ�䣬��ʹ�û�׼ʱ��
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
		OutError("δָ֪��", false);
		return;
	}

	switch (g_nDocMode)
	{
	case DOC_standard:	printf("\n��������������ĵ�ģʽΪ����׼ģʽ\n");	break;
	case DOC_simple:	printf("\n��������������ĵ�ģʽΪ������ģʽ\n");	break;
	case DOC_print:		printf(	"\n��������������ĵ�ģʽΪ����ӡģʽ\n"
								"\n�����ӡ�����齫��ҳ�����޸�ʽ���Ƶ� word �ĵ�������ʹ�� chrome �ں����������ʹ��˫�����ֽ��д�ӡ�������ֽ��������\n");
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
		OutError("δָ֪��", false);
		return;
	}

	if (g_bDirectoryMode)
	{
		printf(	"\n���� �������Ŀ¼����\n");
	}
	else
	{
		printf("\n���� �������Ŀ¼����\n");
	}
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

				else if (isCommandRoot(strCommand, "mode"))
				{
					SetDocMode(strCommand);
				}

				else if (isCommandRoot(strCommand, "directory"))
				{
					SetDirectoryMode(strCommand);
				}

				// ���û�׼����
				else
				{
					SetStandardTime(strCommand);
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

