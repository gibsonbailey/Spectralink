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
#include <math.h>
#include <signal.h>
#include <sys/types.h>

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
std::string type2str(int type);

float h_max = -5;
float h_min = 500;
int edgeThresh = 1;
int lowThreshold = 50;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
cv::Mat src_gray;

struct Point
{
    int max_i;
    int max_j;
    int max_value; //R value
};

void find_max(const cv::Mat& src, int x_partitions, int y_partitions, Point* max_filtered);

const int X_PART = 16;
const int Y_PART = 16;


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
	/*allocate the string arrays for all 20 files*/
	char * saw261 = strdup("./sawtooth261.wav");
	char * saw349 = strdup("./sawtooth349.wav");
	char * saw493 = strdup("./sawtooth493.wav");
	char * saw659 = strdup("./sawtooth659.wav");
	char * saw880 = strdup("./sawtooth880.wav");
	
	char * sine261 = strdup("./sine261.wav");
	char * sine349 = strdup("./sine349.wav");
	char * sine493 = strdup("./sine493.wav");
	char * sine659 = strdup("./sine659.wav");
	char * sine880 = strdup("./sine880.wav");
	//
	char * squ261 = strdup("./square261.wav");
	char * squ349 = strdup("./square349.wav");
	char * squ493 = strdup("./square493.wav");
	char * squ659 = strdup("./square659.wav");
	char * squ880 = strdup("./square880.wav");
	
	char * tri261 = strdup("./triangle261.wav");
	char * tri349 = strdup("./triangle349.wav");
	char * tri493 = strdup("./triangle493.wav");
	char * tri659 = strdup("./triangle659.wav");
	char * tri880 = strdup("./triangle880.wav");
	
	char x_val[128] = {0};
	char y_val[128] = {0};
	char z_val[128] = {0};
	
	

    sl::Mat depth_map(zed.getResolution(), MAT_TYPE_8U_C4);

    cv::namedWindow("ZED Depth Map", cv::WINDOW_AUTOSIZE);
    cv::Mat cv_dmap = slMat2cvMat(depth_map);
    cv::Mat dst;
    dst.create(cv_dmap.size(), cv_dmap.type());
		while (!quit) {
			if (zed.grab() == SUCCESS) {
				
				char ** arg_list = (char **)malloc(sizeof(char)*256*4*128); 
				int arg_count = 0;
				
				// Retrieve a colored RGBA point cloud in GPU memory and update GL viewing window
				// width and height specify the total number of columns and rows for the point cloud dataset
				// In this example, we retrieve and display a half size point cloud using width and height parameters
				//zed.retrieveMeasure(point_cloud, MEASURE_XYZRGBA, MEM_GPU, width, height);
				//viewer.updatePointCloud(point_cloud);
				zed.retrieveImage(depth_map, VIEW_DEPTH);

				//std::cout << "Zed Width: " << depth_map.getWidth() << std::endl;
				//std::cout << "Zed Height: " << depth_map.getHeight() << std::endl;
				//std::cout << "Zed Channels: " << depth_map.getChannels() << std::endl;
				//std::cout << "Zed info: " << depth_map.getInfos() << std::endl;
				//std::cout << "Type: " << type2str(src_gray.type()) << std::endl;

				//std::cout << "Empty: " << cv_dmap.empty() << std::endl;

				CannyThreshold(cv_dmap, dst);
				cvtColor(cv_dmap, src_gray, CV_BGR2GRAY );

	  //          cv::imshow("ZED Depth Map", dst);
	  
				
				//cv::imshow("Filtered Depth Map", find_max(src_gray, 11, 11));
				
				//Point max_filter_edge_system[X_PART * Y_PART];
				Point max_filter_disparity_map[X_PART * Y_PART];
				
				//find_max(dst, X_PART, Y_PART, max_filter_edge_system);
				find_max(src_gray, X_PART, Y_PART, max_filter_disparity_map);
				
				zed.retrieveMeasure(depth_map, MEASURE_XYZRGBA);
				
				//float distance;
				int valid_sources = 0;
				for (int i = 0; i < X_PART*Y_PART; i++){
					
					
					sl::float4 point3D;
					depth_map.getValue(max_filter_disparity_map[i].max_i,max_filter_disparity_map[i].max_j,&point3D);
					//depth_map.getValue(500,400,&point3D);
					
					//std::cout << max_filter[i].max_i << " " << max_filter[i].max_j << " " << max_filter[i].max_value << std::endl;
					//distance = sqrt(point3D.x * point3D.x + point3D.y * point3D.y + point3D.z * point3D.z);
					
					
					//if (std::isnan(point3D.x) && std::isnan(point3D.y) && std::isnan(point3D.x)){
						//std::cout << " Yuuup" << std::endl;
						
					//}
					//std::cout << point3D.x << " " << point3D.y << " " << point3D.z << " " << std::endl;
					

					//float depth_value=0;
					//depth_map.getValue(500,400,&depth_value);
					//std::cout << depth_value << std::endl;
					
					float h = atan(point3D.z/(sqrt(point3D.x * point3D.x + point3D.y * point3D.y)));
					//convert h from radians to degrees
					h = h*180/M_PI;
					//printf("%f\n", h);
					//printf("max: %f\nmin: %f\n", h_max, h_min);
					if(h > h_max)
						h_max = h;
					if(h < h_min)
						h_min = h;
					//sort the values and add the correct strings to argv
					if(!std::isnan(point3D.x)){
						if(max_filter_disparity_map[i].max_value < 51){
							if(h > -22.5){
								arg_list[arg_count] = saw261;
							}
							if(h <= -22.5 && h > -45.0){
								arg_list[arg_count] = squ261;
							}
							if(h <= -45.0 && h > -67.5){
								arg_list[arg_count] = tri261;
							}
							if(h <= -67.5){
								arg_list[arg_count] = sine261;
							}
						}
						else if((max_filter_disparity_map[i].max_value >= 51) && (max_filter_disparity_map[i].max_value < 103)){
							if(h > -22.5){
								arg_list[arg_count] = saw349;
							}
							if(h <= -22.5 && h > -45.0){
								arg_list[arg_count] = squ349;
							}
							if(h <= -45.0 && h > -67.5){
								arg_list[arg_count] = tri349;
							}
							if(h <= -67.5){
								arg_list[arg_count] = sine349;
							}	
						}
								
						else if((max_filter_disparity_map[i].max_value >= 103) && (max_filter_disparity_map[i].max_value < 154)){
							if(h > -22.5){
								arg_list[arg_count] = saw493;
							}
							if(h <= -22.5 && h > -45.0){
								arg_list[arg_count] = squ493;
							}
							if(h <= -45.0 && h > -67.5){
								arg_list[arg_count] = tri493;
							}
							if(h <= -67.5){
								arg_list[arg_count] = sine493;
							}		
						}
						else if((max_filter_disparity_map[i].max_value >= 154) && (max_filter_disparity_map[i].max_value < 205)){
							if(h > -22.5){
								arg_list[arg_count] = saw659;
							}
							if(h <= -22.5 && h > -45.0){
								arg_list[arg_count] = squ659;
							}
							if(h <= -45.0 && h > -67.5){
								arg_list[arg_count] = tri659;
							}
							if(h <= -67.5){
								arg_list[arg_count] = sine659;
							}						
						}
						else if(max_filter_disparity_map[i].max_value >= 205){
							if(h > -22.5){
								arg_list[arg_count] = saw880;
							}
							if(h <= -22.5 && h > -45.0){
								arg_list[arg_count] = squ880;
							}
							if(h <= -45.0 && h > -67.5){
								arg_list[arg_count] = tri880;
							}
							if(h <= -67.5){
								arg_list[arg_count] = sine880;
							}
						
						}
						else {}
						
						memset(x_val, 0, sizeof(char) *128);
						memset(y_val, 0, sizeof(char) *128);
						memset(z_val, 0, sizeof(char) *128);
						
						snprintf(x_val, sizeof(x_val), "%f", point3D.x);
						snprintf(y_val, sizeof(y_val), "%f", point3D.y);
						snprintf(z_val, sizeof(z_val), "%f", point3D.z);
						//printf("%s %s %s\n", x_val, y_val, z_val);
						
						//set this to +0, +1, +2 if the name of the file has not been found otherwise +1 +2 + 3
						arg_list[arg_count +1] = strdup(x_val);
						arg_list[arg_count +2] = strdup(y_val);
						arg_list[arg_count +3] = strdup(z_val);
						arg_count  +=4;
						valid_sources++; //counting how many !isnan()
					}
						
				}
				printf("arg_count: %d\n", arg_count);
				arg_list[arg_count-3] = '\0';
				//printf("%s\n", arg_list[20]);
				int pid = fork(); //fork();
				if(pid == 0){
					execv("./sound_test", arg_list);
					pid = getpid();
					kill(pid, SIGKILL);
					printf("Error: Fork bomb\n");

				}
				
				
				cv::waitKey(1);

			} else sl::sleep_ms(1000);
	}
}

void find_max(const cv::Mat& src, int x_partitions, int y_partitions, Point* max_filter){

    char max = 0;
    int max_i, max_j;
    int x_interval = src.cols / x_partitions;
    int y_interval = src.rows / y_partitions;
    int point_counter = 0;
    cv::Mat dst = cv::Mat(src.size(), src.type());
    for (int a=0; a < src.cols; a++){
				for (int b=0; b < src.rows; b++){
					dst.at<char>(b,a) = (char)0;
				}
			}

    for (int m = 0; m < x_partitions; m++){
        for (int n = 0; n < y_partitions; n++){
            max = 0;
            max_i = 0;
            max_j = 0;
            for (int i = m*x_interval; i <(m+1)*x_interval; i++){
                for (int j = n*y_interval; j < (n+1)*y_interval; j++){
                    if (src.at<char>(j,i) > max){
                        max = src.at<char>(j,i);
                        max_i = i;
                        max_j = j;
                    }
                }
            }
            
            max_filter[point_counter] = {max_i, max_j, max};
            point_counter++;
	        //dst.at<char>(max_j, max_i) = max;
	        
	        //for (int a=n*y_interval; a < n*y_interval + 20; a++){
				//for (int b=m*x_interval; b < m*x_interval + 20; b++){
					//dst.at<char>(a,b) = max;
				//}
			//}
	        //dst.at<char>(n*y_interval, m*x_interval) = max;

        }
    }
}

std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
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
