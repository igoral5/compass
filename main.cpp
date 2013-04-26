/*
 * main.cpp
 *
 *  Created on: 25.04.2013
 *      Author: igor
 */

#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <sstream>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <cmath>
#include "compass.h"

#define NAME_FILE "/dev/i2c-"

volatile bool work = true;

void
sig_handler(int signo)
{
	work = false;
}

int
main(int argc, char *argv[]) try 
{
	if (argc != 2)
	{
	    std::cerr << argv[0] << ": Need argument - the number of the channel" << std::endl;
	    std::cerr << "\tUsage: " << argv[0] << " n " << std::endl;
	    std::cerr << "\tWhere n = 0,1,2..." << std::endl;
	    return EXIT_FAILURE;
	}
    struct sigaction act;
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGINT, &act, NULL) < 0)
	{
		throw std::runtime_error("Error set signal handler");
	}
	std::ostringstream oss;
	oss << NAME_FILE << argv[1];
	Compass compass(oss.str());
	compass.setScale(1.3);
	compass.setMeasurementMode(MEASUREMENT_CONTINUOUS);
	while (work)
	{
		usleep(1000000);
		MagnetometerRaw raw = compass.readRawAxis();
		MagnetometerScaled scaled = compass.readScaledAxis();
		double heading = atan2(scaled.YAxis, scaled.XAxis);
		double declinationAngle = 0.1821; // magnetic declination in radians for Moscow
		heading += declinationAngle;
		if(heading < 0)
			heading += 2 * M_PI;
		if(heading > 2 * M_PI)
			heading -= 2 * M_PI;
		double headingDegrees = heading * 180 / M_PI;
		std::cout << "Raw:" << std::setw(5) << raw.XAxis << std::setw(5) << raw.YAxis << std::setw(5)
				<< raw.ZAxis << "   Scaled:" <<  std::fixed << std::setprecision(3) << std::setw(10) << scaled.XAxis
				<<  std::setw(10) << scaled.YAxis <<  std::setw(10) << scaled.ZAxis << "   Heading:"
				<< std::setprecision(6) << std::setw(10) << heading << " rad" << std::setw(12)
				<< headingDegrees << " deg" << std::endl;
	}
	return EXIT_SUCCESS;
}
catch(const std::exception& er)
{
	std::cerr << argv[0] << ": " << er.what() << std::endl;
	return EXIT_FAILURE;
}
catch(...) 
{
    std::cerr << argv[0] << ": Unknown error" << std::endl;
	return EXIT_FAILURE;
}
