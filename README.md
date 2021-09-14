# QtFrameless

#### 介绍
QT无边框窗体类，老生常谈的话题，简单实现很容易，做到既通用又美观就没那么容易了

###特性
- 支持windows、linux、mac系统
- 支持windows自带阴影边框
- 支持自绘制阴影边框
- windows上支持贴边自动半屏全屏
- windows阻塞窗体边缘闪烁提醒
- 基于QWidget、QDialog、QMainWindow派生类，使用非常简单
- 最大化时拉动标题栏还原窗口


#### 软件架构
Qt5

#### 使用说明
1.  项目中引用qframeless.pri
2.  修改对应的窗口基类，详情参考demo程序

#### 感谢
感谢2位大神，代码中参考了2位前辈的部分代码
1.  https://gitee.com/feiyangqingyun/QWidgetDemo.git
2.  https://github.com/FlyWM/ShadowWidget

#### 效果图
![主窗口](https://images.gitee.com/uploads/images/2021/0914/095315_50e83162_883195.png "1.png")
![windows贴边自动半屏](https://images.gitee.com/uploads/images/2021/0914/095327_c418b918_883195.png "2.png")
![子窗口-Windows系统阴影边框](https://images.gitee.com/uploads/images/2021/0914/095342_e49c7965_883195.png "3.png")
![自绘阴影边框](https://images.gitee.com/uploads/images/2021/0914/095902_4cb7e1b2_883195.png "6.png")
![QWidget子窗口](https://images.gitee.com/uploads/images/2021/0914/095427_71ce665a_883195.png "7.png")
![QMainWindow子窗口](https://images.gitee.com/uploads/images/2021/0914/095433_4617666c_883195.png "8.png")