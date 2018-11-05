#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

const cv::String keys =
    "{help h usage ?    |      | print this message   }"
    "{row               |  0.0 | filas             }"
    "{col               |  0.0 | columnas                }"
    "{size              |  0.0 | size    }"
    "{@file             |      | fichero de calibracion        }"
    "{@video            |      | video        }"
    ;

int main (int argc, char* const* argv){
  int retCode=EXIT_SUCCESS;
  
    try{    
        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Application name v1.0.0");
        if (parser.has("help")){
            parser.printMessage();
            return 0;
        }
        
        float rows = parser.get<float>("row");
        float cols = parser.get<float>("col");
        float tam = parser.get<float>("size");
        cv::FileStorage fs(parser.get<cv::String>(0), cv::FileStorage::READ);
        cv::VideoCapture captura(parser.get<cv::String>(1));
        cv::Size plantillaSize(5, 5);
        cv::Size plantillaSize1(-1, -1);
        std::vector<cv::Point2f> centros;

        if (!parser.check()){
            parser.printErrors();
            return 0;
        }

        if (!captura.isOpened()){
		    std::cout<<"Failed to open file: "<<parser.get<cv::String>(1)<<std::endl;
		    return -1;
    	}

    	cv::Mat cameraMatrix;
    	cv::Mat distCoeffs;
    	std::vector<cv::Point2f>image_points;
    	std::vector<cv::Point3f>modal_view;
    	for (int i = 0; i < rows; i++) {
       		for(int j = 0; j < cols; j++){
          		modal_view.push_back(cv::Point3f(j*tam,i*tam, 0));
        	}
      	}
    	fs["camera_matrix"] >> cameraMatrix;
    	fs["distortion_coefficients"] >> distCoeffs;

    	cv::Mat rotation_vector; 
    	cv::Mat translation_vector;
    	for(;;){
    		cv::Mat frame;
    		captura >> frame;
    		cv::Mat frameAux;
    		cv::cvtColor(frame, frameAux, CV_BGR2GRAY);
        	if (!captura.read(frame))             
            	break;
            bool plantilla = cv::findChessboardCorners(frame, cv::Size(cols,rows), centros, cv::CALIB_CB_FAST_CHECK);
            if(plantilla==1){
	        	cv::cornerSubPix(frameAux, centros, plantillaSize, plantillaSize1, cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03));
	            cv::solvePnP(modal_view, centros, cameraMatrix, distCoeffs, rotation_vector, translation_vector);
	            std::vector<cv::Point2f> puntosProyectados;
	            std::vector<cv::Point3f> puntos3d;
	            puntos3d.push_back(cv::Point3f(0, 0, 0));
	            puntos3d.push_back(cv::Point3f(tam, 0, 0));
	            puntos3d.push_back(cv::Point3f(0, tam, 0));
	            puntos3d.push_back(cv::Point3f(0, 0, -tam));
	            cv::projectPoints(puntos3d, rotation_vector, translation_vector, cameraMatrix, distCoeffs, puntosProyectados);
				cv::line(frame, puntosProyectados[0], puntosProyectados[1], cv::Scalar(0,0,255), 6);
				cv::line(frame, puntosProyectados[0], puntosProyectados[2], cv::Scalar(0,255,0), 6);
				cv::line(frame, puntosProyectados[0], puntosProyectados[3], cv::Scalar(255,0,0), 6);   
	        }    
        	cv::imshow("window", frame);

        	char key = cvWaitKey(20); //Tiempo que espera entre frame y frame
        	if (key == 27) // ESC
            	break;
    	}
    	fs.release();
    }catch (std::exception& e){
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  	}
  	return retCode;
}