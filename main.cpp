
#include "include/globel.h"







int main(int argc, char** argv)
{
	
	helpText();  //��չʾ���ʹ�ò˵�



	srcImage = imread("C:\\Users\\dglzh\\Desktop\\photo\\hg.png");     // ����ͼƬ������ͼƬ�������޸����ƻ�·��



	srcImage_backup = srcImage.clone();  // ������srcImage�ᱻ�ı䣬��������������


	maskImage = Mat(srcImage.size(), CV_8UC1);  // ��ģ������������ǣ�Ȼ�󴫸�findContours����ͨ��
	maskImage = Scalar::all(0);


	imgShow = Mat(srcImage.size(), CV_8UC3);  // ��Ҫչʾ��ͼƬ����ʼ���� ԭͼ��ͬ
	imgShow = srcImage.clone(); 


	resImage_store = Mat(srcImage.size(), CV_8UC3);// �����и������ͼƬ����ʼ��Ϊȫ�� ��ͨ��
	resImage_store = Scalar::all(0);


	

	cv::imshow("window", imgShow); //��չʾԭͼ

	setMouseCallback("window", on_Mouse, 0);  //  �������ص�����


	int areaCount = 1;  // �������ڰ���0��ʱ����ÿ������
    int max_areaCount = 1;



	while (true)
	{
		int c = waitKey(0);


		if ((char)c == 27)	// ����ESC�����˳�
			break;


		if ((char)c == '2')  // ����2���ָ�ԭͼ �����ɰ�
		{

			maskImage = Scalar::all(0);
			imgShow = srcImage;
			//srcImage = srcImage_backup.clone();
			cv::imshow("window", imgShow);

		}


		if ((char)c == '1')  // ����1������ͼƬ
		{
			vector<vector<Point>>     contours;
			vector<Vec4i>             hierarchy;


			findContours(maskImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
			//���ɰ���Ѱ����������������ʹ������������ ��ʡʱ�� ���Ͼ����������ǰ����� ��  
			//ͬʱ��Ҫע����ǣ�  RETR_CCOMP ��������ĺ����� ����������ڵ�С������ ���� ��ͼʹ�õ�ʱ��ע���ˡ� 



			if (contours.size() == 0)  // ���û������ǣ���û�����������˳���if���
				break;
			cout << contours.size() << "������" << endl;

			maskWaterShed = Mat(maskImage.size(), CV_32S);//��ˮ�������ɰ�
			maskWaterShed = Scalar::all(0);

			/* ��maskWaterShed�ϻ������� */
			for (int index = 0; index < contours.size(); index++)
				drawContours(maskWaterShed, contours, index, Scalar::all(index + 1), -1, 8, hierarchy, INT_MAX);


			/* ���imshow���maskWaterShed��
			���ǻᷢ������һƬ�ڣ�
			ԭ��������������ֻ��������1,2,3������ֵ���Ƿǳ����Էֱ�ġ� 
			ͨ������80�еĴ������ǲ�������Ŀ������ */


			new_watershed(srcImage_backup, maskWaterShed);  // ע��һ
			
			vector<Vec3b> colorTab;  // ������ɼ�����ɫ
			for (int i = 0; i < contours.size(); i++)
			{	 
				//�������������
				int b = theRNG().uniform(0, 255);
				int g = theRNG().uniform(0, 255);
				int r = theRNG().uniform(0, 255);

				colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
			}

			Mat resImage = Mat(srcImage.size(), CV_8UC3);  // ����һ�����Ҫ��ʾ��ͼ��
			 int thenumberofarea = 0;
			for (int i = 0; i < maskImage.rows; i++)
			{
				for (int j = 0; j < maskImage.cols; j++)
				{	
					// ���ݾ���watershed�������maskWaterShed������ÿ���������ɫ
					int index = maskWaterShed.at<int>(i, j);  // �����maskWaterShed�Ǿ���watershed�����
					if (index == -1)  // ������ֵ����Ϊ-1���߽磩
						resImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
					else if (index <= 0 || index > contours.size())  // û�б�������������Ϊ0
						resImage.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
					else  // ����ÿ�������ֵ���ֲ��䣺1��2��...��contours.size()
						resImage.at<Vec3b>(i, j) = colorTab[index - 1];  // Ȼ�����Щ������Ƴɲ�ͬ��ɫ


					thenumberofarea = std::max(index -1+1, thenumberofarea);
					//ͬʱ����ͳ�Ƹ�ѭ�������˶����ֻأ���֪�����շָ���˼��������� 
					//ͬʱ ��ͬ����� һ�α����Ϊ��һ��ʼ�������� 


				}
			}
			cv::imshow("resImage", resImage);
			addWeighted(resImage, 0.3, srcImage_backup, 0.7, 0, resImage);
			cv::imshow("��ˮ����", resImage);

			areaCount = 1; 
			max_areaCount = thenumberofarea; 

		}


		if ((char)c == '0')  // ��ε㰴��0��������ʾÿ�����ָ��������Ҫ�Ȱ���1������ͼ��
		{	


			
			Mat resImage = Mat(srcImage.size(), CV_8UC3);  // ����һ�����Ҫ��ʾ��ͼ��

			for (int i = 0; i < maskImage.rows; i++)
			{

				for (int j = 0; j < maskImage.cols; j++)
				{
					int index = maskWaterShed.at<int>(i, j);
					if (index == areaCount)
						resImage.at<Vec3b>(i, j) = srcImage_backup.at<Vec3b>(i, j);
					else
						resImage.at<Vec3b>(i, j) = Vec3b(0, 0, 0);

				}
			}

			//imshow("��ˮ����", resImage);

			resImage_store = resImage.clone(); 

			cv::imshow("����ָ�չʾ����ʹ�ñ��湦�ܱ��桿", resImage_store);


			//������չʾ�ļ���  ;
			areaCount++;
			if (areaCount == max_areaCount+1)
				areaCount = 1;


		}






		//�û�ʹ���ĵ� ��������ַ
		if ((char)c == '3')
		{
			system("start https://blog.csdn.net/KIK9973/article/details/117113424");
		}



		//����ͼƬ�ڵ�ǰ�ļ����£��ļ����Ѿ�����ΪĬ����
		if ((char)c == '5')
		{
			cout << "���������ᱣ�浱ǰչʾ�ķָ������ڵ�ǰ��������Ŀ¼" << endl << endl; 
			cout << "���뱣���ļ�������" << endl << endl; 
	
			imwrite("iloveyou.jpg", resImage_store);

		}

	
		//������Ƥ������С
		if ((char)c == '8'|| (char)c == '9')
		{
			if ((char)c == '8')
				size_of_eraser++;
			if ((char)c == '9')
				if (size_of_eraser >= 2)
					size_of_pencial--;
				else
					;
				

		}


		//��������������С
		if ((char)c == '6' || (char)c == '7')
		{
			if ((char)c == '6')
				size_of_pencial++;
			if ((char)c == '7')
				if (size_of_pencial >= 2)
					size_of_pencial--;
				else
					;

		}




	}



	return 0;

}



/* ע��һ��watershed(srcImage_backup, maskWaterShed);
* ע��������������
* srcImage_backup��û���κ��޸ĵ�ԭͼ��CV_8UC3����
* maskWaterShed����ΪCV_32S���ͣ�32λ��ͨ��������ȫ��Ԫ��Ϊ0
*		Ȼ����ΪdrawContours�ĵ�һ���������룬�������������
*		�����Ϊwatershed�Ĳ���
* ���⣬watershed�ĵڶ�������maskWaterShed��InputOutputArray����
*		����Ϊ���룬Ҳ��Ϊ������溯�����õĽ��
*/


//����Ҫ���һ����Ƥ���Ĺ���
	//��Ƥ��û�д�С
	//����Ҫ���һ�������ĵ����˿��Կ���

//������Ƥ���Ĵ�С
