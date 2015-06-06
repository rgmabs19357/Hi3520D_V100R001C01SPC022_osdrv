1.osdrv使用说明
本目录设计思路为一套源代码支持两种工具链编译，因此需要通过编译参数指定不同的工具链。其中arm-hisiv100nptl-linux是uclibc工具链，arm-hisiv200-linux是glibc工具链。具体命令如下
(1)编译整个osdrv目录：
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3521 all
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520a all
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=full all
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=mini all
	或者
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3521 all
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520a all
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520d all
(2)清除整个osdrv目录的编译文件：
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3521 clean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520a clean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=full clean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=mini clean
	或者
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3521 clean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520a clean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520d clean
(3)彻底清除整个osdrv目录的编译文件，除清除编译文件外，还删除已编译好的镜像：
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3521 distclean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520a distclean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=full distclean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=mini distclean
	或者
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3521 distclean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520a distclean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520d distclean
(4)单独编译kernel：
	待进入内核源代码目录后，执行以下操作
	cp arch/arm/configs/godarm_defconfig .config
	cp arch/arm/configs/godcare_defconfig .config
	cp arch/arm/configs/hi3520d_full_defconfig .config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- menuconfig
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- uImage
	或者
	cp arch/arm/configs/godarm_defconfig .config
	cp arch/arm/configs/godcare_defconfig .config
	cp arch/arm/configs/hi3520d_full_defconfig .config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- menuconfig
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- uImage
(5)单独编译uboot：
	待进入boot源代码目录后，执行以下操作
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- godarm_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- godcare_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- hi3520d_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux-
	或者
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- godarm_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- godcare_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- hi3520d_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux-
(6)制作文件系统镜像：
在osdrv/pub/中有已经编译好的文件系统，因此无需再重复编译文件系统，只需要根据单板上flash的规格型号制作文件系统镜像即可。

	spi flash使用jffs2格式的镜像，制作jffs2镜像时，需要用到spi flash的块大小。这些信息会在uboot启动时会打印出来。建议使用时先直接运行mkfs.jffs2工具，根据打印信息填写相关参数。下面以块大小为64KB为例：
	osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_uclibc -l -e 0x40000 -o osdrv/pub/rootfs_uclibc_256k.jffs2
	或者
	osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_glibc -l -e 0x40000 -o osdrv/pub/rootfs_glibc_256k.jffs2

	nand flash使用yaffs2格式的镜像，制作yaffs2镜像时，需要用到nand flash的pagesize和ecc。这些信息会在uboot启动时会打印出来。建议使用时先直接运行mkyaffs2image工具，根据打印信息填写相关参数。下面以2KB pagesize、1bit ecc为例：
	osdrv/pub/bin/pc/mkyaffs2image osdrv/pub/rootfs_uclibc osdrv/pub/rootfs_uclibc_2k_1bit.yaffs2 1 1
	或者
	osdrv/pub/bin/pc/mkyaffs2image osdrv/pub/rootfs_glibc osdrv/pub/rootfs_glibc_2k_1bit.yaffs2 1 1


2. 镜像存放目录说明
编译完的image，rootfs等存放在osdrv/pub目录下
pub
│  rootfs_uclibc.tgz ------------------------------------------ hisiv100nptl编译出的rootfs文件系统
│  rootfs_glibc.tgz ------------------------------------------- hisiv200编译出的rootfs文件系统
│
├─image_glibc ------------------------------------------------- hisiv200编译出的镜像文件
│      uImage_35xx---------------------------------------------- kernel镜像
│      u-boot-hi35xx_xxMHz.bin---------------------------------- u-boot镜像
│      rootfs_hi35xx_256k.jffs2 ---------------------------------- jffs2 rootfs镜像(对应spi-flash blocksize=256K)
│      rootfs_hi35xx_2k_1bit.yaffs2 ------------------------------ yaffs2 rootfs镜像(对应nand-flash pagesize=2K ecc=1bit)
│
├─image_uclibc ------------------------------------------------ hisiv100nptl编译出的镜像文件
│      uImage_hi35xx-------------------------------------------- kernel镜像
│      u-boot-hi35xx_xxMHz.bin---------------------------------- u-boot镜像
│      rootfs_hi35xx_xxk.jffs2 --------------------------------- jffs2 rootfs镜像(对应spi-flash blocksize=xxK)
│      rootfs_hi35xx_xxk.squashfs ------------------------------ squashfs rootfs镜像(对应spi-flash blocksize=xxK)
│      rootfs_hi35xx_2k_1bit.yaffs2 ---------------------------- yaffs2 rootfs镜像(对应nand-flash pagesize=2K ecc=1bit)
│
└─bin
    ├─pc
    │      mkfs.jffs2
    │      mkimage
    │      mkfs.cramfs
    │      mkyaffs2image
    │
    ├─board_glibc -------------------------------------------- hisiv200编译出的单板用工具
    │      flash_eraseall
    │      flash_erase
    │      nandwrite
    │      mtd_debug
    │      flash_info
    │      sumtool
    │      mtdinfo
    │      flashcp
    │      nandtest
    │      nanddump
    │      parted_glibc
    │      gdb-arm-hisiv200-linux
    │
    └─board_uclibc ------------------------------------------- hisiv100nptl编译出的单板用工具
            flash_eraseall
            flash_erase
            nandwrite
            mtd_debug
            flash_info
            parted_uclibc
            sumtool
            mtdinfo
            flashcp
            nandtest
            gdb-arm-hisiv100nptl-linux
            nanddump


3.osdrv目录结构说明：
osdrv
├─Makefile ------------------------------ osdrv目录编译脚本
├─busybox ------------------------------- 存放busybox源代码的目录
├─tools --------------------------------- 存放各种工具的目录
│  ├─board_tools ----------------------- 各种单板上使用工具
│  │  ├─reg-tools-1.0.0 --------------- 寄存器读写工具
│  │  ├─mtd-utils --------------------- flash裸读写工具
│  │  ├─udev-100 ---------------------- udev工具集
│  │  ├─gdb --------------------------- gdb工具
│  │  ├─parted ------------------------ 大容量硬盘分区工具
│  │  └─e2fsprogs --------------------- mkfs工具集
│  └─pc_tools -------------------------- 各种pc上使用工具
│      ├─mkfs.cramfs ------------------- cramfs文件系统制作工具
│      ├─mkfs.jffs2 -------------------- jffs2文件系统制作工具
│      ├─mkimage ----------------------- uImage制作工具
│      ├─mkyaffs2image301 -------------- yaffs2文件系统制作工具
│      └─uboot_tools ------------------- uboot镜像制作工具、xls文件及ddr初始化脚本、bootrom工具
├─toolchain ----------------------------- 存放工具链的目录
│  ├─arm-hisiv100nptl-linux ---------------- hisiv100nptl交叉工具链
│  └─arm-hisiv200-linux ---------------- hisiv200交叉工具链
├─pub ----------------------------------- 存放各种镜像的目录
│  ├─image_glibc ----------------------- 基于hisiv100nptl工具链编译，可供FLASH烧写的映像文件，包括uboot、内核、文件系统
│  ├─image_uclibc ---------------------- 基于hisiv200工具链编译，可供FLASH烧写的映像文件，包括uboot、内核、文件系统
│  ├─bin ------------------------------- 各种未放入根文件系统的工具
│  │  ├─pc ---------------------------- 在pc上执行的工具
│  │  ├─board_glibc ------------------- 基于hisiv100nptl工具链编译，在单板上执行的工具
│  │  └─board_uclibc ------------------ 基于hisiv200工具链编译，在单板上执行的工具
│  ├─rootfs_uclibc.tgz ----------------- 基于hisiv100nptl工具链编译的根文件系统
│  └─rootfs_glibc.tgz ------------------ 基于hisiv200工具链编译的根文件系统
├─rootfs_scripts ------------------------ 存放根文件系统制作脚本的目录
├─uboot --------------------------------- 存放uboot源代码的目录
└─kernel -------------------------------- 存放kernel源代码的目录


4.注意事项
(1)使用某一工具链编译后，如果需要更换工具链，请先将原工具链编译文件清除，然后再更换工具链编译。
(2)在windows下复制源码包时，linux下的可执行文件可能变为非可执行文件，导致无法编译使用；u-boot或内核下编译后，会有很多符号链接文件，在windows下复制这些源码包, 会使源码包变的巨大，因为linux下的符号链接文件变为windows下实实在在的文件，因此源码包膨胀。因此使用时请注意不要在windows下复制源代码包。
(3)Hi3521无硬浮点单元，文件系统中发布的库都是无硬浮点单元的库。但是，工具链默认的是armv5的指令集，而Hi3521是armv7的指令集，因此请用户注意，所有Hi3521板端代码编译时需要在Makefile里面添加以下命令：
         CFLAGS += -march=armv7-a -mcpu=cortex-a9
         CXXFlAGS +=-march=armv7-a -mcpu=cortex-a9
         其中CXXFlAGS中的XX根据用户Makefile中所使用宏的具体名称来确定，e.g:CPPFLAGS。
