# FlowLayout

一个 Qt 的流式布局，当前只能进行等宽布局。

# 使用

最简单的就是直接作为 CMake 的子项目包含进来。

```cmake
add_subdirectory(QtFlowLayout)
```

```C++
auto*const lay = new Z::FlowLayout;
// 此方法目前必须调用
lay->setWidgetWidth(300);
```

在作为子项目被包含的时候默认使用的是静态构建。作为单独工程使用的时候构建的是动态库。可以使用 make install 安装到标准路径下。

# 说明

该项目最初是作为 https://github.com/cathaysia/digikamflowplugin 的子项目存在的。具体用法可以参考其 v0.0.6 及之前的代码。
