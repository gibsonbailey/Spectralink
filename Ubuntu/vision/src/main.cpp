///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


/*************************************************************************
 ** This sample demonstrates how to capture images and 3D point cloud   **
 ** with the ZED SDK and display the result in an OpenGL window. 		    **
 *************************************************************************/

 // Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ZED includes
#include <sl/Camera.hpp>

// Sample includes
#include "GLViewer.hpp"

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Using std and sl namespaces
//using namespace std;
using namespace sl;

// Create ZED objects (camera, callback, images)
sl::Camera zed;
std::thread zed_callback;
int width, height;
bool quit;

// Point cloud viewer
//GLViewer viewer;

// Sample functions
void startZED();
void run();
void close();
cv::Mat slMat2cvMat(Mat& input);



int edgeThresh = 1;
int lowThreshold = 50;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
cv::Mat src_gray;


void CannyThreshold(cv::Mat src, cv::Mat& dst)
{
    cvtColor(src, src_gray, CV_BGR2GRAY );
    cv::Mat detected_edges;
    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, cv::Size(3,3) );

    /// Canny detector
    Canny( detected_edges, detected_edges, lowThreshold,
    lowThreshold*ratio, kernel_size );

    /// Using Canny's output as a mask, we display our
    //result
    dst = cv::Scalar::all(0);

    src.copyTo( dst, detected_edges);
}






int main(int argc, char **argv) {

    glutInit( &argc, argv);

    // Set configuration parameters for the ZED
    InitParameters initParameters;
    if (argc == 2) initParameters.svo_input_filename = argv[1];
    initParameters.camera_resolution = RESOLUTION_HD720;
    initParameters.depth_mode = DEPTH_MODE_PERFORMANCE;
    initParameters.coordinate_system = COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP; // OpenGL's coordinate system is right_handed
    initParameters.coordinate_units = UNIT_METER;

    // Open the camera
    ERROR_CODE err = zed.open(initParameters);
    if (err != SUCCESS) {
        std::cout << toString(err) << std::endl;
        zed.close();
        //viewer.exit();
        return 1; // Quit if an error occurred
    }

    // Initialize point cloud viewer in half-size
    width = (int) zed.getResolution().width / 2;
    height = (int) zed.getResolution().height / 2;
    std::cout << "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    //viewer.init(width, height);

    // Start the camera thread
    //startZED();
    run();
    // Set the display callback
    glutCloseFunc(close);
    glutMainLoop();
    return 0;
}

/**
    Launch ZED thread. Using a thread here allows to capture a point cloud and update the GL window concurrently.
 **/
void startZED() {
    quit = false;
    //zed_callback = std::thread(run);
}

/**
    This function loops to get image and motion data from the ZED. It is similar to a callback.
    Add your own code here.
 **/
void run() {

    sl::Mat depth_map(zed.getResolution(), MAT_TYPE_8U_C4);

    cv::namedWindow("ZED Depth Map", cv::WINDOW_AUTOSIZE);
    cv::Mat cv_dmap = slMat2cvMat(depth_map);
    cv::Mat dst;
    dst.create(cv_dmap.size(), cv_dmap.type());

    while (!quit) {
        if (zed.grab() == SUCCESS) {
            // Retrieve a colored RGBA point cloud in GPU memory and update GL viewing window
            // width and height specify the total number of columns and rows for the point cloud dataset
            // In this example, we retrieve and display a half size point cloud using width and height parameters
            //zed.retrieveMeasure(point_cloud, MEASURE_XYZRGBA, MEM_GPU, width, height);
            //viewer.updatePointCloud(point_cloud);
            zed.retrieveImage(depth_map, VIEW_DEPTH);
            
            std::cout << "Zed Width: " << depth_map.getWidth() << std::endl;
            std::cout << "Zed Height: " << depth_map.getHeight() << std::endl;
            std::cout << "Zed Channels: " << depth_map.getChannels() << std::endl;
            std::cout << "Zed info: " << depth_map.getInfos() << std::endl;

            //std::cout << "Empty: " << cv_dmap.empty() << std::endl;

            CannyThreshold(cv_dmap, dst);


            cv::imshow("ZED Depth Map", dst);
            cv::waitKey(1);

        } else sl::sleep_ms(1000);
    }
}

/**
    This function closes the ZED camera, its callback (thread) and the GL viewer
 **/
void close() {
    quit = true;

    // Stop callback
    //zed_callback.join();

    // Exit point cloud viewer
    //viewer.exit();

    // Free buffer and close the ZED
    //point_cloud.free(MEM_GPU);
    zed.close();
}


/**
* Conversion function between sl::Mat and cv::Mat
**/
cv::Mat slMat2cvMat(Mat& input) {
    
        // Mapping between MAT_TYPE and CV_TYPE
            int cv_type = -1;
                switch (input.getDataType()) {
                    
                            case MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
                                    case MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
                                            case MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
                                                    case MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
                                                            case MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
                                                                    case MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
                                                                            case MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
                                                                                    case MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
                                                                                            default: break;
                                                                                                }

                                                                                                    // Since cv::Mat data requires a uchar*
                                                                                                    // pointer, we get the uchar1 pointer from
                                                                                                    // sl::Mat (getPtr<T>())
                                                                                                        // cv::Mat and sl::Mat will share a
                                                                                                        // single memory structure
                                                                                                            return cv::Mat(input.getHeight(),
                                                                                                            input.getWidth(), cv_type,
                                                                                                            input.getPtr<sl::uchar1>(MEM_CPU));
                                                                                             }
