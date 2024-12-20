
// system_view.h

// 告诉 C++ 编译器不要对特定的函数进行名称修饰
#ifdef __cplusplus
extern "C" {
#endif

void
SystemViewInit();

void
SystemQuitView();

void
SystemViewOnFrameBegin();

void
SystemViewOnFrameEnd();

void
SystemProcessEvent();

void
SystemViewShowWindows();

#ifdef __cplusplus
}
#endif
