#include "cv.h"
#include "highgui.h"
#include <math.h>

using namespace cv;

CvSeq* cutFPRegion(Mat M)
{
	//�Ƚ���������ȡ,ֻ��ȡ�����������
	IplImage src(M);
	IplImage *dsw = cvCreateImage(cvSize(M.cols,M.rows), 8, 1); 
	IplImage *dst = cvCreateImage(cvSize(M.cols,M.rows), 8, 3); 
	
	CvMemStorage *storage = cvCreateMemStorage(0);  
	CvSeq *first_contour = NULL;   

	cvThreshold(&src, dsw, 100, 255, CV_THRESH_BINARY);
	//���������ĺ�������洢��storage����
	cvFindContours(dsw, storage, &first_contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);    
   
	//������Ҳ�����ж��������������Ҫѡȡ������Ҫ�ģ�Ҳ��������������
	//��һ������cut false positive,��Ȼû����ͼ�����ֳ�������������ֻҪ���������������൱��������һ��
	//��Ϊ����Ĳ���ֻ�õ���������������㼯
	CvSeq *max_contour;//�ñ��������������Ǹ����� 
	double max_area = 0;
	double area = 0;
    int cnt = 0;    

    for(; first_contour != 0; first_contour = first_contour->h_next)    
    {    
		area = cvContourArea(first_contour);
		if(area > max_area)
		{
			max_area = area;
			max_contour = first_contour;
		} 
		cnt++;
    }    
  
    //printf("the num of contours : %d\n", cnt);      
    
	cvZero(dst);  
	CvScalar color = CV_RGB(255, 255, 255);
	max_contour->h_next = 0;//Ϊ�˷�ֹ�Ѻ��������Ҳ����ȥ
	//����������Ч��
    //cvDrawContours(dst, max_contour, color, color, 0, 2, CV_FILLED, cvPoint(0, 0)); 
	cvDrawContours(dst, max_contour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));
    //CvRect rect = cvBoundingRect(first_contour,0);  
    //cvRectangle(dst, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255, 0, 0), 1, 8, 0); 

    //cvNamedWindow( "2.3 RemoveFalsePositive", 1 );    
    //cvShowImage( "2.3 RemoveFalsePositive", dst );    
	cvReleaseImage(&dsw);  
	cvReleaseImage(&dst);  
	cvReleaseMemStorage(&storage);  
	return max_contour;
}