1.osdrv commands description:
The design idea of this catalog is to fulfil the requirement that compiling the same set of source code with two different compilation tool chains. So an extra parameter to specify a specific compiler tool chain is needed. The arm-hisiv100nptl-linux toolchain is for uclibc, and the arm-hisiv200-linux toolchain for glibc. Specific commands are as follows:
(1)Compile the entire osdrv directory:
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3521 all
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520a all
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=full all
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=mini all
	or:
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3521 all
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520a all
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520d all
(2)Clear all compiled files under osdrv directory:
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3521 clean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520a clean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=full clean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=mini clean
	or:
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3521 clean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520a clean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520d clean
(3)Completely remove all compiled files under osdrv directory, and the generated images:
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3521 distclean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520a distclean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=full distclean
	make OSDRV_CROSS=arm-hisiv100nptl-linux CHIP=hi3520d OSDRV_SIZE=mini distclean
	or:
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3521 distclean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520a distclean
	make OSDRV_CROSS=arm-hisiv200-linux CHIP=hi3520d distclean
(4)Separately compile kernel:
	Enter the top directory the kernel source code, do the following:
	cp arch/arm/configs/godarm_defconfig .config
	cp arch/arm/configs/godcare_defconfig .config
	cp arch/arm/configs/hi3520d_full_defconfig .config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- menuconfig
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- uImage
	or:
	cp arch/arm/configs/godarm_defconfig .config
	cp arch/arm/configs/godcare_defconfig .config
	cp arch/arm/configs/hi3520d_full_defconfig .config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- menuconfig
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- uImage
(5)Separately compile uboot:
	Enter the top directory of boot source code, do the following:
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- godarm_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- godcare_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- hi3520d_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux-
	or:
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- godarm_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- godcare_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux- hi3520d_config
	make ARCH=arm CROSS_COMPILE=arm-hisiv200-linux-
(6)Build file system image:
	A compiled file system has already been in osdrv/pub/, so no need to re-build file system. All what you need to do is to build the right file system image according to the flash specification of the single-board. 
	Only image of jffs2 format is available for spi flash. While making jffs2 image, you need to specify the spi flash block size. flash block size will be printed when uboot runs. run mkfs.jffs2 first to get the right parameters from it's printed information. Here the block size is 64KB, for example:

	osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_uclibc -l -e 0x40000 -o osdrv/pub/rootfs_uclibc_256k.jffs2
	or:
	osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_glibc -l -e 0x40000 -o osdrv/pub/rootfs_glibc_256k.jffs2

	Only image of yaffs2 format is available for nand flash. While making yaffs2 image, you need to specify it's page size and ECC. This information will be printed when uboot runs. run mkyaffs2image first to get the right parameters from it's printed information. Here to 2KB pagesize, 1bit ecc, for example:
	
	osdrv/pub/bin/pc/mkyaffs2image osdrv/pub/rootfs_uclibc osdrv/pub/rootfs_uclibc_2k_1bit.yaffs2 1 1
	or:
	osdrv/pub/bin/pc/mkyaffs2image osdrv/pub/rootfs_glibc osdrv/pub/rootfs_glibc_2k_1bit.yaffs2 1 1


2. Image storage directory description
The compiled image, rootfs, etc. stored in osdrv/pub directory
pub
│  rootfs_uclibc.tgz ------------------------------------------ hisiv100nptl compile the rootfs file system
│  rootfs_glibc.tgz ------------------------------------------- hisiv200 compile the rootfs file system
│
├─image_glibc ------------------------------------------------- hisiv200 compile the image file
│      uImage_hi35xx---------------------------------------------- kernel image
│      u-boot-hi35xx_xxMHz.bin --------------------------------- u-boot image
│      rootfs_hi35xx_xxk.jffs2 --------------------------------- jffs2 rootfs image(Corresponding to the spi-flash blocksize = 256K)
│      rootfs_hi35xx_2k_1bit.yaffs2 ---------------------------- yaffs2 rootfs image(Corresponding to the nand-flash pagesize=2K ecc=1bit)
│
├─image_uclibc ----------------------------------------------- hisiv100nptl compile the image file
│      uImage_hi35xx ----------------------------------------- kernel image
│      u-boot-hi35xx_xx_MHz.bin ------------------------------ u-boot image
│      rootfs_hi35xx_xxk.jffs2 -------------------------------- jffs2 rootfs image(Corresponding to the spi-flash blocksize = xxK)
│      rootfs_hi35xx_xxk.squashfs ----------------------------- squashfs rootfs image(Corresponding to the spi-flash blocksize = xxK)
│      rootfs_hi35xx_2k_1bit.yaffs2 --------------------------- yaffs2 rootfs image(Corresponding to the nand-flash pagesize=2K ecc=1bit)
│
└─bin
    ├─pc
    │      mkfs.jffs2
    │      mkimage
    │      mkfs.cramfs
    │      mkyaffs2image
    │
    ├─board_glibc -------------------------------------------- hisiv200 compiles the message board with communications tools
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
    └─board_uclibc ------------------------------------------- hisiv100nptl compiles the message board with communications tools
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


3.osdrv directory structure：
osdrv
├─Makefile ------------------------------ osdrv catalog compiled script
├─busybox ------------------------------- Storage busybox source code directory
├─tools --------------------------------- Storing tools directory
│  ├─board_tools ----------------------- A variety of single-board tools
│  │  ├─reg-tools-1.0.0 --------------- Register read and write tool
│  │  ├─mtd-utils --------------------- tool to read and write flash nude
│  │  ├─udev-100 ---------------------- udev toolset
│  │  ├─gdb --------------------------- gdb tools
│  │  ├─parted ------------------------ Large-capacity hard disk partitioning tool
│  │  └─e2fsprogs --------------------- mkfs tools
│  └─pc_tools -------------------------- Tools on a variety of pc
│      ├─mkfs.cramfs ------------------- tools for making cramfs file system
│      ├─mkfs.jffs2 -------------------- tools for making jffs2 file system
│      ├─mkimage ----------------------- tools for making uImage
│      ├─mkyaffs2image301 -------------- yaffs2 file system creation tools
│      └─uboot_tools ------------------- uboot image creation tools, xls files and the ddr initialization script, bootrom tool
├─toolchain ----------------------------- Tool chain store directory
│  ├─arm-hisiv100nptl-linux ---------------- hisiv100nptl cross tool chain
│  └─arm-hisiv200-linux ---------------- hisiv200 cross tool chain
├─pub ----------------------------------- A variety of image storage directory
│  ├─image_glibc ----------------------- Compiler tool chain based hisiv100nptl available FLASH burning of image files, including uboot, kernel, file system
│  ├─image_uclibc ---------------------- Compiler tool chain based hisiv200 available FLASH burning of image files, including uboot, kernel, file system
│  ├─bin ------------------------------- Not placed in the root file system of various tools
│  │  ├─pc ---------------------------- Tools running on the pc
│  │  ├─board_glibc ------------------- Compiler tool chain based hisiv100nptl, implementation of tools in a single board
│  │  └─board_uclibc ------------------ Compiler tool chain based hisiv200, implementation of tools in a single board
│  ├─rootfs_uclibc.tgz ----------------- Hisiv100nptl tool chain based on the root file system compiled
│  └─rootfs_glibc.tgz ------------------ Hisiv200 tool chain based on the root file system compiled
├─rootfs_scripts ------------------------ Production of storage root file system directory script
├─uboot --------------------------------- Store uboot source code directory
└─kernel -------------------------------- Store kernel source code directory


4.Note
(1)If a tool chain needs to be replaced by the other, remove the original compiled files compiled with the former tool chain, and then replace the compiler tool chain with the other. 
(2)Executable files in Linux may become non-executable after copying them to somewhere else under Windows, and result in souce code cannot be compiled. Many symbolic link files will be generated in the souce package after compiling the u-boot or the kernel. The volume of the source package will become very big, because all the symbolic link files are turning out to be real files in Windows. So, DO NOT copy source package in Windows.
(3)The Hi3521 has no hard floating-point unit. The library provided by the file system is a library without hard floating-points units. The tool chain uses armv5 instruction set by default, but the Hi3521 uses armv7 instruction set. Therefore, add the following commands in Makefile when compiling the code of all Hi3521 SDKs.
	CFLAGS += -march=armv7-a -mcpu=cortex-a9
	CXXFlAGS +=-march=armv7-a -mcpu=cortex-a9
	Among these, CXXFlAGS may be different according to the specific name in user's Makefile. For example, it maay be changed to CPPFLAGS.
