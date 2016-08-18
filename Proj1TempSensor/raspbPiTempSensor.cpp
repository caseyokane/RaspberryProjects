#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
//TODO: Maybe just use this instead of iostream?
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    char buffer[42] = {};
    int devAddr = 0xbf;
    
    //Open the bus to communicate with ADC device
    sprintf(fileName, "/dev/i2c-2");
    if(( fd = open(fileName, O_RDWR)) < 0){
        std::cout << "Issue opening the device bus...\n";
        exit(1);
    }
    
    //Use ioctl to talk to the device
    if(ioctl(fd, I2C_SLAVE, devAddr) < 0) {
        std::cout << "Issue communicating with the device...\n";
        exit(1);
    }    

    //Read the information from the device using i2c one byte at a time
    if(read(fd, buffer, 1) <0){
        std::cout << "Issue reading from device bus...\n";
        exit(1);
    }

    //Return temperature related information
    return buffer[0]; 
} 


/* Driver function that communicates with HTS221 and displays information 
 * received
 */
int main(int argc, char* argv[]){
	std::cout<<"In progress...\n";

	tempInfo temperatureObj;
    	//Use while loop to constantly receive the temperature and print it
    	//Call initDeviceComms to get the associated temperature
	return 0;
}


//TODO: Decide how we want to present information.... 
    /*Constant while loop that continues to print out temperature (inefficient?)
     *Call device communication function when button is pressed
    */
