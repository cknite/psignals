#include <ktl/psignals/psignals.hpp>
#include <iostream>

extern "C" 
int main(int argc, char *argv[])
{
    namespace kps = ktl::psignals;
    kps::this_thread::fill_mask();

    // Spawn threads here

    kps::sigset signals{ SIGTERM, SIGINT, SIGHUP };
    const kps::signum_t signum = kps::wait(signals);
    std::cout << "Received signal: " << signum << std::endl;

    kps::this_thread::clear_mask();
    return 0;
}
