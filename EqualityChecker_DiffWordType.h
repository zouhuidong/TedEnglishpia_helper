/////////////////////////////////////////
//
//	EqualityChecker_DiffWordType.h
//	ͬ�����μ����ж���
// 
//	���Ը��ݲ�ͬ���εĵ��ʣ�
//	�ж������Ƿ��Ӧͬһ��ԭ�ε���
// 
//	���ݲ��ԣ�������ȷ�жϴ󲿷ֵ��ʣ�
//	���Ƕ��ڶ̵��ʵ��ж���ȷ���Ե��ڳ�����
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

	// ��ʼ���ж���
	void InitChecker(
		const char* pszNounsDicPath,	// ���������ʱ仯���ļ�·��
		const char* pszVerbsDicPath,	// �����򶯴ʱ仯���ļ�·��
		const char* pszPrefixDicPath,	// ����ǰ׺���ļ�·��
		const char* pszSuffixDicPath	// ���ʺ�׺���ļ�·��
	);

	// �ж����������Ƿ�ΪͬԴ��
	bool CheckWordsEquality(std::string s1, std::string s2);
}

