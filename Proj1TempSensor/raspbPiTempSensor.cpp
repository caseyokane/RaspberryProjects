#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

const int SLAVEADDR = 0x5f;
const int HTS221whoami = 0x0f;
const int T0DEGCALIBADDR = 0x32;
const int T1DEGCALIBADDR = 0x33;
const int T0T1MSB = 0x35;
const int T0OUTLSB = 0x3c;
const int T0OUTMSB = 0x3d;
const int T1OUTLSB = 0x3e;
const int T1OUTMSB = 0x3f;
//HUMD_OUT and TEMP_OUT are signed 16 bit registers 
const int HUMD_OUT = 0x28 | 0x80;
const int TEMP_OUT = 0x2a | 0x80;
const char * I2CLOC = "/dev/i2c-1";

class tempInfo{
    //TODO: Restructure so that methods are contained within class definition 
    public:
        float tempOutinC;
}; 

/* i2cCommRW()
 * Helper function to handle register R/W using read and write calls 
 *
 */
/*
void i2cCommRW(int &fd, const int addr, char[] &wBuff, char[] &rBuff){
    wBuff[0] = addr;
    write(fd, wBuff, 1);
    if(read(fd, rBuff, 1) != 1){
        //TODO: Include more specific error message 
        std::cout << "Issue here" << std::endl;
    }   
}
*/


/* initDeviceComms()
 * Method that connects with the HTS221 and retreives the temperature based
 * information
 *
 *
 */
float initDeviceComms(){

    //Initialize file descriptor, and buffers for I2C communications
    int fd = 0;
    char data[1] = {0};
    char calibRegAddr[1] = {0};

    //Open the bus to communicate with ADC device
    if(( fd = open(I2CLOC, O_RDWR)) < 0){
        std::cout << "Issue opening the device bus...\n";
        exit(1);
    }

    //Use ioctl to talk to the device
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
 
    //TODO: Start retrieveTempVal here 

    //Get calibration values from memory
    //Access location 0x32 for t0 calibration (8 bits)
    calibRegAddr[0] = T0DEGCALIBADDR;
    write(fd, calibRegAddr, 1); 
    if(read(fd, data, 1) != 1){
        std::cout << "Issue reading calibration values" << std::endl;
    }   
    //i2cCommRW(fd, T0DEGCALIBADDR, calibRegAddr, data);

    //Initializes temporary integer to store read data values
    int t0calReg = data[0]; 

    //Access location 0x33 for t1 calibration
    calibRegAddr[0] = T1DEGCALIBADDR;
    write(fd, calibRegAddr, 1);
    read(fd, data, 1);
    ////i2cCommRW(fd, T1DEGCALIBADDR, calibRegAddr, data);

    int t1calReg = data[0];

    //Retreive MSBs for t0 and t1 calibration register values
    calibRegAddr[0] = T0T1MSB;
    write(fd, calibRegAddr, 1);
    read(fd, data, 1);
    //i2cCommRW(fd, T0T1MSB, calibRegAddr, data);
    int raw = data[0];
    

    //Store MSB for each t0 and t1 by anding with 0011 and 1100
    int t0Msb = raw & 0x03;
    int t1Msb = raw & 0x0c;
    
    //Convert temperature calibration values so they contain the full 10 bits 
    t0calReg +=  t0Msb * 256;
    t1calReg +=  t1Msb * 64;

    //initialize chars to keep track of data retreived from read
    char dataRead0; char dataRead1;
    
    //Read from 0x3c and 0x3d to get the 2 bytes associated with T0_Out reg
    calibRegAddr[0] = T0OUTLSB;
    write(fd, calibRegAddr, 1);
    read(fd, data, 1);
    //i2cCommRW(fd, T0OUTLSB, calibRegAddr, data);    
    dataRead0 = data[0];

    calibRegAddr[0] = T0OUTMSB;
    write(fd, calibRegAddr, 1);
    read(fd, data, 1);
    //i2cCommRW(fd, T0OUTMSB, calibRegAddr, data);      
    dataRead1 = data[0];

    int t0outVal = (dataRead1 * 256) + dataRead0;

    //Read from 0x3e and 0x3f to get the 2 bytes associated with T1_out reg
    calibRegAddr[0] = T1OUTLSB;
    write(fd, calibRegAddr, 1);
    read(fd, data, 1);
    //i2cCommRW(fd, T1OUTLSB, calibRegAddr, data);          
    dataRead0 = data[0];

    calibRegAddr[0] = T1OUTMSB;
    write(fd, calibRegAddr, 1);
    read(fd, data, 1);
    //i2cCommRW(fd, T1OUTMSB, calibRegAddr, data);             
    dataRead1 = data[0];

    int t1outVal = (dataRead1 * 256) + dataRead0;

    //Required to retrieve the temperature in Celsius
    float tempC = 0;
    calibRegAddr[0] = HUMD_OUT;
    write(fd, calibRegAddr, 1);
    if(read(fd, data, 4) != 4){
        std::cout << "Issues getting 4 bytes " << std::endl;
        exit(1);
    }
    //i2cCommRW(fd, HUMD_OUT, calibRegAddr, data);  

    else{

        int tempVal = (data[3] * 256) + data[2];
        if(tempVal > 32767){
            tempVal -= 65536;
        }
        tempC = ((t1calReg - t0calReg)/ 8.0) * (tempVal - t0outVal) / (t1outVal - t0outVal) + (t0calReg / 8.0);  
    }

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
