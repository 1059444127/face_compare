#include <iostream>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include <fstream>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	try
	{
		frontal_face_detector detector = get_frontal_face_detector();//����һ��frontal_face_detctor���ʵ��detector����get_frontal_face_detector������ʼ����ʵ��
		image_window win;//һ����ʾ����

		array2d<unsigned char> img;
		load_image(img, "./test/2007_007763.jpg");// ����һ��ͼƬ����argv[i](ͼƬ·��)���ص�����img

		pyramid_up(img);//��ͼ������ϲ��ã�����С������

		// Now tell the face detector to give us a list of bounding boxes
		// around all the faces it can find in the image.
		//��ʼ��⣬����һϵ�еı߽��
		std::vector<rectangle> dets = detector(img);//detector���������������������һϵ�б߽����

		cout << "Number of faces detected: " << dets.size() << endl;//dets.size ��������
		// ��ԭͼƬ����ʾ���
		win.clear_overlay();
		win.set_image(img);
		win.add_overlay(dets, rgb_pixel(255, 0, 0));

		cout << "Hit enter to process the next image..." << endl;
		cin.get();
	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}
}