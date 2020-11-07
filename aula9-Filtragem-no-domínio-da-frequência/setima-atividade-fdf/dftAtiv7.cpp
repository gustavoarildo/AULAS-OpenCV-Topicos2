//Alunos: Gustavo Arildo Felix e Ronaldo Carvalho, Topicos 2 de informatica, atividade 7 parte unica
// Valores que sugiro para testes em execucao= (Gl 62)(Gh 89)(d -22)(c -11) //quanto menos luz melhor no ambiente
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

// Troca os quadrantes da imagem da DFT
void deslocaDFT(Mat &image)
{
  Mat tmp, A, B, C, D;

  // Se a imagem tiver tamanho impar, recorta a regiao para
  // Evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;

  // Reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

int main(int, char **)
{
  VideoCapture cap;
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, tmp;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

  //*
  float Gh = 1.0, Gl = 1.0, d = 1.0, c = 1.0;
  double radius;
  //*/

  // Guarda tecla capturada
  char key;

  // Valores ideais dos tamanhos da imagem para calculo da DFT
  int dft_M, dft_N;

  // Abre a camera default
  cap.open(0);
  if (!cap.isOpened())
    return -1;

  // Captura uma imagem para recuperar as informacoes de gravacao
  cap >> image;

  // Identifica os tamanhos otimos para calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // Realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // Parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // Prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // A funcao de transferencia (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // Cria uma matriz temporaria para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);
  //*
  // Prepara o filtro homomorfico
  for (int i = 0; i < dft_M; i++)
  {
    for (int j = 0; j < dft_N; j++)
    {
      radius = (double)(i - dft_M / 2) * (i - dft_M / 2) + (j - dft_N / 2) * (j - dft_N / 2);
      tmp.at<float>(i, j) = (Gh - Gl) * (1 - exp(-c * (radius / pow(d, 2)))) + Gl;
    }
  }
  //*/

  // Cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[] = {tmp, tmp};
  merge(comps, 2, filter);

  for (;;)
  {
    cap >> image;
    cvtColor(image, imagegray, CV_BGR2GRAY);
    imshow("original", imagegray);

    // Realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // Limpa o array de matrizes que vao compor a imagem complexa
    planos.clear();
    // Cria a componente real
    realInput = Mat_<float>(padded);
    // Insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // Combina o array de matrizes em uma unica componente complexa
    merge(planos, complexImage);

    // Calcula a DTF//
    dft(complexImage, complexImage);

    // Realiza a troca de quadrantes//
    deslocaDFT(complexImage);

    // Aplica o filtro frequencial//
    mulSpectrums(complexImage, filter, complexImage, 0);

    // Limpa o array de planos//
    planos.clear();

    //*
    //troca os quadrantes denovo
    deslocaDFT(complexImage);

    cout << "Gl = " << Gl << " Gh = " << Gh << " D0 = " << d << " c = " << c << endl;

    //*/

    // cout << complexImage.size().height << endl;//
    // Calcula a DFT inversa//
    idft(complexImage, complexImage);

    // Limpa o array de planos//
    planos.clear();

    // Separa as partes real e imaginaria da imagem filtrada
    split(complexImage, planos);

    // Normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("filtrada", planos[0]);
    //*
    key = (char)waitKey(10);
    if (key == 27)
      break; // ESC pressed!
    switch (key)
    {
      // Aumenta a Iluminancia (Gl)
    case 'a':
      Gl = Gl + 1;
      break;
      // Diminui a Iluminancia (Gl)
    case 's':
      Gl = Gl - 1;
      break;
      // Amplifica Refletancia (Gh)
    case 'd':
      Gh = Gh + 1;
      break;
      // Atenua Refletancia (Gh)
    case 'f':
      Gh = Gh - 1;
      break;
      // Amplifica frequencia de corte (d)
    case 'g':
      d = d + 1;
      break;
      // Atenua frequencia de corte (d)
    case 'h':
      d = d - 1;
      break;
      // Amplifica constante (c)
    case 'j':
      c = c + 1;
      break;
      // Atenua constante (c)
    case 'k':
      c = c - 1;
      break;
    }

    //*/
  }
  return 0;
}
