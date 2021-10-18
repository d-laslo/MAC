#include <iostream>
#include "headers/defines.hpp"
#include "headers/source.hpp"
#include "headers/goppa.hpp"
#include "headers/mcellice.hpp"
#include <vector>
#include <array>
#include <bitset>


using namespace std;

int main(int argc, char const *argv[])
{
    McEllice tmp(7, 19, 12);
    tmp.test();
    // Long tmp(500);
    // tmp.test();
    return 0;
}
