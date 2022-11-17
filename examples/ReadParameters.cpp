#include "BmsI2C.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <array>
#include <ctime>
#include <unistd.h>


inline std::string timeStamp() {
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltime;
    localtime_r(&t, &ltime);
    std::stringstream temp_stream;
    temp_stream << std::put_time(&ltime, "%H:%M:%S");
    return temp_stream.str();
}


int main() {
    BmsI2C BatObj;

    while (true)
    {
        std::cout << timeStamp() << ":  "
                  << BatObj.readTemperature() << "*C, "
                  << BatObj.RelativeStateOfCharge() << "%, "
                  << BatObj.remainaningCapacity() << "ma, "
                  << BatObj.readVoaltage() << "mv, "
                  << BatObj.readCurrent()  << "ma" << std::endl;
        usleep(1E6);
    }
    return 0;
}