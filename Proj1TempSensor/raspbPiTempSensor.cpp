#include <iostream>


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
    //Open the bus to communicate with ADC device
    //Use ioctl to talk to the device
    //Read the information from the device using i2c
    //Return temperature related information
    return 0; 
} 


int main(int argc, char* argv[]){
	std::cout<<"In progress...\n";
    //Use while loop to constantly receive the temperature and print it
    //Call initDeviceComms to get the associated temperature
	return 0;
}


//TODO: Decide how we want to present information.... 
    /*Constant while loop that continues to print out temperature (inefficient?)
     *Call device communication function when button is pressed
    */
