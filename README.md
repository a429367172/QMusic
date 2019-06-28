# QMusic
## 要求
采用QT开发技术，在物联网试验箱上实现一个多媒体播放器，系统上电启动后自动启动此播放器程序。程序自动读取U盘中的所有多媒体格式文件，生成多媒体播放列表，并自动播放第一首歌曲或视频。具有播放、暂停、停止、快进、快退等功能。可以手动选择文件播放。支持自动顺序播放和循环播放。要求支持mp4、WMV、mp3、wav等常见格式文件。播放程序可从开源软件移植到开发板上。
## 环境要求
1.Qt4.7  
2.安装myplayer  
3.搭建qmake-arm交叉编译环境  
## 配置方法
### 1.新建Qt工程
### 2.将文件拷贝的Qt工程下
### 3.交叉编译  
1.执行make clean  
2.执行qmake-arm，生成Makefile文件  
3.执行make命令，生成二进制文件QMusic  
### 4.拷贝QMusic和images文件夹到ARM板上
### 5.U盘挂载
a.插入U盘  
b.执行fdisk -l命令，查看U盘设备，假设为/dev/sda
c.执行mkdir /mnt/usb创建U盘挂载点
d.执行mount /dev/sda /mnt/usb进行挂载
### 6.运行程序
chmod +x QMusic
./QMusic
