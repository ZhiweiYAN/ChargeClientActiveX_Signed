========================================================================
ACTIVEX 控件 DLL：ChargeClientActiveX 项目概述
========================================================================

控件向导已为您的 ChargeClientActiveX ActiveX 控件 DLL 创建了此项目，该 DLL 包含一个控件。

此主干项目不仅演示了编写 ActiveX 控件的基本步骤，而且是编写控件特定功能的起点。

本文件概要介绍组成 ChargeClientActiveX ActiveX 控件 DLL 的每个文件的内容。

ChargeClientActiveX.vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

ChargeClientActiveX.h
这是 ActiveX 控件 DLL 的主包含文件。它包括其他项目特定的文件，如 resource.h。

ChargeClientActiveX.cpp
这是包含 DLL 初始化、终止和其他簿记的代码的主源文件。

ChargeClientActiveX.rc
    
这是项目使用的 Microsoft Windows 资源的清单。

此文件可以直接用 Visual C++ 资源编辑器进行编辑。

ChargeClientActiveX.def
此文件包含为在 Microsoft Windows 中运行 ActiveX 控件 DLL 必须提供的 ActiveX 控件 DLL 的有关信息。

ChargeClientActiveX.idl
此文件包含控件类型库的对象描述语言源代码。

/////////////////////////////////////////////////////////////////////////////
CChargeClientActiveXCtrl 控件：

ChargeClientActiveXCtrl.h
此文件包含 CChargeClientActiveXCtrl C++ 类的声明。

ChargeClientActiveXCtrl.cpp
此文件包含 CChargeClientActiveXCtrl C++ 类的实现。

ChargeClientActiveXPropPage.h
此文件包含 CChargeClientActiveXPropPage C++ 类的声明。

ChargeClientActiveXPropPage.cpp
此文件包含 CChargeClientActiveXPropPage C++ 类的实现。

CChargeClientActiveXCtrl.bmp
此文件包含一个位图，当 CChargeClientActiveXCtrl 控件出现在工具面板上时容器将使用此位图来表示该控件。此位图包括在主资源文件 ChargeClientActiveX.rc 中。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

stdafx.h, stdafx.cpp
这些文件用于生成名为 ChargeClientActiveX.pch 的预编译头 (PCH) 文件和名为 stdafx.obj 的预编译类型文件。

resource.h
这是标准头文件，它定义新的资源 ID。
Visual C++ 资源编辑器读取并更新此文件。

/////////////////////////////////////////////////////////////////////////////
其他注释：

控件向导使用“TODO:”指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////