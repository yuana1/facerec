#ifndef HAAR_CASCADE_H
#define HAAR_CASCADE_H
#include <string>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <QImage>
using namespace std;
using namespace cv;
class haar_cascade
{
private:
    string haar_path;
    string csv_path;
    vector<Mat> images;
    vector<int> labels;
    CascadeClassifier cascade;
    Ptr<FaceRecognizer> model;
public:
    haar_cascade(const string& haar_path,const string& csv_path);
    ~haar_cascade();
    void train();
    CascadeClassifier getHaar_cascade();
    QImage output(QImage& img);
private:
    void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');
};

#endif // HAAR_CASCADE_H
