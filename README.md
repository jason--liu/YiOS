# YiOS
![image](https://user-images.githubusercontent.com/8001473/113387487-fcd7c300-93be-11eb-9bb7-e379af25cc26.png)

## Table of Contents

- [Background](#background)
- [What is YiOS](#What is YiOS)
- [License](#license)

## Background

Since graduating from university until now, I have always had a desire to write an operating system by myself, so I have been learning Linux and hardware architecture-related things. The X86 hardware is too complicated, so I chose the relatively simple ARMV8 architecture. I hope this project can deepen my understanding of ARMV8 and operating system.

## What is YiOS

YiOS is a simple operating system based on ARMv8. It implements basic operating system modules such as SMP, memory management, and process scheduling. I am still improving it. If you like it, please give a star.

## Test YiOS on Raspberry 4B

### Download and build

```
git clone https://github.com/jason--liu/YiOS
cd YiOS
make 
make run
```

### Debug

prepare:

YiOS only supports raspberry 4B currently. you can use qemu to monitor that.

```
git clone -b raspi4_wip https://gitlab.com/philmd/qemu.git
cd qemu
./configure --enable-debug --target-list=x86_64-softmmu --target-list=aarch64-softmmu
make

cd YiOS
make debug
```

debug YiOS

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