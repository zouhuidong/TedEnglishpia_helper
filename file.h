////////////////////////////////
//
//	file.h
//	�����ļ���
//

#pragma once

#include "stdafx.h"

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