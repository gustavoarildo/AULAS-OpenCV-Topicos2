#include <iostream>
#include <opencv2/opencv.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

// Troca os quadrantes da imagem da DFT
void deslocaDFT(Mat &image) {
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
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

int main(int, char**) {
  VideoCapture cap;
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, tmp;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

  // Habilita/desabilita ruido
  int noise = 0;
  // Frequencia do ruido
  int freq = 10;
  // Ganho inicial do ruido
  float gain = 1;

  // Valor do ruido
  float mean;

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

  // Prepara o filtro passa-baixas ideal
  for (int i = 0; i < dft_M; i++) {
     for (int j = 0; j < dft_N; j++) {
        if ((i - dft_M / 2) * (i - dft_M / 2) + (j - dft_N / 2) * (j - dft_N / 2) < RADIUS * RADIUS) {
           tmp.at<float> (i, j) = 1.0;
        }
     }
  }

  // Cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);

  for(;;) {
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

    // Calcula a DTF
    dft(complexImage, complexImage);

    // Realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // Aplica o filtro frequencial
    mulSpectrums(complexImage, filter, complexImage, 0);

    // Limpa o array de planos
    planos.clear();
    // Separa as partes real e imaginaria para modifica-las
    split(complexImage, planos);

    // Usa o valor medio do espectro para dosar o ruido
    mean = abs(planos[0].at<float> (dft_M / 2, dft_N / 2));

    // Insere ruido coerente, se habilitado
    if (noise) {
      // F(u, v) recebe ganho proporcional a F(0, 0)
      planos[0].at<float>(dft_M / 2 + freq, dft_N / 2 + freq) +=
        gain * mean;

      planos[1].at<float>(dft_M / 2 + freq, dft_N / 2 + freq) +=
        gain * mean;

      // F * (-u, -v) = F(u, v)
      planos[0].at<float>(dft_M / 2 - freq, dft_N / 2 - freq) =
        planos[0].at<float>(dft_M / 2 + freq, dft_N / 2 + freq);

      planos[1].at<float>(dft_M / 2 - freq, dft_N / 2 - freq) =
        - planos[1].at<float>(dft_M / 2 + freq, dft_N / 2 + freq);

    }

    // Recompoe os planos em uma unica matriz complexa
    merge(planos, complexImage);

    // Troca novamente os quadrantes
    deslocaDFT(complexImage);

	// cout << complexImage.size().height << endl;
    // Calcula a DFT inversa
    idft(complexImage, complexImage);

    // Limpa o array de planos
    planos.clear();

    // Separa as partes real e imaginaria da imagem filtrada
    split(complexImage, planos);

    // Normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("filtrada", planos[0]);

    key = (char) waitKey(10);
    if (key == 27) break; // ESC pressed!
    switch(key) {
      // Aumenta a frequencia do ruido
    case 'q':
      freq = freq + 1;
      if (freq > dft_M / 2 - 1)
        freq = dft_M / 2 - 1;
      break;
      // Diminui a frequencia do ruido
    case 'a':
      freq = freq - 1;
      if (freq < 1)
        freq = 1;
      break;
      // Amplifica o ruido
    case 'x':
      gain += 0.1;
      break;
      // Atenua o ruido
    case 'z':
      gain -= 0.1;
      if (gain < 0)
        gain = 0;
      break;
      // Insere/remove ruido
    case 'e':
      noise = !noise;
      break;
    }
  }
  return 0;
}
