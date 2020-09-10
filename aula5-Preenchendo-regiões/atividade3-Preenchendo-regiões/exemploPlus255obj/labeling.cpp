#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  Mat image, mask;
  int width, height;
  int nobjects;
  Vec3b valROTULA; //Uso de matriz rgb para superar 255 objetos
  Vec3b valBRANCO; //cor branca
  vector<vector<int>> pilhaVec3b;//guardar as cores

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

  for (int i = 1; i < 255; i++)
  {
    for (int j = 1; j < 255; j++)
    {

      for (int k = 1; k < 255; k++)
      {
        //valROTULA[0] = i; //BLUE//AZUL
        //valROTULA[1] = j; //GREEN//VERDE
        //valROTULA[2] = k; //RED//VERMELHO

        //pilhaVec3b.push_back(valROTULA);
        vector<int> v1;
         v1.push_back(i);
         v1.push_back(j);
         v1.push_back(k);

        //pilhaVec3b.push_back(i);
        //pilhaVec3b.push_back(j);
        //pilhaVec3b.push_back(k);

        pilhaVec3b.push_back(v1);

        //put vairotula
      }
    }
  }

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

        //UTILIZANDO AS CORES
        if (!pilhaVec3b.empty())
        {
          int tamanhoPilha = pilhaVec3b.size();
          valROTULA[0] = pilhaVec3b[tamanhoPilha].size(); //BLUE//AZUL
          pilhaVec3b.pop_back();
          valROTULA[1] = pilhaVec3b[tamanhoPilha].size(); //GREEN//VERDE
          pilhaVec3b.pop_back();
          valROTULA[2] = pilhaVec3b[tamanhoPilha].size(); //RED//VERMELHO
          pilhaVec3b.pop_back();

          floodFill(image, p, valROTULA);
        }
        else{printf("acabaram as cores\n");}
      }
    }
  }

  printf("%d é a quantidade de objetos existentes\n", nobjects);

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
