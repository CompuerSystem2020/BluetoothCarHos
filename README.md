# 这里是state3-Hos软件部分的说明文档

## 主要任务

实现小车蓝牙模块和马达驱动模块的软件设计。

## 配置需要

同state2。

## 步骤

### 1.在arch.h中注册分配地址

由于蓝牙数据读取通过串口模块UART实现，添加UART串口模块地址UART_BASE
由于马达驱动模块通过PMOD接口实现，添加PMOD接口模块地址WHEEL_BASE
![arch.h中注册分配地址](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/base_address.png)

### 2.中断调用解释

对于蓝牙模块和马达驱动模块都通过串口中断来实现，因此观察trap.c文件及，发现在函数trap_dispatch中处理中断是通过调用函数interrupt_handler实现
![trap.dispatch函数](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/trap_dispatch.png)
观察interrupt_handler函数和trapframe结构，interrupt_handler函数对于不同原因的中断请求通过优先级计算进行不同顺序的处理(1)，对于6号cause register保存的待处理中断信号调用serial_int_handler进行处理(2)
![trapframe结构](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/trapframe.png)
![interrupt_handler函数](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/interrupt_handler.png)
PS：但是发现注释与实际情况不符如图所示：（未作修改/doge）
![trouble](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/trouble.png)

### 3.中断处理解释

首先在get_eic函数中根据中断请求来决定处理顺序(1)，由于中断请求由向量位置决定，因此LSB(即0比特位)有最高优先级，MSB有最低优先级，0号比特位用于存储马达驱动对应的中断请求，1号比特位用于存储蓝牙控制对应的中断请求
![get_eic函数](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/get_eic.png)
在serial_int_handler中调用get_eic函数，根据当前需要处理的中断请求分别对马达驱动和蓝牙进行处理，若无中断请求，则完成其他系统操作。
![serial_int_handler函数](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/serial_int_handler.png)
对于马达驱动的中断处理和蓝牙的处理，建议直接看serial_int_handler函数，都是对于指令的一些响应操作和数据更新orz

### 4.模块初始化解释
由于蓝牙模块和马达驱动模块均不是使用新建驱动设备来实现的，因此需要在内核的初始化中进行初始化。
![初始化](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state3/pic/initial.png)