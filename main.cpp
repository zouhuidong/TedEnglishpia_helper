////////////////////////////////////////////////
//
//	TedEnglishpia_helper
//	����Ӣ�ﵥ���ĵ�������
//
//	���뻷����VisualStudio 2022
//	��Ŀ��ַ��https://github.com/zouhuidong/TedEnglishpia_helper
//	�������ߣ�huidong <huidong_mail@163.com>
//	�桡������Ver 0.5
//	����޸ģ�2021.12.18
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
		"TedEnglishpia_helper by huidong %s\n\n"
		"���ݵ����Զ����� Ted �� Englishpia�������ҵЧ��\n\n"
		"�������ѯ���ʣ�ÿ�����ʽ�����س���������� '/' ���س���ʼ��ѯ��\n"
		"'/' ����Դ������������� '/help' �鿴���ࡣ\n\n"
		"�ڴ��������ѯ���ʣ�\n\n", g_strInfo);

	while (true)
	{
		OpenHTML(GetPediaHTML());
		printf("\n done.\n\n");
	}

	return 0;
}

