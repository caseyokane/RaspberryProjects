# Raspberry Pi Temperature sensor 

For this project I used the Rasbperry Pi 2 and Sense HAT add-on board to disply current temperature (in Fahrenheit) using the I2C serial interface. 

### Current Progress
* Determined that the [HTS221 Humidity Sensor](http://www.st.com/content/ccc/resource/technical/document/datasheet/4d/9a/9c/ad/25/07/42/34/DM00116291.pdf/files/DM00116291.pdf/jcr:content/translations/en.DM00116291.pdf) would be utilized to find tempertaure values.
* Determined that communication using I2C can be accomplished using 'ioctl' system calls.
* Register mappings for HTS221 were found as TEMP_OUT_L (0x28) and TEMP_OUT_H (0x29)
* CURRENTLY, working on ensuring that a connection to the HTS221 is made and that temperature/humidity data can be retrieved.

### Structure

initDeviceComms() - Helper functon used to establish a connection with the HTS221.


### Compilation/Execution
TODO: Include compilation instructions when proper executable is being generated

### References
TODO: Add resources as they become apparent 
