//Aluno: Gustavo Arildo Felix, Topicos 2 de informatica, atividade N

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  int altura;
  int largura;
  Mat image;

  image = imread("imagem.png", CV_LOAD_IMAGE_GRAYSCALE);
  if (!image.data)
    cout << "Erro ao abrir o arquivo.png" << endl;// informa erro ao abrir imagem
  namedWindow("janela", WINDOW_AUTOSIZE);

  altura = image.size().height;//recebe altura da imagem
  largura = image.size().width;//recebe largura da imagem














  imshow("janela", image);//exibe a imagem
  waitKey();

  return 0;
}
