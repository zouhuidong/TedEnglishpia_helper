/////////////////////////////
//
//	EncodingConvert.hpp
//	字符串编码转换
//

#pragma once

#include <codecvt>

namespace EncodingConvert {

	// 转为 UTF-8
	std::string ToUTF8(const std::wstring& str)
	{
		static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.to_bytes(str);
	}

	// 转为 ANSI
	std::wstring ToANSI(const std::string& str)
	{
		static std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.from_bytes(str);
	}

	// 任意编码转换
	// locale = "chs" 或 "zh-cn"
	std::string ToEncoding(const std::wstring& str, const std::string& locale)
	{
		typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
		static std::wstring_convert<F> strCnv(new F(locale));
		return strCnv.to_bytes(str);
	}

	// 任意编码转换
	// locale = "chs" 或 "zh-cn"
	std::wstring ToEncoding(const std::string& str, const std::string& locale)
	{
		typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
		static std::wstring_convert<F> strCnv(new F(locale));
		return strCnv.from_bytes(str);
	}

}
