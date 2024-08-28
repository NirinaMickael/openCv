#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#ifdef _WIN32
#define CLEAN "cls"
#define DIR "CWD"
#else
#define CLEAN "clear"
#define DIR "pwd"
#endif
using namespace std;
using namespace cv;
void destruct(int delai);
// Pour quitter le programme
void Fin_prog();
class ImageProcessor {
    public:
        void start(){
            this->menu();
        }
        ImageProcessor();
    private:
        static const int CHOICE = 19;
        int choice;
        Mat image;
        void load_image(Mat &image, string image_name);
        void open_image(Mat image, string label);
        Mat conversion_gray();
        Mat resize_image();
        Mat flou_gaussien();
        Mat translation();
        Mat detection_contours();
        Mat inversion();
        Mat threshold();
        Mat morphologie();
        Mat histogramme();
        Mat filtre_sobel();
        Mat remplacement_couleur();
        Mat perspective();
        Mat coin_harris();
        Mat fusion();
        Mat desaturation();
        Mat medianBlurs();
        Mat FloydSteimberg();
        Mat bilaterFilters();
        // Mat filters2D();
        void menu();
};

int main() {
    ImageProcessor imageProcessor;
    imageProcessor.start();
    return 0;
}
ImageProcessor::ImageProcessor(){
    cout << "...." << endl;
}
void  ImageProcessor:: load_image(Mat &image, string image_name)
{
  Mat img = imread("./" + image_name);
  image = img.clone();
  if (this->image.empty())
  {
    cerr << "Impossible de charger l'image . verfier le chemin de l'image" << endl;
    exit(-1);
  }
}
void  ImageProcessor:: open_image(Mat image, string label)
{
  imshow(label, image);
}
// Mat ImageProcessor :: filters2D() {
//     Mat new_image, img = this->image.clone();
//     int ksize ;	
//     cout << "Entré la taille de noyau (min 1) : ";
//     cin >> ksize;
//     if(ksize<1){
//       Fin_prog();
//     }
//     filter2D	(img,new_image,(ksize,ksize),CV_16F,Point(-1,-1));
//     return new_image;
// }
Mat ImageProcessor :: bilaterFilters() {
  Mat new_image, img = this->image.clone();
  int ksize ;
  cout << "Entré la taille de noyau (min 1) : ";
  cin >> ksize;
  if(ksize<1){
    Fin_prog();
  }
  bilateralFilter(img,new_image,ksize,ksize*2,ksize/2);
  return new_image;
}
Mat ImageProcessor:: conversion_gray()
{
  Mat image_gray, img = this->image.clone();
  cvtColor(img, image_gray, COLOR_BGR2GRAY);
  return image_gray;
}
Mat ImageProcessor:: resize_image()
{
  Mat new_image, img = this->image.clone();
  int w, h;
  cout << "Entrer la taille desiré" << endl;
  cout << "Width ";
  cin >> w;
  cout << "Height ";
  cin >> h;
  Size newSize(w, h);
  resize(img, new_image, newSize);
  return new_image;
}
Mat ImageProcessor:: flou_gaussien()
{
  Mat new_image, img = this->image.clone();
  int ksize ;
  cout << "Entré la taille de noyau (nombre imapaire) : ";
  cin >> ksize;
  GaussianBlur(img, new_image, Size(ksize,ksize), 0);
  return new_image;
}
Mat ImageProcessor:: inversion()
{
  Mat new_image, img = this->image.clone();
  bitwise_not(img, new_image);
  return new_image;
}
Mat ImageProcessor:: detection_contours()
{
  Mat edges;
  double threshold1 = 100;
  double threshold2 = 200;
  int apertureSize ;
  cout << "Entré la la taille du noyau pour le calcul du gradient : ";
  cin >> apertureSize;
  cout << "avec seuil inferieur vaut 100 et seuil superieur 200" << endl;
  Canny(this->image, edges, threshold1, threshold2,apertureSize);
  return edges;
}
Mat ImageProcessor:: histogramme()
{
  Mat new_image, img = this->image.clone();
  cvtColor(img, img, COLOR_RGBA2GRAY, 0);
  equalizeHist(img, new_image);
  return new_image;
}
Mat ImageProcessor:: filtre_sobel()
{
  Mat new_image, img = this->image.clone();
  cvtColor(img, img, COLOR_RGBA2GRAY, 0);
  Sobel(img, new_image, CV_32F, 1, 0);
  return new_image;
}
Mat ImageProcessor:: remplacement_couleur()
{
  Mat new_image, img = this->image.clone();
  cvtColor(img, img, COLOR_BGR2HSV);
  Scalar lower_bound(0, 100, 100);
  Scalar upper_bound(10, 255, 255);
  inRange(img, lower_bound, upper_bound, new_image);
  return new_image;
}
Mat ImageProcessor:: desaturation()
{
  // Convertir une image en niveaux de gris tout en conservant le canal vert
  Mat new_image, img = this->image.clone();
  cvtColor(img, new_image, COLOR_BGR2GRAY);
  return new_image;
}
Mat ImageProcessor:: fusion()
{
  double alpha = 0.5;
  double beta;
  double input;
  Mat src1, src2, dst;
  string path1,path2;
  cout << " Simple Linear Blender " << endl;
  cout << "-----------------------" << endl;
  cout << "* Enter alpha [0.0-1.0]: ";
  cin >> input;
  if (input >= 0 && input <= 1)
  {
    alpha = input;
  }
  cout << "La taille de deux image en entrée sera 400*400" << endl;
  cout << "Enter le nom de l'image 1 : " ;
  cin >> path1; 
  load_image(src1,path1);
  cout << "Enter le nom de l'image 2 : " ;
  cin >> path2;
  load_image(src2,path2);
  Size newSize(400, 400);
  resize(src2, src2, newSize);
  resize(src1, src1, newSize);
  if (src1.empty())
  {
    cout << "Error loading src1" << endl;
    exit(EXIT_FAILURE);
  }
  if (src2.empty())
  {
    cout << "Error loading src2" << endl;
    exit(EXIT_FAILURE);
  }
  beta = (1.0 - alpha);
  addWeighted(src1, alpha, src2, beta, 0.0, dst);
  return dst;
}
Mat ImageProcessor:: medianBlurs(){
  Mat new_image, img = this->image.clone();
  int kernel_size=5;
  cout << "Entrer la valeur de kernel_size : " ;
  cin >> kernel_size;
  medianBlur(img, new_image, kernel_size);
  return new_image;
}
Mat ImageProcessor:: perspective()
{
  Mat img = this->image.clone();

  // Définir les points source (coins de l'image d'origine)
  vector<Point2f> srcPoints;
  srcPoints.push_back(Point2f(0, 0));                       // Coin supérieur gauche
  srcPoints.push_back(Point2f(img.cols - 1, 0));            // Coin supérieur droit
  srcPoints.push_back(Point2f(img.cols - 1, img.rows - 1)); // Coin inférieur droit
  srcPoints.push_back(Point2f(0, img.rows - 1));            // Coin inférieur gauche

  // Définir les points de destination (nouvelle perspective)
  vector<Point2f> dstPoints;
  dstPoints.push_back(Point2f(0, 0));                         // Nouveau coin supérieur gauche
  dstPoints.push_back(Point2f(img.cols - 1, 0));              // Nouveau coin supérieur droit
  dstPoints.push_back(Point2f(img.cols * 0.7, img.rows - 1)); // Nouveau coin inférieur droit
  dstPoints.push_back(Point2f(img.cols * 0.3, img.rows - 1)); // Nouveau coin inférieur gauche

  // Calculer la matrice de transformation de perspective
  Mat perspectiveMatrix = getPerspectiveTransform(srcPoints, dstPoints);

  // Appliquer la transformation de perspective
  Mat new_image;
  warpPerspective(img, new_image, perspectiveMatrix, img.size());
  return new_image;
}
Mat ImageProcessor:: coin_harris()
{
  Mat new_image, img = this->image.clone();
  cvtColor(img, img, COLOR_RGBA2GRAY, 0);
  Mat corners, cornerNorm, cornerScaled;
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;
  // Détecter les coins de Harris
  cornerHarris(img, corners, blockSize, apertureSize, k);
  // Normaliser les coins
  normalize(corners, cornerNorm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
  // Mettre en surbrillance les coins détectés
  convertScaleAbs(cornerNorm, cornerScaled);
  // Dessiner des cercles autour des coins
  for (int i = 0; i < cornerNorm.rows; ++i)
  {
    for (int j = 0; j < cornerNorm.cols; ++j)
    {
      if (static_cast<int>(cornerNorm.at<float>(i, j)) > 150)
      {
        circle(img, Point(j, i), 5, Scalar(0), 2, 8, 0);
      }
    }
  }
  return img;
}
Mat ImageProcessor:: morphologie()
{
  Mat new_image, img = this->image.clone();
  Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
  int choice;
  cout <<"\t\t\t _______________________________________________________"<<endl;
  cout <<"\t\t\t|             (1) Erosion                                 |"<<endl;
  cout <<"\t\t\t|             (2) Dilataoin                               |"<<endl;
  cout <<" \t\t\t-------------------------------------------------------"<<endl;
  do
  {
    cout <<"\t\t\t\t\t\tEntrer votre choix: ";
    cin >> choice;
  } while (choice < 0);
  switch (choice)
  {
  case 1:
  {
    erode(img, new_image, kernel);
    break;
  }
  case 2:
  {
    dilate(img, new_image, kernel);
    break;
  }
  default:
    return image;
    break;
  }
  return new_image;
}
Mat ImageProcessor:: threshold()
{
  Mat new_image, img = this->image.clone();
  cvtColor(img, img, COLOR_RGBA2GRAY, 0);
  int choice;
  cout <<"\t\t\t _______________________________________________________"<<endl;
  cout <<"\t\t\t|             (1) en utilisant ADAPTIVE_THRESH_GAUSSIAN_C |"<<endl;
  cout <<"\t\t\t|             (2) en utilisant DAPTIVE_THRESH_MEAN_C      |"<<endl;
  cout <<" \t\t\t-------------------------------------------------------"<<endl;
  do
  {
    cout <<"\t\t\t\t\t\tEntrer votre choix: ";
    cin >> choice;
  } while (choice < 0);
  switch (choice)
  {
  case 1:
  {
    adaptiveThreshold(img, new_image, 200, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 2);
    break;
  }
  case 2:
  {
    adaptiveThreshold(img, new_image, 200, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 2);
    break;
  }
  default:
    return image;
    break;
  }
  return new_image;
}
Mat ImageProcessor:: translation()

{
  Mat new_image, img = this->image.clone();
  int choice;
  cout <<"\t\t\t _______________________________________________________"<<endl;
  cout <<"\t\t\t|                  (1) Translation affine               |"<<endl;
  cout <<"\t\t\t|                  (2) Rotation affine                  |"<<endl;
  cout <<"\t\t\t|                  (3) Réflexion affine                 |"<<endl;
  cout <<" \t\t\t-------------------------------------------------------"<<endl;
  do
  {
    cout <<"\t\t\t\t\t\tEntrer votre choix: ";
    cin >> choice;
  } while (choice < 0);
  switch (choice)
  {
  case 1:
  {
    int dx = 2, dy = 3;
    Mat translationMatrix = (Mat_<float>(2, 3) << 1, 0, dx, 0, 1, dy);
    warpAffine(this->image, new_image, translationMatrix, image.size());
    break;
  }
  case 2:
  {
    // Calcule le point central de l'image (centercenter) en prenant la
    // moitié de la largeur de l'image pour
    // xx (colonnes) et la moitié de la hauteur de l'image pour yy (lignes).
    Point center = Point(img.cols / 2, img.rows / 2);
    double angle;
    double scale = 0.6;
    cout << endl
         << "Angle de rotation (-180 à 180)";
    cin >> angle;
    Mat rotationMatrix = getRotationMatrix2D(center, angle, scale);
    warpAffine(img, new_image, rotationMatrix, img.size());
    break;
  }

  case 3:
  {
    Mat reflectionMatrix = (Mat_<float>(2, 3) << -1, 0, image.cols, 0, 1, 0);
    warpAffine(this->image, new_image, reflectionMatrix, image.size());
    break;
  }
  default:
    return image;
    break;
  }
  return new_image;
}
Mat ImageProcessor:: FloydSteimberg()
{
    Mat new_image = this->image.clone();
    cvtColor(new_image, new_image, COLOR_RGBA2GRAY, 0);
    int x(0), k(0), ii(0), erreur(0);
    for (int i = 0; i < new_image.cols; i++)
    {
        for (int j = 1; j < new_image.rows; j++)
        {
            x = new_image.data[ii];
            if (x < 128)
                k = 0;
            else
                k = 255;
            new_image.data[ii] = k;
            erreur = x - k;
            new_image.data[ii + 1] = min(255., max(0., new_image.data[ii + 1] + erreur * 7. / 16.));
            new_image.data[ii + new_image.cols - 1] = min(255., max(0., new_image.data[ii + new_image.cols - 1] + erreur * 3. / 16.));
            new_image.data[ii + new_image.cols] = min(255., max(0., new_image.data[ii + new_image.cols] + erreur * 5. / 16.));
            new_image.data[ii + new_image.cols + 1] = min(255., max(0., new_image.data[ii + new_image.cols + 1] + erreur * 1. / 16.));
             ii++;
        }
    }
    return new_image;
}
void ImageProcessor ::  menu(){
  int choice = 1;
  string path;
  cout << "Entrer le chemin de l'image que vous vous voulez modifié : ";
  cin >> path;
  load_image(this->image,path); 
  do
  {
    cout <<"\n\t\t\t+++++++++++++++++++++++++++++++TRANSFORMATION D'IMAGE++++++++++++++++++++++++++++"<<endl;
    cout <<"\t\t\t _______________________________________________________________________________"<<endl;
    cout <<"\t\t\t|                  (1) Conversion de couleur                                      |"<<endl;
    cout <<"\t\t\t|                  (2) Redimensionnement de l'image                               |"<<endl;
    cout <<"\t\t\t|                  (3) Flou gaussien                                              |"<<endl;
    cout <<"\t\t\t|                  (4) Détection de contours                                      |"<<endl;
    cout <<"\t\t\t|                  (5) Transformation affine                                      |"<<endl;
    cout <<"\t\t\t|                  (6) inversion de l'image                                       |"<<endl;
    cout <<"\t\t\t|                  (7) Seuillage binaire                                          |"<<endl;
    cout <<"\t\t\t|                  (8) Morphologie                                                |"<<endl;
    cout <<"\t\t\t|                  (9) Normalisation de l'histograme                              |"<<endl;
    cout <<"\t\t\t|                  (10) Filtre de Sobel                                           |"<<endl;
    cout <<"\t\t\t|                  (11) Remplacement de couleur                                   |"<<endl;
    cout <<"\t\t\t|                  (12) Projection perspectve                                     |"<<endl;
    cout <<"\t\t\t|                  (13) Détecteur de coins de Harris                              |"<<endl;
    cout <<"\t\t\t|                  (14)Désaturation avec conservation d'une couleur spécifique    |"<<endl;
    cout <<"\t\t\t|                  (15) Filtre médian                                             |"<<endl;
    cout <<"\t\t\t|                  (16) Fusion d'images                                           |"<<endl;
    cout <<"\t\t\t|                  (17) FloydSteimberg                                            |"<<endl;
    cout <<"\t\t\t|                  (18) bilaterFilters                                            |"<<endl;
    // cout <<"\t\t\t|                  (19) Filter2D                                                  |"<<endl;
    cout <<"\t\t\t|                  (19) quitter le programme                                      |"<<endl;
    cout <<" \t\t\t--------------------------------------------------------------------------------"<<endl;
    do
    {
      cout <<"\t\t\t\t\t\tEntrer votre choix: ";
      cin >> choice;
    } while (choice < 0);
    if(choice > this->CHOICE){
      Fin_prog();
    }
    switch (choice)
    {
    case 1:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(conversion_gray(), "Image modifié");
      destruct(2500);
      break;
    }
    case 2:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(resize_image(), "Image modifié");
      destruct(2500);
      break;
    }
    case 3:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(flou_gaussien(), "Image modifié");
      destruct(2500);
      break;
    }
    case 4:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(detection_contours(), "Image modifié");
      destruct(2500);
      break;
    }
    case 5:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(translation(), "Image modifié");
      destruct(2500);
      break;
    }
    case 6:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(inversion(), "Image modifié");
      destruct(2500);
      break;
    }
    case 7:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(threshold(), "Image modifié");
      destruct(2500);
      break;
    }
    case 8:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(morphologie(), "Image modifié");
      destruct(2500);
      break;
    }
    case 9:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(histogramme(), "Image modifié");
      destruct(2500);
      break;
    }
    case 10:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(filtre_sobel(), "Image modifié");
      destruct(2500);
      break;
    }
    case 11:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(remplacement_couleur(), "Image modifié");
      destruct(2500);
      break;
    }
    case 12:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(perspective(), "Image modifié");
      destruct(2500);
      break;
    }
    case 13:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(coin_harris(), "Image modifié");
      destruct(2500);
      break;
    }
    case 14:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(desaturation(), "Image modifié");
      destruct(2500);
      break;
    }
    case 15:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(medianBlurs(), "Image modifié");
      destruct(2500);
      break;
    }
    case 16:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(fusion(), "Image modifié");
      destruct(2500);
      break;
    }
    case 17:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(this->FloydSteimberg(), "Image modifié");
      destruct(2500);
      break;
    }
    case 18:
    {
      system(CLEAN);
      this->open_image(this->image, "Image original");
      this->open_image(this->bilaterFilters(), "Image modifié");
      destruct(2500);
      break;
    }
    // case 19:
    // {
    //   system(CLEAN);
    //   this->open_image(this->image, "Image original");
    //   this->open_image(this->filters2D(), "Image modifié");
    //   destruct(2500);
    //   break;
    // }
    default:
      Fin_prog();
      break;
    }
    destroyAllWindows();
  } while (choice != CHOICE);
}

void destruct(int delai)
{
  // Attendre 5000 millisecondes (5 secondes)
  int key = waitKey(delai);

  // Vérifier si une touche a été enfoncée ou si le temps est écoulé
  // if (key == -1)
  // {
  //   cout << "Temps écoulé. La fenêtre va se fermer." << endl;
  // }
  // else
  // {
  //   cout << "Touche enfoncée. La fenêtre va se fermer." << endl;
  // }

  destroyAllWindows();
}
void Fin_prog()
{
  system(CLEAN);
  cout <<"\n+++++++FIN DU PROGRAMME++++"<<endl;
  exit(-1);
}