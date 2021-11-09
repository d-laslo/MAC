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
    // Long pol(7, 4);
    // Long tmp(2, 12);
    // auto tt = tmp.invert_pol(pol);
    return 0;
}
