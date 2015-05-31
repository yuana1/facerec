#include "haar_cascade.h"
#include <fstream>

#include "utils.h"



haar_cascade::haar_cascade(const string& haar_path, const string & csv_path):haar_path(haar_path),csv_path(csv_path)
{
    read_csv(this->csv_path,this->images,this->labels);
}

haar_cascade::~haar_cascade()
{

}

void haar_cascade::read_csv(const string& csv_path, vector<Mat>& images, vector<int>& labels, char separator) {
    std::ifstream file(csv_path.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void haar_cascade::train() {

    // Create a FaceRecognizer and train it on the given images:
//    model = createFisherFaceRecognizer();
    model = createLBPHFaceRecognizer();
    model->train(this->images, this->labels);
    // That's it for learning the Face Recognition model. You now
    // need to create the classifier for the task of Face Detection.
    // We are going to use the haar cascade you have specified in the
    // command line arguments:
    //
    this->cascade.load(this->haar_path);
}

CascadeClassifier haar_cascade::getHaar_cascade() {
    return this->cascade;
}

QImage haar_cascade::output(QImage &img) {
    assert(model != NULL);
    int im_width = this->images[0].cols;
    int im_height =this->images[0].rows;
    cv::Mat original = utils::convertQImage2Mat(img);
//    Mat original = frame.clone();
    // Convert the current frame to grayscale:
    Mat gray;
    cvtColor(original, gray, CV_BGR2GRAY);
    // Find the faces in the frame:
    vector< Rect_<int> > faces;
    this->cascade.detectMultiScale(gray, faces);
    // At this point you have the position of the faces in
    // faces. Now we'll get the faces, make a prediction and
    // annotate it in the video. Cool or what?
    for(int i = 0; i < faces.size(); i++) {
        // Process face by face:
        Rect face_i = faces[i];
        // Crop the face from the image. So simple with OpenCV C++:
        Mat face = gray(face_i);
        // Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
        // verify this, by reading through the face recognition tutorial coming with OpenCV.
        // Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
        // input data really depends on the algorithm used.
        //
        // I strongly encourage you to play around with the algorithms. See which work best
        // in your scenario, LBPH should always be a contender for robust face recognition.
        //
        // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
        // face you have just found:
        Mat face_resized;
        cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
        // Now perform the prediction, see how easy that is:
        int prediction = model->predict(face_resized);
        // And finally write all we've found out to the original image!
        // First of all draw a green rectangle around the detected face:
        rectangle(original, face_i, CV_RGB(0, 255,0), 1);
        // Create the text we will annotate the box with:
        string box_text = format("Prediction = %d", prediction);
        // Calculate the position for annotated text (make sure we don't
        // put illegal values in there):
        int pos_x = std::max(face_i.tl().x - 10, 0);
        int pos_y = std::max(face_i.tl().y - 10, 0);
        // And now put it into the image:
        putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
    }
    return utils::convertMat2QImage(original);
}

