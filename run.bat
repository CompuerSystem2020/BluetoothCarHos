CD /D debug
:call program.bat	%NOTE: this command will overwrite your soft MIPS-core with standard core. Make sure you have vivado in PATH before using it%
start "" PUTTY.EXE -serial COM8 -sercfg 115200
:start cmd /C loadMIPSfpga.bat ../obj/kernel/ucore-kernel-initrd
start cmd /K "C:\Program Files\Imagination Technologies\OpenOCD-0.10.0.3-img\bin\openocd.exe"
mips-mti-elf-gdb.exe ../obj/kernel/ucore-kernel-initrd -x startup-ucore.txt
CD /D ..
EXIT
