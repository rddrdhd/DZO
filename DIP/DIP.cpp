// DIP.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>

using namespace std;


int cv1() {
	cv::Mat src_8uc3_img = cv::imread("images/lena.png", CV_LOAD_IMAGE_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

	if (src_8uc3_img.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	//cv::imshow( "LENA", src_8uc3_img );

	cv::Mat gray_8uc1_img; // declare variable to hold grayscale version of img variable, gray levels wil be represented using 8 bits (uchar)
	cv::Mat gray_32fc1_img; // declare variable to hold grayscale version of img variable, gray levels wil be represented using 32 bits (float)

	cv::cvtColor(src_8uc3_img, gray_8uc1_img, CV_BGR2GRAY); // convert input color image to grayscale one, CV_BGR2GRAY specifies direction of conversion
	gray_8uc1_img.convertTo(gray_32fc1_img, CV_32FC1, 1.0 / 255.0); // convert grayscale image from 8 bits to 32 bits, resulting values will be in the interval 0.0 - 1.0

	int x = 10, y = 15; // pixel coordinates

	uchar p1 = gray_8uc1_img.at<uchar>(y, x); // read grayscale value of a pixel, image represented using 8 bits
	float p2 = gray_32fc1_img.at<float>(y, x); // read grayscale value of a pixel, image represented using 32 bits
	cv::Vec3b p3 = src_8uc3_img.at<cv::Vec3b>(y, x); // read color value of a pixel, image represented using 8 bits per color channel

	// print values of pixels
	printf("p1 = %d\n", p1);
	printf("p2 = %f\n", p2);
	printf("p3[ 0 ] = %d, p3[ 1 ] = %d, p3[ 2 ] = %d\n", p3[0], p3[1], p3[2]);

	gray_8uc1_img.at<uchar>(y, x) = 0; // set pixel value to 0 (black)

	// draw a rectangle
	cv::rectangle(gray_8uc1_img, cv::Point(65, 84), cv::Point(75, 94),
		cv::Scalar(50), CV_FILLED);

	// declare variable to hold gradient image with dimensions: width= 256 pixels, height= 50 pixels.
	// Gray levels wil be represented using 8 bits (uchar)
	cv::Mat gradient_8uc1_img(50, 256, CV_8UC1);

	// For every pixel in image, assign a brightness value according to the x coordinate.
	// This wil create a horizontal gradient.
	for (int y = 0; y < gradient_8uc1_img.rows; y++) {
		for (int x = 0; x < gradient_8uc1_img.cols; x++) {
			gradient_8uc1_img.at<uchar>(y, x) = x;
		}
	}

	// diplay images
	cv::imshow("Gradient", gradient_8uc1_img);
	cv::imshow("Lena gray", gray_8uc1_img);
	cv::imshow("Lena gray 32f", gray_32fc1_img);

	cv::waitKey(0); // wait until keypressed
	return 0;
}

double user_enter_gamma_float() {
	bool rightValue = false;
	double value = 0.1;
	do {
		cout << "Enter float value between 0.0 and 1.0 : ";
		cin >> value;
		if (value < 1.0 && value > 0.0) {
			rightValue = true;
		}
	} while (!rightValue);

	return value;
}

cv::Mat gamma() { 

	cv::Mat src_img= cv::imread("images/lena.png", cv::IMREAD_COLOR);
	cv::imshow("src lena", src_img);
	if (src_img.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	//vytvořím si prázdný obrázek
	cv::Mat output_img(src_img.rows, src_img.cols, CV_32FC3); // gamma_32fc3_img

	src_img.convertTo(output_img, CV_32FC3, 1.0 / 255.0); 
	double gamma = 0.1; //TODO input
	gamma = user_enter_gamma_float();

	for (int x = 0; x < output_img.cols; x++) {
		for (int y = 0; y < output_img.rows; y++) {
			output_img.at<cv::Vec3f>(y, x)[0] = pow(output_img.at<cv::Vec3f>(y, x)[0], gamma);
			output_img.at<cv::Vec3f>(y, x)[1] = pow(output_img.at<cv::Vec3f>(y, x)[1], gamma);
			output_img.at<cv::Vec3f>(y, x)[2] = pow(output_img.at<cv::Vec3f>(y, x)[2], gamma);
		}
	}

	return output_img;
}

int boxBlur(cv::Mat original, int x, int y, int color) {
	int sum = original.at<cv::Vec3b>(x, y)[color] +
		original.at<cv::Vec3b>(x, y + 1)[color] +
		original.at<cv::Vec3b>(x, y + 2)[color] +
		original.at<cv::Vec3b>(x + 1, y)[color] +
		original.at<cv::Vec3b>(x + 1, y + 1)[color] +
		original.at<cv::Vec3b>(x + 1, y + 2)[color] +
		original.at<cv::Vec3b>(x + 2, y)[color] +
		original.at<cv::Vec3b>(x + 2, y + 1)[color] +
		original.at<cv::Vec3b>(x + 2, y + 2)[color];
	//cout << "X: " << x << ", Y: "<< y << ", SUM: " <<sum << ";\n";
	return sum / 9;

}

int gaussBlur(cv::Mat original, int x, int y, int color) {
	/* 1 4  6  4  1
	 * 4 16 24 16 4
	 * 6 24 36 24 6
	 * 4 16 24 16 4
	 * 1 4  6  4  1  */
	int matrix[] = { 1,4,6,4,1,4,16,24,16,4,6,24,36,24,6,4,16,24,16,4,1,4,6,4,1 };
	int sum = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int matrixValue = matrix[j * 5 + i];
			sum += matrixValue * original.at<cv::Vec3b>(x + i, y + j)[color];
		}
	}
	return sum / 256;
	
}

cv::Mat makeBlur(cv::Mat original, cv::Mat matrix) {
	cv::Mat output(original.rows - matrix.rows, original.cols - matrix.cols, CV_8UC3);

	for (int x = 0; x < output.rows; x++) {
		for (int y = 0; y < output.cols; y++) {
			output.at<cv::Vec3b>(x, y)[0] = gaussBlur(original, x, y, 0);
			output.at<cv::Vec3b>(x, y)[1] = gaussBlur(original, x, y, 1);
			output.at<cv::Vec3b>(x, y)[2] = gaussBlur(original, x, y, 2);
		}
	}

	return output;
}
cv::Mat cv3() { // konvoluce 
	 
	cv::Mat src_8uc3_img = cv::imread("images/lena.png", cv::IMREAD_COLOR);// load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
	cv::Mat conv_box_8uc3_img(src_8uc3_img.rows - 2, src_8uc3_img.cols - 2, CV_8UC3); // 8UC3 - 3 třicetidvoubitové kanály unsigned float
	cv::Mat conv_gauss_8uc3_img(src_8uc3_img.rows - 5, src_8uc3_img.cols - 5, CV_8UC3); // 8UC3 - 3 třicetidvoubitové kanály unsigned float

	if (src_8uc3_img.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	for (int x = 0; x < conv_box_8uc3_img.cols; x++) {
		for (int y = 0; y < conv_box_8uc3_img.rows; y++) {
			// BOX metoda 3x3 - použití pro každý pixel
			conv_box_8uc3_img.at<cv::Vec3b>(x, y)[0] = boxBlur(src_8uc3_img, x, y, 0);
			conv_box_8uc3_img.at<cv::Vec3b>(x, y)[1] = boxBlur(src_8uc3_img, x, y, 1);
			conv_box_8uc3_img.at<cv::Vec3b>(x, y)[2] = boxBlur(src_8uc3_img, x, y, 2);
		}
	}

	for (int x = 0; x < conv_gauss_8uc3_img.cols; x++) {
		for (int y = 0; y < conv_gauss_8uc3_img.rows; y++) {
			// GAUSS metoda 5x5 - použití pro každý pixel
			conv_gauss_8uc3_img.at<cv::Vec3b>(x, y)[0] = gaussBlur(src_8uc3_img, x, y, 0);
			conv_gauss_8uc3_img.at<cv::Vec3b>(x, y)[1] = gaussBlur(src_8uc3_img, x, y, 1);
			conv_gauss_8uc3_img.at<cv::Vec3b>(x, y)[2] = gaussBlur(src_8uc3_img, x, y, 2);
		}
	}

	//cv::imshow("Lena BOX 3x3 convolution", conv_box_8uc3_img);
	//cv::imshow("Lena GAUSS 5x5 convolution", conv_gauss_8uc3_img);
	//cv::imshow("Lena src", src_8uc3_img);

	return conv_gauss_8uc3_img;
}

double cv4_g(double deltaI, double sigma) {
	double exponent = (deltaI*deltaI) / (sigma*sigma);
	return exp(-exponent);
}

double cv4_c(double center, double neighbour, double sigma){ // Conductances 
	double deltaI = abs(neighbour - center);
	return cv4_g(deltaI, sigma); // abs(neig-center) = delta I
}

// cv 4
cv::Mat anisotropic_diffusion(cv::Mat valve, int iterations) { // anisotropic filtering
	double sigma = 0.015;
	double lambda = 0.1;
	int iter = iterations;

	cout << "Anisotropic diffusion - " << iter << "x" << endl;
	for (int i = 0; i < iter; i++) {
		cout << " ";
	}
	cout << "v" << endl;
	cv::Mat src_8fc1_img = valve;

	if (src_8fc1_img.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	cv::Mat src; 
	cv::Mat output(src_8fc1_img.rows, src_8fc1_img.cols, CV_32FC1); 

	src_8fc1_img.convertTo(src, CV_32FC1, 1.0 / 255.0);


	int width = src.cols;
	int height = src.rows;

	double I; // hodnota pixelu ve středu
	double In, Iw, Ie, Is; //hodnoty sousedů

	double Cn, Cw, Ce, Cs; //vypočítané hodnoty

	for(int i = 0; i < iter; i++){
		for (int y = 0; y < height; y++) {
			int y_minus = y - 1;
			int y_plus = y + 1;
			if (y_minus < 0)
				y_minus = 0;
			if (y_plus >= height)
				y_plus = height - 1;
			for (int x = 0; x < width; x++) {

				int x_minus = x - 1; 
				int x_plus = x + 1;

				// ošetření krajů
				if (x_minus < 0)
					x_minus = 0;
				if (x_plus >= width)
					x_plus = width - 1;

				// hodnoty jednotlivých pixelů
				I = src.at<float>(y, x);
				Is = src.at<float>(y_plus, x); 
				In = src.at<float>(y_minus, x);
				Iw = src.at<float>(y, x_minus);
				Ie = src.at<float>(y, x_plus);

				Cn = cv4_c(I, In, sigma); // north
				Cw = cv4_c(I, Iw , sigma); // west
				Ce = cv4_c(I, Ie, sigma); // east
				Cs = cv4_c(I, Is, sigma); // south

				output.at<float>(y, x) = I * (1 - lambda * (Cn + Cs + Ce + Cw)) + lambda * (Cn * In + Cs * Is + Ce * Ie + Cw * Iw);
			}
		}
		output.copyTo(src);
		cout << "=";
	}

	cout << " 100% done";
	return output;
}

int main()
{
	// CV2 - gamma
	
	cv::Mat lena_gamma = gamma(); // aplikace gamma filteru
	cv::imshow("gamma", lena_gamma);
	
	//cv::Mat lena_convolution = cv3(); // aplikace gamma filteru
	//cv::imshow("gamma", lena_convolution);
	


	//CV 4 - 29.09.2020
	//Filtering using anisotropic diffusion http://mrl.cs.vsb.cz//people/gaura/dzo/anisotropic.pdf
	
	/* 
	cv::Mat valve_gray = cv::imread("images/valve.png", CV_LOAD_IMAGE_GRAYSCALE);  // input mat
	cv::Mat anisotropic_filtered_image = anisotropic_diffusion(valve_gray, 20); // iterace obrázku anisotropickým filtrem
	cv::imshow("src valve", valve_gray);
	cv::imshow("anisotropic filtering", anisotropic_filtered_image);
	*/
	cv::waitKey(0); // wait until keypressed
	
	//return cv1();
	return 0;
}
