#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "tempInfo.h"

/* i2cCommRW()
 * Helper function to handle register R/W using read and write calls 
 *
 */
void i2cCommRW(int &fd, const int addr, char[] &wBuff, char[] &rBuff){
    wBuff[0] = addr;
    write(fd, wBuff, 1);
    if(read(fd, rBuff, 1) != 1){
        //TODO: Include more specific error message 
        std::cout << "Issue here" << std::endl;
    }   
}

/* Driver function that communicates with HTS221 and displays information 
 * received
 */
int main(int argc, char* argv[]){
	std::cout<<"In progress..."<<std::endl;

	tempInfo tempInfObj;
	tempInfObj.initDeviceComms();
    tempInfObj.calcTempVal();
	std::cout << "Current Temperature (in Celsius): " <<tempInfObj.getCurrTempVal() <<std::endl;
	return 0;
}

