#ifndef CONVERTER_H
#define CONVERTER_H

#include <opencv2/core.hpp>

using namespace cv;

class Converter
{    
public:
    Converter(){}
    Mat convert_img_to_oRGB(Mat origin);
    Mat cool_warm_shift(Mat img_oRGB, double delta_green_red, double delta_blue_yellow);
    Mat convert_img_to_RGB(Mat shifted_img_oRGB);

private:
    Vec2d rotate (Vec2d vec, double angle);
    double calculateTheta0(double theta);
    double calculateTheta(double theta0);
    void my_clamp(Mat &matrix);
    Vec3d convert_pxl_to_oRGB(Vec3b intensity);
    Vec3b convert_pxl_to_RGB(Vec3d intensity);
    const Mat linearTransform_RGB2LCC = (Mat_<double>(3,3)<<0.2990, 0.5870, 0.1140,
                                                      0.5000, 0.5000, -1.0000,
                                                      0.8660, -0.8660, 0.0000);

    const Mat linearTransform_LCC2RGB = (Mat_<double>(3,3)<<1.0000, 0.1140, 0.7436,
                                                      1.0000, 0.1140, -0.4111,
                                                      1.0000, -0.8860, 0.1663);
};

#endif // CONVERTER_H
