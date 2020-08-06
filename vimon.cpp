/*
 VI monitoring board ADC configuration
 Uses precision voltage source for 2.048V on the OpAmp

 Ch0: V1 10.0V to 16.0V = 0 to 2.048V
 Ch1: V2 or PT100 0 to 2.03V = 90 to 134 Ohm = -25 to +88 DegC
 Ch2: I1
 Ch3: I2

 Current sensor INA180A1 gain is 20
 Example: 50mV/50A on Shunt = 1V on Analog Input for 50A
		  50mA per mV

 */


#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

#include <wiringPi.h>

#include "ADS1115.h"

#include "vimon_cal.h"

using namespace std;

ADS1115 vimon_adc(BOARD_ADDRESS);

bool hardwareSetup () {

	//wiringPiSetup();

	//I2Cdev i2c;		// Needed to open I2C bus

    vimon_adc.initialize();

    if (!vimon_adc.testConnection()) {
		printf("ADS1115 not found \n");
		return false;
	}

    // set gain
    vimon_adc.setGain(ADS1115_PGA_2P048);
	vimon_adc.showConfigRegister();

	return true;

}

void readAllChannels() {

	int rawAnalog[4];
	int mVunscaled[4];
	float voltage_mv;
	float current1_ma, current2_ma;
	float pt100ohm, pt100temp;
	int i;

	rawAnalog[0] = vimon_adc.getConversionP0GND();
	rawAnalog[1] = vimon_adc.getConversionP1GND();
	rawAnalog[2] = vimon_adc.getConversionP2GND();
	rawAnalog[3] = vimon_adc.getConversionP3GND();



	for (i=0; i<4; i++) {
		printf("%5d ",rawAnalog[i]);
	}

	printf(" : ");

	for (i=0; i<4; i++) {
		mVunscaled[i] = (float)rawAnalog[i] * ADS1115_MV_2P048;
		printf("%5d ", mVunscaled[i]);
	}

	voltage_mv = (mVunscaled[0] * V1_MV_PER_MV) + V1_OFFSET;

	pt100ohm = (mVunscaled[1] * PT_OHM_PER_MV) + PT_OFFSET_OHM;
	pt100temp = (pt100ohm/PT_REFERENCE_OHM-1.0)/PT_SLOPE;

	current1_ma = mVunscaled[2] * I1_MA_PER_MV;
	current2_ma = mVunscaled[3] * I2_MA_PER_MV;

	printf(" : %5.0f mV", voltage_mv);
	printf(" : %6.2f Ohm : %5.2f DegC", pt100ohm, pt100temp);
	printf(" : %6.1f mA : %6.1f mA", current1_ma, current2_ma);

	printf("\n");

}

void mainLoop() {
	while(1) {
		readAllChannels();
		usleep(1000000);		//micro seconds
	}
}

static void showUsage(void) {
    cout << "usage:" << endl;
    cout << "-cCfgFileName -d -h" << endl;
    cout << "c = name of config file" << endl;
    cout << "d = deamon mode" << endl;
    cout << "h = show help" << endl;
}

static bool parseArguments (int argc, char *argv[])
{
    char buffer[64];
    int i, buflen;
    int retval = true;

    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            strcpy(buffer, argv[i]);
            buflen = strlen(buffer);
            if ((buffer[0] == '-') && (buflen >=2)) {
                switch (buffer[1]) {
                    case 'c':
                        //cfgFileName = std::string(&buffer[2]);
                        break;
                    case 'd':
                        //runAsDaemon = true;
                        break;
                    case 'h':
                        showUsage();
                        retval = false;
                        break;
                    default:
                        std::cerr << "uknown parameter <" << &buffer[1] << ">" << endl;
                        showUsage();
                        retval = false;
                        break;
                }
                ;
            }
        }
    }
    return retval;
}

int main (int argc, char *argv[])
{
    if (! parseArguments(argc, argv) ){
		goto exit_fail;
	}

	if (!hardwareSetup()) {
		goto exit_fail;
	}

	mainLoop();

	exit(EXIT_SUCCESS);

exit_fail:
	exit(EXIT_FAILURE);
	return 0;
}
