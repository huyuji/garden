// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <boost/date_time.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include "fft.h"
#include <complex>
#include <string>
#include <fstream>
#include <cmath>
#include <set>
#include <algorithm>

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
    //return;
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
    //return;
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
    return true;
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
    //std::cout << img2.at<float>(0, 0) << std::endl;

    Mat img1;
    dft(img2, img1, DFT_COMPLEX_OUTPUT, img2.rows);
    //saveComplexMat(img3, "data/dft");
    //std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;

    //int size = img.rows * img.cols;
    //std::vector<std::complex<float> > cn(size);
    //ComplexNumber *cn = new ComplexNumber[size];
    //int k = 0;
    //for(int i = 0; i < img3.rows; i++)
    //{
    //    for(int j = 0; j < img3.cols; j++)
    //    {
    //        std::complex<float> temp = img3.at<std::complex<float> >(i, j);
    //        cn[k].real = temp.real();
    //        cn[k].imag = temp.imag();
    //        k++;
    //    }
    //}

    //ComplexNumber *cn_compare = new ComplexNumber[size];
    //for(int i = 0; i < size; i++)
    //{
    //    cn_compare[i] = cn[i];
    //}

    //FFTShift(img3.cols, img3.rows, cn); // real and imag seems switched

    //ComplexNumber *cn_shifted = new ComplexNumber[size];
    Mat img3 = Mat::ones(img.rows, img.cols, CV_32FC2);

    fftShift((Vec2f*)img3.data, (Vec2f*)img1.data, img3.rows, img3.cols);
    //saveComplexMat(img3, "img3");


    //if(!compareComplexMatrix(cn, cn_shifted, size))
    //{
    //    std::cout << "fftshift mismatch\n";
    //}
    //cn = cn_shifted;

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

    //k = 0;
    //for(int i = 0; i < img3.rows; i++)
    //{
    //    for(int j = 0; j < img3.cols; j++)
    //    {
    //        std::complex<float>& temp = img3.at<std::complex<float> >(i, j);
    //        temp = std::complex<float>(cn[k].imag, cn[k].real);
    //        k++;
    //    }
    //}
    //std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img3, "data/dftshift");

    int kmask = 100;
    float rmin = 1;
    float rmax = 3;

    Mat img4 = Mat::ones(img.rows, img.cols, CV_32FC2);
    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            (img4.at<Vec2f>(i, j))[0] = rmax;
            (img4.at<Vec2f>(i, j))[1] = rmax;
        }
    }
    //std::cout << (img4.at<Vec2f>(0, 0))[0] << std::endl;

    float nmask = std::pow((double)kmask, (double)3)/(rmax-rmin);

    int crow = img.rows/2;
    int ccol = img.cols/2;
    for(int i = -1 * kmask; i < kmask + 1; i++)
    {
        for(int j = -1 * kmask; j < kmask + 1; j++)
        {
            double ir = std::sqrt((double)(i * i + j * j));
            if(ir <= kmask)
            {
                float value = -1 * std::pow((-1 * std::abs(ir) + kmask), (double)3) / nmask + rmax;
                (img4.at<Vec2f>(crow + i, ccol + j))[0] = value;
                (img4.at<Vec2f>(crow + i, ccol + j))[1] = value;
            }
        }
    }
    //saveComplexMat(img4, "data/mask");

    for(int i = 0; i < img3.rows; i++)
    {
        for(int j = 0; j < img3.cols; j++)
        {
            Vec2f& temp = img3.at<Vec2f>(i, j);
            temp[0] *= (img4.at<Vec2f>(i, j))[0];
            temp[1] *= (img4.at<Vec2f>(i, j))[1]; 
        }
    }
    //std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img3, "data/fshift");

    //k = 0;
    //for(int i = 0; i < img3.rows; i++)
    //{
    //    for(int j = 0; j < img3.cols; j++)
    //    {
    //        std::complex<float> temp = img3.at<std::complex<float> >(i, j);
    //        cn[k].real = temp.real();
    //        cn[k].imag = temp.imag();
    //        k++;
    //    }
    //}

    //ComplexNumber *cn2_ishifted = new ComplexNumber[size];
    
    ifftShift((Vec2f*)img1.data, (Vec2f*)img3.data, img.rows, img.cols);
    
    //ComplexNumber *cn2 = new ComplexNumber[img3.rows * img3.cols];
    //ifftShift1(cn2, cn, img3.rows, img3.cols);
    //std::cout << cn2[0].real << " " << cn2[0].imag << std::endl;

    //for(int i =0; i < size; i++)
    //{
    //    if(cn2[i].real != cn2_ishifted[i].real
    //       || cn2[i].imag != cn2_ishifted[i].imag)
    //    {
    //        std::cout << "ifftshift mismatch\n";
    //        break;
    //    }
    //}
    //cn2 = cn2_ishifted;

    //k = 0;
    //for(int i = 0; i < img3.rows; i++)
    //{
    //    for(int j = 0; j < img3.cols; j++)
    //    {
    //        std::complex<float>& temp = img3.at<std::complex<float> >(i, j);
    //        temp = std::complex<float>(cn2[k].imag, cn2[k].real);
    //        k++;
    //    }
    //}
    //std::cout << img3.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img3, "data/f_ishift");

    Mat img5;
    idft(img1, img5);
    //std::cout << img5.at<std::complex<float> >(0, 0) << std::endl;
    //saveComplexMat(img5, "data/idft");

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
    //std::cout << img6.at<float>(0, 0) << std::endl;
    //std::cout << img6.at<float>(1, 0) << std::endl;
    //saveMat<float>(img6, "data/magnitude");

    Mat img7;
    cv::normalize(img6, img7, 0, 255, NORM_MINMAX, CV_8UC1);

    imwrite("cpp.png", img7);
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

void f2()
{
    char* pbuf = "file";
    std::vector<char> imgBuf(0, 0);
    memcpy(imgBuf.data(), pbuf, 0);
    std::vector<std::vector<char> > bufs;
    bufs.push_back(std::move(imgBuf));
    std::cout << bufs.back().size();
}

void f3(const std::string& file)
{
    Mat image = imread(file, 0);

        using namespace boost::posix_time;
        ptime start = boost::posix_time::microsec_clock::universal_time();

    Mat lut_matrix(1, 256, CV_8UC1 );
    uchar * ptr = lut_matrix.ptr();
    for(int i = 0; i < 256; i++)
    {
        ptr[i] = (int)(pow((double)i / 255.0, 5/3) * 255.0);
    }

        ptime mid = boost::posix_time::microsec_clock::universal_time();
        time_duration td = mid - start;
        std::cout << "Duration create lookup table " << to_simple_string(td) << std::endl;

    Mat img_out;
    LUT(image, lut_matrix, img_out);

        ptime end = boost::posix_time::microsec_clock::universal_time();
        td = end - mid;
        std::cout << "Duration LUT " << to_simple_string(td) << std::endl;
}

void testf3()
{
    f3("curve6100.png");
    f3("odd_height.png");
    f3("odd_height_width.png");
    f3("curve6100.png");
    f3("odd_height.png");
    f3("odd_height_width.png");
}

void f5()
{
    std::set<std::string> s1;
    s1.insert("a");
    s1.insert("c");
    s1.insert("d");
    std::set<std::string> s2;
    s2.insert("b.1");
    s2.insert("d.1");

    std::set<std::string> inter;
    auto sorter = [](const std::string & left, const std::string & right) {
                return (left + ".1") < right;
            };

    std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(inter, inter.begin()), sorter);
}

int _tmain(int argc, _TCHAR* argv[])
{
    f5();
    return 0;
}

