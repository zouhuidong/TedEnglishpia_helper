////////////////////////////////////////////////
//
//	TedEnglishpia_helper
//	自制英语单词文档生成器
//
//	编译环境：VisualStudio 2022
//	项目地址：https://github.com/zouhuidong/TedEnglishpia_helper
//	作　　者：huidong <huidong_mail@163.com>
//	版　　本：Ver 0.5.1
//	最后修改：2021.12.24
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
		"\n"
		"*********************************************************************************\n\n"
		"\t┌───────────┐     ┌─────────┐      ┌───────                   \n"
		"\t└────┐ ┌────┘     | ┌───────┘      | ┌────  \\                \n"
		"\t     | |          | └─────┐        | |     \\ \\              \n"
		"\t     | |          | ┌─────┘        | |      \\ \\             \n"
		"\t     | |          | |              | |       \\ \\            \n"
		"\t     | |          | └───────┐      | └────────  \\            \n"
		"\t     └-┘          └─────────┘      └────────────              \n"
		"\n"
		"\tTedEnglishpia_helper %s\n"
		"\tAuthor: huidong <huidong_mail@163.com>\n"
		"\tProject URL: https://github.com/zouhuidong/TedEnglishpia_helper \n\n"
		"*********************************************************************************\n\n"
		"\t根据单词自动生成 Ted 所规定格式的 Englishpedia\n\n"
		"\t输入待查询单词，每个单词结束后回车，最后输入 '/' 并回车开始查询\n"
		"\t使用 '/help' 命令查看更多指令操作\n\n"
		"*********************************************************************************\n\n"
		, g_strInfo);

	while (true)
	{
		OpenHTML(GetPediaHTML());
		printf("\n done.\n\n");
	}

	return 0;
}

