#include "converter.h"

using namespace std;

Vec2d Converter::rotate (Vec2d vec, double angle)
{
    Mat rotation = (Mat_<double>(2,2) << cos(angle),   -sin(angle),
                                         sin(angle),   cos(angle));
    Mat rotatedMatrix = rotation*Mat(vec);
    return Vec2d(rotatedMatrix.at<double>(0,0), rotatedMatrix.at<double>(1,0));
}

Mat Converter::convert_img_to_oRGB(Mat origin)
{
    Mat outputImg(origin.rows, origin.cols, CV_64FC3);
    for (int i = 0; i < origin.cols; i++) {
        for (int j = 0; j < origin.rows; j++) {

            Vec3b intensity =  origin.at<Vec3b>(j, i);
            outputImg.at<Vec3d>(j, i) = convert_pxl_to_oRGB(intensity);
        }
    }
    return outputImg;
}

Mat Converter::cool_warm_shift(Mat img_oRGB, double delta_green_red, double delta_blue_yellow)
{
    Mat output(img_oRGB.rows, img_oRGB.cols, CV_64FC3);
    for (int i = 0; i < img_oRGB.cols; i++) {
        for (int j = 0; j < img_oRGB.rows; j++) {
            output.at<Vec3d>(j, i)[0] = img_oRGB.at<Vec3d>(j, i)[0];
            output.at<Vec3d>(j, i)[1] = img_oRGB.at<Vec3d>(j, i)[1] + delta_blue_yellow;
            output.at<Vec3d>(j, i)[2] = img_oRGB.at<Vec3d>(j, i)[2] + delta_green_red;
        }
    }
    return output;
}
Mat Converter::convert_img_to_RGB(Mat shifted_img_oRGB)
{
    Mat outputImg(shifted_img_oRGB.rows, shifted_img_oRGB.cols, CV_8UC3);
    for (int i = 0; i < shifted_img_oRGB.cols; i++) {
        for (int j = 0; j < shifted_img_oRGB.rows; j++) {

            Vec3d intensity = shifted_img_oRGB.at<Vec3d>(j, i);
            outputImg.at<Vec3b>(j, i) = convert_pxl_to_RGB(intensity);
        }
    }
    return outputImg;
}

double Converter::calculateTheta0(double theta)
{
    double theta0;

    if(theta < CV_PI/3 && theta > -CV_PI/3)
         theta0 = 3.0/2*theta;
    if(theta <= -CV_PI/3 && theta > -CV_PI)
        theta0 = -CV_PI/2 + 3.0/4*(theta + CV_PI/3);
    if(theta <= CV_PI && theta >= CV_PI/3)
        theta0 = CV_PI/2 + 3.0/4*(theta - CV_PI/3);

    return theta0;
}
double Converter::calculateTheta(double theta0)
{
    double theta;

    if(theta0 < CV_PI/2 && theta0 > -CV_PI/2)
         theta = 2./3*theta0;
    if(theta0 <= CV_PI && theta0 >= CV_PI/2)
        theta = CV_PI/3 + 4./3*(theta0 - CV_PI/2);
    if(theta0 <= -CV_PI/2 && theta0 >= -CV_PI)
        theta = -CV_PI/3 + 4./3*(theta0 + CV_PI/2);

    return theta;
}
void Converter::my_clamp(Mat &matrix)
{
    for (int i = 0; i < matrix.cols; i++) {
        for (int j = 0; j < matrix.rows; j++)
            matrix.at<double>(j,i) = max(min(matrix.at<double>(j,i),255.), 0.);
    }
}
Vec3d Converter::convert_pxl_to_oRGB(Vec3b intensity)
{
    Vec3d output;

    Vec3d dIntensity = intensity;

    dIntensity /= 255;

    Mat LCC = linearTransform_RGB2LCC*Mat(dIntensity);

    double L = LCC.at<double>(0,0); //calculate luminance component
    Vec2d colorComponents_of_LCC = Vec2d(LCC.at<double>(1,0), LCC.at<double>(2,0)); //calculate chromatic components

    double theta = atan2(colorComponents_of_LCC[1], colorComponents_of_LCC[0]);
    double theta0 = calculateTheta0(theta);

    Vec2d rotation = rotate(colorComponents_of_LCC, theta0 - theta);

    output[0] = L;
    output[1] = rotation[0];
    output[2] = rotation[1];

    return output;
}

Vec3b Converter::convert_pxl_to_RGB(Vec3d intensity)
{
    Vec3b output;

    Vec2d colorComponents_of_LCC = Vec2d(intensity[1], intensity[2]);

    double theta0 = atan2(colorComponents_of_LCC[1], colorComponents_of_LCC[0]);
    double theta = calculateTheta(theta0);

    Vec2d rotation = rotate(colorComponents_of_LCC, theta - theta0);
    intensity[1] = rotation[0];
    intensity[2] = rotation[1];

    Mat RGB_matrix = linearTransform_LCC2RGB*Mat(intensity, false);

    RGB_matrix *= 255.0;

    my_clamp(RGB_matrix);

    output = RGB_matrix;

    return output;
}
