/*
 * main.cpp
 *
 *  Created on: 25.04.2013
 *      Author: igor
 */

#include <iostream>
#include <cstdlib>
#include <locale>
#include <exception>
#include "i2c.h"
#include "compass.h"

int
main(int argc, char *argv[]) try 
{
	std::locale::global(std::locale(""));

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
