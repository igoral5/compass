Compass
=======

Program working with compass HMC5883L

Build on Host computer (cross compiling) :<br>
cmake -DCMAKE_TOOLCHAIN_FILE=Toolchain-RaspberryPi.cmake .<br>
make

Toolchain from  https://github.com/raspberrypi/tools.git installed into /opt

Build on Raspberry Pi:<br>
cmake .<br>
make

