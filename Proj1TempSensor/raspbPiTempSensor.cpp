#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
//TODO: Maybe just use this instead of iostream?
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Discounting R/W bit for HTS221  

const int SLAVEADDR = 0x5f;
const int HTS221whoami = 0x0f;
const int T0DEGCALIBADDR = 0x32;
const int T1DEGCALIBADDR = 0x33;
const int T0T1MSB = 0x35;
const int T0OUTLSB = 0x3c;
const int T0OUTMSB = 0x3d;
const int T1OUTLSB = 0x3e;
const int T1OUTMSB = 0x3f;
const char * I2CLOC = "/dev/i2c-1";

class tempInfo{
    public:
        float tempOutinC;
}; 

/* Helper function to handle register R/W
 *
 *  Would essentially just use code that is repeated throughout initDeviceComms
 *
 */
 /*
void i2cCommRW(const int addr, char[] &buff){
    buff[0] = addr;
    write(fd, regWrite, 1);
    //TODO: Handle error cases here 
    read(fd, regRead, 1);
    return;
}
*/

/*
 * Method that connects with the HTS221 and retreives the temperature based
 * information
 *
 *
 */
float initDeviceComms(){

    //Initialize file descriptor, buffer and I2C address
    int fd = 0;
    //TODO: Decide on proper buffer value based on data received from module
    char buffer[2] = {};

    //Open the bus to communicate with ADC device
    if(( fd = open(I2CLOC, O_RDWR)) < 0){
        std::cout << "Issue opening the device bus...\n";
        exit(1);
    }

    //Use ioctl to talk to the device
    //TODO: Issue happening here, maybe SLAVEADDR or fd from opening file
    if(ioctl(fd, I2C_SLAVE, SLAVEADDR) < 0) {
        std::cout << "Issue communicating with the device...\n";
        exit(1);
    }    

    //Select average config reg so average samples is set to 16
    char configBuff[2] = {0};
    configBuff[0] = 0x10;
    write(fd, configBuff, 1);
    //Initiate delay for change to take effect
    sleep(1);

    //Sect control reg and select a data rate of 1 HZ
    configBuff[0] = 0x20;
    configBuff[1] = 0x85;
    write(fd, configBuff, 2);
    sleep(1);
 
    //Get calibration values from memory
    char data[1] = {0};
    char calibReg[1] = {0};


    calibReg[0] = T0DEGCALIBADDR;
    write(fd, calibReg, 1); 
    if(read(fd, data, 1) != 1){
        std::cout << "Issue reading calibration values" << std::endl;
    }   

    //Initializes temporary integer to store data values
    int t0calReg = data[0]; 

    calibReg[0] = T1DEGCALIBADDR;
    write(fd, calibReg, 1);
    read(fd, data, 1);
    int t1calReg = data[0];


    calibReg[0] = T0T1MSB;
    write(fd, calibReg, 1);
    read(fd, data, 1);
    int raw = data[0];
    

    //Store MSB for each t0 and t1 by anding with 0011 and 1100
    int t0Msb = raw & 0x03;
    int t1Msb = raw & 0x0c;
    
    //Convert temperature calibration values so they contain the full 10 bits 
    t0calReg +=  t0Msb * 256;
    t1calReg += t1Msb * 64;

    //TODO: Eliminate if unnecessary

    //Convert temperature calibration values 
    //t0calReg = ( (raw & 0x03) * 256) + t0calReg;
    //t1calReg = ( (raw & 0x0c) * 64) + t1calReg;


    //initialize chars to keep track of data retreived from read
    char dataRead0; char dataRead1;
    
    //Read from 0x3c and 0x3d
    calibReg[0] = T0OUTLSB;
    write(fd, calibReg, 1);
    //TODO: include output check here
    read(fd, data, 1);
    dataRead0 = data[0];

    calibReg[0] = T0OUTMSB;
    write(fd, calibReg, 1);
    read(fd, data, 1);
    dataRead1 = data[0];

    int t0outVal = (dataRead1 * 256) + dataRead0;

    //Read from 0x3e and 0x3f
    calibReg[0] = T1OUTLSB;
    write(fd, calibReg, 1);
    read(fd, data, 1);
    dataRead0 = data[0];

    calibReg[0] = T1OUTMSB;
    write(fd, calibReg, 1);
    read(fd, data, 1);
    dataRead1 = data[0];

    int t1outVal = (dataRead1 * 256) + dataRead0;


    calibReg[0] = 0x28 | 0x80;
    write(fd, calibReg, 1);
    float tempC = 0;
    if(read(fd, data, 4) != 4){
        std::cout << "Issues getting 4 bytes " << std::endl;
        exit(1);
    }

    else{

        int tempVal = (data[3] * 256) + data[2];
        if(tempVal > 32767){
            tempVal -= 65536;
        }
        //TODO: Clean up this calculation
        tempC = ((t1calReg - t0calReg)/ 8.0)* (tempVal - t0outVal) / (t1outVal - t0outVal) + (t0calReg / 8.0);  
    }

    
    /*
    //Read the information from the device using i2c one byte at a time
    if(read(fd, buffer, 1) <= 0){
        //No bytes were read if entering this condition 
        std::cout << "Issue reading from device bus...\n";
        exit(1);
    }
    */


    return tempC; 
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
