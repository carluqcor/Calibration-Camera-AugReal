#ifndef FUNCIONESAUXILIARES_HPP_
#define FUNCIONESAUXILIARES_HPP_
#include <iostream>
#include <exception>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


void crearPuntos3D(std::vector<cv::Point3f> &puntos3d);
void dibujarEjes(cv::Mat frame, std::vector<cv::Point2f> puntosProyectados);
void rellenarVistasObjeto(std::vector<cv::Point3f> &objectViews, int tam, int rows, int cols);
void ficherosAvalores(cv::FileStorage fs, cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
void augRealLine(cv::VideoCapture captura, cv::Mat cameraMatrix, cv::Mat distCoeffs, cv::Mat rotation_vector, cv::Mat translation_vector, std::vector<cv::Point2f> image_points, std::vector<cv::Point3f> objectViews, int tam, cv::Size plantillaSize, cv::Size plantillaSize1, std::vector<cv::Point2f> centros, int rows, int cols);
bool digito(std::string nombre);
#endif