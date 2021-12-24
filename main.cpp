////////////////////////////////////////////////
//
//	TedEnglishpia_helper
//	����Ӣ�ﵥ���ĵ�������
//
//	���뻷����VisualStudio 2022
//	��Ŀ��ַ��https://github.com/zouhuidong/TedEnglishpia_helper
//	�������ߣ�huidong <huidong_mail@163.com>
//	�桡������Ver 0.5.1
//	����޸ģ�2021.12.24
//

#include "stdafx.h"
#include "pedia.h"
#include "cmd.h"

// �� HTML
void OpenHTML(string path)
{
	system(("start " + path).c_str());
}

// �����û�ָ��õ����ɵ� HTML �ĵ���·��
string GetPediaHTML()
{
	return OutputHTML(BuildEnglishPia(GetInputWords()));
}

int main()
{
	srand((UINT)time(0));

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

