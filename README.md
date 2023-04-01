# This is android c4droid codes
> 我愿意将此项目称之为C4droid代码考古，因为这些源码大多都很久远了，C4droid已于2020年停止维护
> 感谢n0n3m4大佬为安卓手机提供了C语言开发环境，虽然功能简陋，但手机上学习也够用了，想想看在你蹲坑无聊时候还能掏出手机Codding(bushi)是不是也很爽

![1.jpg](https://img.shields.io/badge/c4droid_example-200+-green)

github: https://github.com/qaiu/c4droid-code  
gitee镜像: https://gitee.com/QAIU/c4droid-code  
C4droid下载链接 https://blog.qaiu.top/archives/c4droid  
QQ群1：1026766509  
QQ群2：172725648  
QQ：736226400  

# 简介
C4droid可以运行的C语言代码，各种demo，有小游戏，小工具，还有各种整理的算法  
整理的代码，大多来自各种博客论坛(cnblog，CSDN)，开源社区(gitee，github)，百度贴吧，QQ群等  
# 开发工具简介
`C4droid`是一款安卓上的C语言和C++开发环境，本质上是一款集成GCC的编辑器而已，功能简单，比较适合C语言初学者入门使用，
在手机端编程比较方便本人利用业余时间做了汉化，并修改了一些bug(包括但不限于中文导出和中文调试的问题)  

# 编译运行
理论支持GCC，clang等编译器，安卓c4droid可以直接运行，其他Linux发行版
需要将Include/Linux下的头文件拷贝到系统include下，或者放在源码目录
也可以使用termux的clang编译

# 参与维护本项目
长远来看，本仓库会持续更新，部分代码仍有待优化，内容会渐渐丰富，同时是分享一些简单好玩的东西
喜欢在C/C++的朋友可以提交分享你们作品，多多益善，同时质量一定要好哦
1. 保证C4droid可以运行
2. 最好能让各个版本编译器像GCC（mingw），clang可以编译
3. 提供详细呢注释，最好能提供运行截图
4. 目录规则代码规范参考本项目代码（好像没啥规范==）

# 主要项目
- [游戏区/命令行和SDL小游戏](https://github.com/qaiu/c4droid-code/tree/main/Game)
- [内存编辑器/其他辅助工具/加密算法等等](https://github.com/qaiu/c4droid-code/tree/main/Tool/memedit)
- [数百个算法示例/c4droid手册包含的代码等等](https://github.com/qaiu/c4droid-code/tree/main/Learn/C%E8%AF%AD%E8%A8%80%E5%9F%BA%E7%A1%80%E7%AE%97%E6%B3%95)

-----
![Screenshot_20200802005600345_com.n0n3m4.droida.jpg](https://blog.qaiu.top/upload/2020/08/Screenshot_2020-08-02-00-56-00-345_com.n0n3m4.droida-5c420cfc3c3d4375b0042716b2d2f9f4.jpg)
![Screenshot_20200802024229807_com.n0n3m4.droida.jpg](https://blog.qaiu.top/upload/2020/08/Screenshot_2020-08-02-02-42-29-807_com.n0n3m4.droida-3d48ade904dc49bf9f991ad9ff448bc0.jpg)
![Screenshot_20200802023614588_com.n0n3m4.droida.jpg](https://blog.qaiu.top/upload/2020/08/Screenshot_2020-08-02-02-36-14-588_com.n0n3m4.droida-ec34f56b785e4d15ad8257d239c1d8a0.jpg)

# 其他优秀的基于C4droid编写的开源项目(未严格验证，可能有各种bug，甚至不能运行，且大多都不再维护)
- 这些年用C4droid写过的代码 https://github.com/cclfly/C4droidProjects/tree/master

- C++编写的SDL2引擎. https://github.com/Kiritow/MiniEngine

- C4droid音频播放器 https://github.com/Kiritow/C4droid-Audio-Player

- code4a(C4droid代码手册)已不再维护 https://github.com/TTHHR/code4a

- Android native(GLES)开发的辅助库 https://github.com/z123k5/NativeInterface

- A Package Management Tool for c4droid app. https://github.com/imkiwa/c4pkg

- 音乐游戏Force Rush的源代码 https://github.com/FaMuLan/Force-Rush

- 蹲坑时候无聊，用C4droid开发的小游戏代码 https://github.com/zjko/GCC_Game

- 飞机大战项目 C4droid+C语言+SDL2 https://github.com/ljp-cyber/air-war



# 更新记录（记录开始于2023-04-01）：

20220101-20230331:
- 新增之前遗留的C4droid代码手册的部分代码
- 修改/新增目录结构Learn/Demo实验下面放一些实验性代码或者Demo，比如生命游戏之类
- 添加GLM库，并附带安装程序（安装程序以C源码形式提供，C4droid用户可以直接运行安装）
- Learn下添加关于SDL2.0+GLES3.0的图形化教程（部分源码依赖GLM库）
- 部分代码bug修复，部分注释修改的更加严谨
- 对未来更新方向的一些规划...

TODO: 完善README，代码规范化，继续引入更多项目



