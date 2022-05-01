//////////////////////
//
//	translator.hpp
//	·­Òë API µÄµ÷ÓÃ
//

#pragma once

#include "web.hpp"

string Translate(string str)
{
	string code = GetWebSrcCode(
		(L"http://fanyi.youdao.com/translate?&doctype=xml&type=AUTO&i=" + stow(str)).c_str(),
		false
	);

	string flag = "<![CDATA[";
	int pos = code.rfind(flag);
	int end = code.find("]", pos);
	if (pos != string::npos)
	{
		return code.substr(pos += flag.size(), end - pos);
	}
	return {};
}
