#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>  
#include<windows.h>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------  
// LPCWSTRתstring
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

// ����winWIN32_FIND_DATA��ȡ�ļ��µ��ļ���
void readImgNamefromFile(char* fileName, std::vector <string> &imgNames)
{
	// vector���� ��������
	imgNames.clear();
	WIN32_FIND_DATA file;
	int i = 0;
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[50];
	// ת�������ļ���
	sprintf_s(tempFilePath, "%s/*", fileName);
	// ���ֽ�ת��
	WCHAR   wstr[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// ���Ҹ��ļ��������ļ���������Զ�ȡ��WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindNextFile(handle, &file);
		FindNextFile(handle, &file);
		// ѭ�������õ��ļ��е������ļ���    
		do
		{
			sprintf(tempFileName, "%s", fileName);
			imgNames.push_back(WChar2Ansi(file.cFileName));
			imgNames[i].insert(0, tempFileName);
			i++;
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);
}
int main(int argc, char** argv)
{
	try
	{
		
		// ���������������ʹ��������ȡһ��ͼƬ�У�ÿ�������ı߽�λ��  
		frontal_face_detector detector = get_frontal_face_detector();

		//������״Ԥ����������Ԥ�����ͼƬ�����߽��ʱ��ı�ǵ��λ�á�  
		//�������Ǵ�shape_predictor_68_face_landmarks.dat�ļ�����ģ��  
		shape_predictor sp;
		deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
		
		//cv::Mat temp;
		//cv::namedWindow("ԭʼͼ");
		//������ʾ��
		image_window win, win_faces;

		char* fileName = "./images/";
		std::vector <string>  imgNames;
		// ��ȡ��Ӧ�ļ����������ļ���
		readImgNamefromFile(fileName, imgNames);
		// ������Ӧ�ļ����������ļ���
		// ѭ������ͼƬ
		for (int i = 0; i < imgNames.size(); ++i)
		{
			DWORD tin = GetTickCount();
			cout << "processing image " << imgNames[i] << endl;
			cv::Mat temp = cv::imread(imgNames[i]);
			cout << temp.cols<<temp.rows<<temp.size() << endl;

			if (temp.empty())
				cout << "ͼƬ��ȡ����" << endl;
			//����Ӧ�Ŵ����СͼƬ���Ա�׼ȷ������ʾ����
			//�������Ҫ���������ܶ��ʱ��ÿ���ͺ�С������Ҫ�Ŵ󣬲��ܼ�⵽�������ⲻ��
			if (temp.rows<600)
				cv::resize(temp, temp, cv::Size(), 2.0, 2.0);
			else if (temp.rows>1000)
				cv::resize(temp, temp, cv::Size(), 0.5, 0.5);

			cv_image<bgr_pixel> img(temp);

			//imshow("ԭʼͼ", temp);
			//cv::waitKey(0);
			//cv::Mat temp1 = temp.clone();
			
			//array2d<rgb_pixel> img;
			//load_image(img, imgNames[i]);
			//if (img.nr()<650)
			//	pyramid_up(img);// �Ŵ�ͼƬ�Ա��⵽�Ƚ�С������. 
			cout << img.nr() << "  " << img.nc() << "   "<<img.size()<<endl;

			//�����������ñ߽��  
			std::vector<rectangle> dets = detector(img);
			cout << "Number of faces detected: " << dets.size() << endl;

			// Now we will go ask the shape_predictor to tell us the pose of  
			// each face we detected.  
			//****����shape_predictor�ຯ��������ÿ������������  
			std::vector<full_object_detection> shapes;//ע����״���������ͣ�full_object_detection  
			for (unsigned long j = 0; j < dets.size(); ++j)
			{
				//Ԥ�����ƣ�ע��������������һ����ͼƬ����һ���ǴӸ�ͼƬ��⵽�ı߽��  
				full_object_detection shape = sp(img, dets[j]);
				cout << "number of parts: " << shape.num_parts() << endl;

				/*��ӡ��ȫ��68����*/
				/*for (int i = 0; i < 68; i++)
				{
				cout << "�� " << i+1 << " ��������꣺ " << shape.part(i) << endl;
				}*/

				shapes.push_back(shape);
			}

			//��ʾ���  
			win.clear_overlay();
			win.set_image(img);
			win.add_overlay(dets, rgb_pixel(255, 0, 0));
			win.add_overlay(render_face_detections(shapes));

			//����Ҳ����ȡÿ�Ŷ�����ú�������ĸ�������ת�����ŵ�һ����׼�ߴ�  
			dlib::array<array2d<rgb_pixel> > face_chips;
			extract_image_chips(img, get_face_chip_details(shapes), face_chips);
			win_faces.set_image(tile_images(face_chips));

			cout << 1.0*(GetTickCount()-tin)/1000<<endl;
			cout << "Hit enter to process the next image..." << endl;
			cin.get();
		}
	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}

	return 0;
}