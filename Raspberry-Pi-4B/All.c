#include "All.h"



uint8 readSensorData(void)
{
    uint8 crc; 
    uint8 i;
  
    pinMode(pinNumber, OUTPUT); // set mode to output
    digitalWrite(pinNumber, 0); // output a high level 
    delay(25);
    digitalWrite(pinNumber, 1); // output a low level 
    pinMode(pinNumber, INPUT); // set mode to input
    pullUpDnControl(pinNumber, PUD_UP);
 
    delayMicroseconds(27);
    if (digitalRead(pinNumber) == 0) //SENSOR ANS
    {
        while (!digitalRead(pinNumber)); //wait to high
 
        for (i = 0; i < 32; i++)
        {
            while (digitalRead(pinNumber)); //data clock start
            while (!digitalRead(pinNumber)); //data start
            delayMicroseconds(HIGH_TIME);
            databuf *= 2;
            if (digitalRead(pinNumber) == 1) //1
            {
                databuf++;
            }
        }
 
        for (i = 0; i < 8; i++)
        {
            while (digitalRead(pinNumber)); //data clock start
            while (!digitalRead(pinNumber)); //data start
            delayMicroseconds(HIGH_TIME);
            crc *= 2;  
            if (digitalRead(pinNumber) == 1) //1
            {
                crc++;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

char illuminance(char *ill)
{
    
    int fd;
    char buf[3];
    char val,value;
    float flight;
    fd=open("/dev/i2c-1",O_RDWR);
    if(fd<0)
    {
        printf("打开文件错误:%s\r\n",strerror(errno)); return 0;
    }
    if(ioctl( fd,I2C_SLAVE,I2C_ADDR)<0 )
    {
        printf("ioctl 错误 : %s\r\n",strerror(errno));
        return 0;
    }
    val=0x01;
    if(write(fd,&val,1)<0)
    {
        printf("上电失败\r\n");
    }
    val=0x11;
    if(write(fd,&val,1)<0)
    {
        printf("开启高分辨率模式2\r\n");
    }
    usleep(200000);
    if(read(fd,&buf,3))
    {
        flight=(buf[0]*256+buf[1])*0.5/1.2;
        sprintf(ill,"%1.2f",flight);
        //返回湿度
        return ill;
        
    }
    else
    {
        printf("读取错误\r\n");
    }
}

char tmpAndhum(char *tmp,char *hum)
{
	 if (-1 == wiringPiSetup())
    {
        printf("Setup wiringPi failed!");
        return 0;
    }
  
    pinMode(pinNumber, OUTPUT); // set mode to output
    digitalWrite(pinNumber, 1); // output a high level 
        pinMode(pinNumber, OUTPUT); // set mode to output
        digitalWrite(pinNumber, 1); // output a high level 
        if (readSensorData())
        {
            sprintf(tmp,"%ld.%ld", (databuf >> 8) & 0xff, databuf & 0xff);
            sprintf(hum,"%ld.%ld", (databuf >> 24) & 0xff, (databuf >> 16) & 0xff); 
            return tmp, hum;
            databuf = 0;            
        }
        else
        {
            printf("Sensor dosent ans!\n");
            databuf = 0;
            return 0;
        }
}
