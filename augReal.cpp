#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

const cv::String keys =
    "{help h usage ? |      | print this message   }"
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
        cv::Size plantillaSize(rows, cols);
        cv::Size plantillaSize1(-1, -1);
        std::vector<cv::Point3d> centros;

        if (!parser.check()){
            parser.printErrors();
            return 0;
        }

        if (!captura.isOpened()){
		    std::cout<<"Failed to open file: "<<parser.get<cv::String>(1)<<std::endl;
		    return -1;
    	}

    	std::cout<<"Hola 1\n";

    	cv::Mat cameraMatrix;
    	cv::Mat distCoeffs;
    	std::vector<cv::Point2d>image_points;
    	std::vector<cv::Point3d>modal_view;
    	/*for (int i = 0; i < cols; i++) {
       		for(int j = 0; j < rows; j++){
          		modal_view.push_back(cv::Point3f(j*tam,i*tam, 0.0f));
        	}
      	}*/
    	std::cout<<"Hola 2\n";
    	fs["camera_matrix"] >> cameraMatrix;
    	std::cout<<"Hola 3\n";
    	fs["distortion_coefficients"] >> distCoeffs;
    	std::cout<<"Hola 4\n";

    	fs["image_point"]>>image_points;
    	std::cout<<"Hola 5\n";

    	cv::Mat frame;
    	cv::Mat rotation_vector; // Rotation in axis-angle form
    	cv::Mat translation_vector;
    	for(;;){

        	if (!captura.read(frame))             
            	break;
            bool plantilla = cv::findChessboardCorners(frame, plantillaSize, centros);
            //cv::drawChessboardCorners(frame, plantillaSize,cv::Mat(centros),plantilla);
        	//cv::cornerSubPix(frame, centros, plantillaSize, plantillaSize1, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 40, 0.001));
            cv::solvePnP(centros, image_points, cameraMatrix, distCoeffs, rotation_vector, translation_vector);
            //cv::projectPoints(front_object_pts, rvec_front, tvec_front, 
             //cameraMatrix_Front, distCoeffs_Front, check_front_image_pts);
        	cv::imshow("window", frame);

        	char key = cvWaitKey(10);
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