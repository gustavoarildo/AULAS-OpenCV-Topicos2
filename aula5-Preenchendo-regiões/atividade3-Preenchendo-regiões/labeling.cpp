//Aluno: Gustavo Arildo Felix, Topicos 2 de informatica, atividade 3

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
