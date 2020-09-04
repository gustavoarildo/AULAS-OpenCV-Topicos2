//Aluno: Gustavo Arildo Felix e Ronaldo Carvalho, Tópicos 2 de informatica, atividade 3

//Ao se observar o código exemplos/labeling.cpp conforme exemplificado,
//é possível verificar que caso existam mais de 255 objetos na cena,
//o processo de rotulação será comprometido.
//Identifique a situação em que isso ocorre,
//pesquise e proponha sua própria solução para este problema.

//RESPOSTA: A situação que isso ocorre é quando existem mais de 255 objetos em cena,
//por que no codigo de exemplo existem apenas 256 tons de cinza,
//no codigo cada objeto é rotulado com um tom diferente
//e por esses fatores o proceso é comprometido

//RESPOSTA: Uma solução para este problema seria aumentar
//a quantidade de tons ou cores que temos para trabalhar a rotulação,
//por exemplo utilizando RGB com suas combinaçoes ou em vez de usar 8 bits (256 tons)
//usar um valor de 9, 10 bits por exemplo, com 10 ja haveria a possibilidade de rotular 1024
//objetos com tons diferentes. É claro com base no exemplo o preto é o espaço entre os objetos
//então na verdade poderiam ser rotulados 1023 objetos com 10 bits para representar a imagem.

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv)//recebe o nome da imagem como argumento
{
  Mat image, mask;
  int width, height;
  int nobjectsBURACO;
  int nobjectsNOTBURACO;
  int preto;
  int branco;

  CvPoint p;
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);//abre a imagem em escala de cinza

  if (!image.data)//verifica se a imagem abriu corretamente
  {
    std::cout << "Erro ao carregar a imagem.\n";
    return (-1);
  }
  width = image.size().width;
  height = image.size().height;

  printf("%d é a altura\n", width);//verifica o valor de altura atual da imagem
  printf("%d é a largura\n", height);//verifica o valor de largura atual da imagem

  p.x = 0;//valor do ponto x
  p.y = 0;//valor do ponto y

  // para não contar bolhas que tocam as bordas da imagem
  preto = 0;//0 é preto
  branco = 255;//255 é branco
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (i == 0)
      {
        if (image.at<uchar>(i, j) == branco)//se encontra branco na borda
        {
          // Encontrou um objeto na borda
          //nobjects++;
          p.x = j;
          p.y = i;
          floodFill(image, p, preto);//enche com preto
        }
      }
      if (j == 0)
      {
        if (image.at<uchar>(i, j) == branco)//se encontra branco na borda
        {
          // Encontrou um objeto na borda
          //nobjects++;
          p.x = j;
          p.y = i;
          floodFill(image, p, preto);//enche com preto
        }
      }
      if (i == height - 1)
      {
        if (image.at<uchar>(i, j) == branco)//se encontra branco na borda
        {
          // Encontrou um objeto na borda
          //nobjects++;
          p.x = j;
          p.y = i;
          floodFill(image, p, preto);//enche com preto
        }
      }
      if (j == width - 1)
      {
        if (image.at<uchar>(i, j) == branco)//se encontra branco na borda
        {
          // Encontrou um objeto na borda
          //nobjects++;
          p.x = j;
          p.y = i;
          floodFill(image, p, preto);//enche com preto
        }
      }
    }
  }

  p.x = 0;
  p.y = 0;
  floodFill(image, p, 1);//o espaço entre objetos agora é diferente de preto
  //quer dizer que de preto só tem buraco agora

  // Busca objetos com buracos
  nobjectsBURACO = 0;//0 porque não procurou ainda, porque quem procura acha
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == preto)//achou o buraco preto
      {
        if (image.at<uchar>(i, j - 1) == branco)//o objeto emburacado branco
        {
          // Encontrou um objeto com buraco
          nobjectsBURACO++;
          p.x = j - 1;
          p.y = i;
          floodFill(image, p, 50);//rotulado pra 50
        }
      }
    }
  }

  // Busca objetos com ausencia de buracos
  nobjectsNOTBURACO = 0;//0 porque não efetuou a busca ainda
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == branco)//os objetos brancos que sobraram não tem buraco
      {
        // Encontrou um objeto que não tem buraco
        nobjectsNOTBURACO++;
        p.x = j;
        p.y = i;
        floodFill(image, p, 150);//rotulado 150
      }
    }
  }
//imprime resultado
  printf("%d é a quantidade de objetos que possuem buraco\n", nobjectsBURACO);
  printf("%d é a quantidade de objetos que não possuem buraco\n", nobjectsNOTBURACO);

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
