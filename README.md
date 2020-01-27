# Easy2D

[![Build status](https://ci.appveyor.com/api/projects/status/ah39otggqy7xu0qm/branch/master?svg=true)](https://ci.appveyor.com/project/Nomango/easy2d/branch/master)
[![GitHub release](https://img.shields.io/github/release/easy2d/easy2d)](https://github.com/Easy2D/Easy2D/releases/latest)
[![GitHub license](https://img.shields.io/github/license/easy2d/easy2d)](https://github.com/Easy2D/Easy2D/blob/master/LICENSE)

## 简介

Easy2D 是一个为 C++ 设计的 2D 游戏引擎，目前仅支持 Windows 平台。

我写这个库的初衷是学习游戏引擎技术，然后写一些自己的小游戏。Easy2D 提供了很多轮子和工具，但它仍然在开发当中。

更多的信息请到 [Easy2D 官方网站](//www.easy2d.cn) 查看。

## 功能

* 场景管理
* 场景间过渡动画
* 动画系统
* 简单的按钮 GUI
* WAV 音频支持
* 数据持久化工具
* ...

## 快速上手

#### 运行 Hello World

第一步，点击页面右上角的 `Clone or download`，克隆本仓库或者下载 zip 源码。

```
git clone https://github.com/nomango/easy2d.git
```

第二步，使用 Visual Studio 2013 或更高版本，打开目录下的 sln 文件。

第三步，直接运行，在几分钟编译后就会自动打开 Hello World 程序。

## 安装

#### 使用简易安装器安装

简易安装器是一个使用 7zip 制作的自解压安装包，它包含在各 Visual Studio 版本上预先编译好的库文件。

请到 [GitHub release](https://github.com/Easy2D/Easy2D/releases/latest) 页面或 [Easy2D 官方网站](//www.easy2d.cn) 下载最新的安装程序。

#### 从源代码安装

在其他项目使用 Easy2D 时，需要先编译 Easy2D 工程，它会在 `/Easy2D/output/` 目录生成 .lib 文件。

在 VS 中打开你自己项目的项目属性，把 `C/C++` ==> `附加包含目录` 设置为 Easy2D 项目的 `/Easy2D/include/` 目录，再把 `链接器` ==> `附加库目录` 设置为 Easy2D 项目的 `/Easy2D/output/` 目录。

然后就可以在你自己的项目中使用 Easy2D 的功能了。

> 注意：必须先把 Easy2D 项目编译生成 .lib 文件

## 计划

Easy2D 是我个人的早期作品，新的游戏引擎项目已经更庞大且更专业，查看详情请移步 [Kiwano 游戏引擎](https://github.com/nomango/kiwano)

## 联系方式

* Website: [www.easy2d.cn](//www.easy2d.cn)
* QQ 群: 608406540
