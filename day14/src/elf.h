#ifndef ELF_H
#define ELF_H

#include <vector>
#include <string>
#include <utility>


class Elves {
    private:
        std::vector<std::string> dish;
    public:
        Elves(char * file_name);
        uint64_t get_load();
        uint64_t get_load_cycle();
};

#endif /* ELF_H */


