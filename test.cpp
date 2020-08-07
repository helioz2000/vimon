#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

#include <wiringPi.h>

#include "ADS1115.h"

#include "vimon.h"

using namespace std;

VImon vimon;

void mainLoop() {
	while(1) {
		vimon.readAllChannels(TRUE);
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

	if (!vimon.initialize( ADS1115_ADDRESS_ADDR_SDA )) {
		goto exit_fail;
	}

	mainLoop();

	exit(EXIT_SUCCESS);

exit_fail:
	exit(EXIT_FAILURE);
	return 0;
}
