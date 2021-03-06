# YiOS
![image](https://user-images.githubusercontent.com/8001473/113387487-fcd7c300-93be-11eb-9bb7-e379af25cc26.png)

## Table of Contents

   * [Background](#background)
   * [What is YiOS](#what-is-yios)
   * [Test YiOS on Qemu](#test-yios-on-qemu)
      * [Download and build](#download-and-build)
      * [Download Qemu](#download-qemu)
      * [Run YiOS on Qemu](#run-yios-on-qemu)
      * [Debug](#debug)
   * [License](#license)


## Background

Since graduating from university until now, I have always had a desire to write an operating system by myself, so I have been learning Linux and hardware architecture-related things. The X86 hardware is too complicated, so I chose the relatively simple ARMV8 architecture. I hope this project can deepen my understanding of ARMV8 and operating system.

## What is YiOS

YiOS is a simple operating system based on ARMv8. It implements basic operating system modules such as SMP, memory management, and process scheduling. I am still improving it. If you like it, please give a star.

## Test YiOS on Qemu

### Download and build

```
# get toolchain
wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/9.2-2019.12/binrel/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu.tar.xz

tar -xvf gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu.tar.xz
sudo mv gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/ /opt/
# get code
git clone https://github.com/jason--liu/YiOS

cd YiOS
export CROSS_COMPILE=/opt/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
make 

```

### Download Qemu

Now you can simulate the development board through QEMU instead of buying a real board.

``` 
git clone -b raspi4_wip https://gitlab.com/philmd/qemu.git
cd qemu
./configure --enable-debug --target-list=x86_64-softmmu --target-list=aarch64-softmmu
make
```

### Run YiOS on Qemu

```
export QEMU_PATH=(path to your qemu project)/aarch64-softmmu/qemu-system-aarch64
cd YiOS
make run
```

### Debug

Before starting to run YiOS, because Ubuntu20.04's qemu does not support Raspberry Pi 4b, you need to compile qemu separately

YiOS only supports raspberry 4B currently. you can use qemu to monitor that.

```
cd YiOS
make debug
```
then,on another terminal

```
gdb-multiarch yios.elf -tui
> target remote localhost:1234
> b kernel_main
> c
```

Hope you enjoy it.

## License

GPL2
