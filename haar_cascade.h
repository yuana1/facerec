#ifndef HAAR_CASCADE_H
#define HAAR_CASCADE_H
#include <string>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <QImage>
#include <QObject>
using namespace std;
using namespace cv;
class haar_cascade : public QObject
{
    Q_OBJECT
private:
    string haar_path;
    string csv_path;
    vector<Mat> images;
    vector<int> labels;
    CascadeClassifier cascade;
    Ptr<FaceRecognizer> model;
public:
    explicit haar_cascade();
    explicit haar_cascade(const string& haar_path,const string& csv_path);
    ~haar_cascade();
    void train();
    CascadeClassifier getHaar_cascade();
    QImage output(QImage& img);
signals:
void _close(int ret_code);
private:
    static int count;
    void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');
};

#endif // HAAR_CASCADE_H
