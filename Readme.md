# 小天的STL^-^

## 介绍
这是我基于C++11实现的一个模板库。

## 环境
- gcc 5.4.0 x86_64-linux-gnu
- cmake 3.14.2
- GTest 1.12.1

## 运行
- 构建
```shell
mkdir build && cd build && cmake ..
cd ..
make
```
- 测试
```shell
./bin/Test
```

## 测试结果
[测试结果输出](./resources/test_result.txt)
### 结果摘要
[==========] 114 tests from 12 test suites ran. (2 ms total)<br/>
[  PASSED  ] 114 tests.

## 文档
正在使用doxygen构建中呢...

## 参考
1. 《STL源码剖析》by侯捷和SGI-STL-2.9：我使用侯捷老师的书籍《STL源码剖析》和对应视频课程作为主要学习材料进行学习，并仿写了其中的实现代码，形成了该项目的初级版本。对于书籍中因篇幅描述不清楚的地方参考了SGI-STL-2.9源码加以学习。
2. 《数据结构与算法(C语言版)》by 严蔚敏和《算法(第四版)》by Robert Sedgewick：前者是我本科的数据结构教材，复习它以构建较为理论化的抽象数据结构概念。后者是普林斯顿大学Robert Sedgewick的著作，在[coursera](https://www.coursera.org/learn/algorithms-part1)上有公开课，虽然其中是以Java语言实现，但其非常直观简洁地讲解了数据结构和算法的原理，仍非常具有参考价值，尤其是红黑树及其平衡部分非常简洁易懂，弥补了上述资料的不足之处。
3. [MyTinySTL](https://github.com/Alinshans/MyTinySTL)by Alinshans：该项目在GitHub上有超过7K的Starred。本项目参考了该项目的构建方式，使用CMake工具替代Make工具。还参考了该项目中涉及C++11新特性的部分，如移动构造和移动赋值的部分，升级了本项目的初级版本。最后参考了该项目的测试方法，并对其进行了一些改造：1、使用GTest工具代替Alinshans在该项目中编写的测试框架。2、在测试容器类API时，不仅测试该API是否能成功运行，而且测试了其正确性。具体方法是本项目编写了一些函数模板封装了GTest的测试宏使用迭代器遍历容器，以比较是否与相应SGI-STL-5中的容器类表现出相同的行为。
4. SGI-STL-5：目前参考了较新STL版本中的命名方式，如数据成员变量使用_M_开头以及模板参数以下划线开头，使得代码更加清晰不易混淆。