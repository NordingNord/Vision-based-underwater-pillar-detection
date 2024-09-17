// -- Includes --
#include "Test_Methods.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
test_methods::test_methods(){
}

// runs test of desired type
void test_methods::run_test(int desired_test){
    if(desired_test == 0){
        test_0;
    }
    else{
        cout << "Specified test does not exist." << endl;
    }
}

//  Test 0: Sift with settings 500,3,0.05,100,1.6,0,false
void test_methods::test_0(){
    sift_settings desired_settings = {500,3,0.05,100,1.6,0,false};

}
