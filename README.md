# TedEnglishpia_helper

关于 "Englishpia" !!
---
创建项目的时候错把 "Englishpedia" 写成了 "Englishpia"，于是一错再错……

但是影响不大，我也懒得改了，大家都知道正确拼法就行了。

"Englishpia" 也可以当做是此工具的特色名称吧~（手动滑稽）

运行效果
---
![效果图](https://github.com/zouhuidong/TedEnglishpia_helper/blob/main/screenshot/1.png)

编译环境
---
VisualStudio 2019
 
项目简介
---
我的高中英语老师 Ted 要求我们每天查至少 5 个生词，
并记录在 Englishpia 上，也就是自己制作英语词典。

由于在线翻译网站的排版问题，通常查询一个单词的各项
信息比较耗费时间，查字典的效率则更低，所以特写此
工具快速生成 Englishpia。

Ted 对 Englishpia 有格式要求，
一条合格的 Englishpia 词条记录应该符合以下要求：
1. 有记录时间
2. 音标
3. 释义
4. 例句，至少两个，例句中的对应单词要用红笔高亮

此程序生成的 Englishpia 满足以上要求。

由于时间紧迫，为了快速、方便地开发，生成的 Englishpia
以 HTML 的形式显示。

P.S.英语单词的详细信息来自 dict.cn
 
操作说明
---
运行程序，在 cmd 中输入要查询的单词，输入每个单词之后
都要回车，若输入 ‘/’ 字符后再回车即可开始查询。

更新日志
---

**Ver0.3**

1. 修复部分单词获取例句失败的 bug
2. 增强基准时间设置命令的兼容性，增加支持 "/月份.天数" 格式命令
3. 微调单词抄录默认耗时和随机偏移量
4. 微调 Englishpia 页面

**Ver 0.2**

1. 例句中的单词使用原句内容
2. 显示单词数量
3. 支持设置基准时间，在程序中输入 "/help" 查看更多。


