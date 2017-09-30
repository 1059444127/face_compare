#pragma once
#include "HttpClient.h"
#include "Comon_Function.h "
#include "HttpClient.h"

class EyeKeyApi
{
public:
    EyeKeyApi(CString strAppID, CString strAppKey);
    virtual ~EyeKeyApi(void);
    BOOL Check(CString strImg, CString& strRes); // �������
    BOOL ComPareFace(CString strFaceID1, CString strFaceID2, CString& strRes); // �Ƚ�����
	BOOL CheckMark(CString strFaceID, CString& strRes); // ��������
    BOOL CreatePeople(CString strPeopleName, CString strFaceID, CString& strRes);
    BOOL DeletePeople(CString strPeopleName, CString& strRes);
    BOOL GetPeople(CString strPeopleName, CString& strRes);
    BOOL RenamePeople(CString strPeopleOldName, CString strPeopleNewName, CString& strRes);

private:
    CString m_strAppID_Key;
    //CString m_strAppKey;
    CHttpClient m_req;
};
