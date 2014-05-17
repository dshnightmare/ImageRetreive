#include "cv.h"
#include "highgui.h"
#include <opencv2/nonfree/nonfree.hpp>

int main(int argc, char** argv)
{
   cv::Mat img = cv::imread("image.jpg");

   cv::SIFT sift(10);   //number of keypoints

   cv::vector<cv::KeyPoint> key_points;

   cv::Mat descriptors, mascara;
   cv::Mat output_img;

   sift(img,mascara,key_points,descriptors);
   drawKeypoints(img, key_points, output_img);

   cv::namedWindow("Image");
   cv::imshow("Image", output_img);
   cv::waitKey(0);

   return 0;
}