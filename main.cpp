int main() {
#include "Console.h"


    int main(int argc, char** argv)
    {
        // Сборка для запуска из консоли


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



        //Сборка для запуска руками

        //Вызов консоли со справкой
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