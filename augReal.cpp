#include <iostream>
#include <exception>
#include <string>
#include <ctime>
#include <cstdio>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "funcionesAuxiliares.hpp"
#include "macros.hpp"

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
        
        if(!fs.isOpened()){
        	std::cout<<BIRED<<"The output "<<BIYELLOW<<parser.get<cv::String>(0)<<BIRED<<" from calibration is not opened!!"<<RESET<<std::endl;
        	exit(-1);
        }

        cv::VideoCapture captura;

        if(digito(parser.get<cv::String>(1))){
            captura.open(0);
        }else
            captura.open(parser.get<cv::String>(1));

        cv::Size plantillaSize(5, 5);
        cv::Size plantillaSize1(-1, -1);
        std::vector<cv::Point2f> centros;

        if (!parser.check()){
            parser.printErrors();
            return 0;
        }

        if (!captura.isOpened()){
		    std::cout<<BIRED<<"Failed to open file: "<<BIYELLOW<<parser.get<cv::String>(1)<<RESET<<std::endl;
		    return -1;
    	}

    	cv::Mat cameraMatrix;
    	cv::Mat distCoeffs;
    	cv::Mat rotation_vector; 
    	cv::Mat translation_vector;
    	std::vector<cv::Point2f>image_points;
    	std::vector<cv::Point3f>objectViews;

    	rellenarVistasObjeto(objectViews, tam, rows, cols);   	

    	ficherosAvalores(fs, cameraMatrix, distCoeffs);
    	
    	augRealLine(captura, cameraMatrix, distCoeffs, rotation_vector, translation_vector, image_points, objectViews, tam, plantillaSize, plantillaSize1, centros, rows, cols);

    	fs.release();

    }catch (std::exception& e){
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  	}
  	return retCode;
}