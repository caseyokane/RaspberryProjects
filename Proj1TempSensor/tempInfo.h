#ifndef _tempInfo_included_
#define _tempInfo_included_

class tempInfo{
    
    private:

        float tempOutinC;
        int tempInfoFD;


    public:

        //TODO: Create constructor that calls initDeviceComms()

        void initDeviceComms();
        void calcTempVal();

        float getCurrTempVal(){
            return this.tempOutinC;
        }
}; 

#endif