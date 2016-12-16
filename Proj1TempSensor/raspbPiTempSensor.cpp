#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
//TODO: Maybe just use this instead of iostream?
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int slaveAddr = 0xbf; 
const int HTS221whoami = 0xbc;
const char * i2cLoc = "/dev/i2c-1";

class tempInfo{
    public:
        int tempOutinC;
}; 

/*
 * Method that connects with the HTS221 and retreives the temperature based
 * information
 *
 *
 */
int initDeviceComms(){
    //Initialize file descriptor, buffer and I2C address
    int fd = 0;
    char* fileName = NULL;
    //TODO: Decide on proper buffer value based on data received from module
    char buffer[10] = {};
    int devAddr = 0xbf;
    //Open the bus to communicate with ADC device
    if(( fd = open(i2cLoc, O_RDWR)) < 0){
        std::cout << "Issue opening the device bus...\n";
        exit(1);
    }

    //Use ioctl to talk to the device
    if(ioctl(fd, I2C_SLAVE, slaveAddr) < 0) {
        std::cout << "Issue communicating with the device...\n";
        exit(1);
    }    

	std::cout << "assigning buffer to whoami" << std::endl;	
	
    buffer[0] = HTS221whoami;

	std::cout << "reading information from whoami" << std::endl;
    //Read the information from the device using i2c one byte at a time
    if(read(fd, buffer, 1) <0){
        std::cout << "Issue reading from device bus...\n";
        exit(1);
    }

    std::cout << "Returned: " << buffer[0] << std::endl;

    return buffer[0]; 
} 


/* Driver function that communicates with HTS221 and displays information 
 * received
 */
int main(int argc, char* argv[]){
	std::cout<<"In progress..."<<std::endl;

	tempInfo temperatureObj;
	temperatureObj.tempOutinC = initDeviceComms();
	std::cout<<"Current Temperature (in Celsius): "<<temperatureObj.tempOutinC<<std::endl;
	return 0;
}


//TODO: Decide how we want to present information.... 
    /*Constant while loop that continues to print out temperature (inefficient?)
     *Call device communication function when button is pressed
    */
