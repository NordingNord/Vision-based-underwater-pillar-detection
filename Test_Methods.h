#ifndef TEST_METHODS_H
#define TEST_METHODS_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include "Data_Structures.h"
#include "Obstacle_Detection.h"

// -- Class --
class test_methods
{
public:
    test_methods();

    // runs test of desired type
    void run_test(int desired_test);

    //  Test 0: Sift with settings 500,3,0.05,100,1.6,0,false
    void test_0();

private:
};

#endif // TEST_METHODS_H
