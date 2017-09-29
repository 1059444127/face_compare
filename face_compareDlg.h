
// face_compareDlg.h : ͷ�ļ�
//

#pragma once

#include "cJSON.h"
#include"CvvImage.h"
#include"cv.h"
// Cface_compareDlg �Ի���
class Cface_compareDlg : public CDialogEx
{
// ����
public:
	Cface_compareDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FACE_COMPARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	BOOL ParseResult(CString &strRes, cJSON*& pJson);

// ʵ��
protected:
	HICON m_hIcon;
	CString m_strCfgPath;
	IplImage *image1 = nullptr; //ͼ�� 1
	IplImage *image2 = nullptr; //
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void WriteConfig();

	void LoadConfig();
	void DrawPicToHDC(IplImage *img, UINT ID);
public:
	//CString m_strAppID;
	//CString m_strAppKey;
	afx_msg void OnBnClickedButtonImg1();
	afx_msg void OnBnClickedButtonImg2();
	afx_msg void OnBnClickedMfcbuttonDetect();
	afx_msg void OnBnClickedMfcbuttonFeature();
	afx_msg void OnBnClickedMfcbuttonCompare();
	CString m_strAppID;
	CString m_strAppKey;
	afx_msg void OnBnClickedMfcbuttonExit();
	CString m_filepath1;
	CString m_filepath2;
};
