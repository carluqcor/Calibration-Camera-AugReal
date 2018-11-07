#include <iostream>
#include <exception>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


void crearPuntos3D(std::vector<cv::Point3f> &puntos3d, int tam){
    puntos3d.push_back(cv::Point3f(0, 0, 0)); //Primer punto de la plantilla
    puntos3d.push_back(cv::Point3f(tam, 0, 0)); //Punto hasta donde se dibujará la linea del azul, desde el primer punto
    puntos3d.push_back(cv::Point3f(0, tam, 0)); //Punto hasta donde se dibujará la linea del verde, desde el primer punto
    puntos3d.push_back(cv::Point3f(0, 0, -tam)); //Punto hasta donde se dibujará la linea del rojo, desde el primer punto
}

void dibujarEjes(cv::Mat frame, std::vector<cv::Point2f> puntosProyectados){
	cv::line(frame, puntosProyectados[0], puntosProyectados[1], cv::Scalar(0,0,255), 6); //Se crea la línea en Azul
	cv::line(frame, puntosProyectados[0], puntosProyectados[2], cv::Scalar(0,255,0), 6); //Se crea la línea en Verde
	cv::line(frame, puntosProyectados[0], puntosProyectados[3], cv::Scalar(255,0,0), 6); //Se crea la línea en Rojo
}

void rellenarVistasObjeto(std::vector<cv::Point3f> &objectViews, int tam, int rows, int cols){
	for (int i = 0; i < rows; i++) {
   		for(int j = 0; j < cols; j++){
      		objectViews.push_back(cv::Point3f(j*tam,i*tam, 0)); //Creación de las vistas del objeto según el size introducido por argumentos
    	}
  	}
}

void ficherosAvalores(cv::FileStorage fs, cv::Mat &cameraMatrix, cv::Mat &distCoeffs){
	fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
} 

void augRealLine(cv::VideoCapture captura, cv::Mat cameraMatrix, cv::Mat distCoeffs, cv::Mat rotation_vector, cv::Mat translation_vector, std::vector<cv::Point2f> image_points, std::vector<cv::Point3f> objectViews, int tam, cv::Size plantillaSize, cv::Size plantillaSize1, std::vector<cv::Point2f> centros, int rows, int cols){
	for(;;){
		cv::Mat frame;
		captura >> frame;
		cv::Mat frameAux;
		
		cv::cvtColor(frame, frameAux, CV_BGR2GRAY); //Se necesita tener una auxiliar del frame para poder convertirlo en grayscale para la refinación de las esquinas (cornerSubPix)
    	
    	if (!captura.read(frame))             
        	break;

        //La flag fast_check sirve para agilizar la obtencion de las esquinas cuando no las encuentra
        bool plantilla = cv::findChessboardCorners(frame, cv::Size(cols,rows), centros, cv::CALIB_CB_FAST_CHECK); //Encuentra las esquinas que separan los cuadrados con importancia del diseño

        if(plantilla==1){
        	cv::cornerSubPix(frameAux, centros, plantillaSize, plantillaSize1, cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03)); //Refina las esquinas encontradas por findchessboardcorners
            cv::solvePnP(objectViews, centros, cameraMatrix, distCoeffs, rotation_vector, translation_vector); //Nos devuelve los parámetros extrínsecos (Vector de rotación y de traslación)
			
			std::vector<cv::Point3f> puntos3d;
			std::vector<cv::Point2f> puntosProyectados;
            
            crearPuntos3D(puntos3d, tam); //Crea los puntos3D de la plantilla donde se situará el eje de coordadas y sus puntos máximos según el tamaño
            
            cv::projectPoints(puntos3d, rotation_vector, translation_vector, cameraMatrix, distCoeffs, puntosProyectados); //Convertirá los puntos 3D obtenidos antes en puntos 2D para dibujarlos
            dibujarEjes(frame, puntosProyectados); //Dibuja los puntos 2D creando los ejes de coordenadas 3D
        }    
    	cv::imshow("window", frame);

    	char key = cvWaitKey(20); //Tiempo de espera entre frame y frame
    	if (key == 27) // ESC
        	break;
	}
}

bool digito(std::string nombre){
    if(nombre[0]=='0'){
        return true;   
    }else {
        return false;
    }
}