
// face_compareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "face_compare.h"
#include "face_compareDlg.h"
#include "afxdialogex.h"
#include"EyeKeyApi.h"
#include<iostream>
#include<fstream>
#include<string.h>
#include <io.h>  
#include <fcntl.h> 
#include <locale.h> 
#include<stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAFE_DELETE_CJson(pJson)     if (pJson){ cJSON_Delete(pJson); pJson = NULL;}
using namespace std;
using namespace cv;
//ȫ�ֱ���
//static CString m_strAppID;
//static CString m_strAppKey;
ofstream ojson("result.json");
DWORD tin = 0, tout = 0;
//��ʾ����̨
void InitConsole()
{
	int nRet = 0;
	FILE* fp;
	AllocConsole();
	system("color b0");
	system("title  �����Ա�ϵͳ�������");
	nRet = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nRet, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cface_compareDlg �Ի���



Cface_compareDlg::Cface_compareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cface_compareDlg::IDD, pParent)
	, m_strAppID(_T(""))
	, m_strAppKey(_T(""))
	, m_filepath1(_T(""))
	, m_filepath2(_T(""))
	//, image1(nullptr)
	//, image2(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cface_compareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strAppID);
	DDX_Text(pDX, IDC_EDIT2, m_strAppKey);
	DDX_Text(pDX, IDC_EDIT_PATH1, m_filepath1);
	DDX_Text(pDX, IDC_EDIT_PATH2, m_filepath2);
}

BEGIN_MESSAGE_MAP(Cface_compareDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMG1, &Cface_compareDlg::OnBnClickedButtonImg1)
	ON_BN_CLICKED(IDC_BUTTON_IMG2, &Cface_compareDlg::OnBnClickedButtonImg2)
	ON_BN_CLICKED(IDC_MFCBUTTON_DETECT, &Cface_compareDlg::OnBnClickedMfcbuttonDetect)
	ON_BN_CLICKED(IDC_MFCBUTTON_FEATURE, &Cface_compareDlg::OnBnClickedMfcbuttonFeature)
	ON_BN_CLICKED(IDC_MFCBUTTON_COMPARE, &Cface_compareDlg::OnBnClickedMfcbuttonCompare)
	ON_BN_CLICKED(IDC_MFCBUTTON_EXIT, &Cface_compareDlg::OnBnClickedMfcbuttonExit)
END_MESSAGE_MAP()

//��ͼ��pic�ؼ�
void Cface_compareDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ  
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������  
	ReleaseDC(pDC);
}

// Cface_compareDlg ��Ϣ�������

BOOL Cface_compareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	InitConsole();
	//��ʼ���ڵ�ͼƬ
	m_filepath1 = _T("./image/id.jpg");
	m_filepath2 = _T("./image/1.jpg");
	SetDlgItemText(IDC_EDIT_PATH1, m_filepath1);
	SetDlgItemText(IDC_EDIT_PATH2, m_filepath2);


	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strCfgPath = szPath;
	m_strCfgPath = m_strCfgPath.Left(m_strCfgPath.ReverseFind('\\') + 1);
	m_strCfgPath += _T("config.ini");
	LoadConfig();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cface_compareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cface_compareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	UpdateData(TRUE);
	image1 = cvLoadImage(wstring2string(m_filepath1.GetBuffer(0)).c_str(), 1); //��ʾͼƬ
	DrawPicToHDC(image1, IDC_STATIC_PIC1);
	image2 = cvLoadImage(wstring2string(m_filepath2.GetBuffer(0)).c_str(), 1);
	DrawPicToHDC(image2, IDC_STATIC_PIC2);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cface_compareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL Cface_compareDlg::ParseResult(CString &strRes, cJSON*& pJson)
{
	pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
	if (!pJson)
	{
		CString strMsg = _T("�����û�ʧ��!\n") + strRes;
		AfxMessageBox(strMsg);
		return FALSE;
	}
	else{
		cJSON* pRet = cJSON_GetObjectItem(pJson, "res_code");
		if (!pRet || strcmp(pRet->valuestring, "0000"))
		{
			cJSON* pMsg = cJSON_GetObjectItem(pJson, "message");
			CString strMsg;
			strMsg.Format(_T("%s\n"), pMsg ? string2wstring(pMsg->valuestring).c_str() :
				(pRet ? string2wstring(pRet->valuestring).c_str() : _T("������:-1")));
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	return TRUE;
}

void Cface_compareDlg::LoadConfig()
{
	GetPrivateProfileString(_T("AppInfo"), _T("app_id"), _T(""), m_strAppID.GetBuffer(MAX_PATH), MAX_PATH, m_strCfgPath);
	GetPrivateProfileString(_T("AppInfo"), _T("app_key"), _T(""), m_strAppKey.GetBuffer(MAX_PATH), MAX_PATH, m_strCfgPath);
	UpdateData(FALSE);
}
void Cface_compareDlg::WriteConfig()
{
	UpdateData(true);
	WritePrivateProfileString(_T("AppInfo"), _T("app_id"), m_strAppID, m_strCfgPath);
	WritePrivateProfileString(_T("AppInfo"), _T("app_key"), m_strAppKey, m_strCfgPath);
}

void Cface_compareDlg::OnBnClickedButtonImg1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_strAppID=="" || m_strAppKey=="")
	{
		AfxMessageBox(_T("app_id �� app_key ����Ϊ��!\n"));
		return;
	}

	CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp||"));
	dlg.m_ofn.lpstrTitle = _T("��ѡ������ͷ��");
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CString strRes, strFaceId;
	EyeKeyApi test(m_strAppID, m_strAppKey);
	if (!test.Check(dlg.GetPathName(), strRes))
	{
		CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
		AfxMessageBox(strMsg);
		return;
	}
	else
	{
		m_filepath1 = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_PATH1, m_filepath1);
		image1 = cvLoadImage(wstring2string(m_filepath1.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
		DrawPicToHDC(image1, IDC_STATIC_PIC1);
	}
}


void Cface_compareDlg::OnBnClickedButtonImg2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp||"));
	dlg.m_ofn.lpstrTitle = _T("��ѡ������ͷ��");
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CString strRes, strFaceId;
	EyeKeyApi test(m_strAppID, m_strAppKey);
	if (!test.Check(dlg.GetPathName(), strRes))
	{
		CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
		AfxMessageBox(strMsg);
		return;
	}
	else
	{
		m_filepath2 = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_PATH2, m_filepath2);
		image2 = cvLoadImage(wstring2string(m_filepath2.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
		DrawPicToHDC(image2, IDC_STATIC_PIC2);
	}
}


void Cface_compareDlg::OnBnClickedMfcbuttonDetect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_strAppID == "" || m_strAppKey == "")
	{
		AfxMessageBox(_T("app_id �� app_key ����Ϊ��!\n"));
		return;
	}

	//CString m_filepath1, m_filepath2;
	GetDlgItemText(IDC_EDIT_PATH1, m_filepath1);
	GetDlgItemText(IDC_EDIT_PATH2, m_filepath2);

	if (m_filepath1.IsEmpty() || m_filepath2.IsEmpty())
	{
		MessageBox(_T("·������Ϊ��"));
		return;
	}
	image1 = cvLoadImage(wstring2string(m_filepath1.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
	DrawPicToHDC(image1, IDC_STATIC_PIC1);
	image2 = cvLoadImage(wstring2string(m_filepath2.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
	DrawPicToHDC(image2, IDC_STATIC_PIC2);

	EyeKeyApi test(m_strAppID, m_strAppKey);
	CString strRes; strRes.Empty();

	CString strFaceId1, strFaceId2;

	// ��faceid1
	BOOL bFind1 = FALSE;
	{
		tin = GetTickCount();
		if (!test.Check(m_filepath1, strRes))
		{
			CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
			AfxMessageBox(strMsg);
			return;
		}

		cJSON *pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
		if (pJson)
		{
			cJSON *face=NULL, *attribute=NULL, *position=NULL;
			strFaceId1 = CJSON_FindFirst(pJson, "face_id");
			 //char *cheight = CJSON_Find(pJson, "height");
			/*printf("%S\n", (LPCTSTR)strFaceId1);*/
			if (strFaceId1 != "")
			{
				bFind1 = TRUE;
				face= cJSON_GetObjectItem(pJson, "face");
				//printf("face:%s\n", cJSON_Print(face));
				cJSON *pArray;
				pArray = cJSON_GetArrayItem(face, 0);
				attribute = cJSON_GetObjectItem(pArray, "attribute");
				//printf("attribute:%s\n", cJSON_Print(attribute));
				position = cJSON_GetObjectItem(pArray, "position");
				//printf("position:%s\n", cJSON_Print(position));
				cJSON *age, *gender;
				age = cJSON_GetObjectItem(attribute, "age");
				gender = cJSON_GetObjectItem(attribute, "gender");
				char *age_out, *gender_out;
				age_out = cJSON_Print(age);
				gender_out = cJSON_Print(gender);
				//printf("age:%s\n", age_out);
				//printf("gender:%s\n", gender_out);
				cJSON *center, *height, *width,*centerx,*centery;
				center = cJSON_GetObjectItem(position, "center");
				height = cJSON_GetObjectItem(position, "height");
				width = cJSON_GetObjectItem(position, "width");
				//printf("center:%s\n", cJSON_Print(center));
				//printf("height:%s\n", cJSON_Print(height));
				//printf("width:%s\n", cJSON_Print(width));
				centerx = cJSON_GetObjectItem(center, "x");
				centery = cJSON_GetObjectItem(center, "y");
				char * cen_x = cJSON_Print(centerx);
				int c_x = atoi(cen_x);
				int c_y = atoi(cJSON_Print(centery));
				int fh = atoi(cJSON_Print(height));
				int fw = atoi(cJSON_Print(width));
				//char *img_height, *img_width;	//ͼƬ��С�ߴ�
				//img_height = CJSON_Find(pJson, "img_height");
				//img_width = CJSON_Find(pJson, "img_width");
				//int imgh = atoi(img_height);
				//int imgw = atoi(img_width);
				rectangle(cv::Mat(image1), cv::Rect(c_x , c_y, fw, fh), cv::Scalar(255, 0, 0),2);
				DrawPicToHDC(image1, IDC_STATIC_PIC1);
				//cout << "age:" << cJSON_Print(age) << "    " << "gender:" << cJSON_Print(gender) << endl;
				//wcout << "age" << age.GetBuffer(MAX_PATH) << endl;
				//cout<<"value:"<<node->valuestring << endl;
				//cout << cJSON_Print(node) << endl;					
				//age = CJSON_FindFirst(pJson, "age");
				//gender = CJSON_FindFirst(pJson, "gender");
				char *buf = cJSON_Print(pJson);
				cout << buf << endl;
				ojson << buf;
				//cout << *(attribute->string) << endl;
				//cout << *(attribute->valuestring) << endl;
				//cout << wstring2string(attribute.GetBuffer(0)) << endl;
				//wcout.imbue(std::locale("chs"));
				//const char *constgender = gender_out;
				//int qisirenle = strcmp(gender_out, "Male");	///��֪��Ϊʲô������strcmp�Ƚϣ��õ���gender_out�ַ�������֣������ǵ�����
				//female����male,������һЩб��'\'֮���
				//cout << "gender_out�ĳ���" << strlen(gender_out) << endl;
				if (strlen(gender_out)==6)
					gender_out=("��");
				else
					gender_out=("Ů");
				setlocale(LC_ALL, "chs");
				tout = GetTickCount() - tin;
				strRes.Format(_T("�������\r\none:\r\n��ʱ %d ms\r\n���� %S\r\n�Ա� %S\r\n���ĵ����� (%d,%d)\r\n�����ߴ� (%d,%d)\r\n"), tout,age_out, gender_out, c_x, c_y, fw, fh);
				SetDlgItemText(IDC_EDIT_OUTPUT, strRes);
				//SAFE_DELETE_CJson(centerx);
				//SAFE_DELETE_CJson(centery);
				//SAFE_DELETE_CJson(center);
				//SAFE_DELETE_CJson(width);
				//SAFE_DELETE_CJson(height); 
				//SAFE_DELETE_CJson(age);
				//SAFE_DELETE_CJson(gender);
				//SAFE_DELETE_CJson(attribute);
				//SAFE_DELETE_CJson(position);
				SAFE_DELETE_CJson(face);

			}

		}
		else 
		{
			MessageBox(_T("���ͼ1�쳣"));
			return;
		}
	}

	// ��faceid2
	BOOL bFind2 = FALSE;
	{
		tin = GetTickCount();
		if (!test.Check(m_filepath2, strRes))
		{
			CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
			AfxMessageBox(strMsg);
			return;
		}

		cJSON *pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
		if (pJson)
		{
			cJSON *face = NULL, *attribute = NULL, *position = NULL;
			strFaceId2 = CJSON_FindFirst(pJson, "face_id");
			//char *cheight = CJSON_Find(pJson, "height");
			/*printf("%S\n", (LPCTSTR)strFaceId1);*/
			if (strFaceId2 != "")
			{
				bFind2 = TRUE;
				face = cJSON_GetObjectItem(pJson, "face");
				//printf("face:%s\n", cJSON_Print(face));
				cJSON *pArray;
				pArray = cJSON_GetArrayItem(face, 0);
				attribute = cJSON_GetObjectItem(pArray, "attribute");
				//printf("attribute:%s\n", cJSON_Print(attribute));
				position = cJSON_GetObjectItem(pArray, "position");
				//printf("position:%s\n", cJSON_Print(position));
				cJSON *age, *gender;
				age = cJSON_GetObjectItem(attribute, "age");
				gender = cJSON_GetObjectItem(attribute, "gender");
				char *age_out, *gender_out;
				age_out = cJSON_Print(age);
				gender_out = cJSON_Print(gender);
				//printf("age:%s\n", age_out);
				//printf("gender:%s\n", gender_out);
				cJSON *center, *height, *width, *centerx, *centery;
				center = cJSON_GetObjectItem(position, "center");
				height = cJSON_GetObjectItem(position, "height");
				width = cJSON_GetObjectItem(position, "width");
				//printf("center:%s\n", cJSON_Print(center));
				//printf("height:%s\n", cJSON_Print(height));
				//printf("width:%s\n", cJSON_Print(width));
				centerx = cJSON_GetObjectItem(center, "x");
				centery = cJSON_GetObjectItem(center, "y");
				char * cen_x = cJSON_Print(centerx);
				int c_x = atoi(cen_x);
				int c_y = atoi(cJSON_Print(centery));
				int fh = atoi(cJSON_Print(height));
				int fw = atoi(cJSON_Print(width));
				//char *img_height, *img_width;	//ͼƬ��С�ߴ�
				//img_height = CJSON_Find(pJson, "img_height");
				//img_width = CJSON_Find(pJson, "img_width");
				//int imgh = atoi(img_height);
				//int imgw = atoi(img_width);
				rectangle(cv::Mat(image2), cv::Rect(c_x, c_y, fw, fh), cv::Scalar(255, 0, 0), 2);
				DrawPicToHDC(image2, IDC_STATIC_PIC2);
				//cout << "age:" << cJSON_Print(age) << "    " << "gender:" << cJSON_Print(gender) << endl;

				//wcout << "age" << age.GetBuffer(MAX_PATH) << endl;
				//cout<<"value:"<<node->valuestring << endl;
				//cout << cJSON_Print(node) << endl;

				//age = CJSON_FindFirst(pJson, "age");
				//gender = CJSON_FindFirst(pJson, "gender");
				char *buf = cJSON_Print(pJson);
				cout << buf << endl;
				ojson << buf;
				//cout << *(attribute->string) << endl;
				//cout << *(attribute->valuestring) << endl;
				//cout << wstring2string(attribute.GetBuffer(0)) << endl;
				/*wcout.imbue(std::locale("chs"));*/

				//const char *constgender = gender_out;
				//int qisirenle = strcmp(gender_out, "Male");	///��֪��Ϊʲô������strcmp�Ƚϣ��õ���gender_out�ַ�������֣������ǵ�����
				//female����male,������һЩб��'\'֮���
				//cout << "gender_out�ĳ���" << strlen(gender_out) << endl;
				if (strlen(gender_out) == 6)
					gender_out = ("��");
				else
					gender_out = ("Ů");
				//printf("�Ա�%s\n", gender_out);
				//printf("�Ա�%s\n", gender_out);
				setlocale(LC_ALL, "chs");
				CString edit_temp;
				GetDlgItemText(IDC_EDIT_OUTPUT, edit_temp);
				tout = GetTickCount()-tin;
				strRes.Format(_T("two:\r\n��ʱ %d ms\r\n���� %S\r\n�Ա� %S\r\n���ĵ����� (%d,%d)\r\n�����ߴ� (%d,%d)\r\n"), tout,age_out, gender_out, c_x, c_y, fw, fh);
				strRes = edit_temp+strRes;
				SetDlgItemText(IDC_EDIT_OUTPUT, strRes);
				//SAFE_DELETE_CJson(centerx);
				//SAFE_DELETE_CJson(centery);
				//SAFE_DELETE_CJson(center);
				//SAFE_DELETE_CJson(width);
				//SAFE_DELETE_CJson(height); 
				//SAFE_DELETE_CJson(age);
				//SAFE_DELETE_CJson(gender);
				//SAFE_DELETE_CJson(attribute);
				//SAFE_DELETE_CJson(position);
				SAFE_DELETE_CJson(face);
			}
			//SAFE_DELETE_CJson(pJson);
		}

		if (!bFind2)
		{
			MessageBox(_T("���ͼ2�쳣"));
			return;
		}
	}
}


void Cface_compareDlg::OnBnClickedMfcbuttonFeature()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_strAppID == "" || m_strAppKey == "")
	{
		AfxMessageBox(_T("app_id �� app_key ����Ϊ��!\n"));
		return;
	}

	//CString m_filepath1, m_filepath2;
	GetDlgItemText(IDC_EDIT_PATH1, m_filepath1);
	GetDlgItemText(IDC_EDIT_PATH2, m_filepath2);

	if (m_filepath1.IsEmpty() || m_filepath2.IsEmpty())
	{
		MessageBox(_T("·������Ϊ��"));
		return;
	}
	image1 = cvLoadImage(wstring2string(m_filepath1.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
	DrawPicToHDC(image1, IDC_STATIC_PIC1);
	image2 = cvLoadImage(wstring2string(m_filepath2.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
	DrawPicToHDC(image2, IDC_STATIC_PIC2);

	EyeKeyApi test(m_strAppID, m_strAppKey);
	CString strRes; strRes.Empty();

	CString strFaceId1, strFaceId2;

	// ��faceid1
	BOOL bFind1 = FALSE;
	{
		tin = GetTickCount();
		if (!test.Check(m_filepath1, strRes))
		{
			CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
			AfxMessageBox(strMsg);
			return;
		}

		cJSON *pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
		if (pJson)
		{
			strFaceId1 = CJSON_FindFirst(pJson, "face_id");
			if (strFaceId1 != "")
			{
				bFind1 = TRUE;
			}
			SAFE_DELETE_CJson(pJson);
		}

		if (!bFind1)
		{
			MessageBox(_T("���ͼ1�쳣"));
			return;
		}
		if (test.CheckMark(strFaceId1, strRes))
		{
			cJSON *checkJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
			if (checkJson)
			{
				//printf("%s\n", cJSON_Print(checkJson));
				cJSON *mark, *point;
				mark = cJSON_GetObjectItem(checkJson, "mark");
				printf("%s\n", cJSON_Print(mark));
				char ch[3] = "pi";
				cJSON *markx, *marky;
				cv::Point markp;
				for (int i = 1; i < 10; ++i)
				{
					ch[1] = i + '0';
					point = cJSON_GetObjectItem(mark, ch);
					//printf("point%s\n", cJSON_Print(point));
					markx = cJSON_GetObjectItem(point, "x");
					marky = cJSON_GetObjectItem(point, "y");
					markp.x = atoi(cJSON_Print(markx));
					markp.y = atoi(cJSON_Print(marky));
					cv::circle(cv::Mat(image1), markp, 2, cv::Scalar(0, 255, 0), 1, -1);
				}
				char str_ch[4] = "p10";
				for (int i = 10; i < 50; ++i)
				{
					char c1[3]; 
					_itoa(i,c1,10);
					str_ch[1] = c1[0];
					str_ch[2] = c1[1];
					point = cJSON_GetObjectItem(mark, str_ch);
					markx = cJSON_GetObjectItem(point, "x");
					marky = cJSON_GetObjectItem(point, "y");
					markp.x = atoi(cJSON_Print(markx));
					markp.y = atoi(cJSON_Print(marky));
					cv::circle(cv::Mat(image1), markp, 2, cv::Scalar(0, 255, 0), 1, -1);
				}
				DrawPicToHDC(image1, IDC_STATIC_PIC1);
				tout = GetTickCount()-tin;
				strRes.Format(_T("������������\r\none\r\n��ʱ %d ms\r\n�ɹ���⵽43��������\r\n����ɫ����\r\n"),tout);
				SetDlgItemText(IDC_EDIT_OUTPUT, strRes);
			}

		}
		else
		{
			MessageBox(_T("fail"));
		}
	}


	// ��faceid2
	BOOL bFind2 = FALSE;
	{
		tin = GetTickCount();
		if (!test.Check(m_filepath2, strRes))
		{
			CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
			AfxMessageBox(strMsg);
			return;
		}

		cJSON *pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
		if (pJson)
		{
			strFaceId2 = CJSON_FindFirst(pJson, "face_id");
			if (strFaceId2 != "")
			{
				bFind2 = TRUE;
			}
			SAFE_DELETE_CJson(pJson);
		}

		if (!bFind2)
		{
			MessageBox(_T("���ͼ2�쳣"));
			return;
		}
		if (test.CheckMark(strFaceId2, strRes))
		{
			cJSON *checkJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
			if (checkJson)
			{
				//printf("%s\n", cJSON_Print(checkJson));
				cJSON *mark, *point;
				mark = cJSON_GetObjectItem(checkJson, "mark");
				printf("%s\n", cJSON_Print(mark));
				char ch[3] = "pi";
				cJSON *markx, *marky;
				cv::Point markp;
				for (int i = 1; i < 10; ++i)
				{
					ch[1] = i + '0';
					point = cJSON_GetObjectItem(mark, ch);
					markx = cJSON_GetObjectItem(point, "x");
					marky = cJSON_GetObjectItem(point, "y");
					markp.x = atoi(cJSON_Print(markx));
					markp.y = atoi(cJSON_Print(marky));
					cv::circle(cv::Mat(image1), markp, 1, cv::Scalar(0, 255, 0), 1, -1);
				}
				char str_ch[4] = "p10";
				for (int i = 10; i < 50; ++i)
				{
					char c1[3];
					_itoa(i, c1, 10);
					str_ch[1] = c1[0];
					str_ch[2] = c1[1];
					point = cJSON_GetObjectItem(mark, str_ch);
					markx = cJSON_GetObjectItem(point, "x");
					marky = cJSON_GetObjectItem(point, "y");
					markp.x = atoi(cJSON_Print(markx));
					markp.y = atoi(cJSON_Print(marky));
					cv::circle(cv::Mat(image2), markp, 2, cv::Scalar(0, 255, 0), 1, -1);
				}
				DrawPicToHDC(image2, IDC_STATIC_PIC2);
				CString edit_str;
				GetDlgItemText(IDC_EDIT_OUTPUT, edit_str);
				tout = GetTickCount()-tin;
				strRes.Format(_T("two\r\n��ʱ %d ms\r\n�ɹ���⵽43��������\r\n����ɫ����\r\n"),tout);
				strRes = edit_str + strRes;
				SetDlgItemText(IDC_EDIT_OUTPUT, strRes);
			}

		}
		else
		{
			MessageBox(_T("fail"));
		}
	}
}


void Cface_compareDlg::OnBnClickedMfcbuttonCompare()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_strAppID=="" || m_strAppKey=="")
	{
		AfxMessageBox(_T("app_id �� app_key ����Ϊ��!\n"));
		return;
	}

	//CString m_filepath1, m_filepath2;
	GetDlgItemText(IDC_EDIT_PATH1, m_filepath1);
	GetDlgItemText(IDC_EDIT_PATH2, m_filepath2);

	if (m_filepath1.IsEmpty() || m_filepath2.IsEmpty())
	{
		MessageBox(_T("·������Ϊ��"));
		return;
	}
	image1 = cvLoadImage(wstring2string(m_filepath1.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
	DrawPicToHDC(image1, IDC_STATIC_PIC1);
	image2 = cvLoadImage(wstring2string(m_filepath2.GetBuffer(0)).c_str(), 1); //��ʾͼƬ  
	DrawPicToHDC(image2, IDC_STATIC_PIC2);

	EyeKeyApi test(m_strAppID, m_strAppKey);
	CString strRes; strRes.Empty();

	CString strFaceId1, strFaceId2;

	// ��faceid1
	BOOL bFind1 = FALSE;
	{
		tin = GetTickCount();
		if (!test.Check(m_filepath1, strRes))
		{
			CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
			AfxMessageBox(strMsg);
			return;
		}

		cJSON *pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
		if (pJson)
		{
			strFaceId1 = CJSON_FindFirst(pJson, "face_id");
			if (strFaceId1!="")
			{
				bFind1 = TRUE;
			}
			SAFE_DELETE_CJson(pJson);
		}

		if (!bFind1)
		{
			MessageBox(_T("���ͼ1�쳣"));
			return;
		}
	}

	// ��faceid2
	BOOL bFind2 = FALSE;
	{
		if (!test.Check(m_filepath2, strRes))
		{
			CString strMsg = _T("ͼƬ̫��, ��ѡ��С��3M��ͼƬ!\n");
			AfxMessageBox(strMsg);
			return;
		}

		cJSON *pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
		if (pJson)
		{
			strFaceId2 = CJSON_FindFirst(pJson, "face_id");
			if (strFaceId2!="")
			{
				bFind2 = TRUE;
			}
			SAFE_DELETE_CJson(pJson);
		}

		if (!bFind2)
		{
			MessageBox(_T("���ͼ2�쳣"));
			return;
		}
	}

	if (test.ComPareFace(strFaceId1, strFaceId2, strRes))
	{
		cJSON *pJson = cJSON_Parse(wstring2string(strRes.GetBuffer(0)).c_str());
		if (pJson)
		{
			CString strMsg;
			cJSON* pSimilarity = cJSON_GetObjectItem(pJson, "similarity");
			tout = GetTickCount()-tin;
			strMsg.Format(_T("�����Ա�\r\n��ʱ %d ms\r\n���ƶ�:%.2f%%"), tout,pSimilarity ? pSimilarity->valuedouble : 0);
			SetDlgItemText(IDC_EDIT_OUTPUT, strMsg);
			SAFE_DELETE_CJson(pSimilarity);
			AfxMessageBox(strMsg);
		}
		
	}
	else
	{
		MessageBox(_T("fail"));
	}
}




void Cface_compareDlg::OnBnClickedMfcbuttonExit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;

	// ��ʾ��Ϣ�Ի���   
	nRes = MessageBox(_T("��ȷ��Ҫ�˳������Ա�ϵͳ��"), _T("��ܰ��ʾ"), MB_OKCANCEL | MB_ICONWARNING);
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDCANCEL == nRes)
		return;
	if (ojson)
		ojson.close();
	cvReleaseImage(&image1);
	cvReleaseImage(&image2);
	CDialog::OnDestroy();
	WriteConfig();
	CDialogEx::OnOK();
}
