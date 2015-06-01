#include "utils.h"
utils::utils()
{

}

utils::~utils()
{

}

std::string utils::csv_path = "/home/zhaokaiyuan/QtProject/build-v4l2-Desktop_Qt_5_4_1_GCC_64bit-Debug/res/face.csv";
//std::string utils::haar_path = "./res/haarcascade_frontalface_alt.xml";
std::string utils::haar_path = "/home/zhaokaiyuan/QtProject/build-v4l2-Desktop_Qt_5_4_1_GCC_64bit-Debug/res/lbpcascade_frontalface.xml";
//std::string utils::haar_path = "./res/cascade.xml";
QImage utils::convertMat2QImage(cv::Mat &img_cv) {
    cv::Mat temp; // make the same cv::Mat
    cvtColor(img_cv, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

cv::Mat utils::convertQImage2Mat(QImage &img_qt) {
    cv::Mat tmp(img_qt.height(),img_qt.width(),CV_8UC3,(uchar*)img_qt.bits(),img_qt.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(tmp, result,CV_BGR2RGB);
    return result;
}
