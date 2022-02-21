# This is

A Custom Layout with RowLayout, ColLayout, SquareLayout

# Use

The simplest way to use this is include this project a CMake's subproject:

```cmake
add_subdirectory(QtFlowLayout)
```

```C++
setLayout(new Z::FlowLayout(this));
layout()->setStyle(Z::FlowLayout::Col);
```

# Note

This project is a project derived from https://github.com/cathaysia/digikamflowplugin, please read
it's source before using this
