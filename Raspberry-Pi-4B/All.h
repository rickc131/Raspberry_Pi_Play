#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
# include <sys/ioctl.h>
#include <linux/i2c-dev.h>


#define HIGH_TIME 32
#define I2C_ADDR 0x23

typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;


uint8 readSensorData(void);
//获取光照
char illuminance(char *ill);
//获取温湿度
char tmpAndhum(char *tmp,char *hum);
// gcc 1.c -o 1 -lwiringPi



uint32 databuf;
int pinNumber = 1;
