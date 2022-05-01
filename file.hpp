////////////////////////////////
//
//	file.hpp
//	处理文件流
//

#pragma once

#include "stdafx.hpp"

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

// 选择文件
// isSave标志着是否为保存模式
const WCHAR* SelectFile(bool isSave = false)
{
	OPENFILENAME ofn;
	static WCHAR szFile[256] = { 0 };
	static WCHAR szFileTitle[256] = { 0 };
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
