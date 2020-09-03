//Aluno: Gustavo Arildo Felix, Topicos 2 de informatica, atividade 2

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>//para o uso de atoi

using namespace cv;
using namespace std;

int main(int argc, char** argv) {//As entradas são atraves de argumentos do terminal, ex ./regions 100 100 250 200
  Mat image;
 
  int ponto1y;
  int ponto1x;
  int ponto2y;
  int ponto2x;
  ponto1y = atoi(argv[1]);//converte a entrada para inteiro com a funcao atoi
  ponto1x = atoi(argv[2]);
  ponto2y = atoi(argv[3]);
  ponto2x = atoi(argv[4]);

  image = imread("biel.png", CV_LOAD_IMAGE_GRAYSCALE);
  if (!image.data)// se não foi possivel abrir a imagem erro
    cout << "Erro ao abrir o arquivo biel.png" << endl;

  namedWindow("janela", WINDOW_AUTOSIZE);
//os nomes estão como Y e X para evitar confusoes(em mim), estou ciente do sistema referencial do tipo destrógiro.
//os laços vão acessar os pixels atraves das posiçoes x e y informadas
  for (int y=ponto1y; y < ponto2y; y++) {//que é X na verdade
    for (int x=ponto1x; x < ponto2x; x++) {//que é Y na verdade
      image.at<uchar>(y,x)= 255 - image.at<uchar>(y,x); //branco menos imagem é o negativo da imagem
    }
  }

  imshow("janela", image);
  waitKey();

  return 0;
}
