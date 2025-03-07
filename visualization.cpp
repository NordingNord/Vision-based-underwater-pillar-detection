// -- Includes --
#include "visualization.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
visualization::visualization(){}


// -- Methods for visualizing text --
void visualization::visualize_matrix(Mat matrix, string matrix_name){
    try{
        // Prepare decimal limit
        cout << fixed;
        cout << setprecision(6);
        // Print title
        cout << matrix_name << endl;
        cout << "-----------------------------------------------" << endl;
        for(int row = 0; row < matrix.rows; row++){
            for(int col = 0; col < matrix.cols; col++){
                if(col == matrix.cols-1){
                    cout << setfill(' ') << setw(11) << matrix.at<double>(row,col); // Matrix is currently exprected to be CV_64FC1 -> access using  double
                }
                else{
                    cout << setfill(' ') << setw(11) << matrix.at<double>(row,col) << ", ";
                }
            }
            cout << endl;
        }
        cout << "-----------------------------------------------" << endl;
        cout << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }

}
