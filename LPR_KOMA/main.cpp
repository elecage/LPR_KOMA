#pragma warning(disable: 4819)
#include <opencv/cv.hpp>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <io.h>
#include <conio.h>
#include <iostream>
using namespace std;
using namespace cv;

const int lower	 = 50;
const int higher = 200;
const double ratio_low = 0.7;
const double ratio_high = 3.5;
const int area_low = 520;
const int area_high = 7000;
double dist = 3000;
int main(int argc, char** argv)
{
	_finddata_t fd;
	intptr_t handle;
	int result = 1;
	string path = "D:\\Projects\\2016_KOMA\\images\\*.*";

	Mat image, image2, image3, drawing;
	Mat mask = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
	Rect rect, temp_rect;	// Make temporary rectangles

	vector<vector<Point>>contours;	// findcontour 함수를 위한 벡터
	vector<Vec4i> hierarch;

	

	double ratio, delta_x, delta_y, gradient;	// 'Snake' Algorithm
	int select, plate_width, count, friend_count = 0, refinery_count = 0;

	handle = _findfirst(path.c_str(), &fd);
	char str[200];

	do
	{
		if (fd.name[0] != '.')
		{
			sprintf(str, "D:\\Projects\\2016_KOMA\\images\\%s", fd.name);
			// 흑백 전환
			image = imread(str);
			image.copyTo(image2);
			image.copyTo(image3);
		//	img2 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
			cvtColor(image2, image2, CV_BGRA2GRAY);
			
			//adaptiveThreshold(image2, image3, 100, THRESH_OTSU, THRESH_BINARY_INV, 5, 0);
			adaptiveThreshold(image2, image2, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);
			//equalizeHist(image2, image2);
			erode(image2, image2, mask, Point(-1, -1), 3);
			dilate(image2, image2, mask, Point(-1, -1), 3);
			
			
			
			


			// 캐니 검출기
			Canny(image2, image2, lower, higher, 3);
		//	img3 = doCanny(img2, 100, 300, 3);

		//	cvNamedWindow(str, CV_WINDOW_AUTOSIZE);

			// find contour
			
			
			findContours(image2, contours, hierarch, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());
			vector<vector<Point>>contours_poly(contours.size());
			vector<Rect>boundRect(contours.size());
			vector<Rect>boundRect2(contours.size());

			for (int i = 0; i < contours.size(); i++) {
				approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
				boundRect[i] = boundingRect(Mat(contours_poly[i]));
			}

			drawing = Mat::zeros(image2.size(), CV_8UC3);
			refinery_count = 0;
			for (int i = 0; i < contours.size(); i++) {
				ratio = (double)boundRect[i].height / boundRect[i].width;

				// Filtering rectangles height / width ratio, and size
				if ((ratio <= ratio_high) && (ratio >= ratio_low) && (boundRect[i].area() <= area_high) && (boundRect[i].area() >= area_low)) {
					drawContours(drawing, contours, i, Scalar(0, 255, 255), 1, 8, hierarch, 0, Point(0,0));
					rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);
					// include only suitable rectangles
					refinery_count += 1;
					
					boundRect2[refinery_count] = boundRect[i];
				}
			}
			boundRect2.resize(refinery_count);	// Resize refinery rectangle array
			
			//vector<vector<Point>>contours_poly(contours.size());
			//vector<Rect>boundRect(contours.size());
			//vector<Rect>boundRect2(contours.size());
			
			//cvReleaseImage(&img);
			//cvReleaseImage(&img2);
			//cvReleaseImage(&img3);

			//  Bubble Sort accordance with X-coordinate.
	


			
			//imshow(str, drawing);
			//imshow(str, image3);

			imshow(str, image2);

			waitKey(0);
			destroyWindow(str);

			contours_poly.clear();
			boundRect.clear();
			boundRect2.clear();
		}
	} while (_findnext(handle, &fd) == 0);

	
	_findclose(handle);
	

	
	// cvDestroyWindow("Example1");
}