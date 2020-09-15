# 这里是state1-Hos软件部分的说明文档

## 主要任务

将Hos系统写入到开发板当中，并能够通过端口访问运行系统指令

## 配置需要

1. 开发板已经成功烧入MIPS CPU
2. openOCD最新版本 + Zadig2.2+版本
3. mips-mti-elf安装，最新版本即可
4. cygwin64下安装make、gcc、g++、perl任意版本都可

## 步骤

### 1.安装下载驱动

在OpenOCD安装目录下找到Zadig执行程序，要求版本号为2.2以上，否则可能找不到，可以在本项目目录下的CompileTool目录下找到2.2版本执行文件，复制到OpenOCD安装目录下执行即可。之后按照指导书指示完成即可。

### 2.编译Hos

使用cygwin64自带的shell完成内核的编译，如果找不到图标则可以到cygwin64的安装目录下找到Cygwin.bat执行脚本，运行即可。随后切换到Hos目录下make即可。忽略warning。

### 3.下载到开发板

1. 修改debug目录下的openocd.cfg文件，修改方式即为指定本机安装OpenOCD的目录。![openocd_cfg_modify](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state1/pics/openocd_cfg_modify.png)
2. 修改Hos根目录下的run.bat脚本。![run_bat_modify](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state1/pics/run_bat_modify.png)

需要修改PUTTY的读取端口，具体的端口需要到设备管理器中读取具体的端口号，这在指导书上有。此外，找到端口后需要在其属性中修改每秒读取的速率为115200，电脑可能默认为9600。

将 “start cmd /C loadMIPSfpga.bat ../obj/kernel/ucore-kernel-initrd” 注释掉，这一行指令属于脏代码（在项目文件中也将loadMIPSfpga.bat脚本删除了），根据实验测试，其主要的作用为启动本机的openOCD，因此可以使用以下语句进行代替，其中cmd为打开一个命令行，/K为保持窗口，因为我们要观察openOCD中的内容，后面引号为你本机中openOCD的执行文件路径。

```shell
start cmd /K "C:\Program Files\Imagination Technologies\OpenOCD-0.10.0.3-img\bin\openocd.exe"
```

### 4.最终效果

使用cygwin64自带的shell工具运行./run.bat脚本（或者直接用cmd运行也可以），该窗口显示为

![cygwin64_wait_enter](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state1/pics/cygwin64_wait_enter.png)

即等待键入以继续，键入后，该窗口显示为（需要等一段时间）

![cygwin_final_show](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state1/pics/cygwin_final_show.png)

此时还会代开openOCD以及putty窗口，分别如下

![openocd_final_show](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state1/pics/openocd_final_show.png)

![putty_show](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state1/pics/putty_show.png)

可以看到此时putty已经可以从端口处获取系统运行结果了，但是支持的指令不是很多，目前支持ls、cat等。

![putty_ls](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state1/pics/putty_ls.png)

至此第一阶段完成。

此外，如果想要重复写入Hos系统的话，最好将开发板的CPU重置，否则可能会有一些执行错误，比如栈帧找不到之类的。

## 感想

哎，Hos的运行脚本太脏了。

