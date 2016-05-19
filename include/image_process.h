#ifndef _IMAGE_PROCESS_H
#define _IMAGE_PROCESS_H

/*******BMP 文件头**********/
typedef struct {
    unsigned short   bfType;  
    unsigned long    bfSize;  
    unsigned short   bfReserved1;  
    unsigned short   bfReserved2;  
    unsigned long    bfOffBits;  
}BMPFILEHEADER_T;  

/*******BMP 信息头**********/ 
typedef struct{
    unsigned long    biSize;  
    long             biWidth;  
    long             biHeight;  
    unsigned short   biPlanes;  
    unsigned short   biBitCount;  
    unsigned long    biCompression;  
    unsigned long    biSizeImage;  
    long             biXPelsPerMeter;  
    long             biYPelsPerMeter;  
    unsigned long    biClrUsed;  
    unsigned long    biClrImportant;  
} BMPINFOHEADER_T;


#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))


#endif
