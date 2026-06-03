#include <cassert>
#include <iostream>

#include "WatchFileIO.h"

int main()
{
    std::cout << "=== FileIO Validation Test ===\n";

    // Create file handler
    WatchGA::FileIO::WatchFileIO fileIO("test.dat");

    // Verify initial state
    assert(!fileIO.isOpen());

    // Open file
    bool result = fileIO.open(true);
    assert(result);
    assert(fileIO.isOpen());

    std::cout << "[PASS] open()\n";

    // Get watch count
    unsigned int count = fileIO.getWatchCount();

    std::cout << "[PASS] getWatchCount() returned "
              << count << '\n';

    // Close file
    fileIO.close();
    assert(!fileIO.isOpen());

    std::cout << "[PASS] close()\n";

    std::cout << "=== All FileIO Tests Passed ===\n";

    return 0;
}