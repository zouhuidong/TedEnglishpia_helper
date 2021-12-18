////////////////////////////////////////////////
//
//	TedEnglishpia_helper
//	自制英语单词文档生成器
//
//	编译环境：VisualStudio 2022
//	项目地址：https://github.com/zouhuidong/TedEnglishpia_helper
//	作　　者：huidong <huidong_mail@163.com>
//	版　　本：Ver 0.5
//	最后修改：2021.12.18
//

#include "stdafx.h"
#include "pedia.h"
#include "cmd.h"

// 打开 HTML
void OpenHTML(string path)
{
	system(("start " + path).c_str());
}

// 根据用户指令得到生成的 HTML 文档的路径
string GetPediaHTML()
{
	return OutputHTML(BuildEnglishPia(GetInputWords()));
}

int main()
{
	srand((UINT)time(0));

	printf(
		"TedEnglishpia_helper by huidong %s\n\n"
		"根据单词自动生成 Ted 的 Englishpia，提高作业效率\n\n"
		"输入待查询单词，每个单词结束后回车，最后输入 '/' 并回车开始查询。\n"
		"'/' 后可以带参数，请输入 '/help' 查看更多。\n\n"
		"在此输入待查询单词：\n\n", g_strInfo);

	while (true)
	{
		OpenHTML(GetPediaHTML());
		printf("\n done.\n\n");
	}

	return 0;
}

