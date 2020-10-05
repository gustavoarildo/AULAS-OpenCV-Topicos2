//Alunos: Gustavo Arildo Felix e Ronaldo Carvalho, Topicos 2 de informatica, atividade 4
#include <iostream>
#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs/legacy/constants_c.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat image;
  Mat histR, histG, histB;

  VideoCapture cap;
  vector<Mat> planes;

  float range[] = {0, 256};
  const float *histrange = { range };

  int width, height;
  int nbins = 64, key = 0;
  int histw, histh;
  int somaVermelho;
  int somaVermelhoAntigo;

  bool uniform = true;
  bool acummulate = false;

  cap.open(0);			// Dispositivo com identificador 0

  if (!cap.isOpened()) {
    cout << "Erro de acesso, cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  histw = nbins;
  histh = nbins / 2;

  Mat histImgR(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  Mat vermelhoAntigo(histh, histw, CV_8UC3, Scalar(0, 0, 0));

  while (1) {
    cap >> image;
    split(image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for (int i=0; i < nbins; i++) {
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG,
           Point(i, histh),
           Point(i, histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB,
           Point(i, histh),
           Point(i, histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0        , nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh    , nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2 * histh, nbins, histh)));



    for(int i = 0; i<histh; i++){//vermelho filmado é somado na imagem atual e na anterior
      for (int j = 0; j<histw; j++){//se uma tiver mais vermelho que a outra, algo mudou/moveu
        somaVermelho = somaVermelho + histImgR.at<uchar>(i, j);
        somaVermelhoAntigo = somaVermelhoAntigo + vermelhoAntigo.at<uchar>(i,j);
      }
    }
    if(abs(somaVermelho - somaVermelhoAntigo)>=4000){//comparados os valores de vermelho
      putText(image, "ALGO SE MOVEU!", cvPoint(0,470),//se a diferença supera 8000
      CV_FONT_NORMAL,2.0,cvScalar(250,0,0),1,CV_AA);//o alarme ALGO SE MOVEU aparece

    }





    imshow("image", image);
    key = waitKey(30);
    // if (waitKey(30) >= 0) break;
    vermelhoAntigo = histImgR.clone();//Imagem é capturada para futura comparação
    somaVermelho=0;
    somaVermelhoAntigo=0;//valores zerados para que continue o ciclo sem fim
  }

  return 0;
}
