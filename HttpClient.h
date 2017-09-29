//MFC ��Http  Https  ������ͨ����
//���ߣ�csw
//ʱ�䣺2015.4.22

#pragma once

#include <afxinet.h>
#include <string>
using namespace std;

// �����ɹ�
#define SUCCESS        0
// ����ʧ��
#define FAILURE        1
// ������ʱ
#define OUTTIME        2

#define  BUFFER_SIZE       1024*10

//flag
#define  NORMAL_CONNECT             INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT             NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST             INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define  SECURE_REQUEST             NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID

class CHttpClient
{
public:
	CHttpClient(void);
	~CHttpClient(void);

	int HttpGet(CString strUrl, CString strPostData, CString &strResponse);
	int HttpPost(CString strUrl, CString strPostData, CString &strResponse);

private:
	int ExecuteRequest(CString strMethod, CString strUrl, CString strPostData, CString &strResponse);
	void Clear();
private:
	CInternetSession *m_pSession;
	CHttpConnection *m_pConnection;
	CHttpFile *m_pFile;
};