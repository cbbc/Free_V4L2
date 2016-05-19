#include "v4l2.h"
#include "image_process.h"

void yuyv_to_rgb(unsigned char* yuv,unsigned char* rgb)
{
    unsigned int i;
    unsigned char* y0 = yuv + 0;   
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;
   
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(i = 0; i <= (WIDTH * HEIGHT) / 2 ;i++) {
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128); 
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128); 
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128); 
   
    	bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128); 
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128); 
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128); 
    
      
       	if(rt0 > 250)  	rt0 = 255;
		if(rt0 < 0)    	rt0 = 0;	

		if(gt0 > 250) 	gt0 = 255;
		if(gt0 < 0)		gt0 = 0;	

		if(bt0 > 250)	bt0 = 255;
		if(bt0 < 0)		bt0 = 0;	

		if(rt1 > 250)	rt1 = 255;
		if(rt1 < 0)		rt1 = 0;	

		if(gt1 > 250)	gt1 = 255;
		if(gt1 < 0)		gt1 = 0;	

		if(bt1 > 250)	bt1 = 255;
		if(bt1 < 0)		bt1 = 0;	
					
		*r0 = (unsigned char)rt0;
		*g0 = (unsigned char)gt0;
		*b0 = (unsigned char)bt0;
	
		*r1 = (unsigned char)rt1;
		*g1 = (unsigned char)gt1;
		*b1 = (unsigned char)bt1;

        yuv = yuv + 4;
        rgb = rgb + 6;
        if(yuv == NULL)
          break;

        y0 = yuv;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;
  
        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;
    }   
}

void rgb_to_bmp(unsigned char* pdata, FILE* bmp_fd)     
{
    //分别为rgb数据，要保存的bmp文件名，图片长宽  
  
#if 0

	int size = DEST_WIDTH * DEST_HEIGHT * 3 * sizeof(char); // 每个像素点3个字节   
   
    BMPFILEHEADER_T bfh;  
    BMPINFOHEADER_T bih; 

    bfh.bfType = (unsigned short)0x4d42;  //bm  
    bfh.bfSize = size  // data size  
        + sizeof( BMPFILEHEADER_T ) // first section size  
        + sizeof( BMPINFOHEADER_T ) // second section size  
        ;  
    bfh.bfReserved1 = 0; // reserved  
    bfh.bfReserved2 = 0; // reserved  
    bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//真正的数据的位置 

	
    // 位图第二部分，数据信息  
    bih.biSize = sizeof(BMPINFOHEADER_T);  
    bih.biWidth = DEST_WIDTH;
	bih.biHeight = -DEST_HEIGHT;
    bih.biPlanes = 1;//为1，不用改  
    bih.biBitCount = BITCOUNT;  
	
    bih.biCompression = 0;//不压缩  
    bih.biSizeImage = size;  
    bih.biXPelsPerMeter = 0;//像素每米  
    bih.biYPelsPerMeter = 0;  
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同  
    bih.biClrImportant = 0;//每个像素都重要   
    
    fwrite( &bfh, 8, 1, bmp_fd); 
    fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, bmp_fd);  
    fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, bmp_fd);  
    fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, bmp_fd);  
    fwrite(pdata, size, 1, bmp_fd);   
	
#endif
	int size = WIDTH * HEIGHT * 3 * sizeof(char); // 每个像素点3个字节
	
    // 位图第一部分，文件信息  
    BMPFILEHEADER_T bfh;  
    bfh.bfType = (unsigned short)0x4d42;  //bm  
    bfh.bfSize = size  // data size  
        + sizeof( BMPFILEHEADER_T ) // first section size  
        + sizeof( BMPINFOHEADER_T ) // second section size  
        ;  
    bfh.bfReserved1 = 0; // reserved  
    bfh.bfReserved2 = 0; // reserved  
    bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//真正的数据的位置 
	
    // 位图第二部分，数据信息  
    BMPINFOHEADER_T bih;  
    bih.biSize = sizeof(BMPINFOHEADER_T);  
    bih.biWidth = WIDTH;  
    bih.biHeight = -HEIGHT;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了  
	
    bih.biPlanes = 1;//为1，不用改  
    bih.biBitCount = BITCOUNT;  
    bih.biCompression = 0;//不压缩  
    bih.biSizeImage = size;  
    bih.biXPelsPerMeter = 0;//像素每米  
    bih.biYPelsPerMeter = 0;  
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同  
    bih.biClrImportant = 0;//每个像素都重要   
    
    fwrite( &bfh, 8, 1, bmp_fd); 
    fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, bmp_fd);  
    fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, bmp_fd);  
    fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, bmp_fd);  
    fwrite(pdata, size, 1, bmp_fd);   
} 
