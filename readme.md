# Ubuntu 安装 KinectV1 驱动 OpenNI+SensorKinect+NITE

---

Creation Date: 2020-05-15

---

[TOC]

## 一、环境与准备

### 1.1 环境

* Ubuntu1804x64(VMware)
* Kinect 需要和电脑的 USB3.0 连接，并且开启虚拟机的兼容USB3.0（步骤：虚拟机-->设置-->USB控制器，右方设置）
* 安装 OpenNI 需要 `gcc` `g++`  4.x 版本（[OpenNI的Github有说明](https://github.com/OpenNI/OpenNI)）。

### 1.2 准备

软件下载（**版本的不同可能会影响安装**，建议选择我所提及的版本安装）

* [OpenNI下载](https://github.com/OpenNI/OpenNI/releases)（版本 `Stable-1.5.7.10`）
* [SensorKinect下载](https://github.com/ph4m/SensorKinect/archive/unstable.zip)（版本 `5.1.2.1`）（这是参考1 给的下载地址，参考3 给的[下载链接](https://github.com/avin2/SensorKinect)安装会出错）
* [NITE下载](https://github.com/arnaud-ramey/NITE-Bin-Dev-Linux-v1.5.2.23)（版本 `1.5.2.23`）

## 二、安装

### 2.1 安装顺序也可能会有所影响

安装顺序：OpenNI --> SensorKinect --> NITE

### 2.2 安装 OpenNI

#### step1：安装 gcc 和 g++ 4.x 版本

```shell
# 1. 查看自己当前的 gcc 版本
gcc -v
# 2. 安装低版本的 gcc
sudo apt-get install gcc-4.8
# 3. 管理 gcc 版本
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 40 # gcc-7 是原本系统安装的高版本 40是优先级
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 30 # gcc-4.8 是刚安装的低版本 30是优先级
# 4. 配置默认的 gcc
sudo update-alternatives --config gcc
# 5. 按照提示输入 gcc4.8 的序号
```

```shell
# g++ 同理
sudo apt-get install g++-4.8
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 40
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 30
sudo update-alternatives --config g++
```

#### step2：安装依赖

```shell
sudo apt-get install libusb-1.0-0-dev freeglut3-dev openjdk-8-jdk
```

#### step3：安装OpenNI

```shell
cd OpenNI/Platform/Linux/CreateRedist
./RedistMaker

cd ../Redist/OpenNI-Bin-Dev-Linux-[xxx] # (where [xxx] is your architecture and this particular OpenNI release)
sudo ./install.sh
```

### 2.3 安装 SensorKinect

```shell
cd SensorKinect/Platform/Linux/CreateRedist
./RedistMaker

cd ../Redist/Sensor-Bin-Linux-[xxx] # (where [xxx] is your architecture and this particular SensorKinect release)
sudo ./install.sh
```

### 2.4 安装 NITE

```SHELL
cd NITE/x64
sudo ./install.sh
```

### 2.5 把 `gcc` 和 `g++` 的版本换回高版本的

要不然下次编译其他项目报错也不知道哪里的问题

## 三、配置环境

1. 新建文件 `libopenni.pc`

    ```shell
    sudo vim /usr/lib/pkgconfig/libopenni.pc
    ```

2. 添加以下内容

    ```txt
    prefix=/usr
    exec_prefix=${prefix}
    libdir=${exec_prefix}/lib
    includedir=${prefix}/include/ni

    Name: OpenNI
    Description: A general purpose driver for all OpenNI cameras.
    Version: 1.5.7.10
    Cflags: -I${includedir}
    Libs: -L${libdir} -lOpenNI
    ```

## 四、测试

### 4.1 RGBD 摄像头测试

说明：运行后，左方为深度图像，右方为 RGB 图像。此时可以用鼠标点击界面选择功能，按键盘的数字也可以切换。

```shell
./OpenNI/Platform/Linux/Bin/x64-Release/NiViewer
```

### 4.2 检测并跟踪手掌

说明：运行后，需要晃动手掌，以便被 Kinect 检测。

```shell
./OpenNI/Platform/Linux/Bin/x64-Release/Sample-NiHandTracker
```

### 4.3 联合 opencv 测试

* 说明：该代码会从 Kinect 中读取 RGB 图像并通过 opencv 显示出来（需要更新环境变量）。

```shell
# 1. 没有创建 libopenni.pc 用下面命令编译
g++ test.cpp -o cap -I/usr/include/ni -lOpenNI `pkg-config --cflags --libs opencv`
# 2. 创建了 libopenni.pc 用下面命令编译
g++ test.cpp -o cap `pkg-config --cflags --libs opencv libopenni`
```

## 四、问题

1. Unable to create GestureGenerator.Viewer init failed: Can't create any node of the requested type!
    * 问题：当我运行 `OpenNI/Platform/Linux/Bin/x64-Release/Sample-NiHandTracker` 的时候，报的错误。
    * 解决方法：重新安装 NITE 。（参考9，10）

2. Open failed: Bad Parameter sent to the device!
    * 问题：移动虚拟机后或者隔段时间没有用Kinect会出现此问题
    * 解决方法：卸载`SensorKinect`，重新安装

    ```shell
    cd SensorKinect/Platform/Linux/Redist/Sensor-Bin-Linux-x64-v5.1.2.1
    sudo ./install.sh -u # 卸载
    sudo ./install.sh # 重新安装
    ```

## 五、参考

1. [Cannot Build OpenNI 1.5.7 in Ubuntu 18.04 LTS #127](https://github.com/OpenNI/OpenNI/issues/127)（安装OpenNI出错）
2. [安装 SensorKinect 出错](https://stackoverflow.com/questions/28319336/xndevicesensorv2-failed-on-raspberry-pi/28323843#28323843)
3. [Kinect on Ubuntu with OpenNI](https://www.20papercups.net/programming/kinect-on-ubuntu-with-openni/)
4. [Ubuntu安装Kinect驱动（openni、NITE、Sensor）及遇到的问题](https://blog.csdn.net/u013453604/article/details/48013959)（该博主给的安装包应该是安装好然后打包的）
5. [Ubuntu + Kinect + OpenNI + PrimeSense](http://mitchtech.net/ubuntu-kinect-openni-primesense/)
6. [Kinect开发教程一：OpenNI的安装与开发环境配置](https://blog.csdn.net/chenxin_130/article/details/6693390)
7. [OpenNI的 Github](https://github.com/OpenNI/OpenNI)（注意版本，参考安装步骤）
8. [OpenNI 1.5.4 Documents](https://documentation.help/OpenNI/index.html)
9. [Can't create any node of the requested type! #2](https://github.com/tomoto/kinect-ultra/issues/2)
10. [kinect-ultra-问题＃2](https://code.google.com/archive/p/kinect-ultra/issues/2)

## 六、更新说明

1. 2020/06/09 更新：
    * 补充环境配置
    * 补充问题说明
2. 2020/06/04 更新：
    * 补充安装 OpenNI 的依赖说明
3. 2020/05/29 更新：
    * 修改 NITE 版本编写错误。
    * 优化 `4.3` 测试代码。
    * 增加问题说明
