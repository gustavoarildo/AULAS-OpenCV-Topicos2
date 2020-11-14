//Alunos: Gustavo Arildo Felix e Ronaldo Carvalho, Topicos 2 de informatica, atividade 8 parte unica

//O codigo é uma combinação dos codigos de exemplo canny.cpp e pontilhismo.cpp
//ele usa pontilhismo.cpp para gerar a imagem pontilhista com raio 3
//ele usa canny.cpp para produzir as bordas
//com essas bordas ele faz o pontilhismo com raio 1 tornando a borda suave

#include <iostream>
#include "opencv2/opencv.hpp"

#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP   5
#define JITTER 3
#define RAIO   3

int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border;
Mat  frame, points;

void on_trackbar_canny(int, void *) {
  Canny(image, border, top_slider, 3 * top_slider);
  imshow("canny", border);
}

int main(int argc, char **argv) {
  int width, height, gray;

  vector<int> yrange;
  vector<int> xrange;

  int x, y;

  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

   srand(time(0));

    if (!image.data) {
	cout << "Erro ao abrir o arquivo" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width = image.size().width;
  height = image.size().height;

  sprintf(TrackbarName, "Threshold inferior", top_slider_max);

  namedWindow("canny", 1);
  createTrackbar(TrackbarName, "canny",
                &top_slider,
                top_slider_max,
                on_trackbar_canny);

  on_trackbar_canny(top_slider, 0);

  xrange.resize(height / STEP);
  yrange.resize(width / STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for (uint i = 0; i < xrange.size(); i++) {
    xrange[i] = xrange[i] * STEP + STEP / 2;
  }

  for (uint i = 0; i < yrange.size(); i++) {
    yrange[i] = yrange[i] * STEP + STEP / 2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for (auto i : xrange) {
    random_shuffle(yrange.begin(), yrange.end());
    for (auto j : yrange) {
      x = i + rand() % (2 * JITTER) - JITTER + 1;
      y = j + rand() % (2 * JITTER) - JITTER + 1;
      gray = image.at<uchar>(x, y);
      circle(points,
             cv::Point(y, x),
             RAIO,
             CV_RGB(gray, gray, gray),
             -1,
             CV_AA);
    }
  }


///*
//nesse trecho que o codigo ja fez o pontilhismo e ja tem a borda
//somente por onde passa a borda que recebe o pontilhismo
//com o raio 1, 3-2=1 , com circulos com raio menor as bordas da
//imagem são suavizadas
for (auto i : xrange) {
    //random_shuffle(yrange.begin(), yrange.end());
    for (auto j : yrange) {
      x = i + rand() % (2 * JITTER) - JITTER + 1;
      y = j + rand() % (2 * JITTER) - JITTER + 1;
      gray = image.at<uchar>(x, y);
      if(border.at<uchar>(x,y)==255){
      circle(points,
             cv::Point(y, x),
             RAIO-2,
             CV_RGB(gray, gray, gray),
             -1,
             CV_AA);
      }
    }
  }

//*/


  waitKey();
  imwrite("cannyborders.png", border);
  imwrite("pontos.jpg", points);
  return 0;
}