# Ubuntu 安装 KinectV1 驱动 OPenNI+SensorKinect+NITE

---

## 一、环境与准备

### 1.1 环境

* Ubuntu1804x64(VMware)
* Kinect 需要和电脑的 USB3.0 连接，并且开启虚拟机的兼容USB3.0（步骤：虚拟机-->设置-->USB控制器，右方设置）
* 安装 OpenNI 需要 `gcc` `g++`  4.x 版本（[百度百科说的](https://baike.baidu.com/item/OpenNI)）。

### 1.2 准备

```shell
git clone git@github.com:yiyah/KinectV1_Driver-SensorKinect.git
````

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

#### step2：安装OpenNI

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

## 三、测试

### 3.1 RGBD 摄像头测试

说明：运行后，左方为深度图像，右方为 RGB 图像。此时可以用鼠标点击界面选择功能，按键盘的数字也可以切换。

```shell
./OpenNI/Linux/Bin/x64-Release/NiViewer
```

### 3.2 检测并跟踪手掌

说明：运行后，需要晃动手掌，以便被 Kinect 检测。

```shell
./OpenNI/Platform/Linux/Bin/x64-Release/Sample-NiHandTracker
```

### 3.3 联合 opencv 测试

* 说明：该代码会从 Kinect 中读取 RGB 图像并通过 opencv 显示出来。

```shell
cd KinectV1_Driver-SensorKinect
g++ test.cpp -o cap `pkg-config --cflags --libs opencv libopenni`
./cap
```

## 四、参考

1. [Cannot Build OpenNI 1.5.7 in Ubuntu 18.04 LTS #127](https://github.com/OpenNI/OpenNI/issues/127)（安装OpenNI出错）
2. [安装 SensorKinect 出错](https://stackoverflow.com/questions/28319336/xndevicesensorv2-failed-on-raspberry-pi/28323843#28323843)
3. [Kinect on Ubuntu with OpenNI](https://www.20papercups.net/programming/kinect-on-ubuntu-with-openni/)
4. [Ubuntu安装Kinect驱动（openni、NITE、Sensor）及遇到的问题](https://blog.csdn.net/u013453604/article/details/48013959)（该博主给的安装包应该是安装好然后打包的）
5. [Ubuntu + Kinect + OpenNI + PrimeSense](http://mitchtech.net/ubuntu-kinect-openni-primesense/)
6. [Kinect开发教程一：OpenNI的安装与开发环境配置](https://blog.csdn.net/chenxin_130/article/details/6693390)
7. [OpenNI的 Github](https://github.com/OpenNI/OpenNI)（注意版本，参考安装步骤）
