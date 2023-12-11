#include <iostream>
#include <string>
#include "Input.h"
#include "Day.h"
#include "Day22/Day22.h"

int main()
{
    Day* d = new Day22();
    int day = d->GetDay();
    std::cout << "Hello Advent of Code! Day "<<day<<"\n";

    std::string TestInput = Input::ReadTestInput(day);
    std::string Input = Input::ReadInput(day);


    std::cout << "Star1 TestOutput: " << d->Star1(TestInput, true) << "\n";
    std::cout << "Star1 Output: " << d->Star1(Input) << "\n";

    //std::cout << "Star2 TestOutput: " << d->Star2(TestInput,true) << "\n";
    //std::cout << "Star2 Output: " << d->Star2(Input) << "\n";

    system("pause");
    delete d;
}
