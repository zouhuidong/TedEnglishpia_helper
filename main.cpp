////////////////////////////////////////////////
//
//	TedEnglishpia_helper
//	����Ӣ�ﵥ���ĵ�������
//
//	���뻷����VisualStudio 2022
//	��Ŀ��ַ��https://github.com/zouhuidong/TedEnglishpia_helper
//	�������ߣ�huidong <huidong_mail@163.com>
//	�桡������Ver 0.6.0
//	����޸ģ�2022.05.01
//

#include "stdafx.hpp"
#include "pedia.hpp"
#include "cmd.hpp"

// �� HTML
void OpenHTML(string path)
{
	system(("start " + path).c_str());
}

// �����û�ָ��õ����ɵ� HTML �ĵ���·��
string GetPediaHTML()
{
	vector<string> vecWords;
	vector<EWORD> vecEWords;
	if (GetInputWords(vecWords, vecEWords) == 0)
	{
		return OutputHTML(BuildEnglishPia(vecWords));
	}
	else
	{
		return OutputHTML(BuildEnglishPia(vecEWords, GetStringList(vecEWords)));
	}
}

int main()
{
	srand((UINT)time(0));
	setlocale(NULL, "chs");

	printf(
		"\n"
		"*********************************************************************************\n\n"
		"\t��������������������������     ����������������������      ����������������                   \n"
		"\t������������ ������������     | ������������������      | ����������  \\                \n"
		"\t     | |          | ��������������        | |     \\ \\              \n"
		"\t     | |          | ��������������        | |      \\ \\             \n"
		"\t     | |          | |              | |       \\ \\            \n"
		"\t     | |          | ������������������      | ������������������  \\            \n"
		"\t     ��-��          ����������������������      ��������������������������              \n"
		"\n"
		"\tTedEnglishpia_helper %s\n"
		"\tAuthor: huidong <huidong_mail@163.com>\n"
		"\tProject URL: https://github.com/zouhuidong/TedEnglishpia_helper \n\n"
		"*********************************************************************************\n\n"
		"\t���ݵ����Զ����� Ted ���涨��ʽ�� Englishpedia\n\n"
		"\t�������ѯ���ʣ�ÿ�����ʽ�����س���������� '/' ���س���ʼ��ѯ\n"
		"\tʹ�� '/help' ����鿴����ָ�����\n\n"
		"*********************************************************************************\n\n"
		, g_strInfo);

	while (true)
	{
		OpenHTML(GetPediaHTML());
		printf("\n done.\n\n");
	}

	return 0;
}

