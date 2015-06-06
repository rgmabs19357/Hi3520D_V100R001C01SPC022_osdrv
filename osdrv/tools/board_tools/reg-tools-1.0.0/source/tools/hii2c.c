#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "memmap.h"
#include "hi.h"
#include "strfunc.h"


#ifdef BVT_I2C 

#ifdef HI_GPIO_I2C
#include "gpio_i2c.h"
#else
#include "hi_i2c.h"
#endif

#else

//#include "hi_unf_ecs.h"

#endif
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifndef BVT_I2C 
extern int HI_UNF_I2C_Open (void);
extern int HI_UNF_I2C_Close(void);
extern int HI_UNF_I2C_Read(U32 u32I2cNum, U8 u8DevAddress, U32 u32RegAddr,
                       U32 u32RegAddrCount, U8 *pu8Buf, U32 u32Length);
extern int HI_UNF_I2C_Write(U32 u32I2cNum, U8 u8DevAddress, U32 u32RegAddr,
                       U32  u32RegAddrCount, U8 * pu8Buf, U32 u32Length);
#endif
                  
HI_RET hier(int argc , char* argv[])
{
    U32 I2C_port;
    U32 device_addr;
    U32 reg_addr;
    U32 regaddr_count=1;
    U32 regbytes=1;
    U8* data;
    U32 len = 0x40;
    int result;

    if (argc < 4)
    {
        printf("usage: %s <i2c port> <device address> <register address> [byte numbers] [regaddr count] [reg bytes]. \n", argv[0]);
        return -1;
    }

    if(StrToNumber(argv[1], &I2C_port) != HI_SUCCESS ) {
	printf("Please input i2c port like 0x100 or 256.\r\n");
  return -1;
    }

    if(StrToNumber(argv[2], &device_addr) != HI_SUCCESS ) {
	printf("Please input dev addr like 0x100 or 256.\r\n");
	return -1;
    }

    if(StrToNumber(argv[3], &reg_addr) != HI_SUCCESS ) {
	 printf("Please input reg addr like 0x100 0r 256.\r\n");
	 return -1;
    }


    if (argc >= 5) {
         if(StrToNumber(argv[4], &len) != HI_SUCCESS ) {
        	printf("Please input len like 0x100\n");
	 return -1;
         }
    }
    if (argc >= 6) {    
        if(StrToNumber(argv[5], &regaddr_count) != HI_SUCCESS ) {
            printf("Please input reg addr count like 0x1 0r 1 from 1 to 4\r\n");
            return -1;
        }
    }
    if (argc >= 7)
        if(StrToNumber(argv[6], &regbytes) != HI_SUCCESS ) {
        printf("Please input reg addr count like 0x1 0r 1 from 1 to 4\r\n");
        return -1;
    }
    
    data = (U8*)malloc(len);
    if (data == NULL)
    {
        /*EXIT("I2C open error.", result);*/
    	printf("[error]not enough memory.\r\n");
    	return -1;
    }
#ifndef BVT_I2C 
    result = HI_UNF_I2C_Open();
    if (result != HI_SUCCESS)
    {
        EXIT("I2C open error.", result);
    } 
#else
    int fd;
#ifdef HI_GPIO_I2C
    fd = open("/dev/gpioi2c", 0);  
#else
    fd = open("/dev/hi_i2c", 0);  
#endif
#endif


    printf("====I2C read:<%#x> <%#x> <%#x>====\r\n", (unsigned int)device_addr, (unsigned int)reg_addr, (unsigned int)len);

#ifndef BVT_I2C
    result = HI_UNF_I2C_Read(I2C_port,device_addr, reg_addr, regaddr_count, data, len);
        if (result != HI_SUCCESS)
        {
            EXIT("I2C read error.", result);

        }
        
#else
    U32 count=0;
    while (count < len)
    {
#ifdef HI_GPIO_I2C
        int value;
	value = ((device_addr&0xff)<<24) | (((reg_addr+count)&0xff)<<16);        
        ioctl(fd, GPIO_I2C_READ, &value);        
        data[count] = value&0xff;
        count++;
#else
        U32  tmp;
        I2C_DATA_S  i2c_data ;
	i2c_data.dev_addr = device_addr ;
        i2c_data.reg_addr = reg_addr+count    ;
        i2c_data.addr_byte_num   = regaddr_count  ;
        i2c_data.data_byte_num   = regbytes ;
        ioctl(fd, CMD_I2C_READ, &i2c_data);
        tmp = i2c_data.data;
        int i;
        for (i=0;i<regbytes;i++) {
        	data[count] = (char)tmp &&0xff ;
        	tmp =tmp >> 8;
            count++;
        }
#endif        
        
    }
#endif     
    if (len >0)
    	hi_hexdump(STDOUT, (void*)data, len, 16);

#ifndef BVT_I2C
    result = HI_UNF_I2C_Close();
    if (result != HI_SUCCESS)
    {
        EXIT("I2C close error.", -1);
    }
#else
    close(fd);
#endif
    return 0;
}

HI_RET hiew(int argc , char* argv[])
{
    U32 I2C_port;
    U32 device_addr;
    U32 reg_addr;
    U32 regaddr_count=1;
    U32 regbytes=1;
    U32 new_data;
    U32 value;
    int result;


    if (argc < 5)
    {
        printf("usage: %s <i2c_port> <device address> <register address> <value> [regaddr count] [reg bytes].\n", argv[0]);
        return -1;

    }

    if(StrToNumber(argv[1], &I2C_port) != HI_SUCCESS ) {
	printf("Please input i2c port like 0x100 or 256.\r\n");
        return -1;
    }

    if(StrToNumber(argv[2], &device_addr) != HI_SUCCESS ) {
	printf("Please input dev addr like 0x100 or 256.\r\n");
	return -1;
    }

    if(StrToNumber(argv[3], &reg_addr) != HI_SUCCESS ) {
	printf("Please input reg addr like 0x100 0r 256.\r\n");
	return -1;
    }

    if(StrToNumber(argv[4], &new_data) != HI_SUCCESS ) {
	printf("Please input len like 0x100\n");
        return -1;
    }
    if(argc >=6) {
        if(StrToNumber(argv[5], &regaddr_count) != HI_SUCCESS ) {
            printf("Please input reg addr count like 0x1 0r 1 from 1 to 4\r\n");
            return -1;
	}
    }
    if(argc >=7) {
        if(StrToNumber(argv[6], &regbytes) != HI_SUCCESS ) {
            printf("Please input reg bytes like 0x1 0r 1 from 1 to 4\r\n");
            return -1;
        }
    }
    value = new_data;

    printf("====I2C write:<%#x> <%#x> <%#x>====\n", (unsigned int)device_addr, (unsigned int)reg_addr, (unsigned int)new_data);
    
#ifndef BVT_I2C    
    result = HI_UNF_I2C_Open();
    if (result != HI_SUCCESS)
    {
        EXIT("I2C open error.", -1);

    }
    result = HI_UNF_I2C_Write(I2C_port, device_addr, reg_addr, regaddr_count, (U8 *)&value, regbytes);
    if (result != HI_SUCCESS)
    {
	EXIT("I2C write error.", result);
    }
    
    result = HI_UNF_I2C_Close();
    if (result != HI_SUCCESS)
    {
        EXIT("I2C close error.", -1);
    }
#else

    int fd;

#ifdef HI_GPIO_I2C
    int tmp;
    fd = open("/dev/gpioi2c", 0);  
    tmp = ((device_addr&0xff)<<24) | ((reg_addr&0xff)<<16) | (value&0xffff);
    ioctl(fd, GPIO_I2C_WRITE, &tmp);
#else
    I2C_DATA_S  i2c_data ;
    fd = open("/dev/hi_i2c", 0);  
    i2c_data.dev_addr = device_addr ; 
    i2c_data.reg_addr = reg_addr    ; 
    i2c_data.addr_byte_num = regaddr_count  ; 
    i2c_data.data     = value ; 
    i2c_data.data_byte_num = regbytes ;
    ioctl(fd, CMD_I2C_WRITE, &i2c_data);
#endif

    close(fd);
#endif

    return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
