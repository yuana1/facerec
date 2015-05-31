#ifndef UTILS_H
#define UTILS_H
#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
class utils
{
public:
    static std::string csv_path;
    static std::string haar_path;
    utils();
    ~utils();
    static QImage convertMat2QImage(cv::Mat& img_cv);
    static QImage convertMat2QImage(cv::Mat& img_cv,QImage& img_qt);
    static cv::Mat convertQImage2Mat(QImage &img_qt);
};

#endif // UTILS_H
