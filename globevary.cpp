


#include "include/globel.h"
//������ȫ�ֱ����� �� ����
 unsigned int size_of_eraser=5; //��Ƥ����С��ȫ�ֱ���
 unsigned int size_of_pencial=5; //���ʵĴ�С

							/*


							srcImage  ---  �����ԭͼ
							srcImage_backup  --- ԭͼ�ı���
							maskImage  ---   �ɰ�
							imgShow  --- չʾ��ͼƬ
							maskImage_RGB    --- ��ͨ�����ɰ�
							resImage_store   --- �ָ�����ͼ������


							*/
 Mat srcImage, srcImage_backup, maskImage, imgShow, maskImage_RGB, resImage_store;
 Mat maskWaterShed;  // watershed()�����Ĳ���
 Point clickPoint;	// ������ȥ��λ��
