#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat image;
  //Vec3b val;
  int ponto1y;
  int ponto1x;
  int ponto2y;
  int ponto2x;
  ponto1y = atoi(argv[1]);
  ponto1x = atoi(argv[2]);
  ponto2y = atoi(argv[3]);
  ponto2x = atoi(argv[4]);

  image = imread("biel.png", CV_LOAD_IMAGE_GRAYSCALE);
  if (!image.data)
    cout << "Erro ao abrir o arquivo biel.png" << endl;

  namedWindow("janela", WINDOW_AUTOSIZE);
//os nomes estão como Y e X para evitar confusoes(em mim), estou ciente do sistema referencial do tipo destrógiro.
  for (int y=ponto1y; y < ponto2y; y++) {
    for (int x=ponto1x; x < ponto2x; x++) {
      image.at<uchar>(y,x)= 255 - image.at<uchar>(y,x);
    }
  }

  imshow("janela", image);
  waitKey();

  return 0;
}
