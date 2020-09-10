#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv)
{
  Mat image, mask;
  int width, height;
  int nobjects;
  Vec3b valROTULA; //Uso de matriz rgb para superar 255 objetos
  Vec3b valBRANCO; //cor branca

  CvPoint p;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR); //cores RGB = 255*255*255 = mais de 16 milhões

  if (!image.data)
  {
    std::cout << "Erro ao carregar a imagem.\n";
    return (-1);
  }
  width = image.size().width;
  height = image.size().height;

  p.x = 0;
  p.y = 0;

  valBRANCO[0] = 255; //BLUE//AZUL
  valBRANCO[1] = 255; //GREEN//VERDE
  valBRANCO[2] = 255; //RED//VERMELHO

  valROTULA[0] = 0; //BLUE//AZUL
  valROTULA[1] = 0; //GREEN//VERDE
  valROTULA[2] = 0; //RED//VERMELHO


  // Busca objetos com buracos
  nobjects = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<Vec3b>(i, j) == valBRANCO)
      {
        // Encontrou um objeto
        nobjects++;
        p.x = j;
        p.y = i;

        ///*

        //UTILIZANDO AS CORES
     
        {
          if (valROTULA[0] < 255 && valROTULA[1] == 0 && valROTULA[2] == 0) //B
          {
            valROTULA[0]++; //AZUL
          }
          if (valROTULA[0] >= 255 && valROTULA[1] < 255 && valROTULA[2] == 0) //BG
          {
            valROTULA[1]++; //VERDE
          }
          if (valROTULA[0] >= 255 && valROTULA[1] >= 255 && valROTULA[2] < 255) //BGR
          {
            valROTULA[2]++; //VERMELHO
          }

        
        //*/

        floodFill(image, p, valROTULA);
      }
    }
  }
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
