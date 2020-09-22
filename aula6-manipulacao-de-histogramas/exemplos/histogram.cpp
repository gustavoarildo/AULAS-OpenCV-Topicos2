#include <iostream>
#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs/legacy/constants_c.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat image;
  Mat histR, histG, histB;

  VideoCapture cap;
  vector<Mat> planes;

  float range[] = {0, 256};
  const float *histrange = { range };

  int width, height;
  int nbins = 64, key = 0;
  int histw, histh;

  bool uniform = true;
  bool acummulate = false;

  cap.open(0);			// Dispositivo com identificador 0

  if (!cap.isOpened()) {
    cout << "Erro de acesso, cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  histw = nbins;
  histh = nbins / 2;

  Mat histImgR(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0, 0, 0));

  while (1) {
    cap >> image;
    split(image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for (int i=0; i < nbins; i++) {
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG,
           Point(i, histh),
           Point(i, histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB,
           Point(i, histh),
           Point(i, histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0        , nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh    , nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2 * histh, nbins, histh)));
    imshow("image", image);
    key = waitKey(30);
    // if (waitKey(30) >= 0) break;
  }

  return 0;
}
