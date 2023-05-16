#include <iostream>
#include "hw2.h"

int main() {
    ImageProcess gr;
    gr.loadImgFromFile("TEST.txt");
    gr.erosion();
    gr.dilatation();
    gr.saveImgToFile("TEST1.txt", 1);
    return 0;
}
