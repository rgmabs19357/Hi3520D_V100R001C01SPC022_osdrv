
all:
	tar xzf udev-100.tgz
	make -C udev-100 CROSS_COMPILE=$(OSDRV_CROSS)- 
	mkdir $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin -p
	cp udev-100/udevd $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	cp udev-100/udevstart $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	cp udev-100/udevinfo $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/bin
	cp -af rootfs/. $(OSDRV_DIR)/pub/$(PUB_ROOTFS)/
clean:
	rm udev-100 -rf
 
