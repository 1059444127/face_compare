#include"stdafx.h"
using namespace cv;
using namespace std;

#define WORKBUF_SIZE        (40*1024*1024)
static ofstream fout("�����.txt", ios::ate | ios::out | ios::app);//| ios::app ����ԭ��������
static ofstream fouterror("�����error.txt", ios::ate | ios::out | ios::app);//| ios::app ����ԭ��������
static ofstream fresult("���result.txt", ios::ate | ios::out);//| ios::app ����ԭ��������
static ofstream ferrorres("���error.txt", ios::ate | ios::out);//| ios::app ����ԭ��������

const char* fileName1 = "./test_image/sfz/";		//���֤�ļ���
const char* fileName2 = "./test_image/face_bmp1/";	//����ͼ���ļ���
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
void readImgNamefromFile(const char* fileName, vector <string> &imgNames)
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
//��ȡbmp�ļ�
bool readBmp(const char* path, uint8_t **imageData, int *pWidth, int *pHeight)
{
	if (path == NULL || imageData == NULL || pWidth == NULL || pHeight == NULL)
	{
		fprintf(stderr, "ReadBmp para error\r\n");
		return false;
	}
	FILE *fp = fopen(path, "rb");
	if (fp == 0)
	{
		fprintf(stderr, "Bmp file open failed\r\n");
		return false;
	}
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	*pWidth = head.biWidth;
	*pHeight = head.biHeight;
	int biBitCount = head.biBitCount;
	int lineByte = ((*pWidth) * biBitCount / 8 + 3) / 4 * 4;
	*imageData = (uint8_t *)malloc(lineByte * (*pHeight));
	for (int i = 0; i < *pHeight; i++)
	{
		fseek(fp, (*pHeight - 1 - i) * lineByte + 54, SEEK_SET);
		fread(*imageData + i * (*pWidth) * 3, 1, (*pWidth) * 3, fp);
	}
	fclose(fp);
	return true;
}
//��ȡͼ���ļ�
bool readImg(const Mat &img, uint8_t **imageData, int *pWidth, int *pHeight)
{
	*pWidth = img.cols;
	*pHeight = img.rows;
	*imageData = img.data;
	return true;
}
LPWSTR ConvertCharToLPWSTR(const char * szString)
{
	int dwLen = strlen(szString) + 1;
	int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);//������ʵĳ���
	LPWSTR lpszPath = new WCHAR[dwLen];
	MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
	return lpszPath;
}
//��ȡini�ļ�
double ReadConfigini(string filepath, const char *key)
{
	char *sect = 0;
	double value = 0;
	iniFileLoad(filepath.c_str());
	value = iniGetDouble(sect, key, value);
	return value;
}
int ReadConfiginiInt(string filepath, const char *key)
{
	char *sect = 0;
	int value = 0;
	iniFileLoad(filepath.c_str());
	value = iniGetInt(sect, key, value);
	return value;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//��ʼ��
	double thresh = 0.45;	//���õ����ƶ���ֵ
	int renum = 30;	//�ظ�ִ�еĴ���
	int selectone = 1;
	const char *inipath = "config.ini";
	char *key1 = "thresh";
	char *key2 = "repeat";
	char *key3 = "select";
	double temp = ReadConfigini(inipath, key1);
	int retemp = ReadConfiginiInt(inipath, key2);
	int selecttemp = ReadConfiginiInt(inipath, key3);
	if (temp<1 && temp>0)
		thresh = temp;
	if (retemp > 0)
		renum = retemp;
	if (selecttemp > 0)
		selectone = selecttemp;

	MRESULT nRet = MERR_UNKNOWN;
	MHandle hEngine1 = nullptr;
	MHandle hEngine2 = nullptr;
	MInt32 nScale = 16;
	MInt32 nMaxFace = 10;
	MByte *pWorkMem1 = new MByte[WORKBUF_SIZE];// (MByte *)malloc(WORKBUF_SIZE);
	MByte *pWorkMem2 = new MByte[WORKBUF_SIZE]; //(MByte *)malloc(WORKBUF_SIZE);
	if (pWorkMem1 == nullptr)
	{
		fprintf(stderr, "fail to malloc workbuf\r\n");
		return -1;
	}
	nRet = AFD_FSDK_InitialFaceEngine(APPID, SDKKey1, pWorkMem1, WORKBUF_SIZE, &hEngine1, AFD_FSDK_OPF_0_ONLY, nScale, nMaxFace);
	nRet = AFR_FSDK_InitialEngine(APPID, SDKKey2, pWorkMem2, WORKBUF_SIZE, &hEngine2);
	if (nRet != MOK || hEngine2 == nullptr)
	{
		fprintf(stderr, "InitialFaceEngine failed , errorcode is %d \r\n", nRet);
		return -1;
	}
	std::vector <string>  imgNames1;
	readImgNamefromFile(fileName1, imgNames1);	// ��ȡ��Ӧ�ļ����������ļ���
	std::vector <string>  imgNames2;
	readImgNamefromFile(fileName2, imgNames2);
	ASVLOFFSCREEN offInput1 = { 0 };
	AFR_FSDK_FACEMODEL faceModels1 = { 0 };
	ASVLOFFSCREEN offInput2 = { 0 };
	AFR_FSDK_FACEMODEL faceModels2 = { 0 };	

	if (selectone == 1)
	{
		cout << "����ִ�е��ǲ������Ա�Ĳ��ԣ�\n����ͬ�˵����֤�������Ա�.\n���������ѡȡһ�����֤��һ����������\n";
		cout << "���μ����ֵΪ  " << thresh << endl;
		cout << "���μ������Ա�  " << renum << "  ��" << endl;
		int renumtemp = renum;
		srand(time(nullptr));
		while (renum--)
		{
			//��һ��ͼ��
			{
				int in1 = rand() % 30;
				offInput1 = { 0 };
				faceModels1 = { 0 };
				offInput2 = { 0 };
				faceModels2 = { 0 };
				string path1 = imgNames1[in1];
				cout << renumtemp - renum << " " << path1 << "  ��Ӧ�ķ�Χ��  " << 24 * in1 << "--" << 24 * (in1 + 1) << endl;
				Mat img1 = imread(path1);
				if (!img1.data)
				{
					delete[] pWorkMem1;
					delete[] pWorkMem2;
					cout << "��ȡͼ��ʧ��" << endl;
					return -1;
				}
				imshow("img1", img1);
				cv::waitKey(5);

				offInput1.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
				offInput1.ppu8Plane[0] = nullptr;
				readImg(img1, (uint8_t**)&offInput1.ppu8Plane[0], &offInput1.i32Width, &offInput1.i32Height);
				//readBmp(path1.c_str(), (uint8_t**)&offInput1.ppu8Plane[0], &offInput1.i32Width, &offInput1.i32Height);
				if (!offInput1.ppu8Plane[0])
				{
					fprintf(stderr, "fail to ReadBmp(%s)\r\n", path1);
					AFR_FSDK_UninitialEngine(hEngine2);
					delete [] pWorkMem1;
					delete [] pWorkMem2;
					return -1;
				}
				offInput1.pi32Pitch[0] = offInput1.i32Width * 3;
				{
					AFR_FSDK_FACEINPUT faceResult;
					//��һ��������Ϣͨ��face detection\face tracking���
					LPAFD_FSDK_FACERES	FaceRes = nullptr;
					nRet = AFD_FSDK_StillImageFaceDetection(hEngine1, &offInput1, &FaceRes);
					if (nRet == MOK)
					{
						for (int i = 0; i < FaceRes->nFace; ++i)
						{
							faceResult.lOrient = FaceRes->lfaceOrient[i];//��������
							faceResult.rcFace.left = FaceRes->rcFace[i].left;//������λ��
							faceResult.rcFace.top = FaceRes->rcFace[i].top;
							faceResult.rcFace.right = FaceRes->rcFace[i].right;
							faceResult.rcFace.bottom = FaceRes->rcFace[i].bottom;
						}
					}

					//��ȡ��һ����������
					AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
					nRet = AFR_FSDK_ExtractFRFeature(hEngine2, &offInput1, &faceResult, &LocalFaceModels);
					if (nRet != MOK)
					{
						fprintf(stderr, "fail to Extract 1st FR Feature, error code: %d\r\n", nRet);
					}
					faceModels1.lFeatureSize = LocalFaceModels.lFeatureSize;
					faceModels1.pbFeature = (MByte*)malloc(faceModels1.lFeatureSize);
					memcpy(faceModels1.pbFeature, LocalFaceModels.pbFeature, faceModels1.lFeatureSize);
				}
				//�ڶ���ͼ��
				{
					int i = rand() % 720;
					Mat img2 = imread(imgNames2[i]);
					if (!img2.data)
					{
						cout << "��ȡͼ��ʧ��" << endl;
						return -1;
					}
					offInput2.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
					offInput2.ppu8Plane[0] = nullptr;
					readImg(img2, (uint8_t**)&offInput2.ppu8Plane[0], &offInput2.i32Width, &offInput2.i32Height);
					//readBmp(imgNames2[i].c_str(), (uint8_t**)&offInput2.ppu8Plane[0], &offInput2.i32Width, &offInput2.i32Height);
					if (!offInput2.ppu8Plane[0])
					{
						fprintf(stderr, "fail to ReadBmp(%s)\r\n", imgNames2[i]);
						free(offInput1.ppu8Plane[0]);
						AFR_FSDK_UninitialEngine(hEngine2);
						delete [] pWorkMem1;
						delete [] pWorkMem2;
						return -1;
					}
					offInput2.pi32Pitch[0] = offInput2.i32Width * 3;

					{
						AFR_FSDK_FACEINPUT faceResult;
						//�ڶ���������Ϣͨ��face detection\face tracking���
						LPAFD_FSDK_FACERES	FaceRes = nullptr;
						nRet = AFD_FSDK_StillImageFaceDetection(hEngine1, &offInput2, &FaceRes);
						if (nRet == MOK)
						{
							for (int i = 0; i < FaceRes->nFace; ++i)
							{
								faceResult.lOrient = FaceRes->lfaceOrient[i];//��������
								faceResult.rcFace.left = FaceRes->rcFace[i].left;//������λ��
								faceResult.rcFace.top = FaceRes->rcFace[i].top;
								faceResult.rcFace.right = FaceRes->rcFace[i].right;
								faceResult.rcFace.bottom = FaceRes->rcFace[i].bottom;
							}
						}
						//��ȡ�ڶ�����������
						AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
						nRet = AFR_FSDK_ExtractFRFeature(hEngine2, &offInput2, &faceResult, &LocalFaceModels);
						if (nRet != MOK)
						{
							fprintf(stderr, "fail to Extract 2nd FR Feature, error code: %d\r\n", nRet);
						}
						faceModels2.lFeatureSize = LocalFaceModels.lFeatureSize;
						faceModels2.pbFeature = (MByte*)malloc(faceModels2.lFeatureSize);
						memcpy(faceModels2.pbFeature, LocalFaceModels.pbFeature, faceModels2.lFeatureSize);
					}
					//�Ա�����������������ñȶԽ��
					MFloat  fSimilScore = 0.0f;
					nRet = AFR_FSDK_FacePairMatching(hEngine2, &faceModels1, &faceModels2, &fSimilScore);
					if (nRet == MOK)
					{
						cout << imgNames2[i] << endl;
						fprintf(stdout, "fSimilScore =  %f\r\n", fSimilScore);
						if (fSimilScore < thresh)
						{
							if (24 * in1>i || 24 * (in1 + 1) <= i)
								fout << fSimilScore << endl;
							else
								fout << 1 << endl;
						}
						else
						{
							if (24 * in1 > i || 24 * (in1 + 1) <= i)	//����ͬһ���ˣ������ƶȴ�����ֵ
							{
								cout << "���  " << fSimilScore << endl;
								cout << imgNames2[i] << endl;
								fouterror << path1 << "    " << imgNames2[i] << "   ���ƶ�   " << fSimilScore << endl;
							}
						}
					}
					else
					{
						fprintf(stderr, "FacePairMatching failed , errorcode is %d \r\n", nRet);
					}
				}
			}
		}
	}
	else
	{
		cout << "����ִ�е��������Ա�Ĳ��ԣ�\n��ѡȡͬһ���˵����֤��Ƭ��������Ƭ���жԱ�\n";
		cout << "���μ����ֵΪ  " << thresh << endl;
		for (int in1 = 0; in1 < imgNames1.size(); ++in1)
		{
			offInput1 = { 0 };
			faceModels1 = { 0 };
			offInput2 = { 0 };
			faceModels2 = { 0 };
			string path1 = imgNames1[in1];
			cout << path1 << "   ��Ӧ�ķ�Χ��" << 24 * in1 << "--" << 24 * (in1 + 1) << endl;
			ferrorres << path1 << "   ��Ӧ�ķ�Χ��" << 24 * in1 << "--" << 24 * (in1 + 1) << endl;
			Mat img1 = imread(path1);
			if (!img1.data)
			{
				cout << "��ȡͼ��ʧ��" << endl;
				return -1;
			}
			imshow("img1", img1);
			cv::waitKey(8);

			offInput1.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
			offInput1.ppu8Plane[0] = nullptr;
			readImg(img1, (uint8_t**)&offInput1.ppu8Plane[0], &offInput1.i32Width, &offInput1.i32Height);
			//readBmp(path1.c_str(), (uint8_t**)&offInput1.ppu8Plane[0], &offInput1.i32Width, &offInput1.i32Height);
			if (!offInput1.ppu8Plane[0])
			{
				fprintf(stderr, "fail to ReadBmp(%s)\r\n", path1);
				AFR_FSDK_UninitialEngine(hEngine2);
				delete [] pWorkMem1;
				delete [] pWorkMem2;
				return -1;
			}
			offInput1.pi32Pitch[0] = offInput1.i32Width * 3;


			{
				AFR_FSDK_FACEINPUT faceResult;
				//��һ��������Ϣͨ��face detection\face tracking���

				LPAFD_FSDK_FACERES	FaceRes = nullptr;
				nRet = AFD_FSDK_StillImageFaceDetection(hEngine1, &offInput1, &FaceRes);
				if (nRet == MOK)
				{
					for (int i = 0; i < FaceRes->nFace; ++i)
					{
						faceResult.lOrient = FaceRes->lfaceOrient[i];//��������
						faceResult.rcFace.left = FaceRes->rcFace[i].left;//������λ��
						faceResult.rcFace.top = FaceRes->rcFace[i].top;
						faceResult.rcFace.right = FaceRes->rcFace[i].right;
						faceResult.rcFace.bottom = FaceRes->rcFace[i].bottom;
					}
				}

				//��ȡ��һ����������
				AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
				nRet = AFR_FSDK_ExtractFRFeature(hEngine2, &offInput1, &faceResult, &LocalFaceModels);
				if (nRet != MOK)
				{
					fprintf(stderr, "fail to Extract 1st FR Feature, error code: %d\r\n", nRet);
				}
				faceModels1.lFeatureSize = LocalFaceModels.lFeatureSize;
				faceModels1.pbFeature = (MByte*)malloc(faceModels1.lFeatureSize);
				memcpy(faceModels1.pbFeature, LocalFaceModels.pbFeature, faceModels1.lFeatureSize);
			}
			for (int i = 24 * in1; i < 24 * (in1 + 1); ++i)
			{
				Mat img2 = imread(imgNames2[i]);
				if (!img2.data)
				{
					cout << "��ȡͼ��ʧ��" << endl;
					return -1;
				}
				offInput2.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
				offInput2.ppu8Plane[0] = nullptr;
				readImg(img2, (uint8_t**)&offInput2.ppu8Plane[0], &offInput2.i32Width, &offInput2.i32Height);
				//readBmp(imgNames2[i].c_str(), (uint8_t**)&offInput2.ppu8Plane[0], &offInput2.i32Width, &offInput2.i32Height);
				if (!offInput2.ppu8Plane[0])
				{
					fprintf(stderr, "fail to ReadBmp(%s)\r\n", imgNames2[i]);
					free(offInput1.ppu8Plane[0]);
					AFR_FSDK_UninitialEngine(hEngine2);
					delete [] pWorkMem1;
					delete [] pWorkMem2;
					return -1;
				}
				offInput2.pi32Pitch[0] = offInput2.i32Width * 3;

				{
					AFR_FSDK_FACEINPUT faceResult;
					//�ڶ���������Ϣͨ��face detection\face tracking���
					LPAFD_FSDK_FACERES	FaceRes = nullptr;
					nRet = AFD_FSDK_StillImageFaceDetection(hEngine1, &offInput2, &FaceRes);
					if (nRet == MOK)
					{
						for (int i = 0; i < FaceRes->nFace; ++i)
						{
							faceResult.lOrient = FaceRes->lfaceOrient[i];//��������
							faceResult.rcFace.left = FaceRes->rcFace[i].left;//������λ��
							faceResult.rcFace.top = FaceRes->rcFace[i].top;
							faceResult.rcFace.right = FaceRes->rcFace[i].right;
							faceResult.rcFace.bottom = FaceRes->rcFace[i].bottom;
						}
					}
					//��ȡ�ڶ�����������
					AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
					nRet = AFR_FSDK_ExtractFRFeature(hEngine2, &offInput2, &faceResult, &LocalFaceModels);
					if (nRet != MOK)
					{
						fprintf(stderr, "fail to Extract 2nd FR Feature, error code: %d\r\n", nRet);
					}
					faceModels2.lFeatureSize = LocalFaceModels.lFeatureSize;
					faceModels2.pbFeature = (MByte*)malloc(faceModels2.lFeatureSize);
					memcpy(faceModels2.pbFeature, LocalFaceModels.pbFeature, faceModels2.lFeatureSize);
				}
				//�Ա�����������������ñȶԽ��
				MFloat  fSimilScore = 0.0f;
				nRet = AFR_FSDK_FacePairMatching(hEngine2, &faceModels1, &faceModels2, &fSimilScore);
				if (nRet == MOK)
				{
					fprintf(stdout, "fSimilScore =  %f\r\n", fSimilScore);
					fresult << fSimilScore << endl;
					if (fSimilScore < thresh)
					{
						cout << imgNames2[i] << endl;
						ferrorres << imgNames2[i] << "   " << fSimilScore << endl;
					}
				}
				else
				{
					fprintf(stderr, "FacePairMatching failed , errorcode is %d \r\n", nRet);
				}
			}
			fresult << endl;
		}
	}
	//����ʼ��
	fresult.close();
	ferrorres.close();
	fout.close();
	fouterror.close();
	free(offInput1.ppu8Plane[0]);
	free(offInput2.ppu8Plane[0]);
	nRet = AFR_FSDK_UninitialEngine(hEngine2);
	nRet = AFD_FSDK_UninitialFaceEngine(hEngine1);
	if (nRet != MOK)
	{
		fprintf(stderr, "UninitialFaceEngine failed , errorcode is %d \r\n", nRet);
	}
	delete [] pWorkMem1;
	delete [] pWorkMem2;
	free(faceModels1.pbFeature);
	free(faceModels2.pbFeature);
	return 0;
}

