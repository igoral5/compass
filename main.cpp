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
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <cmath>
#include "i2c.h"
#include "compass.h"

#define NAME_FILE "/dev/i2c-1"

volatile bool work = true;

void
sig_handler(int signo)
{
	work = false;
}

int
main(int argc, char *argv[]) try 
{
	struct sigaction act;
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGINT, &act, NULL) < 0)
	{
		throw std::runtime_error("Error set signal handler");
	}
	Compass compass(NAME_FILE);
	compass.setScale(1.3);
	compass.setMeasurementMode(MEASUREMENT_CONTINUOUS);
	while (work)
	{
		usleep(1000000);
		MagnetometerRaw raw = compass.readRawAxis();
		MagnetometerScaled scaled = compass.readScaledAxis();
		// int MilliGauss_OnThe_XAxis = scaled.XAxis;
		double heading = atan2(scaled.YAxis, scaled.XAxis);
		double declinationAngle = 0.1821; // declination Angle in radians for Moscow
		heading += declinationAngle;
		if(heading < 0)
			heading += 2*M_PI;
		if(heading > 2*M_PI)
			heading -= 2*M_PI;
		double headingDegrees = heading * 180/M_PI;
		std::cout << "Raw:" << std::setw(5) << raw.XAxis << std::setw(5) << raw.YAxis << std::setw(5)
				<< raw.ZAxis << "   Scaled:" <<  std::fixed << std::setprecision(3) << std::setw(10) << scaled.XAxis
				<<  std::setw(10) << scaled.YAxis <<  std::setw(10) << scaled.ZAxis << "   Heading: "
				<< std::setprecision(6) << std::setw(10) << heading << " rad" << std::setw(12)
				<< headingDegrees << " deg" << std::endl;
	}
	return EXIT_SUCCESS;
}
catch(const std::exception& er)
{
	std::cerr << argv[0] << " : " << er.what() << std::endl;
	return EXIT_FAILURE;
}
catch(...) 
{
    std::cerr << argv[0] << " : Unknown error" << std::endl;
	return EXIT_FAILURE;
}
