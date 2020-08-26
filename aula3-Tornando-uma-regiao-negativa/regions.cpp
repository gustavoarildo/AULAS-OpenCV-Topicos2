#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat image;
  Vec3b val;
  int ponto1x;
  int ponto1y;
  int ponto2x;
  int ponto2y;
  ponto1x = atoi(argv[1]);
  ponto1y = atoi(argv[2]);
  ponto2x = atoi(argv[3]);
  ponto2y = atoi(argv[4]);

  image = imread("biel.png", CV_LOAD_IMAGE_GRAYSCALE);
  if (!image.data)
    cout << "Erro ao abrir o arquivo biel.png" << endl;

  namedWindow("janela", WINDOW_AUTOSIZE);

  for (int y=ponto1y; y < ponto2y; y++) {
    for (int x=ponto1x; x < ponto2x; x++) {
      image.at<uchar>(y,x)=0;
    }
  }

  imshow("janela", image);
  waitKey();

  image = imread("biel.png", CV_LOAD_IMAGE_COLOR);

  val[0] = 0;   //B
  val[1] = 0;   //G
  val[2] = 255; //R

  for (int i=200; i < 210; i++){
    for (int j=10; j < 200; j++){
      image.at<Vec3b>(i,j) = val;
    }
  }

  imshow("janela", image);
  waitKey();
  return 0;
}
