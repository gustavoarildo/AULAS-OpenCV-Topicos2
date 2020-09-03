//Aluno: Gustavo Arildo Felix, Topicos 2 de informatica, atividade 2

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
    cout << "Erro ao abrir o arquivo biel.png" << endl;// informa erro ao abrir imagem
  namedWindow("janela", WINDOW_AUTOSIZE);

  Mat image2;
  image2 = image.clone();//necessario para auxiliar na montagem, a imagem dois é o alvo da montagem que não muda a original


  alturaBiel = image.size().height;//recebe altura da imagem
  larguraBiel = image.size().width;//recebe largura da imagem

  Mat quadrante1 = image(Rect(0, 0, (larguraBiel/2), (alturaBiel/2)));//copia um quadrante da imagem,em forma retangular na posiçao especificada, quadrante superior esquerdo no caso
  Mat quadrante2 = image(Rect((larguraBiel/2), 0, (larguraBiel/2), (alturaBiel/2)));
  Mat quadrante3 = image(Rect(0,(alturaBiel/2),(larguraBiel/2),(alturaBiel/2)));
  Mat quadrante4 = image(Rect((larguraBiel/2),(alturaBiel/2),(larguraBiel/2),(alturaBiel/2)));

  quadrante1.copyTo(image2(Rect((larguraBiel/2),(alturaBiel/2),(larguraBiel/2),(alturaBiel/2))));// baixo direita, imagem dois vai receber todos os quadrantes nela
	quadrante2.copyTo(image2(Rect(0,(alturaBiel/2),(larguraBiel/2),(alturaBiel/2))));//baixo esquerda
	quadrante3.copyTo(image2(Rect((larguraBiel/2), 0,(larguraBiel/2),(alturaBiel/2))));//cima direita
	quadrante4.copyTo(image2(Rect(0, 0, (larguraBiel/2), (alturaBiel/2))));//cima esquerda


  //imshow("janela", quadrante1);//necessario para testar quadrantes, no desenvolvimento
  imshow("janela", image2);//exibe a imagem remontada
  waitKey();

  return 0;
}
