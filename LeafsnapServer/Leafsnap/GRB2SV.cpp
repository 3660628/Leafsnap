#include"cv.h"  
#include"cxcore.h"  
#include"highgui.h" 

using namespace std;
using namespace cv;

IplImage* GRB2SV(char *filename)  //opencv��ʹ��mat������ͼ��ȽϺã������ϰ汾ʹ����IplImage����Ҫ�ֶ�������ͷſռ�
{  
    IplImage *src, *image, *change, *H, *S, *V, *two, *Zero;  
    //����ͼ����ʾ����  

    src = cvLoadImage(filename);//����ͼ��  
	CvSize sz;  
    double scale = 0.2;  
	sz.width = src->width*scale;  
    sz.height = src->height*scale;  
    image = cvCreateImage(sz,src->depth,src->nChannels);  
    cvResize(src, image, CV_INTER_CUBIC);  

    //����ͼ��ռ�  
    change=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,3);    
    H=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
    S=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
    V=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
    two=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,3);  
    Zero=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
  
    cvZero(Zero);//�ڽ�����ͨ���ϲ��ǣ�����û�е�����ͨ�������ǽ���ͨ����Ϊ�հף���ȫ0   
  
    cvCvtColor(image,change,CV_RGB2HSV);  //��RGBɫϵת��ΪHSVɫϵ  
    
    cvSplit(change,H,S,V,0);//�����ͨ��  
  
    cvMerge(Zero,S,V,0,two);  
    cvCvtColor(two,change,CV_HSV2RGB);//cvShowImage�ǰ���RGBɫ�ʷ�ʽ��ʾͼ��ģ���Ҫͨ��RGBɫϵչʾ��HSVɫϵ�ĸ���Ч��  
    //��ʾvs�ռ��µ�ͼ
	//cvNamedWindow("2.1 RGB->SV",CV_WINDOW_AUTOSIZE); 
	//cvShowImage("2.1 RGB->SV",change);   
  
    //cvDestroyAllWindows();  
    cvReleaseImage(&image);  
	cvReleaseImage(&H);  
	cvReleaseImage(&S);  
	cvReleaseImage(&V);  
	cvReleaseImage(&two);  
	cvReleaseImage(&Zero);  
    //cvReleaseImage(&change);  

    return change;  
}  