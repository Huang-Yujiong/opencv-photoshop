


#include "include/globel.h"
//以下是全局变量及 的 定义
 unsigned int size_of_eraser=5; //橡皮擦大小，全局变量
 unsigned int size_of_pencial=5; //画笔的大小

							/*


							srcImage  ---  读入的原图
							srcImage_backup  --- 原图的备份
							maskImage  ---   蒙版
							imgShow  --- 展示的图片
							maskImage_RGB    --- 三通道的蒙版
							resImage_store   --- 分割区域图的贮存


							*/
 Mat srcImage, srcImage_backup, maskImage, imgShow, maskImage_RGB, resImage_store;
 Mat maskWaterShed;  // watershed()函数的参数
 Point clickPoint;	// 鼠标点下去的位置
