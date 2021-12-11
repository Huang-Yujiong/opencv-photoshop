
#include "include/globel.h"







int main(int argc, char** argv)
{
	
	helpText();  //先展示软件使用菜单



	srcImage = imread("C:\\Users\\dglzh\\Desktop\\photo\\hg.png");     // 读入图片，其他图片请自行修改名称或路径



	srcImage_backup = srcImage.clone();  // 程序中srcImage会被改变，所以这里做备份


	maskImage = Mat(srcImage.size(), CV_8UC1);  // 掩模，在上面做标记，然后传给findContours，单通道
	maskImage = Scalar::all(0);


	imgShow = Mat(srcImage.size(), CV_8UC3);  // 需要展示的图片，初始化和 原图相同
	imgShow = srcImage.clone(); 


	resImage_store = Mat(srcImage.size(), CV_8UC3);// 贮存切割区域的图片，初始化为全黑 三通道
	resImage_store = Scalar::all(0);


	

	cv::imshow("window", imgShow); //先展示原图

	setMouseCallback("window", on_Mouse, 0);  //  调用鼠标回调函数


	int areaCount = 1;  // 计数，在按【0】时绘制每个区域
    int max_areaCount = 1;



	while (true)
	{
		int c = waitKey(0);


		if ((char)c == 27)	// 按【ESC】键退出
			break;


		if ((char)c == '2')  // 按【2】恢复原图 重置蒙版
		{

			maskImage = Scalar::all(0);
			imgShow = srcImage;
			//srcImage = srcImage_backup.clone();
			cv::imshow("window", imgShow);

		}


		if ((char)c == '1')  // 按【1】处理图片
		{
			vector<vector<Point>>     contours;
			vector<Vec4i>             hierarchy;


			findContours(maskImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
			//在蒙版上寻找轮廓，这里我们使用特征点轮廓 节省时间 ，毕竟画出来的是白线条 。  
			//同时需要注意的是，  RETR_CCOMP 这个参数的含义是 不检测轮廓内的小轮廓， 所以 抠图使用的时候注意了。 



			if (contours.size() == 0)  // 如果没有做标记，即没有轮廓，则退出该if语句
				break;
			cout << contours.size() << "个轮廓" << endl;

			maskWaterShed = Mat(maskImage.size(), CV_32S);//分水岭所用蒙版
			maskWaterShed = Scalar::all(0);

			/* 在maskWaterShed上绘制轮廓 */
			for (int index = 0; index < contours.size(); index++)
				drawContours(maskWaterShed, contours, index, Scalar::all(index + 1), -1, 8, hierarchy, INT_MAX);


			/* 如果imshow这个maskWaterShed，
			我们会发现它是一片黑，
			原因是在上面我们只给它赋了1,2,3这样的值，是非常难以分辨的。 
			通过代码80行的处理我们才能清楚的看出结果 */


			new_watershed(srcImage_backup, maskWaterShed);  // 注释一
			
			vector<Vec3b> colorTab;  // 随机生成几种颜色
			for (int i = 0; i < contours.size(); i++)
			{	 
				//随机数字生成器
				int b = theRNG().uniform(0, 255);
				int g = theRNG().uniform(0, 255);
				int r = theRNG().uniform(0, 255);

				colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
			}

			Mat resImage = Mat(srcImage.size(), CV_8UC3);  // 声明一个最后要显示的图像
			 int thenumberofarea = 0;
			for (int i = 0; i < maskImage.rows; i++)
			{
				for (int j = 0; j < maskImage.cols; j++)
				{	
					// 根据经过watershed处理过的maskWaterShed来绘制每个区域的颜色
					int index = maskWaterShed.at<int>(i, j);  // 这里的maskWaterShed是经过watershed处理的
					if (index == -1)  // 区域间的值被置为-1（边界）
						resImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
					else if (index <= 0 || index > contours.size())  // 没有标记清楚的区域被置为0
						resImage.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
					else  // 其他每个区域的值保持不变：1，2，...，contours.size()
						resImage.at<Vec3b>(i, j) = colorTab[index - 1];  // 然后把这些区域绘制成不同颜色


					thenumberofarea = std::max(index -1+1, thenumberofarea);
					//同时我们统计该循环进行了多少轮回，就知道最终分割成了几个轮廓。 
					//同时 不同的类别 一次被标记为从一开始的正数。 


				}
			}
			cv::imshow("resImage", resImage);
			addWeighted(resImage, 0.3, srcImage_backup, 0.7, 0, resImage);
			cv::imshow("分水岭结果", resImage);

			areaCount = 1; 
			max_areaCount = thenumberofarea; 

		}


		if ((char)c == '0')  // 多次点按【0】依次显示每个被分割的区域，需要先按【1】处理图像
		{	


			
			Mat resImage = Mat(srcImage.size(), CV_8UC3);  // 声明一个最后要显示的图像

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

			//imshow("分水岭结果", resImage);

			resImage_store = resImage.clone(); 

			cv::imshow("区域分割展示【可使用保存功能保存】", resImage_store);


			//分区域展示的计数  ;
			areaCount++;
			if (areaCount == max_areaCount+1)
				areaCount = 1;


		}






		//用户使用文档 打开如下网址
		if ((char)c == '3')
		{
			system("start https://blog.csdn.net/KIK9973/article/details/117113424");
		}



		//保存图片在当前文件夹下，文件名已经设置为默认了
		if ((char)c == '5')
		{
			cout << "接下来将会保存当前展示的分割区域于当前程序所在目录" << endl << endl; 
			cout << "输入保存文件的名称" << endl << endl; 
	
			imwrite("iloveyou.jpg", resImage_store);

		}

	
		//调整橡皮增大或减小
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


		//调整画笔增大或减小
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



/* 注释一：watershed(srcImage_backup, maskWaterShed);
* 注意它的两个参数
* srcImage_backup是没做任何修改的原图，CV_8UC3类型
* maskWaterShed声明为CV_32S类型（32位单通道），且全部元素为0
*		然后作为drawContours的第一个参数传入，在上面绘制轮廓
*		最后作为watershed的参数
* 另外，watershed的第二个参数maskWaterShed是InputOutputArray类型
*		即作为输入，也作为输出保存函数调用的结果
*/


//我需要设计一个橡皮擦的功能
	//橡皮擦没有大小
	//我需要设计一个帮助文档让人可以看见

//调整橡皮擦的大小
