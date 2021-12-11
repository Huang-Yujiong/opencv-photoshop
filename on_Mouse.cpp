


#include"include/globel.h"


void on_Mouse(int event, int x, int y, int flags, void*)
{
	// 鼠标不在“抠图助手”的窗口中  则什么也不做
	if (x < 0 || x >= srcImage.cols || y < 0 || y >= srcImage.rows)
		return;

	// 如果鼠标左键被按下，获取鼠标当前位置；当鼠标左键按下并且移动时，绘制白线；
	if (event == EVENT_LBUTTONDOWN)
	{
		clickPoint = Point(x, y); //本质上 这个步骤是为了 初始化click 为（-1，-1）避免出现，画一条飞线的情况
	}
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point point(x, y);

		line(maskImage, clickPoint, point, Scalar::all(255), size_of_pencial, 8, 0);

		cvtColor(maskImage, maskImage_RGB, COLOR_GRAY2RGB);
		//显示图像的时候 ，我觉得应该  将 模板中的白色元素和原图融合 ，而不是直接在原图上画 ， 看不到橡皮擦的效果。 
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
		//通过黑色的填充， 将模板中原来画好的白色线条擦去 ，实现橡皮擦的功能。负数厚度实现填充效果， 复原模板 
				

																				  
		cvtColor(maskImage, maskImage_RGB, COLOR_GRAY2RGB);
		//显示图像的时候 ，我觉得应该  将 模板中的白色元素和原图融合 ，而不是直接在原图上画 ， 看不到橡皮擦的效果。 
		imgShow = srcImage + maskImage_RGB;  //将蒙版的白色线条覆盖在原图上


		circle(imgShow, clickPoint, size_of_eraser, Scalar(120, 120, 120), 1, 8, 0); //显示一个圆形橡皮擦

		clickPoint = point;

		imshow("imshow", imgShow);


	}

}

