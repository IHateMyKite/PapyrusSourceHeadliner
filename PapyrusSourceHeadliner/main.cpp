#include <iostream>

#include "Headliner.h"

int main(int argc, char *argv[])
{
    Headliner* loc_headliner;
    if (argc == 1) loc_headliner = new Headliner();
    else loc_headliner = new Headliner(argv[1]);
    
    return  0;
}