#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

#include <wiringPi.h>

#include "ADS1115.h"

#include "vimon.h"

using namespace std;

VImon vimon;

static string execName;
bool detectTempProblem = false;
long intervalTime = 1000000;		// in usec
#define MIN_INTERVAL_TIME 100000

void printTimeNow() {
    struct tm *time_now ;
    time_t tim ;

	tim = time (NULL);
	time_now = localtime (&tim);
	printf ("%02u:%02u:%02u", time_now->tm_hour, time_now->tm_min, time_now->tm_sec );
}

void mainLoop() {
	int16_t lastValue = 0, newValue, tolerance = 500;;
	string result;

	if(detectTempProblem)
		printf("detecting Temp Problem .....\n");

	while(1) {
		vimon.readAllChannels(result, TRUE);
		if (detectTempProblem) {
			newValue = vimon.rawValue[1];
			if ( (newValue > (lastValue+tolerance)) || (newValue < (lastValue-tolerance)) ) {
				printTimeNow();
				cout << ": " << result << endl;
			}
			lastValue = newValue;
		} else {
			printTimeNow();
			cout << ": " << result << endl;
		}
		//vimon.readRaw();
		//printf ("%5d %5d %5d %5d\n", vimon.rawValue[0], vimon.rawValue[1], vimon.rawValue[2], vimon.rawValue[3]);
		//if (vimon.rawValue[1] < 9000) printf ("!!!!! ^^^^^ !!!!!\n");

		usleep(intervalTime);		//micro seconds
	}
}

static void showUsage(void) {
    cout << "usage:" << endl;
    cout << execName <<" -d -tXXXX -h" << endl;
    cout << "d = detect temp transient" << endl;
	cout << "i = read interval [ms] (min=100)" << endl; 
    cout << "h = show help" << endl;
}

static bool parseArguments (int argc, char *argv[])
{
    char buffer[64];
    int i, buflen;
	long lValue;
    int retval = true;
	string str;
	execName = std::string(basename(argv[0]));

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
                        detectTempProblem = true;
                        break;
					case 'i':
						str = std::string(&buffer[2]);
						lValue = std::stoi(str,NULL);
						intervalTime = lValue * 1000;
						if (intervalTime < MIN_INTERVAL_TIME) intervalTime = MIN_INTERVAL_TIME;
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
