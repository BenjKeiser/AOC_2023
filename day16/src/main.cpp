#include "elf.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    Elves elves(argv[1]);
    std::cout << elves.get_energized({0,0,RIGHT}) << std::endl;
    std::cout << elves.get_max_energized() << std::endl;
    return 0;
}