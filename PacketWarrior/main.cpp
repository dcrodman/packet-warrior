#include <iostream>
#include "PacketEngine.h"

int main(int argc, const char * argv[]) {
    PacketEngine engine;
    char *error_buf = nullptr;
    const char **devs = engine.getAvailableDevices(error_buf);
//    int size = sizeof(devs) / sizeof(char);
    for (int i = 0; i < 5; i++)
        std::cout << "Dev " << i << ": " << devs[i] << std::endl;
    engine.selectDevice(devs[0]);
    return 0;
}
