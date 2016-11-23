#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kcftracker.hpp"

#include <dirent.h>

using namespace std;
using namespace cv;

float xMin = 0;
float yMin = 0;
float height = 0;
float width = 0;
bool flag_first_bottom = true;
bool flag_bottom_finish = false;

Mat image_roi;


//set tracking roi
void onMouse(int event, int x, int y, int, void*);

int main(){

    //if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;

//	for(int i = 0; i < argc; i++){
//		if ( strcmp (argv[i], "hog") == 0 )
//			HOG = true;
//		if ( strcmp (argv[i], "fixed_window") == 0 )
//			FIXEDWINDOW = true;
//		if ( strcmp (argv[i], "singlescale") == 0 )
//			MULTISCALE = false;
//		if ( strcmp (argv[i], "show") == 0 )
//			SILENT = false;
//		if ( strcmp (argv[i], "lab") == 0 ){
//			LAB = true;
//			HOG = true;
//		}
//		if ( strcmp (argv[i], "gray") == 0 )
//			HOG = false;
//	}
	
	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Frame readed
    //Mat frame;

	// Tracker results
	Rect result;

    //
    Mat frame;

//	// Path to list.txt
//	ifstream listFile;
//	string fileName = "images.txt";
//  	listFile.open(fileName);

//  	// Read groundtruth for the 1st frame
//  	ifstream groundtruthFile;
//	string groundtruth = "region.txt";
//  	groundtruthFile.open(groundtruth);
//  	string firstLine;
//  	getline(groundtruthFile, firstLine);
//	groundtruthFile.close();
  	
//  	istringstream ss(firstLine);

//  	// Read groundtruth like a dumb
//  	float x1, y1, x2, y2, x3, y3, x4, y4;
//  	char ch;
//	ss >> x1;
//	ss >> ch;
//	ss >> y1;
//	ss >> ch;
//	ss >> x2;
//	ss >> ch;
//	ss >> y2;
//	ss >> ch;
//	ss >> x3;
//	ss >> ch;
//	ss >> y3;
//	ss >> ch;
//	ss >> x4;
//	ss >> ch;
//	ss >> y4;

	// Using min and max of X and Y for groundtruth rectangle
//	float xMin =  min(x1, min(x2, min(x3, x4)));
//	float yMin =  min(y1, min(y2, min(y3, y4)));
//	float width = max(x1, max(x2, max(x3, x4))) - xMin;
//	float height = max(y1, max(y2, max(y3, y4))) - yMin;

	
//	// Read Images
//	ifstream listFramesFile;
//	string listFrames = "images.txt";
//	listFramesFile.open(listFrames);
//	string frameName;


//	// Write Results
//	ofstream resultsFile;
//	string resultsPath = "output.txt";
//	resultsFile.open(resultsPath);

	// Frame counter
	int nFrames = 0;

    // open camera
    VideoCapture capture;
    capture.open(0);
    if(!capture.isOpened()){
        printf("can not open camera");
        while(1);
    }

    //set roi xMin, yMin, width, height
    //namedWindow("image_roi", 0);
    capture>>image_roi;
    imshow("image_roi", image_roi);
    setMouseCallback("image_roi", onMouse, 0);
    while(1){
        capture>>image_roi;
        imshow("image_roi", image_roi);
        char key = waitKey(1);
        if(key == 'p'){
            //destroyAllWindows();
            break;
        }
    }

    printf("finished set roi\n");


    while (1){
        //frameName = frameName;
        capture >> frame;

		// Read each frame from the list
        //frame = imread(frameName, CV_LOAD_IMAGE_COLOR);

		// First frame, give the groundtruth to the tracker
		if (nFrames == 0) {
			tracker.init( Rect(xMin, yMin, width, height), frame );
			rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );
            //resultsFile << xMin << "," << yMin << "," << width << "," << height << endl;
		}
		// Update
		else{
			result = tracker.update(frame);
			rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 1, 8 );
            //resultsFile << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
		}

		nFrames++;

			imshow("Image", frame);
            char key = waitKey(1);
            if(key == 'b'){
                break;
            }
	}
    //resultsFile.close();

    //listFile.close();

}

void onMouse(int event, int x, int y, int, void *)
{
    //std::cout<<"x:"<<x<<"  "<<"y:"<<y<<endl;
    switch(event){
    case EVENT_LBUTTONDOWN:
        //set roi x,y
        if(flag_first_bottom){
            xMin = x;
            yMin = y;
            flag_first_bottom = false;
            std::cout<<"x:"<<xMin<<"  "<<"y:"<<yMin<<std::endl;
            return;
        }
    case EVENT_LBUTTONUP:
        //set width, height
        //and destroy windows
        width = x - xMin;
        height = y - yMin;
        flag_bottom_finish = false;
        std::cout<<"width:"<<width<<"  "<<"height:"<<height<<std::endl;
        return ;
    }
    //imshow("image_roi", image_roi);
    //waitKey(1);
}
