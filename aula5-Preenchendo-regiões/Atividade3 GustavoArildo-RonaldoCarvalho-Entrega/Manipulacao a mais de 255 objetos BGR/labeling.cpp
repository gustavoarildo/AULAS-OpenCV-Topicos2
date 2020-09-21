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
  int nobjects;                   //o numero de objetos brancos na imagem//que são 32, testado com o exemplo original
  Vec3b valROTULA;                //Uso de matriz rgb para superar 255 objetos
  Vec3b valBRANCO;                //cor branca
  vector<vector<int>> pilhaVec3b; //guardar todas as cores
  int contaMatriz;

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

  contaMatriz = 0;
  //Caso desejar a manipulação da imagem com cores mais contrastantes coloque os valores +=80 para 
  //o primeiro for,+=100 para o segundo for,+=100 para o terceiro, ao manipular bolhas.png 

  for (int i = 1; i < 255; i += 1) //vai passar por todas as cores possiveis com 8 bits//+=80
  {
    for (int j = 1; j < 255; j += 1) //inicia com 1 para não alterar o preto 0//+=100
    {
      for (int k = 1; k < 255; k += 1) //uma matriz é usada como pilha de armazenamento//+=100
      {

        vector<int> v1; //esse vetor recebe as 3 cores rgb combinadas
        v1.push_back(i);
        v1.push_back(j);
        v1.push_back(k);

        pilhaVec3b.push_back(v1); //o vetor é inserido na matriz com seus valores
        contaMatriz++;
      }
    }
  }

  // Busca objetos
  nobjects = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<Vec3b>(i, j) == valBRANCO)
      {

        //UTILIZANDO AS CORES
        if (contaMatriz>0) //se existirem cores disponiveis na pilha
        {
          // Encontrou um objeto
          nobjects++; //vai contar o objeto encontrado
          p.x = j;
          p.y = i;

                 //usa-se a pilha para preencher o vec3b
          valROTULA[0] = pilhaVec3b[contaMatriz-1][0]; //BLUE//AZUL
          valROTULA[1] = pilhaVec3b[contaMatriz-1][1]; //GREEN//VERDE
          valROTULA[2] = pilhaVec3b[contaMatriz-1][2]; //RED//VERMELHO
                 //quando um valor é utilizado a combinação das cores BGR é tida como rotulo

                                   contaMatriz--;

          floodFill(image, p, valROTULA); //o objeto é rotulado
        }
        else
        {
          printf("acabaram as cores\n"); //se existirem mais objetos que combinaçoes de cores
        }
      }
    }
  }

  printf("%d é a quantidade de objetos existentes\n", nobjects); //numero de objetos brancos rotulados

  imshow("image", image);//exibe imagem
  imwrite("labeling.png", image);//salva imagem manipulada
  waitKey();
  return 0;
}
