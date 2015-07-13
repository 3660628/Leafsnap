#include"cv.h"  
#include"cxcore.h"  
#include"highgui.h" 
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;
using namespace cv;

Mat trainEM(Mat img_sv, int height, int width)
{
	Mat samples;
	Mat img_binary(height, width, CV_8UC1);
	Mat labels;
	Mat probs;
	CvEM em;
	CvEMParams em_params;

	//��EM�㷨��ֵ����ֵ�������Ȩֵ����kmeans��������õ�
    em_params.means = NULL;
    em_params.covs = NULL;
    em_params.weights = NULL;
    em_params.nclusters = 2;
    em_params.start_step = CvEM::START_AUTO_STEP;
    em_params.cov_mat_type = CvEM::COV_MAT_SPHERICAL;
    //�ﵽ�������������ߵ������С��һ��ֵ��Ӧ����ϵͳĬ�ϵ�ֵ
    em_params.term_crit.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;

	samples = Mat(height * width, 2, CV_64FC1); 
	int num = 0;
	
	//��ȡimg_sv�еڶ�ͨ����S�����͵���ͨ����V�������ֱ���Ϊ�������������洢��samples����
	for(int i = 0; i < img_sv.rows; i++)
	{
		for(int j = 0 ; j < img_sv.cols; j++)
		{
			samples.at<double>(num, 0) = img_sv.at<Vec3b>(i, j)[1];//S����
			samples.at<double>(num, 1) = img_sv.at<Vec3b>(i, j)[2];//V����
			num++;
		}
	}
	//EM�㷨ѵ������
    em.train( samples, Mat(), em_params, &labels);
	
	//����������label�ó���Ӧ�Ķ�ֵ��ͼ��
	num = 0;
	for(int i = 0; i < img_sv.rows; i++)
	{
		for(int j = 0 ; j < img_sv.cols; j++)
		{
			img_binary.at<uchar>(i, j) = labels.at<int>(num, 0);
			num++;
		}
	}

	
	//����Ϊ����ʾЧ�������ǰ�1���255
	for(int i = 0; i < img_binary.rows; i++)
	{
		for(int j = 0 ; j < img_binary.cols; j++)
		{
			if(img_binary.at<uchar>(i, j) == 1)
				img_binary.at<uchar>(i, j) = 255;
		}
	}

	//IplImage ipl_img(img_binary);
    //cvShowImage("2.2 Segmentation_EM", &ipl_img); 
	//imshow("after segmentation", img_binary);
	return img_binary; 
}