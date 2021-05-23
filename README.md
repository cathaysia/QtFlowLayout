# FlowLayout

一个 Qt 的流式布局，当前只能进行等宽布局。

# 使用

最简单的就是直接作为 CMake 的子项目包含进来。默认是使用的静态构建

```C++
auto*const lay = new Z::FlowLayout;
lay->setWidgetWidth(300);
```
