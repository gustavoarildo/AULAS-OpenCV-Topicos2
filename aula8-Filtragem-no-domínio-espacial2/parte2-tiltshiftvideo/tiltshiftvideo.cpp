//Alunos: Gustavo Arildo Felix e Ronaldo Carvalho, Topicos 2 de informatica, atividade 6 parte 1

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int main(int argvc, char **argv)
{
  const string source = "televideo02.mp4";
  int cont = 0, taxa = 2;

  //testa a abertura do video logo apos obtem largura e altura do mesmo
  VideoCapture inputVideo(source);
  if (!inputVideo.isOpened())
  {
    cout << "nao e possivel abrir o video: " << source << endl;
    return -1;
  }

  Size S = Size((int)inputVideo.get(CAP_PROP_FRAME_WIDTH),
                (int)inputVideo.get(CAP_PROP_FRAME_HEIGHT));

  //O video que vai ser salvo para saida esta abaixo
  VideoWriter outputVideo;
  outputVideo.open("saida.mkv", CV_FOURCC('D', 'I', 'V', '3'), inputVideo.get(CAP_PROP_FPS) / taxa, S, true);
  if (!outputVideo.isOpened())
  {
    cout << "arquivo de escrita nao pode ser aberto = " << source << endl;
    return -1;
  }

  //parametros do tiltshift abaixo
  Mat mask(9, 9, CV_32F), mask1;
  Mat image32f, imageFiltered, result;
  Mat image1, image2, res;

  double alfa;

  double altura = 0.3 * S.height; // altura da regiao central
  double centro = 0.5 * S.height; // posicao vertical do centro da regiao que estara em foco
  double decaimento = 10;         // o decaimento da regiao que tera o borramento aqui e 10
  double l1 = centro - altura / 2;
  double l2 = centro + altura / 2;

  float media[] = {1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1};

  mask = Mat(9, 9, CV_32F, media);
  scaleAdd(mask, 1 / 81.0, Mat::zeros(9, 9, CV_32F), mask1);
  swap(mask, mask1);

  for (;;)
  {
    cont++;
    if (cont > 100)
    {
      inputVideo >> image1;
      if (image1.empty())
        break; //faz a verificacao se o video ja acabou

      image2 = image1.clone();
      image2.convertTo(image32f, CV_32F);

      //imagem borrada diversas vezes para evidenciar o efeito desejado
      for (int i = 0; i < 100; i++)
        filter2D(image32f, imageFiltered, image32f.depth(), mask, Point(2, 2), 0);

      //a variavel result guarda o resultado que e a imagem borrada
      imageFiltered.convertTo(result, CV_8U);

      for (int i = 0; i < result.size().height; i++)
      {
        alfa = 0.5 * (tanh((i - l1) / decaimento) - tanh((i - l2) / decaimento));
        addWeighted(image1.row(i), alfa, result.row(i), 1.0 - alfa, 0.0, image2.row(i));
      }

      outputVideo << image2;
      cont = 0;
    }
  }

  return 0;
}