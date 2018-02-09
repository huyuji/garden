// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include "fft.h"
#include <complex>
#include <string>
#include <fstream>

using namespace cv;

template<class T> void fftShift(T *out, const T* in, size_t nx, size_t ny)
{
    size_t xoffset = nx/2;
    size_t yoffset = ny/2;
    for(size_t x = 0; x < nx; x++)
    {
        for(size_t y = 0; y < ny; y++)
        {
            size_t r = (x + xoffset) % nx;
            size_t c = (y + yoffset) % ny;
            out[r * ny + c] = in[x * ny + y];
        }
    }
}


template<class T> void ifftShift(T *out, const T* in, size_t nx, size_t ny)
{
    size_t xoffset = nx/2;
    size_t yoffset = ny/2;
    for(size_t x = 0; x < nx; x++)
    {
        for(size_t y = 0; y < ny; y++)
        {
            size_t r = (x - xoffset + nx) % nx;
            size_t c = (y - yoffset + ny) % ny;
            out[r * ny + c] = in[x * ny + y];
        }
    }
}

template<class T> void ifftShift1(T *out, const T* in, size_t nx, size_t ny)
{
    const size_t hlen1 = (ny+1)/2;
    const size_t hlen2 = ny/2;
    const size_t shft1 = ((nx+1)/2)*ny + hlen1;
    const size_t shft2 = (nx/2)*ny + hlen2;

    const T* src = in;
    for(T* tgt = out; tgt < out + shft1 - hlen1; tgt += ny, src += ny) { // (nx+1)/2 times
        copy(src, src+hlen1, tgt + shft2);          //1->4
        copy(src+hlen1, src+ny, tgt+shft2-hlen2); } //2->3
    src = in;
    for(T* tgt = out; tgt < out + shft2 - hlen2; tgt += ny, src += ny ){ // nx/2 times
        copy(src+shft1, src+shft1+hlen2, tgt);         //4->1
        copy(src+shft1-hlen1, src+shft1, tgt+hlen2); } //3->2
};

template <class T> void saveMat(const Mat& img, const std::string& fileName)
{
    fstream f(fileName, ios_base::out);

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            f << img.at<T>(i,j) << std::endl;
        }
    }
}

void saveComplexMat(const Mat& img, const std::string& fileName)
{
    fstream f(fileName, ios_base::out);

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            auto temp = img.at<std::complex<float> >(i,j);
            f << temp.real() << std::endl;
            f << temp.imag() << std::endl;
        }
    }
}

bool compareComplexMatrix(ComplexNumber* m1, ComplexNumber* m2, size_t size)
{
    for(int i = 0; i < size; i++)
    {
        if(m1[i].real != m2[i].real || m1[i].imag != m2[i].imag)
        {
            return false;
        }
    }

    return true;
}

void f4()
{
    Mat img = imread("infile.png", 0);

    Mat img2 = cv::Mat::zeros( img.rows, img.cols, CV_32FC1 ); 
    for( int r=0; r<img.rows; ++r)
        for( int c=0; c<img.cols; ++c)
            img2.at<float>(r,c) = (float) img.at<unsigned char>( r,c ); 
    std::cout << img2.at<float>(0, 0) << std::endl;

    Mat img3;
    dft(img2, img3, DFT_COMPLEX_OUTPUT, img2.rows);
    std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;

    int size = img3.rows * img3.cols;
    ComplexNumber *cn = new ComplexNumber[size];
    int k = 0;
    for(int i = 0; i < img3.rows; i++)
    {
        for(int j = 0; j < img3.cols; j++)
        {
            std::complex<float> temp = img3.at<std::complex<float> >(i, j);
            cn[k].real = temp.real();
            cn[k].imag = temp.imag();
            k++;
        }
    }

    ComplexNumber *cn_compare = new ComplexNumber[size];
    for(int i = 0; i < size; i++)
    {
        cn_compare[i] = cn[i];
    }

    FFTShift(img3.cols, img3.rows, cn); // real and imag seems switched

    ComplexNumber *cn_shifted = new ComplexNumber[size];
    fftShift(cn_shifted, cn_compare, img3.rows, img3.cols);

    if(!compareComplexMatrix(cn, cn_shifted, size))
    {
        std::cout << "fftshift mismatch\n";
    }
    cn = cn_shifted;

    //ComplexNumber *cn_ishifted = new ComplexNumber[size];
    //ifftShift1(cn_ishifted, cn, img3.rows, img3.cols);
    //if(!compareComplexMatrix(cn, cn_compare, size))
    //{
    //    std::cout << "ifftshift1 doesn't revert FFTShift\n";
    //}

    //ComplexNumber *cn_shifted_ishifted = new ComplexNumber[size];
    //ifftShift(cn_shifted_ishifted, cn, img3.rows, img3.cols);
    //if(!compareComplexMatrix(cn_compare, cn_shifted_ishifted, size))
    //{
    //    std::cout << "ifftshift doesn't revert fftshift\n";
    //}

    k = 0;
    for(int i = 0; i < img3.rows; i++)
    {
        for(int j = 0; j < img3.cols; j++)
        {
            std::complex<float>& temp = img3.at<std::complex<float> >(i, j);
            temp = std::complex<float>(cn[k].real, cn[k].imag);
            k++;
        }
    }
    std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img3, "fftshift");

    int rows = img.rows;
    int cols = img.cols;
    int crow = rows/2;
    int ccol = cols/2;
    int imax = 100;
    int jmax = imax;
    float mmax = 1.0;
    float mmin = 3.0;

    Mat img4 = Mat::ones(rows, cols, CV_32FC2);
    std::cout << (img4.at<Vec2f>(0, 0))[0] << std::endl;

    for(int i = 0; i < img4.rows; i++)
    {
        for(int j = 0; j < img4.cols; j++)
        {
           img4.at<Vec2f>(i, j)[0] = mmax; 
           img4.at<Vec2f>(i, j)[1] = mmax;
        }
    }
    std::cout << (img4.at<Vec2f>(0, 0))[0] << std::endl;

    for(int i = crow-imax; i < crow+imax; i++)
    {
        for(int j = ccol-jmax; j < ccol+jmax; j++)
        {
           img4.at<Vec2f>(i, j)[0] = mmin; 
           img4.at<Vec2f>(i, j)[1] = mmin;
        }
    }
    std::cout << (img4.at<Vec2f>(crow-imax, ccol-jmax))[0] << std::endl;

    k = 0;
    for(int i = 0; i < img3.rows; i++)
    {
        for(int j = 0; j < img3.cols; j++)
        {
            std::complex<float>& temp = img3.at<std::complex<float> >(i, j);
            float real = temp.real() * (img4.at<Vec2f>(i, j))[0];
            float imag = temp.imag() * (img4.at<Vec2f>(i, j))[1]; 
            temp = std::complex<float>(real, imag);
            k++;
        }
    }
    std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img3, "mask");

    k = 0;
    for(int i = 0; i < img3.rows; i++)
    {
        for(int j = 0; j < img3.cols; j++)
        {
            std::complex<float> temp = img3.at<std::complex<float> >(i, j);
            cn[k].real = temp.real();
            cn[k].imag = temp.imag();
            k++;
        }
    }

    ComplexNumber *cn2_ishifted = new ComplexNumber[size];
    ifftShift(cn2_ishifted, cn, img3.rows, img3.cols);
    
    ComplexNumber *cn2 = new ComplexNumber[img3.rows * img3.cols];
    ifftShift1(cn2, cn, img3.rows, img3.cols);
    std::cout << cn2[0].real << " " << cn2[0].imag << std::endl;

    for(int i =0; i < size; i++)
    {
        if(cn2[i].real != cn2_ishifted[i].real
           || cn2[i].imag != cn2_ishifted[i].imag)
        {
            std::cout << "ifftshift mismatch\n";
            break;
        }
    }
    //cn2 = cn2_ishifted;

    k = 0;
    for(int i = 0; i < img3.rows; i++)
    {
        for(int j = 0; j < img3.cols; j++)
        {
            std::complex<float>& temp = img3.at<std::complex<float> >(i, j);
            temp = std::complex<float>(cn2[k].real, cn2[k].imag);
            k++;
        }
    }
    std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img3, "ifftshift");

    Mat img5;
    idft(img3, img5);
    std::cout << img5.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img5, "idft");

    //Mat imgReal(img5.rows, img5.cols, CV_32FC1);
    //Mat imgImag(img5.rows, img5.cols, CV_32FC1);
    //for(int i = 0; i < img5.rows; i++)
    //{
    //    for(int j = 0; j < img5.cols; j++)
    //    {
    //        imgReal.at<float>(i, j) = img5.at<std::complex<float> >(i, j).real();
    //        imgImag.at<float>(i, j) = img5.at<std::complex<float> >(i, j).imag();
    //    }
    //}

    //Mat img6;
    //magnitude(imgReal, imgImag, img6);

    Mat imgs[2];
    split(img5, imgs);

    Mat img6;
    magnitude(imgs[0], imgs[1], img6);
    std::cout << img6.at<float>(0, 0) << std::endl;
    std::cout << img6.at<float>(1, 0) << std::endl;
    //saveMat<float>(img6, "magnitude_ifftshift1");

    Mat img7;
    cv::normalize(img6, img7, 0, 255, NORM_MINMAX, CV_8UC1);

    imwrite("fcpp.png", img7);
}

void f1()
{
    ComplexNumber cn[9] = {
        {1,1},
        {2,2},
        {3,3},
        {4,4},
        {5,5},
        {6,6},
        {7,7},
        {8,8},
        {9,9}
    };

    ComplexNumber cn2[9];
    //FFTShift(2, 3, cn);
    ifftShift1(cn2, cn, 3, 3);
    FFTShift(3, 3, cn);
}

int _tmain(int argc, _TCHAR* argv[])
{
    f1();
    return 0;
}

