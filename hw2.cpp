#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include "hw2.h"

Img::Img() {
    width = 0;
    height = 0;
    x_c = 0;
    y_c = 0;
    srcImg = new int[width*height];
    for (int i = 0; i < width*height; ++i) {
        srcImg[i] = 0;
    }
}

Img::Img(int w, int h) {
    width = w;
    height = h;
    x_c = w >> 1;
    y_c = h >> 1;
    srcImg = new int[width*height];
    for (int i = 0; i < width*height; ++i) {
        srcImg[i] = 0;
    }
}

Img::Img(const int* src, int w, int h){
    width = w;
    height = h;
    x_c = w >> 1;
    y_c = h >> 1;
    srcImg = new int[width*height];
    for (int i = 0; i < width*height; ++i) {
        srcImg[i] = src[i];
    }
}

Img::~Img() {
    delete[] srcImg;
}

ImageProcess::ImageProcess() {
    srcImg = new Img;
    processedImg = nullptr;
    mask = nullptr;
}

ImageProcess::ImageProcess(int w, int h) {
    srcImg = new Img(w,h);
    processedImg = nullptr;
    mask = new Img;
}

ImageProcess::ImageProcess(const Img *img) {
    srcImg = new Img(img->srcImg, img->width,img->height);
    processedImg = nullptr;
    mask = new Img;
}

ImageProcess::ImageProcess(const char *fileName) {
    FILE* pFile = fopen(fileName, "r");
    int w;
    fscanf(pFile, "%d", &w);
    int h;
    fscanf(pFile, "%d", &h);
    int *source = new int[w*h];
    char* str = new char[w*h];
    fscanf(pFile, "%s", str);
    for (int i = 0; i < w*h; ++i) {
       source[i] = str[i] - '0';
    }
    delete[] str;
    fclose(pFile);
    srcImg = new Img(source, w, h);
    processedImg = nullptr;
    mask = new Img;
    delete[] source;
}

ImageProcess::~ImageProcess() {
    delete srcImg;
    delete processedImg;
    delete mask;
}

int ImageProcess::updateMask(const Img &mask) {
    this->mask = new Img(mask.srcImg, mask.width, mask.height);
    return 0;
}

int ImageProcess::updateSrcImg() {
    srcImg = new Img(processedImg->srcImg, processedImg->width, processedImg->height);
    return 0;
}

int ImageProcess::dilatation(int srcImg) {
    int increaseHeight = 0;
    int increaseWidth = 0;
    if(srcImg == 0){
        this->srcImg = new Img(*processedImg);
    }
    for (int i = 0; i < this->srcImg->width; ++i) {
        if(this->srcImg->srcImg[i] == 1 || this->srcImg->srcImg[this->srcImg->width*this->srcImg->height - i - 1] == 1){
            increaseHeight = 1;
            break;
        }
    }
    for (int i = 0; i < this->srcImg->width * this->srcImg->height ; i+=this->srcImg->width) {
        if(this->srcImg->srcImg[i] == 1 || this->srcImg->srcImg[i + this->srcImg->width - 1] == 1){
            increaseWidth = 1;
            break;
        }
    }
    processedImg = new Img((this->srcImg->width + increaseWidth), (this->srcImg->height + increaseHeight));
    for (int i = 0; i < this->srcImg->height*this->srcImg->width; ++i) {
        if(this->srcImg->srcImg[i] == 1){
           processedImg->srcImg[i] = processedImg->srcImg[i - processedImg->width] =
           processedImg->srcImg[i + processedImg->width] =
           processedImg->srcImg[i + 1] = processedImg->srcImg[i - 1] = 1;
        }
    }
    this->updateSrcImg();
    return 0;
}

int ImageProcess::erosion(int srcImg) {
    if(srcImg == 0){
        this->srcImg = new Img(*processedImg);
    }
    processedImg = new Img((this->srcImg->width), (this->srcImg->height));
    std::vector<int> ObjCoordinates;
    for (int i = 0; i < this->srcImg->height*this->srcImg->width; ++i) {
        if(this->srcImg->srcImg[i] == 1 && this->srcImg->srcImg[i - this->srcImg->width] == 1 &&
                this->srcImg->srcImg[i + this->srcImg->width] ==1 &&
                this->srcImg->srcImg[i + 1] == 1 && this->srcImg->srcImg[i - 1] == 1){
            ObjCoordinates.push_back(i);
        }
    }
    for (int i = 0; i < processedImg->width * processedImg->height; ++i) {
        processedImg->srcImg[i] = 0;
    }
    for(int i = 0; i < ObjCoordinates.size(); i++){
        processedImg->srcImg[ObjCoordinates[i]] = 1;
    }
    this->updateSrcImg();
    return 0;
}

int ImageProcess::loadImgFromFile(const char *fileName, int format) {
    FILE* pFile = fopen(fileName, "r");
    if(format == 0){
        int w;
        fscanf(pFile, "%d", &w);
        int h;
        fscanf(pFile, "%d", &h);
        int *source = new int[w*h];
        char* str = new char[w*h];
        fscanf(pFile, "%s", str);
        for (int i = 0; i < w*h; ++i) {
            source[i] = str[i] - '0';
        }
        delete[] str;
        fclose(pFile);
        srcImg = new Img(source, w, h);
        processedImg = nullptr;
        return 0;
    }
    int w;
    int h;
    fscanf(pFile, "%d", &w);
    fscanf(pFile, "%d", &h);
    int *source = new int[w*h];
    char str = '1';
    int iter = 0;
    while(iter < w*h){
        fscanf(pFile, "%c", &str);
        if(str != '\n'){
            source[iter++] = str - '0';
        }
    }
    fclose(pFile);
    srcImg = new Img(source, w, h);
    processedImg = nullptr;
    mask = new Img;
    return 0;
}

int ImageProcess::saveImgToFile(const char *fileName, int format) {
    FILE* pFile = fopen(fileName, "w");
    fprintf(pFile, "%d\t%d\n", srcImg->width, srcImg->height);
    if(format == 0) {
        for (int i = 0; i < srcImg->width * srcImg->height; ++i) {
            fprintf(pFile, "%d", srcImg->srcImg[i]);
        }
        return 0;
    }
    for (int i = 0; i < srcImg->height; ++i) {
        for (int j = i * srcImg->width; j < srcImg->width + i * srcImg->width; ++j) {
            fprintf(pFile, "%d", srcImg->srcImg[j]);
        }
        fprintf(pFile, "\n");
    }
    fclose(pFile);
    return 0;
}