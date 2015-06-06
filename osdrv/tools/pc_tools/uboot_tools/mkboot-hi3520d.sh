#! /bin/sh

[ -n "$1" ] || { 
	echo ""; 
	echo "usage:"; 
	echo "     mkboot.sh <config file1> <config file2> <output file>"; 
	echo "     e.g." 
		echo "     mkboot.sh reg_info1.bin reg_info2.bin u-boot-ok.bin"; 
	echo ""; 
	exit 1; 
}

[ -n "$2" ] || {
	echo ""; 
	echo "usage:"; 
	echo "     mkboot.sh <config file1> <config file2> <output file>"; 
	echo "     e.g." 
		echo "     mkboot.sh reg_info1.bin reg_info2.bin u-boot-ok.bin"; 
	echo ""; 
	exit 1; 
}

[ -n "$3" ] || {
	echo ""; 
	echo "usage:"; 
	echo "     mkboot.sh <config file1> <config file2> <output file>"; 
	echo "     e.g." 
		echo "     mkboot.sh reg_info1.bin reg_info2.bin u-boot-ok.bin"; 
	echo ""; 
	exit 1; 
}

dd if=./u-boot.bin of=./fb1 bs=1 count=64
dd if=$1 of=./fb2 bs=2400 conv=sync
dd if=$2 of=./fb3 bs=2400 conv=sync
dd if=./u-boot.bin of=./fb4 bs=1 skip=4864
cat fb1 fb2 fb3 fb4 > $3
rm -f fb1 fb2 fb3 fb4

