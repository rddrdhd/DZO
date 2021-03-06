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

cv::Mat convolution(cv::Mat src_8uc3_img) { // konvoluce 

	cout << "Convolution ";
	//src_8uc3_img = cv::imread("images/lena.png", cv::IMREAD_COLOR);// load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
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

cv::Mat make_bigger(cv::Mat src) {
	cv::Mat out;

	cv::Size size(300, 300);

	cv::resize(src, out, size, 0, 0, cv::INTER_AREA);

	return out;
}
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
cv::Mat prehodit_kvadranty(cv::Mat input) {
	//cv::Mat out_fixed(input.rows, input.cols, CV_32FC1);

	cv::Rect roi1(0, 0, input.rows / 2, input.cols / 2);
	cv::Rect roi2(input.rows / 2, 0, input.rows / 2, input.cols / 2);
	cv::Rect roi3(0, input.rows / 2, input.rows / 2, input.cols / 2);
	cv::Rect roi4(input.rows / 2, input.rows / 2, input.rows / 2, input.cols / 2);

	cv::Mat c1(cv::Mat(input, roi1).clone());
	cv::Mat c2(cv::Mat(input, roi2).clone());
	cv::Mat c3(cv::Mat(input, roi3).clone());
	cv::Mat c4(cv::Mat(input, roi4).clone());

	c1.copyTo(input(cv::Rect(input.rows / 2, input.rows / 2, c1.rows, c1.cols)));
	c4.copyTo(input(cv::Rect(0, 0, c3.rows, c3.cols)));
	c3.copyTo(input(cv::Rect(input.rows / 2, 0, c2.rows, c2.cols)));
	c2.copyTo(input(cv::Rect(0, input.rows / 2, c4.rows, c4.cols)));

	return input;

}

cv::Mat fourier_transform(cv::Mat original_img) {
	//cv::Mat original_img = cv::imread("images/earth.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	cv::Mat src(original_img.rows, original_img.cols, CV_32FC1); 
	cv::Mat realna_slozka(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat imaginarni_slozka(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat fazovy_posuv(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat amplituda(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat spektrum(original_img.rows, original_img.cols, CV_32FC1);
	cv::Mat spektrum_normalized(original_img.rows, original_img.cols, CV_32FC1); // logaritmus a hodnoty mezi 0-1
	cv::Mat spektrum_fixed(original_img.rows, original_img.cols, CV_32FC1); // prohozene kvartaly
	cv::Mat frekvencni_domena(original_img.rows, original_img.cols, CV_32FC2); // slozky frekvencniho spektra
	original_img.convertTo(src, CV_32FC1, 1.0 / 255.0);
	cout << endl << "DFT    ";for (int k = 8; k < src.rows; k++){ cout << " "; } cout << "v" << endl;
	float spektrum_min = 0;
	float spektrum_max = 0;
	float spektrum_kl = 0;
	float spektrum_log = 0;
	float spektrum_final_value = 0;
	for( int k = 0; k < src.rows; k++ ){
		for (int l = 0; l < src.cols; l++) {
			std::complex<float> frekvenci_spektrum = fourier(k, l, src);

			realna_slozka = real(frekvenci_spektrum); // realna slozka
			imaginarni_slozka = imag(frekvenci_spektrum); // imaginarni slozka
			float imag_kl = imaginarni_slozka.at<float>(k, l);
			float real_kl = realna_slozka.at<float>(k, l);
			frekvencni_domena.at<cv::Vec2f>(k, l)[0] = imag_kl;
			frekvencni_domena.at<cv::Vec2f>(k, l)[1] = real_kl;

			amplituda.at<float>(k, l) = sqrt((real_kl * real_kl) + (imag_kl * imag_kl));
			fazovy_posuv.at<float>(k, l) = atan(imag_kl / real_kl); //arctg( I(k,l)/R(k,l) )

			spektrum_kl = amplituda.at<float>(k, l) * amplituda.at<float>(k, l);
			
			// najdu si min a max logaritmu, abych mohla spektrum normalizovat
			spektrum.at<float>(k, l) = spektrum_kl;
			spektrum_log = log(spektrum_kl);
			if (l == 0 && k == 0) spektrum_min = spektrum_max = spektrum_log;
			if (spektrum_log > spektrum_max) { spektrum_max = spektrum_log; }
			if (spektrum_log < spektrum_min) { spektrum_min = spektrum_log; }

			spektrum_normalized.at<float>(k, l) = spektrum_log;

		} cout << "="; 
	}

	// normalizuji spektrum, aby jeho logaritmus byl v rozmezi 0-1 a dal se dobre zobrazit
	for( int k = 0; k < src.rows; k++ ){
		for ( int l = 0; l < src.cols; l++ ){
			spektrum_kl = spektrum_normalized.at<float>(k, l);
			spektrum_final_value = (spektrum_kl - spektrum_min) / (spektrum_max - spektrum_min);
			spektrum_normalized.at<float>(k, l) = spektrum_final_value;
		} 
	}

	// vymenim 1. se 3. kvadrantem, a 2. se 4.
	spektrum_fixed = prehodit_kvadranty(spektrum_normalized);

	cv::Size size(300, 300);
	cv::Mat amplituda_big, spektrum_big, fazovy_posuv_big, original_big, spektrum_normalized_big;
	//cv::resize(amplituda, amplituda_big, size, 0, 0, cv::INTER_AREA);
	//cv::resize(spektrum, spektrum_big, size, 0, 0, cv::INTER_AREA);
	cv::resize(spektrum_normalized, spektrum_normalized_big, size, 0, 0, cv::INTER_AREA);
	//cv::resize(fazovy_posuv, fazovy_posuv_big, size, 0, 0, cv::INTER_AREA);
	cv::resize(original_img, original_big, size, 0, 0, cv::INTER_AREA);

	cv::imshow("Original", original_big);
	//cv::imshow("Amp", amplituda_big);
	//cv::imshow("Phase", fazovy_posuv_big);
	cv::imshow("Power", spektrum_normalized_big);
	return frekvencni_domena;
}

//================== 6 ==================
// In the previous lesson, we’ve implemented the Discrete Fourier Transform.
// Today, we’ll implement its inverse called the Inverse Discrete Fourier Transform(IDFT)
//  http://mrl.cs.vsb.cz//people/gaura/dzo/idft_en.pdf

float deFourier(int k, int l, cv::Mat src){
	int M = src.rows;
	int N = src.cols;
	std::complex<float> out(0, 0);
	for (int m = 0; m < M; m++) {
		for (int n = 0; n < N; n++) {
			float angle = 2 * CV_PI * (((float)(m * k) / (float)(M)) + (float)(n * l) / (float)N);
			std::complex<float> phi(cos(angle), sin(angle));
			std::complex<float> F_mn(src.at<cv::Vec2f>(m, n)[1], src.at<cv::Vec2f>(m, n)[0]);
			out += F_mn * phi;
		}
	}
	return out.real();
}

cv::Mat inverse_fourier_transform(cv::Mat src) {
	cout << endl << "IDFT   "; for(int k = 8 ; k<src.rows ; k++){ cout<<" "; } cout<<"v"<<endl;

	cv::Mat out(src.rows, src.cols, CV_32FC1);
	int M = src.rows;
	int N = src.cols;
	float odmocnina = float(1 / sqrt(M * N));

	for (int k = 0;k < M; k++) {
		for (int l = 0; l < N; l++) {
			out.at<float>(k,l) = deFourier(k, l, src) * odmocnina;
		}
		cout << "=";
	}

	cv::Mat out_fixed(src.rows, src.cols, CV_32FC1);

	cv::Size size(300, 300);
	cv::Mat out_big;


	cv::resize(out, out_big, size, 0, 0, cv::INTER_AREA);
	return out;
}

//================== 7 ==================
// dolni propust, horni propust, pasmova propust

const int HORNI_PROPUST = 0;
const int DOLNI_PROPUST = 1;
const int VERTIKALNI_PROPUST = 2;

cv::Mat get_circle_mask(cv::Mat src, int circle_r, int propust) {
	cv::Mat circle_mask(src.rows, src.cols, CV_32FC1);
	int a, b, r;
	a = (int)src.rows / 2;
	b = (int)src.rows / 2;
	r =  circle_r ;
	//cv::Mat circle_mask = cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(src.cols, src.rows));
	for (int k = 0; k < src.rows; k++) {
		for (int l = 0; l < src.cols; l++) {
			bool is_in_circle = ((k - a) * (k - a) + (l - b) * (l - b) <= r * r);
			if (propust == HORNI_PROPUST) {
				circle_mask.at<float>(k, l) = is_in_circle ? 1 : 0; //horní propust
			}
			else if (propust == DOLNI_PROPUST) {
				circle_mask.at<float>(k, l) = is_in_circle ? 0 : 1;
			}
		}
		cout << endl;
	}
	cv::imshow("koechko"+propust, circle_mask);
	return circle_mask;
}
cv::Mat get_vertical_mask(cv::Mat src) {
	cv::Mat mask(src.rows, src.cols, CV_32FC1);
	int pulka = (int)(src.rows / 2);
	int tretina = (int)(src.rows / 3);
	bool is_in_mask;

	for (int k = 0; k < src.rows; k++) {
		for (int l = 0; l < src.cols; l++) {
			is_in_mask = (
				   ((k == pulka && l < tretina) || (k == pulka && l > 2 * tretina)) 
				|| ((k-1 == pulka && l < tretina) || (k-1 == pulka && l > 2 * tretina)) 
				|| ((k+1 == pulka && l < tretina) || (k+1 == pulka && l > 2 * tretina)) ) ;
			mask.at<float>(k, l) = is_in_mask ? 1:0;
		}
	}
	cv::imshow("maska", mask);
	return mask;

}

cv::Mat fourier_filter(cv::Mat src, int filter_type) {
	cv::Mat out(src.rows, src.cols, CV_32FC2);
	cv::Mat frekvencni_domena = prehodit_kvadranty(src);
	cv::Mat maska;

	switch (filter_type) {
	case HORNI_PROPUST:
		maska = get_circle_mask(src, 2, HORNI_PROPUST); break;
	case DOLNI_PROPUST:
		maska = get_circle_mask(src, 20, DOLNI_PROPUST); break;
	case VERTIKALNI_PROPUST:
		maska = get_vertical_mask(src); break;
	}

	for (int k = 0; k < src.rows; k++) {
		for (int l = 0; l < src.cols; l++) {
			out.at<cv::Vec2f>(k, l) = frekvencni_domena.at<cv::Vec2f>(k, l);

			if (maska.at<float>(k, l) == 1) {
				out.at<cv::Vec2f>(k, l) = (0, 0); 
			}
		
		}
	} 
	return prehodit_kvadranty(out);
}

//================== 8 ==================
// Removing geometrical disortion + bilinear interpolation

cv::Mat remove_geometrical_disortion(cv::Mat src, float K1, float K2) {
	cv::Mat out(src.rows, src.cols, src.type());
	float cu, cv, R, x_, y_, r2, fi_r2;
	float xd, yd, tmp1, tmp2, val_r, val_g, val_b;
	int xd_int, yd_int;
	cu = (float) (src.cols / 2);
	cv = (float) (src.rows / 2);
	R = (float) (sqrt(cu * cu + cv * cv));

	for (int i = 0; i < src.cols; i++) { cout << (i%5==0?" ":""); }cout << "v" << endl;

	for (int xn = 0; xn < src.cols; xn++) { // naopak?
		x_ = (float)((xn - cu) / R);
		for (int yn = 0;yn < src.rows;yn++) {
			y_ = (float) ((yn - cv) / R);
			r2 = (float) (x_ * x_ + y_ * y_);
			fi_r2 = (float) (1 + (K1 * r2) + (K2 * r2 * r2));
			xd = (((xn - cu) * (1 / fi_r2)) + cu);
			yd = (((yn - cv) * (1 / fi_r2)) + cv);
			xd_int = (int)xd;
			yd_int = (int)yd;

			 // bilinear interpolation
			int x1, x2, y1, y2;
			x1 = floor(xd);
			x2 = ceil(xd);
			y1 = floor(yd);
			y2 = ceil(yd);

			tmp1 = (((x2 - xd) / (x2 - x1)) * src.at<cv::Vec3b>( y1, x1)[0])
				 + (((xd - x1) / (x2 - x1)) * src.at<cv::Vec3b>( y1, x2)[0]);
			tmp2 = (((x2 - xd) / (x2 - x1)) * src.at<cv::Vec3b>( y2, x1)[0])
				 + (((xd - x1) / (x2 - x1)) * src.at<cv::Vec3b>( y2, x2)[0]);
			val_b = ((y2 - yd) / (y2 - y1)) * tmp1 + ((yd-y1)/(y2-y1)) * tmp2;

			tmp1 = (((x2 - xd) / (x2 - x1)) * src.at<cv::Vec3b>( y1, x1)[1])
				 + (((xd - x1) / (x2 - x1)) * src.at<cv::Vec3b>( y1, x2)[1]);
			tmp2 = (((x2 - xd) / (x2 - x1)) * src.at<cv::Vec3b>( y2, x1)[1])
				 + (((xd - x1) / (x2 - x1)) * src.at<cv::Vec3b>( y2, x2)[1]);
			val_g = ((y2 - yd) / (y2 - y1)) * tmp1 + ((yd-y1)/(y2-y1)) * tmp2;

			tmp1 = (((x2 - xd) / (x2 - x1)) * src.at<cv::Vec3b>( y1, x1)[2])
				 + (((xd - x1) / (x2 - x1)) * src.at<cv::Vec3b>( y1, x2)[2]);
			tmp2 = (((x2 - xd) / (x2 - x1)) * src.at<cv::Vec3b>( y2, x1)[2])
				 + (((xd - x1) / (x2 - x1)) * src.at<cv::Vec3b>( y2, x2)[2]);
			val_r = ((y2 - yd) / (y2 - y1)) * tmp1 + ((yd-y1)/(y2-y1)) * tmp2;

			out.at<cv::Vec3b>(yn, xn) = cv::Vec3b(val_b, val_g, val_r);
			if (out.at<cv::Vec3b>(yn, xn)[0]<1&&out.at<cv::Vec3b>(yn, xn)[1]<1&&out.at<cv::Vec3b>(yn, xn)[2]<1)
				out.at<cv::Vec3b>(yn, xn) = src.at<cv::Vec3b>(yd_int, xd_int);

		} cout << (xn % 5 == 0 ? "=" : "");
	}
	return out;
}

//================== 9 ==================
// Histogram Equalization

void display_histogram(int histogram[], const char* name)
{
	int hist[256];
	for (int i = 0; i < 256; i++)
	{
		hist[i] = histogram[i];
	}
	// draw the histograms
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = hist[0];
	for (int i = 1; i < 256; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows

	for (int i = 0; i < 256; i++) {
		hist[i] = ((double)hist[i] / max) * histImage.rows;
	}


	// draw the intensity line for histogram
	for (int i = 0; i < 256; i++)
	{
		line(histImage, cv::Point(bin_w * (i), hist_h),
			cv::Point(bin_w * (i), hist_h - hist[i]),
			cv::Scalar(0, 0, 0), 1, 8, 0);
	}

	// display histogram
	cv::namedWindow(name, CV_WINDOW_AUTOSIZE);
	imshow(name, histImage);
}

cv::Mat histogram_equalization(cv::Mat src) {
	cv::Mat out(src.rows, src.cols, CV_8UC1);//=src.clone();
	int mat_area = src.cols * src.rows;
	const int brightness_levels = 256;

	int cdf_min;
	int original_histogram[brightness_levels];
	int cdf[brightness_levels];
	int equalized_histogram[brightness_levels];

	for (int i = 0; i < brightness_levels; i++) {
		original_histogram[i] = 0;
		equalized_histogram[i] = 0;
		cdf[i] = 0;
	}

	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			original_histogram[(int)src.at<uchar>(y, x)]++;
		}
	}

	equalized_histogram[0] = original_histogram[0];
	cdf[0] = original_histogram[0];
	cdf_min = 1;

	for (int i = 1; i < brightness_levels; i++) {

		for (int j = 0; j < i; j++) {
			cdf[i]+=original_histogram[j];
		}
	}

	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			int new_brightness_level =  round( ( ( ( cdf[src.at<uchar>(y, x)] - cdf_min) * (brightness_levels - 1)) / ((mat_area) - cdf_min)));

			out.at<uchar>(y, x) = new_brightness_level;
			//out.at<uchar>(y, x) = src.at<uchar>(y,x);

			equalized_histogram[new_brightness_level] = original_histogram[src.at<uchar>(y, x)];
		}
	}

	// Display the original Histogram
	display_histogram(original_histogram, "Original Histogram");

	// Display the cumulative Histogram
	display_histogram(cdf, "Cumulative Histogram");

	// Display the equilzed histogram
	display_histogram(equalized_histogram, "Equilized Histogram");

	return out;
}

//================= 10 ==================
// WIP: Perspective Transform 

cv::Mat perspective_transform(cv::Mat vsb_mat, cv::Mat flag_mat) {
	/*cv::Vec2b f_a, f_b, f_c, f_d, v_a, v_b, v_c, v_d;
	f_a = cv::Vec2b(0, 0);
	f_b = cv::Vec2b(323, 0);
	f_c = cv::Vec2b(323, 215);
	f_d = cv::Vec2b(0, 215);
	v_a = cv::Vec2b(69, 107);
	v_b = cv::Vec2b(227, 76);
	v_c = cv::Vec2b(228, 122);
	v_d = cv::Vec2b(66, 134);*/

	CvPoint2D32f vsb_points[4];
	CvPoint2D32f flag_points[4];

	cv::Mat out = vsb_mat.clone();
	cv::Mat lambda = cv::Mat::zeros(vsb_mat.size(), vsb_mat.type());

	cv::Mat leva_matice(8, 8, CV_32FC1); //FLOAT!

	leva_matice.at<float>(0, 1) = 1;
	leva_matice.at<float>(0, 2) = 0;
	leva_matice.at<float>(0, 3) = 0;
	leva_matice.at<float>(0, 4) = 0;
	leva_matice.at<float>(1, 0) = 0;
	leva_matice.at<float>(1, 1) = 0;
	leva_matice.at<float>(1, 4) = 1;
	
	cv::Mat prava_matice(1, 8, CV_32FC1);


	prava_matice.at<float>(1, 0) = 0;

	cv::Mat transformacni_matice(3, 3, CV_32FC1);
	transformacni_matice.at<float>(0, 0); // zbytek doplnim pomoci solve

	flag_points[0] = cv::Point2f(0,0); // A s vlnovkou
	flag_points[1] = cv::Point2f(323, 0);
	flag_points[2] = cv::Point2f(323, 215);
	flag_points[3] = cv::Point2f(0, 215);
	vsb_points[0] = cv::Point2f(69, 107);
	vsb_points[1] = cv::Point2f(227, 76);
	vsb_points[2] = cv::Point2f(228, 122);
	vsb_points[3] = cv::Point2f(66, 134);



	// vypocitam si transformacni matici

	for (int y = 0; y < vsb_mat.rows;y++) {
		for (int x = 0; x < vsb_mat.cols; x++) {
			prava_matice.at<float>(0, 0) = y;
			leva_matice.at<float>(0, 0) = y;





			//solve(leva,prava,out) vrátí 8x1, je to tech 8 pecek

		}
	}


	return out;
}

//================= 11 ==================
// Sobel operator

cv::Mat sobel_operator(cv::Mat original_img) {
	cv::Mat src;
	original_img.convertTo(src, CV_32FC1, 1.0 / 255.0);
	cv::Mat out(src.size(), src.type());
	float a, b, c, d, f, g, h, i, f_x, f_y;

	for (int x = 1; x < src.rows-1; x++) {
		for (int y = 1; y < src.cols-1; y++) {
			/* matice
			a   b   c
			d (x,y) f
			g   h   i
			*/
			a = src.at<float>(x-1, y-1);
			b = src.at<float>(x, y-1);
			c = src.at<float>(x+1, y-1);
			d = src.at<float>(x-1, y);
			f = src.at<float>(x+1, y);
			g = src.at<float>(x-1, y+1);
			h = src.at<float>(x, y+1);
			i = src.at<float>(x+1, y+1);

			f_x = ((c-a)+2*(f-d)+(i-g))/4;
			f_y = ((a-g)+2*(b-h)+(c-i))/4;

			out.at<float>(x, y) = sqrt(f_x*f_x+f_y*f_y);
		}
	}
	return out;
}

//================= 11 ==================
// Edge thinning and double tresholding
// WIP
cv::Mat edge_thinnig(cv::Mat original_img) {
	cv::Mat src;
	original_img.convertTo(src, CV_32FC1, 1.0 / 255.0);
	cv::Mat out(src.size(), src.type());
	cv::Mat thicc_edges(src.size(), src.type());
	float alpha = 0.5f;
	float f_x, f_y, f_x_dia, f_y_dia; // not diagonal and diagonal f 
	float thicc_edge, thicc_edge_dia, thiccness_plus, thiccness_minus, thiccness_plus_minus;
	float u, r, d, l, ur, dr, ul, dl; //neighbours

	for (int x = 1; x < src.rows-1; x++) {
		for (int y = 1; y < src.cols-1; y++) {
			// neighbours of pixel - up, right, down, left
			u = src.at<float>(x, y - 1); // pixel above our x, y
			r = src.at<float>(x + 1, y); // right neighbour
			d = src.at<float>(x, y + 1); // left neighbour
			l = src.at<float>(x - 1, y); // pixel under our x, y
			ur = src.at<float>(x + 1, y - 1); 
			dr = src.at<float>(x + 1, y + 1); 
			ul = src.at<float>(x - 1, y - 1);
			dl = src.at<float>(x - 1, y + 1); 
			
			// central difference method of finding thicc edges
			f_x = (l - r) ;
			f_y = (u - d) ;
			thicc_edge = sqrt(f_x * f_x + f_y * f_y);
			//f_x_dia = alpha * (abs(ur)) + ((1 - alpha) * abs(r));
			//f_y_dia = alpha * (abs(dl)) + ((1 - alpha) * abs(l));
			f_x_dia = ul-dr;
			f_y_dia = dl-ur;
			//thicc_edge_dia = sqrt(f_x_dia * f_x_dia + f_y_dia * f_y_dia);

			thiccness_plus = alpha * (abs(src.at<float>(x + 1, y + 1))) + ((1 - alpha) * abs(src.at<float>(x + 1, y)));
			thiccness_minus = alpha * (abs(src.at<float>(x - 1, y - 1))) + ((1 - alpha) * abs(src.at<float>(x - 1, y)));
			//thiccness_plus_minus = sqrt(thiccness_plus * thiccness_plus + thiccness_minus * thiccness_minus);

			if (l<thicc_edge && thicc_edge>r || u<thicc_edge && thicc_edge>d) {
				
				out.at<float>(x, y) = 1;// thiccness;
			}
			/*else if ( dl<thicc_edge_dia && thicc_edge_dia>ur || ul<thicc_edge_dia && thicc_edge_dia>dr) {

				out.at<float>(x, y) = 0.5;// thiccness;
			}*/
			else if (ur<thicc_edge&& thicc_edge>dl || ul<thicc_edge && thicc_edge>dr) {

				out.at<float>(x, y) = 0.5;// thiccness;
			}
			
			else {
				out.at<float>(x, y) = 0;
			}
			/* Our goal is to get edges of image represend using a single pixel edges. 
			This is called edge thinning and we will use non-maxima supression
			to achieve this goal. What non-maxima supression does is that it supresses (sets to 0) 
			all values that are not maximum in ther context. By context, we mean the close neighbour
			of a pixel. */
		}
	}
	return out;
}

//================ MAIN =================
int main() {

// 1 - 8.9.2020
	cv::Mat lena = cv::imread("images/lena.png", cv::IMREAD_COLOR);
	cv::Mat lena64 = cv::imread("images/lena64.png", cv::IMREAD_COLOR);
	cv::Mat lena64_bw = cv::imread("images/lena64.png", cv::IMREAD_GRAYSCALE);
	cv::Mat lena64_bw_noise = cv::imread("images/lena64_noise.png", cv::IMREAD_GRAYSCALE);
	cv::Mat lena64_bw_noise2 = cv::imread("images/lena64_noise2.png", cv::IMREAD_GRAYSCALE);
	cv::Mat lena64_bw_bars = cv::imread("images/lena64_bars.png", cv::IMREAD_GRAYSCALE);
	cv::Mat earth = cv::imread("images/earth.png", cv::IMREAD_COLOR);
	cv::Mat valve_bw = cv::imread("images/valve.png", cv::IMREAD_GRAYSCALE);
	cv::Mat panorama = cv::imread("images/distorted_panorama.jpg", cv::IMREAD_COLOR);
	cv::Mat window = cv::imread("images/distorted_window.jpg", cv::IMREAD_COLOR);
	cv::Mat uneq = cv::imread("images/uneq.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat flag = cv::imread("images/flag.png", cv::IMREAD_COLOR);
	cv::Mat vsb = cv::imread("images/vsb.jpg", cv::IMREAD_COLOR);
	
// 2 - 15.09.2020
	/*
	cv::Mat lena_gamma = gamma(); // aplikace gamma filteru
	cv::imshow("Gamma", lena_gamma);
	*/
// 3 - 22.09.2020
	/*
	cv::imshow("Convolution", convolution());
	cv::imshow("Original", lena);
	*/

// 4 - 29.09.2020
	//Filtering using anisotropic diffusion http://mrl.cs.vsb.cz//people/gaura/dzo/anisotropic.pdf
	/*
	cv::imshow("Original Valve", valve_bw);
	cv::imshow("Anisotropic Filtering",anisotropic_diffusion(100)); // iterace obrázku anisotropickým filtrem
	*/

// 5 - 6.10.2020/13.10.2020
	// DFT http://mrl.cs.vsb.cz//people/gaura/dzo/dft.pdf
	/*
	cv::Mat dft = myDft(lena64_bw);
	cv::imshow("Phase", dft);
	*/
// 6 - 13.10.2020/20.10.2020
	// IDFT
	/*
	cv::Mat dft = myDft(lena64_bw);
	cv::Mat idft = myIdft(dft);
	cv::imshow("after IDFT", idft);
	*/

// 7 - 20.10.2020/27.10.2020
	// Filtrace obrazu ve frekvencni domene
	/*	
	cv::Mat dft = fourier_transform(lena64_bw_noise);
	cv::Mat dft2 = dft.clone();
	cv::Mat dft3 = fourier_transform(lena64_bw_bars);

	// low & high pass filters in the frequency domain
	cv::Mat dol_filter = fourier_filter(dft,DOLNI_PROPUST);
	cv::Mat hor_filter = fourier_filter(dft2,HORNI_PROPUST);
	cv::Mat ver_filter = fourier_filter(dft3,VERTIKALNI_PROPUST);

	cv::Mat dol_idft = inverse_fourier_transform(dol_filter);
	cv::Mat hor_idft = inverse_fourier_transform(hor_filter);
	cv::Mat ver_idft = inverse_fourier_transform(ver_filter);

	cv::imshow("Original", make_bigger(lena64_bw_noise));
	cv::imshow("after dol", make_bigger(dol_idft));
	cv::imshow("after hor", make_bigger(hor_idft));
	cv::imshow("after ver", make_bigger(ver_idft));
	*/

// 8 - 27.10.2020
	// geometricke nerovnosti a bilinearni interpolace
	/*
	cv::imshow("Before",window);
	cv::Mat tmp1 = remove_geometrical_disortion(window,0.21,0.01);
	//cv::Mat tmp2 = remove_geometrical_disortion(panorama,0.42,0.01);

	cv::imshow("After with BI",tmp1);
	//cv::imshow("After2",tmp2);
	*/
// 9 - 3.11.2020/10.11.220
	// Histogram Equalization
	/*
	cv::imshow("before", uneq);
	cv::Mat idk = histogram_equalization(uneq);
	cv::imshow("after", idk);
	*/
// 10 - 10.11.2020/24.11.2020
	// Perspective transform T
	/*
	cv::Mat tmp = perspective_transform(vsb, flag);
	cv::imshow("after", tmp); 
	*/
// 11 - 24.11.2020
	// Sobel operator
	//http://mrl.cs.vsb.cz/people/sojka/dzo/digitalni_zpracovani_obrazu.pdf vzorce 8.4 8.5
	/*
	cv::Mat tmp = sobel_operator(valve_bw);
	cv::imshow("sobel", tmp); 
	*/

// 11 - 24.11.2020
	// Edge thinning and double tresholding
	cv::Mat tmp = edge_thinnig(valve_bw);
	cv::imshow("thin edges", tmp);

	cv::waitKey(0); // wait until keypressed
	return 0;
}

//zpetna projekce nemusi byt