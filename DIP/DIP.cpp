// DIP.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>

using namespace std;

//================== 1 ==================
int tutorial() {
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

cv::Mat tmp(cv::Mat img) {

		cout << ""; for (int k = 17; k < img.rows; k++) { cout << " "; } cout << "v" << endl;

		for (int k = 0; k < img.rows; k++) {
			for (int l = 0; l < img.cols; l++) {



			}
			cout << "=";
		}
		return img;
	}


//================== 2 ==================
cv::Mat gamma() { 

	cout << "Gamma ";
	cv::Mat src_img= cv::imread("images/lena.png", cv::IMREAD_COLOR);
	if (src_img.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	//vytvořím si prázdný obrázek
	cv::Mat output_img(src_img.rows, src_img.cols, CV_32FC3); // gamma_32fc3_img

	src_img.convertTo(output_img, CV_32FC3, 1.0 / 255.0); 
	double gamma = 0.1;

	for (int x = 0; x < output_img.cols; x++) {
		for (int y = 0; y < output_img.rows; y++) {
			output_img.at<cv::Vec3f>(y, x)[0] = pow(output_img.at<cv::Vec3f>(y, x)[0], gamma);
			output_img.at<cv::Vec3f>(y, x)[1] = pow(output_img.at<cv::Vec3f>(y, x)[1], gamma);
			output_img.at<cv::Vec3f>(y, x)[2] = pow(output_img.at<cv::Vec3f>(y, x)[2], gamma);
		}
	}

	cout << "- DONE" << endl;
	return output_img;
}

//================== 3 ==================
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

cv::Mat convolution() { // konvoluce 

	cout << "Convolution ";
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

	cout << "- DONE" << endl;
	return conv_gauss_8uc3_img;
}

//================== 4 ==================
double g(double deltaI, double sigma) {
	double exponent = (deltaI*deltaI) / (sigma*sigma);
	return exp(-exponent);
}

double c(double center, double neighbour, double sigma){ // Conductances 
	double deltaI = abs(neighbour - center);
	return g(deltaI, sigma); // abs(neig-center) = delta I
}

cv::Mat anisotropic_diffusion(int iterations) { // anisotropic filtering
	double sigma = 0.015;
	double lambda = 0.1;
	int iter = iterations;

	cout << "Anisotropic diffusion - " << iter << "x" << endl;
	for (int i = 0; i < iter; i++) {
		cout << " ";
	}
	cout << "v" << endl;
	cv::Mat src_8fc1_img = cv::imread("images/valve.png", CV_LOAD_IMAGE_GRAYSCALE);

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

				Cn = c(I, In, sigma); // north
				Cw = c(I, Iw , sigma); // west
				Ce = c(I, Ie, sigma); // east
				Cs = c(I, Is, sigma); // south

				output.at<float>(y, x) = I * (1 - lambda * (Cn + Cs + Ce + Cw)) + lambda * (Cn * In + Cs * Is + Ce * Ie + Cw * Iw);
			}
		}
		output.copyTo(src);
		cout << "=";
	}

	cout << " - DONE";
	return output;
}

//================== 5 ==================
// Discrete Fourier Transform - http://mrl.cs.vsb.cz//people/gaura/dzo/dft_en.pdf
// The Fourier Transform computes frequency spectrum of the given input image f

const int REAL = 0;
const int IMAG = 1;

std::complex<float> fourier(int k, int l, cv::Mat src) {
	std::complex<float> out(0,0);
	int M = src.rows;
	int N = src.cols;
	float odmocnina = float(1 / sqrt(M * N));

	for (int m = 0; m < M; m++) {
		for (int n = 0; n < N; n++) {
			float slozka = float(-CV_PI * 2 * float(float(float(m*k)/float(M)) + float(float(n*l)/float(N))));
			complex<float> fi(cos(slozka), sin(slozka));
			complex<float> f(src.at<float>(m, n), 0); // hodnota pixelu na pozici m n, a 0 jako imaginarni slozka
			out += fi*f;
		}
	}
	out *= odmocnina;
	return out;
}

cv::Mat myDft(cv::Mat original_img) {
	//cv::Mat original_img = cv::imread("images/earth.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	cv::Mat src(original_img.rows, original_img.cols, CV_32FC1); 
	cv::Mat realna_slozka(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat imaginarni_slozka(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat fazovy_posuv(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat amplituda(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat spektrum(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat slozky(original_img.rows, original_img.cols, CV_32FC2);
	original_img.convertTo(src, CV_32FC1, 1.0 / 255.0);
	cout << "DFT    ";for (int k = 8; k < src.rows; k++){ cout << " "; } cout << "v" << endl;

	for( int k = 0; k < src.rows; k++ ){
		for ( int l = 0; l < src.cols; l++ ){
			std::complex<float> frekvenci_spektrum = fourier(k, l, src);

			realna_slozka = real(frekvenci_spektrum); // realna slozka
			imaginarni_slozka = imag(frekvenci_spektrum); // imaginarni slozka
			float imag_kl = imaginarni_slozka.at<float>(k, l);
			float real_kl = realna_slozka.at<float>(k, l);
			slozky.at<cv::Vec2f>(k, l)[0] = imag_kl;
			slozky.at<cv::Vec2f>(k, l)[1] = real_kl;

			amplituda.at<float>(k, l) = sqrt( (real_kl * real_kl) + (imag_kl * imag_kl) );
			fazovy_posuv.at<float>(k, l) = atan(imag_kl / real_kl); //arctg( I(k,l)/R(k,l) )
			spektrum.at<float>(k, l) = amplituda.at<float>(k, l) * amplituda.at<float>(k, l);
		} cout << "="; 
	}

	cv::Size size(300, 300);
	cv::Mat amplituda_big, spektrum_big, fazovy_posuv_big, original_big;
	cv::resize(amplituda, amplituda_big, size, 0, 0, cv::INTER_AREA);
	cv::resize(spektrum, spektrum_big, size, 0, 0, cv::INTER_AREA);
	cv::resize(fazovy_posuv, fazovy_posuv_big, size, 0, 0, cv::INTER_AREA);
	cv::resize(original_img, original_big, size, 0, 0, cv::INTER_AREA);

	cv::imshow("Original", original_big);
	//cv::imshow("Amp", amplituda_big);
	cv::imshow("Phase", fazovy_posuv_big);
	cv::imshow("Power", spektrum_big);
	return slozky;
}

//================== 6 ==================
// In the previous lesson, we’ve implemented the Discrete Fourier Transform.
// Today, we’ll implement its inverse called the Inverse Discrete Fourier Transform(IDFT)
//  http://mrl.cs.vsb.cz//people/gaura/dzo/idft_en.pdf
/*cv::Mat deFourier(int k, int l, cv::Mat src){
	return src.at<cv::Vec2f>(k, l);
}

cv::Mat myIdft(cv::Mat src) {
	cout << endl << "IDFT    "; for(int k = 8 ; k<src.rows ; k++){ cout<<" "; } cout<<"v"<<endl;

	int M = src.rows;
	int N = src.cols;
	for (int k = 0; k < src.rows; k++){
		for (int l = 0; l < src.cols; l++) {

			//std::complex<float> frekvenci_spektrum = deFourier(k, l, dft_image);


		}
		cout << "=";
	}
	return src;
}*/


//================ MAIN =================
int main()
{
// 1 - 8.9.2020
	cv::Mat lena = cv::imread("images/lena.png", cv::IMREAD_COLOR);
	cv::Mat lena64 = cv::imread("images/lena64.png", cv::IMREAD_COLOR);
	cv::Mat earth = cv::imread("images/earth.png", cv::IMREAD_COLOR);
	cv::Mat valve_bw = cv::imread("images/valve.png", cv::IMREAD_GRAYSCALE);

// 2 - 15.09.2020
	
	//cv::Mat lena_gamma = gamma(); // aplikace gamma filteru
	//cv::imshow("Gamma", lena_gamma);

// 3 - 22.09.2020
	
	//cv::imshow("Convolution", convolution());
	//cv::imshow("Original", lena);
	

// 4 - 29.09.2020
	//Filtering using anisotropic diffusion http://mrl.cs.vsb.cz//people/gaura/dzo/anisotropic.pdf

	//cv::imshow("Original Valve", valve_bw);
	//cv::imshow("Anisotropic Filtering",anisotropic_diffusion(100)); // iterace obrázku anisotropickým filtrem
	

// 5 - 6.10.2020
	// DFT http://mrl.cs.vsb.cz//people/gaura/dzo/dft.pdf

	cv::Mat dft = myDft(earth);
	//cv::imshow("Phase", dft);

// 6 - 13.10.2020
	// IDFT

	//cv::Mat idft = myIdft(dft);
	//cv::imshow("after IDFT", idft);

//
	cv::waitKey(0); // wait until keypressed
	return 0;
}