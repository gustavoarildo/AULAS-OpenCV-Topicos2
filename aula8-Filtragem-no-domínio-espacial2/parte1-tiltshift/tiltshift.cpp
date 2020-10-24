//Alunos: Gustavo Arildo Felix e Ronaldo Carvalho, Topicos 2 de informatica, atividade 6 parte 1
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double alfa;
double l1, l2;

int y_max;

int altura = 0;
int centro = 0;
int decaimento = 1;

int regiao_slider = 0;
int decaimento_slider = 0;
int centro_slider = 0;

int maximo = 100;

Mat image1, image2;
Mat image32f, imageFiltered, result;
Mat mask(3, 3, CV_32F), mask1;

char TrackbarName[50];

//ajuste de altura da regiao central focalizada
void on_trackbar_regiao(int, void *)
{
  //centro da imagem definido
  altura = (double)(regiao_slider * y_max) / maximo;

  //definidas as operacoes matematicas de l1 e l2
  l1 = centro - altura / 2;
  l2 = centro + altura / 2;

  for (int i = 0; i < result.size().height; i++)
  {
    alfa = 0.5 * (tanh((i - l1) / decaimento) - tanh((i - l2) / decaimento));
    addWeighted(image1.row(i), alfa, result.row(i), 1.0 - alfa, 0.0, image2.row(i));
  }

  imshow("Tiltshift", image2);
  imwrite("Tiltshift.png", image2);
}

//ajuste regulador de forca de decaimento da regiao borrada
void on_trackbar_decaimento(int, void *)
{
  //o minimo de decaimento é 1 por causa do alfa
  decaimento = (double)decaimento_slider + 1;
  on_trackbar_regiao(regiao_slider, 0);
}

//ajuste regulador da posicao vertical do centro da regiao focalisada
void on_trackbar_centro(int, void *)
{
  centro = (double)(centro_slider * y_max) / maximo;
  on_trackbar_regiao(regiao_slider, 0);
}

int main(int argvc, char **argv)
{
  float media[] = {1, 1, 1,
                   1, 1, 1,
                   1, 1, 1};

  image1 = imread("blend2.jpg");
  image2 = image1.clone();

  y_max = image1.size().height;

  mask = Mat(3, 3, CV_32F, media);
  scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask1);
  swap(mask, mask1);
  image2.convertTo(image32f, CV_32F);

  // para que possa ficar mais evidente o borramento na imagem ele ocorre muitas vezes
  for (int i = 0; i < 15; i++)
    filter2D(image32f, imageFiltered, image32f.depth(), mask, Point(2, 2), 0);

  //result armazena a imagem borrada
  imageFiltered.convertTo(result, CV_8U);

  namedWindow("Tiltshift", 1);

  sprintf(TrackbarName, "Regiao Central: ");
  createTrackbar(TrackbarName, "Tiltshift",
                 &regiao_slider,
                 maximo,
                 on_trackbar_regiao);
  on_trackbar_regiao(regiao_slider, 0);

  sprintf(TrackbarName, "Decaimento: ");
  createTrackbar(TrackbarName, "Tiltshift",
                 &decaimento_slider,
                 maximo,
                 on_trackbar_decaimento);
  on_trackbar_decaimento(decaimento_slider, 0);

  sprintf(TrackbarName, "Centro da Regiao: ");
  createTrackbar(TrackbarName, "Tiltshift",
                 &centro_slider,
                 maximo,
                 on_trackbar_centro);
  on_trackbar_centro(centro_slider, 0);

  waitKey(0);
  return 0;
}