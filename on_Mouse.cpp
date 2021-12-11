


#include"include/globel.h"


void on_Mouse(int event, int x, int y, int flags, void*)
{
	// ��겻�ڡ���ͼ���֡��Ĵ�����  ��ʲôҲ����
	if (x < 0 || x >= srcImage.cols || y < 0 || y >= srcImage.rows)
		return;

	// ��������������£���ȡ��굱ǰλ�ã������������²����ƶ�ʱ�����ư��ߣ�
	if (event == EVENT_LBUTTONDOWN)
	{
		clickPoint = Point(x, y); //������ ���������Ϊ�� ��ʼ��click Ϊ��-1��-1��������֣���һ�����ߵ����
	}
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point point(x, y);

		line(maskImage, clickPoint, point, Scalar::all(255), size_of_pencial, 8, 0);

		cvtColor(maskImage, maskImage_RGB, COLOR_GRAY2RGB);
		//��ʾͼ���ʱ�� ���Ҿ���Ӧ��  �� ģ���еİ�ɫԪ�غ�ԭͼ�ں� ��������ֱ����ԭͼ�ϻ� �� ��������Ƥ����Ч���� 
		imgShow = srcImage + maskImage_RGB;


		clickPoint = point;


		imshow("window", imgShow);

	}

	if (event == EVENT_RBUTTONDOWN)
	{
		clickPoint = Point(x, y);
	}
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_RBUTTON))
	{


		Point point(x, y);
	


		circle(maskImage, clickPoint, size_of_eraser, Scalar(0, 0, 0), -1, 8, 0); 
		//ͨ����ɫ����䣬 ��ģ����ԭ�����õİ�ɫ������ȥ ��ʵ����Ƥ���Ĺ��ܡ��������ʵ�����Ч���� ��ԭģ�� 
				

																				  
		cvtColor(maskImage, maskImage_RGB, COLOR_GRAY2RGB);
		//��ʾͼ���ʱ�� ���Ҿ���Ӧ��  �� ģ���еİ�ɫԪ�غ�ԭͼ�ں� ��������ֱ����ԭͼ�ϻ� �� ��������Ƥ����Ч���� 
		imgShow = srcImage + maskImage_RGB;  //���ɰ�İ�ɫ����������ԭͼ��


		circle(imgShow, clickPoint, size_of_eraser, Scalar(120, 120, 120), 1, 8, 0); //��ʾһ��Բ����Ƥ��

		clickPoint = point;

		imshow("imshow", imgShow);


	}

}

