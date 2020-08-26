#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  int alturaBiel;
  int larguraBiel;
  Mat image;

  image = imread("biel.png", CV_LOAD_IMAGE_GRAYSCALE);
  if (!image.data)
    cout << "Erro ao abrir o arquivo biel.png" << endl;
  namedWindow("janela", WINDOW_AUTOSIZE);

  Mat image2;
  image2 = image.clone();//necessario para auxiliar na montagem


  alturaBiel = image.size().height;
  larguraBiel = image.size().width;

  Mat quadrante1 = image(Rect(0, 0, (larguraBiel/2), (alturaBiel/2)));//replicado ok//retangulo sem perda
  Mat quadrante2 = image(Rect((larguraBiel/2), 0, (larguraBiel/2), (alturaBiel/2)));//replicado ok
  Mat quadrante3 = image(Rect(0,(alturaBiel/2),(larguraBiel/2),(alturaBiel/2)));//replicado ok
  Mat quadrante4 = image(Rect((larguraBiel/2),(alturaBiel/2),(larguraBiel/2),(alturaBiel/2)));//replicado ok// todos quadrantes replicados abaixo

  quadrante1.copyTo(image2(Rect((larguraBiel/2),(alturaBiel/2),(larguraBiel/2),(alturaBiel/2))));//OK//definitivo baixo direita
	quadrante2.copyTo(image2(Rect(0,(alturaBiel/2),(larguraBiel/2),(alturaBiel/2))));//OK//definitivo baixo esquerda
	quadrante3.copyTo(image2(Rect((larguraBiel/2), 0,(larguraBiel/2),(alturaBiel/2))));//OK//definitivo cima direita
	quadrante4.copyTo(image2(Rect(0, 0, (larguraBiel/2), (alturaBiel/2))));//OK//definitivo cima esquerda


  //imshow("janela", quadrante1);//necessario para testar quadrantes
  imshow("janela", image2);
  waitKey();

  return 0;
}
