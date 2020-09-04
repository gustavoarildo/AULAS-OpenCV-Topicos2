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

int main(int argc, char** argv) {
  Mat image, mask;
  int width, height;
  int nobjects;

  CvPoint p;
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  if (!image.data) {
    std::cout << "Erro ao carregar a imagem.\n";
    return(-1);
  }
  width = image.size().width;
  height = image.size().height;

  p.x = 0;
  p.y = 0;

  // Busca objetos com buracos
  nobjects = 0;
  for (int i=0;  i < height;  i++) {
    for (int j=0;  j < width;  j++) {
      if (image.at<uchar>(i, j) == 255) {
		// Encontrou um objeto
		nobjects++;
		p.x = j;
		p.y = i;
		floodFill(image, p, nobjects);
	  }
	}
  }
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
