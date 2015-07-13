#include "cv.h"
#include "highgui.h"
#include"cxcore.h"  
#include <vector>

using namespace cv;
using namespace std;

#define MAX_SCALE 26//����scale
#define PI 3.1415

//��ͬscaleԲ�����
double CircleArea[MAX_SCALE];

void CalArea()
{
	for(int i = 0; i < MAX_SCALE - 1; i++)
		CircleArea[i] = PI * (i + 2)* (i + 2);
}

void NMattoOne(Mat &res , Mat hist , int n)
{
    int loc = 0;
    for(int i = hist.rows*n ; i < hist.rows*n + hist.rows ; i ++)
	{
        res.at<float>(i,0) = hist.at<float>(loc,0);
        loc++;
    }
}

/*
IplImage *DrawHistogram(Mat hist, float scaleX = 1, float scaleY = 1){  // ��ֱ��ͼ  
    float histMax = 0;  

    cvGetMinMaxHistValue(hist, 0 , &histMax, 0, 0);  // ȡ��ֱ��ͼ�е���ֵ  
    IplImage *imgHist = cvCreateImage(cvSize(256 * scaleX, 64*scaleY), 8, 1);  
    cvZero(imgHist); //// ������ֵ  
    for(int i = 0; i < 255; i++)  
    {  
        float histValue = cvQueryHistValue_1D(hist, i); // ȡ��ֱ��ͼ�е�iֵ  
        float nextValue = cvQueryHistValue_1D(hist, i+1);  
        int numPt = 5;  
        CvPoint pt[5];  
        pt[0] = cvPoint(i*scaleX, 64*scaleY);  
        pt[1] = cvPoint((i+1)*scaleX, 64*scaleY);  
        pt[2] = cvPoint((i+1)*scaleX, (1 -(nextValue/histMax))* 64 * scaleY);  
        pt[3] = cvPoint((i+1)*scaleX, (1 -(histValue/histMax))* 64 * scaleY);  
        pt[4] = cvPoint(i*scaleX, 64*scaleY);  
        cvFillConvexPoly(imgHist, pt, numPt, cvScalarAll(255));  
    }  
    return imgHist;  
}  
*/

//ʹ������������HoCS
Mat CalCurvatures(CvSeq* contour, int height, int width)
{
	Mat *allcontours = new Mat[MAX_SCALE];
	float curvature;//������ʽ��
	CvPoint p;
	uchar *p1;
    uchar *p2;
	Mat res((MAX_SCALE-1)*21,1,CV_32FC1);
	int channels[] = {0};
    int histSize[] = {21};
    float hranges[] = { 0, 1 };
    Mat hist;
    const float* ranges[] = {hranges};
	
	CalArea();

	//������������ͼ
	IplImage *img_origin = cvCreateImage(cvSize(width, height), 8, 1); 
	CvScalar color = CV_RGB(255, 255, 255);   
	cvDrawContours(img_origin, contour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));
	Mat img(img_origin);

	//��ȻCvSeq*����ת��Ϊ<vetor<Point>>,�ͱ���ת��
	vector<CvPoint> LeafPoints;
	for(int i = 0; i < contour->total; i++)
	{
		CvPoint* tmp = (CvPoint*)cvGetSeqElem(contour, i);
		LeafPoints.push_back(*tmp);
	}

	for(int r = 2 ;r <= MAX_SCALE ; r++)//���㲻ͬscale���������
	{
        Mat curvatureforscale(height, width, CV_32FC1, Scalar::all(-1));
        for(int i = 394 ; i < LeafPoints.size() ; i++)
		{
            int count = 0;
            Mat img2 = Mat::zeros(height, width, CV_8UC1);
            p.x = LeafPoints[i].x;
            p.y = LeafPoints[i].y;
            circle(img2, p, r, color, CV_FILLED, 8, 0);//���˸�Բ
			//test
			//IplImage img_test = img2;
			//cvShowImage("test circle", &img_test); 

            for(int row = (int)(p.y-r); row <= p.y + r && row < height && row >= 0; row++)
			{
                for(int col = (int)(p.x-r); col <= p.x + r && col < width && col >= 0; col++)
				{
                    if(img.at<uchar>(row, col) == 255 && img2.at<uchar>(row, col) == 255){
                        count++;//�����ص����ֵĹ������
                    }
                }
            }
            curvature = count / CircleArea[r-2];//ʹ���������������
            curvatureforscale.at<float>((int)p.y,(int)p.x) = curvature;

            img2.release();
        }
        allcontours[r-2] = curvatureforscale;
    }

    for(int j = 0 ; j < MAX_SCALE-1 ; j ++){
        channels[0] = j;
		//��һ�������Ƕ���ָ�룬ѡ��channel ���൱��ѡ��scale
        calcHist(allcontours,MAX_SCALE-1,channels,Mat(),hist,1,histSize,ranges,true,false);
        normalize(hist,hist);
        //cout<<hist.rows<<" "<<hist.cols<<endl<<" "<<hist<<endl;
		NMattoOne(res,hist,j);
    }
	cvReleaseImage(&img_origin); 
    return  res;
}
