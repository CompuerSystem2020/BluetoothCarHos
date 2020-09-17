# 这里是state2-Hos软件部分的说明文档

## 主要任务

实现VScode的交叉编译以及联调，实现用户态和核态的“hello world！”。

## 配置需要

同state1，除此之外需要最新版的VScode，并安装相应的插件（Native Debug）。

## 步骤

### 1.烧录硬件

要求同state1，使用vivado烧录即可。

### 2.修改run.bat文件

此处修改为将mips-mti-elf-gdb的启用行注释掉，因为我们需要使用VScode进行联调，因此需要不能再run.bat中启用，而是在VScode中启用。

![run.bat修改](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/run_bat_change.png)

### 3.实现hello world的系统调用

此部分系统调用的实现需要结合之前的操作系统课设理解，可以分成以下步骤完成。

#### a.添加系统调用

在kern-ucore目录下找到syscall.c文件，首先找到syscalls函数指针数组，在里面做如下的添加。

![syscalls添加系统调用号和函数的对应关系](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/syscall_change_1.png)

这里我们先假设调用号为SYS_hello，后面会有定义，对应的函数名为sys_hello。

#### b.实现sys_hello函数

这里sys_hello的实现属于调用底层驱动的实现，因此不可能使用stdio.h，而是mips_io，可以参考kputc的实现，因为只是打印hello world，因此可以没有参数。

![sys_hello函数实现](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/syscall_change_2.png)

#### c.定义SYS_hello

在kern-ucore\indlude\lib目录下添加SYS_hello对应的系统调用号，随便指定即可。

![SYS_hello的定义](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/unistd_change.png)

### 4.用户态实现

用户在进行系统调用时，使用的是用户态的syscall，这个可以在user目录下找到，可以看到函数原型为：

```C
uintptr_t syscall(int num, ...)
{
	va_list ap;
	va_start(ap, num);
	uint32_t arg[MAX_ARGS];
	int i, ret;
	for (i = 0; i < MAX_ARGS; i++) {
		arg[i] = va_arg(ap, uint32_t);
	}
	va_end(ap);

	//num += SYSCALL_BASE;//modified
	asm volatile (".set noreorder;\n" "move $v0, %1;\n"	/* syscall no. */
		      "move $a0, %2;\n"
		      "move $a1, %3;\n"
		      "move $a2, %4;\n"
		      "move $a3, %5;\n"
		      "move $t0, %6;\n"
		      "syscall;\n" "nop;\n" "move %0, $v0;\n":"=r" (ret)
		      :"r"(num), "r"(arg[0]), "r"(arg[1]), "r"(arg[2]),
		      "r"(arg[3]), "r"(arg[4])
		      :"a0", "a1", "a2", "a3", "v0", "t0");
	return ret + 1 - 1;
}
```

num为系统调用号，其余参数为系统调用的参数，因此如果我们想要调用sys_hello的内核函数，则只需要syscall(SYS_hello)即可。但是由于我们无法从内核include SYS_hello，因此我们需要重新将SYS_hello定义一边，具体的实现形式为在user/include目录下的unistd.h中添加SYS_hello即可，注意要跟之前的添加相同。

![user下的SYS_hello定义](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/user_unistd_change.png)

这里的SYS_hello定义是供syscall(SYS_hello)使用。

最后为了方便其余程序调用sys_hello核函数，我们可以在用户态实现一下，类似与接口的实现。

首先在user/include目录下找到syscall.h，做如下的添加。

![syscall.h中添加函数头部](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/user_syscall_h_change.png)

之后在syscall.c中实现sys_hello函数。

![syscall.c中的实现](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/user_syscall_change.png)

核心就是实现syscall(SYS_hello)。

### 5.程序调用

指导书要求我们能够将hello world程序当作系统指令实现，因此我们在user/user-core下创建一个hello.c文件，里面的具体实现如下。

![hello.c文件实现](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/hello_c.png)

其中使用printf的实现为调用已经封装好的用户态函数fprintf实现，该函数保存在stdio.h中；sys_hello的实现为调用syscall.h中的sys_hello。

之后在项目根目录下找到Makefile.config，将hello指令添加进去即可。

```shell
USER_APPLIST:= pwd cat sh ls cp echo mount umount hello # link mkdir rename unlink lsmod insmod 
```

### 6.运行测试

如下对.vscode文件下的launch.json文件进行修改。

![launch.json文件修改](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/launch_json.png)

(1)(2)不修改也可以，这里是为了明确执行路径；(3)原先为mips-sde-elf-gdb，我在运行时系统提示找不到可执行文件，跟老师扯了一会儿他也没有给出明确的问题，还说我交叉编译环境有问题，然后发了他的环境跟我的一模一样（，然后根据run.bat的提示可以知道，这里显然要改成mips-mti-elf-gdb；(4)这里最开始的时候是在kern_init那里打了一个断点，但是我在后面的执行中发现，如果在那里打断点，无论怎么前进，系统老是卡在tlb_invalidate_all那一条语句，而且openOCD会报错，不过感觉没有必要在这里纠结，我觉得可能是VScode的原因，这里就直接先把那个断点去掉了。后面是可以正常执行并且也是可以打断点执行的。

使用cygwin编译一下，执行run.bat脚本，之后在VScode中按F5，等一会儿。

![最终结果展示](https://github.com/CompuerSystem2020/BluettoothCarHos/blob/state2/pic/hello_world.png)

## 感想

环境、基础部分全部完成，后面才是真的硬核，希望不要出事。