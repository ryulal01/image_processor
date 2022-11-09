#include "Console.h"


int main(int argc, char** argv)
{
    // Assembly to run from the console


    if (argc == 1) {
        Console console(true);
    } else
    {
        Console console(false);

        std::string input;

        for (int k = 1; k < argc; k++) {
            input += std::string(argv[k]) + ' ';
        }

        console.EnterCommand(input);
    }



    // Build to run by hand

    //Call console with help
    /*
    console Console(true);

    while (true)
    {
        std::string Input;
        std::getline(std::cin, Input);
        Console.enterCommand(Input);
    }*/



    return 0;

}
