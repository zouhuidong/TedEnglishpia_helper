////////////////////////////////
//
//	string_func.hpp
//	处理字符串相关
//

#pragma once

#include <algorithm>
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "wininet.lib")


std::string wtos(const std::wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	std::string result = pchar;
	return result;
}

std::wstring stow(const std::string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	std::wstring result = pwchar;
	return result;
}

std::string strlwr(std::string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

std::string strupr(std::string s)
{
	transform(s.begin(), s.end(), s.begin(), ::toupper);
	return s;
}

// 替换字符串中的所有指定字符串
std::string& replace_all(std::string& str, const std::string& old_value, const std::string& new_value)
{
	while (true)
	{
		std::string::size_type   pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return   str;
}

