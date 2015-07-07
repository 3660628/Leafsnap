#include"cv.h"  
#include"cxcore.h"  
#include"highgui.h" 

using namespace std;

int GRB2SV(char *filename)  
{  
    IplImage *image,*change,*H,*S,*V,*two,*Zero;  
    //����ͼ����ʾ����  
    cvNamedWindow("image",CV_WINDOW_AUTOSIZE);   
    cvNamedWindow("VS",CV_WINDOW_AUTOSIZE);  
    cvNamedWindow("Zero",CV_WINDOW_AUTOSIZE);  
  
    image=cvLoadImage(filename);//����ͼ��  
  
    //����ͼ��ռ�  
    change=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,3);    
    H=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
    S=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
    V=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
    two=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,3);  
    Zero=cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);  
  
    cvZero(Zero);//�ڽ�����ͨ���ϲ��ǣ�����û�е�����ͨ�������ǽ���ͨ����Ϊ�հף���ȫ0  
  
    cout<<"IPL_DEPTH_8U = "<<IPL_DEPTH_8U<<endl;  
    cout<<"CV_RGB2HSV = "<<CV_RGB2HSV<<endl;  
    cout<<"CV_HSV2RGB = "<<CV_HSV2RGB<<endl;  
  
    cvCvtColor(image,change,CV_RGB2HSV);  //��RGBɫϵת��ΪHSVɫϵ  
          
    cvSplit(change,H,S,V,0);//�����ͨ��  
  
    //��ʾRGBͼ��ͨ��ͼ  
    cvShowImage("image",image);  
  
    cvMerge(Zero,S,V,0,two);  
    cvCvtColor(two,change,CV_HSV2RGB);//cvShowImage�ǰ���RGBɫ�ʷ�ʽ��ʾͼ��ģ���Ҫͨ��RGBɫϵչʾ��HSVɫϵ�ĸ���Ч��  
    cvShowImage("VS",change);   
  
    cvWaitKey(0);  
  
    cvDestroyAllWindows();  
    cvReleaseImage(&image);  
    cvReleaseImage(&change);  
 
    system("pause");  
    return 1;  
}  