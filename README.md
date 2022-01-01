# TedEnglishpia_helper

![License](https://img.shields.io/github/license/zouhuidong/TedEnglishpia_helper)
![Downloads](https://img.shields.io/github/downloads/zouhuidong/TedEnglishpia_helper/total)
![GitHub Version](https://img.shields.io/github/v/release/zouhuidong/TedEnglishpia_helper)

## Table of Contents

* [TedEnglishpia\_helper](#tedenglishpia_helper)
  * [项目简介](#项目简介)
    * [关于 "Englishpia" 项目名称](#关于-englishpia-项目名称)
    * [编译环境](#编译环境)
  * [凭什么要用这个工具](#凭什么要用这个工具)
  * [运行效果](#运行效果)
  * [操作说明](#操作说明)
  * [一些经验](#一些经验)
  * [打印建议](#打印建议)
  * [更新日志](#更新日志)

## 项目简介

高一上，我的英语老师是 Ted。

Ted 要求我们每天查至少 5 个生词，并记录在 Englishpedia（自制英语词典）上。

但是我个人写 pedia 一般是很花时间的，如果平时在学校，需要一个个词查字典，看释义，理解例句，纠结哪个例句、哪个释义适合抄下来……然后抄了半天。

因为种种原因，特写此工具快速生成 Englishpedia。

Ted 对一条合格的 Englishpedia 词条记录有以下格式要求：

* 记录时间
* 音标
* 释义
* 例句（至少两个，例句中的对应单词要用红笔高亮）

此程序通过爬取网络数据，让你只需要输入待查询词汇，就可以自动生成合格的 Englishpedia。

生成的 Englishpedia 以 HTML 网页的形式展示，可以进行打印（详见后文“打印建议”）。

P.S. 英语单词的详细信息来自 dict.cn

### 关于 "Englishpia" 项目名称

该项目名称是 "TedEnglishpia_helper"

创建项目的时候错把 "Englishpedia" 写成了 "Englishpia"，于是一错再错……

这也可以当做是此工具的特色名称吧~（手动滑稽）

### 编译环境

VisualStudio 2022

Windows 10

## 凭什么要用这个工具

* 如果你嫌累从来不写 pedia，那么用这个工具你可以轻松制作 pedia。

* 如果你有时候写一点 pedia，没时间的时候就不写了，那么这个工具可以节省你大量的时间。

* 如果你有很多单词想抄进 pedia，但是还是因为时间不够只挑了一些抄（就比如你想把两个相似词都抄录下来进行对比，或者是想抄录一个好像不那么重要的单词，但是因为没那么多时间就放弃了），那么用这个工具你不再需要担心花时间的问题，多少单词都行。

总的来说，这个工具可以为你省时省力，更能帮助从来不写 pedia 的人群。

如果你感兴趣，可以继续往下了解。

## 运行效果

**先看看程序效果吧~**

生成的 pedia 文档：

![效果图](https://github.com/zouhuidong/TedEnglishpia_helper/blob/main/screenshot/5.png)

控制台窗口：

![效果图](https://github.com/zouhuidong/TedEnglishpia_helper/blob/main/screenshot/6.png)

## 操作说明

运行程序，在 cmd 中输入要查询的单词，输入每个单词之后都要回车，若输入 `/` 字符后再回车即可开始查询。

更多指令请在程序中输入 `/help` 查看。

## 一些经验

一开始我做好这个工具的时候，我都是先用程序把要查询的单词生成出文档，然后再在电脑上对着抄在作业本上。
由于高中住校，所以我都是周末回家才能接触电脑，平时我没有查的词全部都集中到周末用电脑来查，大概每周都要查四五十个单词，然后抄在作业本上。
一个单词大约要消耗我两三分钟的时间来抄录，而且是在电脑上抄，我还在边抄的时候边琢磨例句，查例句里面的生词，或者因为别的事情耽搁，
就会导致每次周末在家都要花两三个小时做上一周的 Englishpedia。

过了一段时间之后，我就开始把 pedia 打印出来，利用周日返校的晚修时间来
把打印出来的 pedia 抄到作业本上。然而这还是十分耗时的一件事情，我周日的晚修一般都是整个晚修都花在了做 pedia 上。后来，我实在不想用
这么多时间来做 pedia，于是我打印出 pedia 之后不再抄到作业本上，而是直接用打印版来看。结果我没想到的是，我直接看打印版的 pedia 记
单词的效果和抄录到作业本上记单词的效果差不多，或许直接看打印版的效果还更好。我原先以为抄录 pedia 的这一过程十分有益于记单词，但是实践
才是检验真理的唯一途径，现在看来，直接用打印版的 pedia 记单词效果没啥两样，而且还不用花那么多时间抄录，何乐而不为呢？

所以我建议，如果使用本工具，可以将生成的 pedia 直接打印出来看，不需要再抄录。（当然这只是个人观点）

如果你想尝试借助此工具打印 pedia，请看下一节 【打印建议】。

## 打印建议

打印效果图：
![打印效果图](https://github.com/zouhuidong/TedEnglishpia_helper/blob/main/screenshot/4.jpg)

直接将此工具生成的单词文档打印出来，可以使你做 pedia 的效率提升到最高。但是如果要打印，还需要注意一些事项。

以下是使用此工具打印 TedEnglishpedia 的一个完整流程：

1. 运行程序，在程序中使用 `/mode:print` 指令，将文档模式设置为【打印模式】（设置为简易模式亦可）。

2. 如果单词数量较多，希望生成单词目录，可以使用 `/directory` 指令。

3. 如果单词数量较多，可以将所有单词写在一个 txt 文件中，每个单词以换行隔开，然后在程序中使用 `/file` 指令，在打开的对话框中选择到 txt 文件。
   
   若不使用 `/file` 指令，则直接在 cmd 中输入单词即可。

4. 程序将自动打开生成的 TedEngpia 页面，此时不要直接在网页上选择打印。
   
   请首先确保你所使用的浏览器是 Chromium 内核的浏览器（如 Chrome，Edge），而不是火狐浏览器等其他内核的浏览器。
   
   因为如果使用类似火狐的浏览器，复制得到的网页内容格式不太对，对后续的打印会造成一些影响。
   
   如果你的默认浏览器不是 Chromium 内核的，可以将 TedEngpia 页面的网址复制到 Chromium 内核的浏览器中打开。
   
5. 确保你使用的浏览器使用的是 Chromium 内核后，使用 `Ctrl` + `A` 快捷键全选整个网页，然后复制下来，先黏贴到一个 txt 文档中，再新建一个 Word 文档，
   把 txt 文档中的内容复制到 Word 文档中。
   
   先复制到 txt 再复制到 Word 文档其实是为了黏贴到 Word 文档的内容是无格式的，你也可以复制整个网页之后在 Word 中选择【只黏贴文本】。
   
   复制到 Word 文档之后，在 Word 界面中选择顶部的【页面布局】->【分栏】->【两栏】。
   
   分两栏的目的是使纸张的利用率提高，你可以尝试一下，如果不分栏的话，50 个单词的 pedia 大约需要 9 面 A4 纸才能打印完，而分栏之后只需要 6 面纸。
   
   这样 pedia 的内容就整合好到 Word 文档中了，可以开始打印了~
   
虽然看起来文字很多，操作很麻烦，其实这都是因为说的比较详细，动手试试其实很简单的。

**要是笼统地说一说，那你只需要：**

*把你要查询的单词输入程序，将会生成出一个网页文档，然后把文档内容无格式复制到 Word 中分两栏打印即可。*

复制到 Word 文档中（图）：
![Word 文档示意图](https://github.com/zouhuidong/TedEnglishpia_helper/blob/main/screenshot/3.png)

## 更新日志

**Ver 0.5.1** (2021.12.24)

上周的更新还是有一些小细节让我不太满意，所以进行了一点小修改。因为更新内容比较少，所以版本号就再加一位小数点吧。

1. 在用户选择生成单词目录的前提下，在每个单词前面加上序号。

2. 更新了 pedia 文档的配色样式，将传统的 “传统白绿色单行本样式” 改为了 “Notebook 现代风格”。（P.S. 用户也可以自行修改 `./res/module/css/` 下的 .css 文件来调整配色样式哦~）

3. 更新了控制台窗口的首页样式，现在变得更整洁了（还有 TED logo 坐镇）。但是经测试，在 Windows 7 下会出现严重的字符错位现象，所以还是 Windows 10 yes 吧~。

**Ver 0.5** (2021.12.18)

距离上次更新已经有将近两个月了。这次更新把这段时间以来发现的不足之处进行了改善，具体如下：

1. 将模版 HTML 网页与 css 结合，用户可以在程序中设置生成的文档页面为【标准模式】或【简易模式】或【打印模式】。

2. 可以选择生成单词目录。如果有大量单词，生成目录可以清晰地概览所有单词（该功能主要是用在打印模式下）。

3. 将项目环境升级到了 VisualStudio 2022，并且调整了项目代码结构，分离了不同的功能模块。

4. 微调了 Englishpia 文档页面。

具体的指令格式请在程序中输入 `/help` 查看。

**Ver 0.4** (2021.10.29)

好久没更新，跨越了一个月，于是根据自己使用中的感受更新了一些内容：

1. 支持打开含有英语单词的 txt 文件，如需要，请使用 `/file` 命令。（文件格式：每个单词后面空一行即可~）

   这样做的原因是：
   
   如果直接在 cmd 中输入大量的待查询单词，如果其中有输入错误的单词就难以改正；
   
   在文件中调整大量待查询单词的拼写，顺序等比较方便。
   
2. 又微调了单词抄录默认耗时和随机偏移量，之前是 6min 的抄录时间，2min 的随机偏移，现在分别改成了 4min 和 1min。

   调整的原因是我觉得可以用这个时间来检验自己抄录的速度是否太慢了，所以就按照我自己大概的抄写速度进行了调整。
   
   
3. 支持直接在单词后面加 `/`，这样就可以直接开始查询，省去了换一行再 `/` 的麻烦。

4. 同样微调了 Englishpia 页面。

**Ver 0.3** (2021.9.19)

1. 修复部分单词获取例句失败的 bug
2. 增强基准时间设置命令的兼容性，增加支持 `/月份.天数` 格式命令
3. 微调单词抄录默认耗时和随机偏移量
4. 微调 Englishpia 页面

**Ver 0.2** (2021.9.19)

1. 例句中的单词使用原句内容
2. 显示单词数量
3. 支持设置基准时间，在程序中输入 `/help` 查看更多。

**Ver 0.1** (2021.9.19)

第一个版本
