
// system_view.h

#pragma once

// 告诉 C++ 编译器不要对特定的函数进行名称修饰
#ifdef __cplusplus
extern "C" {
#endif

void // 初始化系统视图
SystemViewInit();

void // 退出系统视图
SystemQuitView();

void // 开始帧
SystemViewOnFrameBegin();

void // 结束帧
SystemViewOnFrameEnd();

void // 处理事件
SystemProcessEvent();

void // 显示窗口
SystemViewShowWindows();

#ifdef __cplusplus
}
#endif
