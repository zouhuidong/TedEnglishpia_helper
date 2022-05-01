/////////////////////////////////////////
//
//	EqualityChecker_DiffWordType.h
//	同词异形简易判断器
// 
//	可以根据不同词形的单词，
//	判断它们是否对应同一个原形单词
// 
//	根据测试，可以正确判断大部分单词，
//	但是对于短单词的判断正确率略低于长单词
// 
//	by huidong<huidong_mail@163.com>
//	2022.05.01
//

#pragma once

#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <sstream>

namespace EChecker_DiffWT {

	typedef std::vector<std::string> WordNames;

	// 初始化判定器
	void InitChecker(
		const char* pszNounsDicPath,	// 不规则名词变化表文件路径
		const char* pszVerbsDicPath,	// 不规则动词变化表文件路径
		const char* pszPrefixDicPath,	// 单词前缀表文件路径
		const char* pszSuffixDicPath	// 单词后缀表文件路径
	);

	// 判断两个单词是否为同源词
	bool CheckWordsEquality(std::string s1, std::string s2);
}

