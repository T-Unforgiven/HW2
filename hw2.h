//
// Created by mrbru on 14.05.2023.
//

#ifndef HW2_HW2_H
#define HW2_HW2_H
#include <list>
struct Img {
        int* srcImg;
        int width;
        int height;

        int x_c;
        int y_c;

        Img();
        Img(int w, int h);
        Img( const int* src, int w, int h);
        ~Img();
};

class ImageProcess {
private :
    Img* srcImg;
    Img* processedImg;
    Img* mask;
public :
    ImageProcess();
    /* выделяется память для картинки размера w*h */
    ImageProcess(int w, int h);
    /* выделяется память для картинки размера w*h и копируется картинка*/
    ImageProcess(const Img* img);
    /* выделяется память для картинки копируется картинка из файла, первая строчка в файле содержит ширину и высоту картинки*/
    ImageProcess(const char* fileName);
    ~ImageProcess();

    /*задание маски*/
    int updateMask(const Img& mask);
    /*перезаписать исходную картинку картинкой, которая была получена в результате дилатации и/или эрозии*/
    int updateSrcImg();
    /*дилатация картинки, результат записать в processedImg*/
    /*
        input		:	srcImg = 1, обрабатывается srcImg  - результат записывается в processedImg
                        srcImg = 0, обрабатывается processedImg - результат записывается в processedImg
    */
    int dilatation(int srcImg = 1);	//исправить на dilatation
    /*эрозия картинки, результат записать в processedImg*/
    int erosion(int srcImg = 1);
    /*
        загрузить/сохранить из файла с имененм fileName картинку
        input		:	fileName - имя файла
                        format - формат файла 	0 - последовательность 0 и 1, без перехода на новую строку
                                                1 - последовательность 0 и 1, с переходом на новую строку
    */
    int loadImgFromFile(const char* fileName, int format = 1);
    int saveImgToFile(const char* fileName, int format = 1);

    /*на зачёт без зачёта*/
    std::list<std::list<std::pair<int /*x*/,int /*y*/>>> getListContours();
};

#endif //HW2_HW2_H
