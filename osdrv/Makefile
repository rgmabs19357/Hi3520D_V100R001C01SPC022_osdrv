
##########################################################################################
#	prepare param
##########################################################################################
export OSDRV_DIR=$(shell pwd)
export OSDRV_CROSS
export OSDRV_SIZE ?= full
export CHIP
export REG_INFO_BIN
export UBOOT_IMAGE_BIN
export ARCH=arm
export CROSS_COMPILE=$(OSDRV_CROSS)-

# if CHIP is support, we set SUPPORT_CHIP = "yes"
SUPPORT_CHIP := "no"

ifeq ($(OSDRV_CROSS), )
$(error you must set OSDRV_CROSS first!)
endif

ifeq ($(CHIP), )
$(error you must set CHIP first!)
endif

ifeq ($(OSDRV_CROSS),arm-hisiv100nptl-linux)
OSDRV_CROSS_LIB=uclibc
BUSYBOX_CFG:=busybox_cfg_godarm_nptl
TOOLCHAIN_RUNTIME_LIB:=armv7a_soft
TOOLCHAIN_RUNTIME_LIB_C:=lib.uClibc.tgz
export OSDRV_CROSS_CFLAGS:= -march=armv7-a -mcpu=cortex-a9
endif

ifeq ($(OSDRV_CROSS),arm-hisiv200-linux)
OSDRV_CROSS_LIB=glibc
BUSYBOX_CFG:=busybox_cfg_godarm_v200
TOOLCHAIN_RUNTIME_LIB:=armv7a_soft
TOOLCHAIN_RUNTIME_LIB_C:=lib.glibc.tgz
export OSDRV_CROSS_CFLAGS:= -march=armv7-a -mcpu=cortex-a9
endif

export PUB_IMAGE:=image_$(OSDRV_CROSS_LIB)
export PUB_BOARD:=board_$(OSDRV_CROSS_LIB)
export PUB_ROOTFS:=rootfs_$(OSDRV_CROSS_LIB)

ifeq ($(CHIP),hi3521)
CHIP_KERNEL_CFG:=godarm_defconfig
CHIP_BOOT_CFG:=godarm_config
REG_INFO_BIN:=reg_info_930_310_620_ddr0_slow.bin
UBOOT_IMAGE_BIN:=u-boot-hi3521_930MHz.bin
KERNEL_IMAGE_BIN:=uImage_hi3521
YAFFS2_IMAGE_BIN:=rootfs_hi3521_2k_1bit.yaffs2
SUPPORT_CHIP = "yes"
endif

ifeq ($(CHIP),hi3520a)
CHIP_KERNEL_CFG:=godcare_defconfig
CHIP_BOOT_CFG:=godcare_config
REG_INFO_BIN:=reg_info_Hi3520A_930_310_620_ddr.bin
UBOOT_IMAGE_BIN:=u-boot-hi3520a_930MHz.bin
KERNEL_IMAGE_BIN:=uImage_hi3520a
YAFFS2_IMAGE_BIN:=rootfs_hi3520a_2k_1bit.yaffs2
SUPPORT_CHIP = "yes"
endif

ifeq ($(CHIP),hi3520d)

MKBOOT = mkboot-hi3520d.sh
REG_INFO_BIN := reg_info_Hi3520D-bvt_No1_660_330_660_ddr_innerFEPHY.bin
REG_INFO_BIN1 := reg_info_Hi3515A-bvt_No1_600_300_600_ddr_innerFEPHY.bin
CHIP_BOOT_CFG:=hi3520d_config
UBOOT_IMAGE_BIN:=u-boot_hi3520d.bin

ifeq ($(OSDRV_SIZE),mini)
CHIP_KERNEL_CFG:=hi3520d_mini_defconfig
#CHIP_BOOT_CFG:=hi3520d_mini_config
#UBOOT_IMAGE_BIN:=u-boot_hi3520d_mini.bin
KERNEL_IMAGE_BIN:=uImage_hi3520d_mini
BUSYBOX_CFG:=busybox_cfg_hi3520d_nptl_mini
SQUASHFS_IMAGE_BIN_256K:=rootfs_$(CHIP)_256k.squashfs
SQUASHFS_IMAGE_BIN_64K:=rootfs_$(CHIP)_64k.squashfs
SUPPORT_CHIP = "yes"
endif #end mini

ifeq ($(OSDRV_SIZE),full)
CHIP_KERNEL_CFG:=hi3520d_full_defconfig
#CHIP_BOOT_CFG:=hi3520d_config
#UBOOT_IMAGE_BIN:=u-boot_hi3520d_full.bin
KERNEL_IMAGE_BIN:=uImage_hi3520d_full
SUPPORT_CHIP = "yes"
endif #end full

ifeq ($(OSDRV_SIZE),)
$(error You must set OSDRV_SIZE first!)
endif

endif #end hi3520d

JFFS2_IMAGE_BIN_64K:=rootfs_$(CHIP)_64k.jffs2
JFFS2_IMAGE_BIN_256K:=rootfs_$(CHIP)_256k.jffs2

ifneq ($(SUPPORT_CHIP), "yes")
$(error $(CHIP) not surpport!)
endif


##########################################################################################
#	set task
##########################################################################################
ifeq ($(OSDRV_SIZE),full)
all: prepare hiboot hikernel hirootfs_prepare hibusybox hiboardtools hipctools hirootfs_build

notools: prepare hiboot hikernel hirootfs_prepare hibusybox hirootfs_build
endif

ifeq ($(OSDRV_SIZE),mini)
all: prepare hiboot hikernel hirootfs_prepare hibusybox hiboardtools hipctools hirootfs_mini_build

notools: prepare hiboot hikernel hirootfs_prepare hibusybox hirootfs_mini_build
endif

clean: hiboot_clean hikernel_clean hibusybox_clean hiboardtools_clean hipctools_clean hirootfs_clean

distclean:clean pub_clean

##########################################################################################
#task [0]	prepare
##########################################################################################
prepare:
	mkdir $(OSDRV_DIR)/pub/$(PUB_IMAGE) -p
	mkdir $(OSDRV_DIR)/pub/bin/$(PUB_BOARD) -p
	mkdir $(OSDRV_DIR)/pub/bin/pc -p

##########################################################################################
#task [1]	build uboot
##########################################################################################
hiboot:
	@echo "---------task [1]	build boot"
	rm $(OSDRV_DIR)/uboot/u-boot-2010.06 -rf
	tar xzf $(OSDRV_DIR)/uboot/u-boot-2010.06.tgz -C $(OSDRV_DIR)/uboot
	find $(OSDRV_DIR)/uboot/u-boot-2010.06 | xargs touch
	make -C $(OSDRV_DIR)/uboot/u-boot-2010.06 ARCH=arm CROSS_COMPILE=$(OSDRV_CROSS)- $(CHIP_BOOT_CFG) 
	pushd $(OSDRV_DIR)/uboot/u-boot-2010.06;make ARCH=arm CROSS_COMPILE=$(OSDRV_CROSS)- -j 20 >/dev/null;popd
	cp $(OSDRV_DIR)/uboot/u-boot-2010.06/u-boot.bin $(OSDRV_DIR)/pub/$(PUB_IMAGE)
ifneq ($(CHIP), hi3520d)
	cp $(OSDRV_DIR)/tools/pc_tools/uboot_tools/mkboot.sh $(OSDRV_DIR)/pub/$(PUB_IMAGE)
	cp $(OSDRV_DIR)/tools/pc_tools/uboot_tools/$(REG_INFO_BIN) $(OSDRV_DIR)/pub/$(PUB_IMAGE)
	chmod 777 $(OSDRV_DIR)/pub/$(PUB_IMAGE)/mkboot.sh
	pushd $(OSDRV_DIR)/pub/$(PUB_IMAGE); ./mkboot.sh $(REG_INFO_BIN) $(UBOOT_IMAGE_BIN); popd;
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/u-boot.bin -f
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/mkboot.sh -f
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(REG_INFO_BIN) -f
else
	cp $(OSDRV_DIR)/tools/pc_tools/uboot_tools/$(MKBOOT) $(OSDRV_DIR)/pub/$(PUB_IMAGE)
	cp $(OSDRV_DIR)/tools/pc_tools/uboot_tools/$(REG_INFO_BIN) $(OSDRV_DIR)/pub/$(PUB_IMAGE)
	cp $(OSDRV_DIR)/tools/pc_tools/uboot_tools/$(REG_INFO_BIN1) $(OSDRV_DIR)/pub/$(PUB_IMAGE)
	chmod 777 $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(MKBOOT)
	pushd $(OSDRV_DIR)/pub/$(PUB_IMAGE); ./$(MKBOOT) $(REG_INFO_BIN) $(REG_INFO_BIN1) $(UBOOT_IMAGE_BIN); popd;
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/u-boot.bin -f
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(MKBOOT) -f
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(REG_INFO_BIN) -f
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(REG_INFO_BIN1) -f
endif
	
#ifeq ($(OSDRV_SIZE),mini)
#	cp $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(UBOOT_IMAGE_BIN) $(OSDRV_DIR)/uboot/u-boot-2010.06/full-boot.bin
#	cp $(OSDRV_DIR)/tools/pc_tools/uboot_tools/$(REG_INFO_BIN) $(OSDRV_DIR)/uboot/u-boot-2010.06/.reg1
#	cp $(OSDRV_DIR)/tools/pc_tools/uboot_tools/$(REG_INFO_BIN1) $(OSDRV_DIR)/uboot/u-boot-2010.06/.reg2
#	pushd $(OSDRV_DIR)/uboot/u-boot-2010.06;make ARCH=arm CROSS_COMPILE=$(OSDRV_CROSS)- mini-boot.bin >/dev/null;popd
#	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(UBOOT_IMAGE_BIN)
#	cp $(OSDRV_DIR)/uboot/u-boot-2010.06/mini-boot.bin $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(UBOOT_IMAGE_BIN)
#	rm $(OSDRV_DIR)/uboot/u-boot-2010.06/.reg*
#endif

hiboot_clean:
	rm $(OSDRV_DIR)/uboot/u-boot-2010.06 -rf

##########################################################################################
#task [2]	build kernel
##########################################################################################
hikernel:
	@echo "---------task [2] build kernel"
	rm $(OSDRV_DIR)/kernel/linux-3.0.y -rf
	tar xzf $(OSDRV_DIR)/kernel/linux-3.0.y.tgz -C $(OSDRV_DIR)/kernel/
	find $(OSDRV_DIR)/kernel/linux-3.0.y | xargs touch
	make -C $(OSDRV_DIR)/kernel/linux-3.0.y ARCH=arm CROSS_COMPILE=$(OSDRV_CROSS)- $(CHIP_KERNEL_CFG)
	pushd $(OSDRV_DIR)/kernel/linux-3.0.y;make ARCH=arm CROSS_COMPILE=$(OSDRV_CROSS)- -j 20 >/dev/null;popd
	pushd $(OSDRV_DIR)/kernel/linux-3.0.y;make ARCH=arm CROSS_COMPILE=$(OSDRV_CROSS)- uImage -j 20 >/dev/null;popd
	cp $(OSDRV_DIR)/kernel/linux-3.0.y/arch/arm/boot/uImage $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(KERNEL_IMAGE_BIN)

hikernel_clean:
	rm $(OSDRV_DIR)/kernel/linux-3.0.y -rf

##########################################################################################
#task [3]	prepare rootfs
##########################################################################################
hirootfs_prepare:
	@echo "---------task [3] prepare rootfs"
	tar xzf $(OSDRV_DIR)/rootfs_scripts/rootfs.tgz -C $(OSDRV_DIR)/pub
	mv $(OSDRV_DIR)/pub/rootfs $(OSDRV_DIR)/pub/$(PUB_ROOTFS)
	tar xzf $(OSDRV_DIR)/toolchain/$(OSDRV_CROSS)/runtime_lib/$(TOOLCHAIN_RUNTIME_LIB)/$(TOOLCHAIN_RUNTIME_LIB_C) \
		-C $(OSDRV_DIR)/pub/$(PUB_ROOTFS)
	tar xzf $(OSDRV_DIR)/toolchain/$(OSDRV_CROSS)/runtime_lib/$(TOOLCHAIN_RUNTIME_LIB)/lib.stdc++.tgz \
		-C $(OSDRV_DIR)/pub/$(PUB_ROOTFS)
ifeq ($(CHIP),hi3521)
	cp $(OSDRV_DIR)/kernel/linux-3.0.y/drivers/net/stmmac/stmmac.ko $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/hitoe
	chmod 777 $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/hitoe/*
endif
ifneq ($(CHIP),hi3521)
	rm $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/etc/init.d/S81toe
	rm $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/hitoe -rf;
endif

##########################################################################################
#task [4]	build busybox
##########################################################################################
hibusybox:
	@echo "---------task [4] build busybox"
	rm $(OSDRV_DIR)/busybox/busybox-1.16.1 -rf
	tar xzf $(OSDRV_DIR)/busybox/busybox-1.16.1.tgz -C $(OSDRV_DIR)/busybox
	find $(OSDRV_DIR)/busybox/busybox-1.16.1 | xargs touch
	cp $(OSDRV_DIR)/busybox/busybox-1.16.1/$(BUSYBOX_CFG) $(OSDRV_DIR)/busybox/busybox-1.16.1/.config
	pushd $(OSDRV_DIR)/busybox/busybox-1.16.1/;make -j 20 >/dev/null;popd
	make -C $(OSDRV_DIR)/busybox/busybox-1.16.1 install
	cp -af $(OSDRV_DIR)/busybox/busybox-1.16.1/_install/* $(OSDRV_DIR)/pub/$(PUB_ROOTFS)

hibusybox_clean:
	rm $(OSDRV_DIR)/busybox/busybox-1.16.1 -rf

##########################################################################################
#task [5]	build board tools
##########################################################################################
hiboardtools:
	@echo "---------task [5] build tools which run on board"
	make -C $(OSDRV_DIR)/tools/board_tools/e2fsprogs
	make -C $(OSDRV_DIR)/tools/board_tools/reg-tools-1.0.0
	cp -af $(OSDRV_DIR)/tools/board_tools/reg-tools-1.0.0/btools $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	cp -af $(OSDRV_DIR)/tools/board_tools/reg-tools-1.0.0/him* $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	cp -af $(OSDRV_DIR)/tools/board_tools/reg-tools-1.0.0/hiddrs $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	cp -af $(OSDRV_DIR)/tools/board_tools/reg-tools-1.0.0/hil2s $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	cp -af $(OSDRV_DIR)/tools/board_tools/reg-tools-1.0.0/hie* $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	make -C $(OSDRV_DIR)/tools/board_tools/udev-100
	cp $(OSDRV_DIR)/tools/board_tools/gdb/gdb-$(OSDRV_CROSS) $(OSDRV_DIR)/pub/bin/$(PUB_BOARD)
	#make -C $(OSDRV_DIR)/tools/board_tools/gdb
	find $(OSDRV_DIR)/tools/board_tools/mtd-utils/ -name 'configure' | xargs chmod +x
	pushd $(OSDRV_DIR)/tools/board_tools/mtd-utils;make >/dev/null;popd
	cp $(OSDRV_DIR)/tools/board_tools/mtd-utils/bin/arm/* $(OSDRV_DIR)/pub/bin/$(PUB_BOARD)
	rm $(OSDRV_DIR)/pub/bin/$(PUB_BOARD)/ubi* -rf
	cp $(OSDRV_DIR)/tools/board_tools/parted/parted_$(OSDRV_CROSS_LIB) $(OSDRV_DIR)/pub/bin/$(PUB_BOARD)
ifeq ($(CHIP),hi3520d)
	cp $(OSDRV_DIR)/tools/board_tools/hifat/$(OSDRV_CROSS_LIB)/static/himount $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/sbin
	cp $(OSDRV_DIR)/tools/board_tools/hifat/$(OSDRV_CROSS_LIB)/static/libhimount_api.a $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/lib
endif

hiboardtools_clean:
	make -C $(OSDRV_DIR)/tools/board_tools/e2fsprogs clean
	make -C $(OSDRV_DIR)/tools/board_tools/reg-tools-1.0.0 clean
	make -C $(OSDRV_DIR)/tools/board_tools/udev-100 clean
	#make -C $(OSDRV_DIR)/tools/board_tools/gdb clean
	pushd $(OSDRV_DIR)/tools/board_tools/mtd-utils;make distclean;popd

##########################################################################################
#task [6]	build pc tools
##########################################################################################
hipctools:
	@echo "---------task [6] build tools which run on pc"
	make -C $(OSDRV_DIR)/tools/pc_tools/mkyaffs2image301
	cp $(OSDRV_DIR)/tools/pc_tools/mkyaffs2image301/mkyaffs2image $(OSDRV_DIR)/pub/bin/pc
	cp $(OSDRV_DIR)/tools/pc_tools/mkfs.jffs2 $(OSDRV_DIR)/pub/bin/pc
	cp $(OSDRV_DIR)/tools/pc_tools/mkfs.cramfs $(OSDRV_DIR)/pub/bin/pc
	cp $(OSDRV_DIR)/tools/pc_tools/mkimage $(OSDRV_DIR)/pub/bin/pc
	pushd $(OSDRV_DIR)/tools/pc_tools/squashfs4.2;make;popd
	cp $(OSDRV_DIR)/tools/pc_tools/squashfs4.2/mksquashfs $(OSDRV_DIR)/pub/bin/pc
	#cp $(OSDRV_DIR)/tools/pc_tools/mkfs.xfs $(OSDRV_DIR)/pub/bin/pc

hipctools_clean:
	make -C $(OSDRV_DIR)/tools/pc_tools/mkyaffs2image301 clean
	pushd $(OSDRV_DIR)/tools/pc_tools/squashfs4.2;make distclean;popd

##########################################################################################
#task [7]	build rootfs
##########################################################################################
hirootfs_build:
	@echo "---------task [7] build rootfs"
	chmod 777 $(OSDRV_DIR)/pub/bin/$(PUB_BOARD)/*
	chmod 777 $(OSDRV_DIR)/pub/bin/pc/*
	rm $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/* -rf
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/console c 5 1
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/ttyAMA0 c 204 64
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/ttyAMA1 c 204 65
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/ttyS000 c 204 64
	pushd $(OSDRV_DIR)/pub/$(PUB_ROOTFS);ln -s sbin/init init;popd
	pushd $(OSDRV_DIR)/pub/bin/pc; ./mkfs.jffs2 -d $(OSDRV_DIR)/pub/$(PUB_ROOTFS) -l -e 0x40000 -o $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(JFFS2_IMAGE_BIN_256K);popd
	pushd $(OSDRV_DIR)/pub/bin/pc; ./mkfs.jffs2 -d $(OSDRV_DIR)/pub/$(PUB_ROOTFS) -l -e 0x10000 -o $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(JFFS2_IMAGE_BIN_64K);popd
ifneq ($(CHIP),hi3520d)
	pushd $(OSDRV_DIR)/pub/bin/pc; ./mkyaffs2image $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/ $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(YAFFS2_IMAGE_BIN) 1 1;popd
endif
	find $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/ -name '*svn' | xargs rm -rf
	pushd $(OSDRV_DIR)/pub;tar czf $(PUB_ROOTFS).tgz $(PUB_ROOTFS);rm $(PUB_ROOTFS) -rf;popd
	@echo "---------finish osdrv work"

hirootfs_mini_build:
	@echo "---------task [7] build rootfs"
	chmod 777 $(OSDRV_DIR)/pub/bin/$(PUB_BOARD)/*
	chmod 777 $(OSDRV_DIR)/pub/bin/pc/*
	rm $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/* -rf
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/console c 5 1
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/ttyAMA0 c 204 64
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/ttyAMA1 c 204 65
#	mknod $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/dev/ttyS000 c 204 64
	pushd $(OSDRV_DIR)/pub/$(PUB_ROOTFS);ln -s sbin/init init;popd
	pushd $(OSDRV_DIR)/pub;find $(PUB_ROOTFS)/ -perm +700 ! -name "*.ko" -exec $(OSDRV_CROSS)-strip {} \;;popd
	pushd $(OSDRV_DIR)/pub/bin/pc; ./mksquashfs $(OSDRV_DIR)/pub/$(PUB_ROOTFS) $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(SQUASHFS_IMAGE_BIN_256K) -b 256K -comp xz;popd
	pushd $(OSDRV_DIR)/pub/bin/pc; ./mksquashfs $(OSDRV_DIR)/pub/$(PUB_ROOTFS) $(OSDRV_DIR)/pub/$(PUB_IMAGE)/$(SQUASHFS_IMAGE_BIN_64K) -b 64K -comp xz;popd
	find $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/ -name '*svn' | xargs rm -rf
	pushd $(OSDRV_DIR)/pub;tar czf $(PUB_ROOTFS).tgz $(PUB_ROOTFS);rm $(PUB_ROOTFS) -rf;popd
	@echo "---------finish osdrv work"

hirootfs_clean:
	rm $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/ -rf

##########################################################################################
#task [8]	clean pub
##########################################################################################
pub_clean:
	rm $(OSDRV_DIR)/pub/$(PUB_IMAGE)/ -rf
	rm $(OSDRV_DIR)/pub/bin/$(PUB_BOARD)/ -rf
	rm $(OSDRV_DIR)/pub/bin/pc/ -rf
	rm $(OSDRV_DIR)/pub/$(PUB_ROOTFS).tgz -f

