#include "stdafx.h"
#include "CppUnitTest.h"
#include "../name.h"
#include <filesystem>
#include <vector>
#include <arctl/coinitializer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// ���e�X�g�p�ݒ�l -----------------------------------------
// ����ڑ��p
const CString TEST_NORMAL_SERVER = _T("coral2");
const CString TEST_NORMAL_USER = _T("ronly");
const CString TEST_NORMAL_PASS = _T("ronly");
const CString TEST_NORMAL_VERSION = _T("SDE.DEFAULT");
const CString TEST_NORMAL_PORT = _T("5151");

// �ُ�ڑ��p
const CString TEST_ERROR_SERVER = _T("yakumo10");
const CString TEST_ERROR_USER = _T("csetest10");
const CString TEST_ERROR_PASS = _T("csetest20");
const CString TEST_ERROR_VERSION = _T("SDE.DEFAULT10");
const CString TEST_ERROR_PORT = _T("5050");
// ��--------------------------------------------------------

// �v���p�e�B���m�F�p�Œ蕶����
const CString ITEM_SERVER				= _T("SERVER");
const CString ITEM_INSTANCE				= _T("INSTANCE");
const CString ITEM_DBCLIENT				= _T("DBCLIENT");
const CString ITEM_DB_CONN_PROPERTIES	= _T("DB_CONNECTION_PROPERTIES");

// �ݒ�l�m�F�p�Œ蕶����
const CString AS_INSTANCE = _T("5151");
const CString DC_INSTANCE = _T("sde:oracle11g:");
const CString DC_DBCLIENT = _T("oracle");

// �ڑ��p������
const CString TOKEN_AT				= _T("@");
const CString TOKEN_SLASH			= _T("/");
const CString TOKEN_FRONT_BRACKET	= _T("(");
const CString TOKEN_REAR_BRACKET	= _T(")");
const CString TOKEN_NONE			= _T("");
const CString TOKEN_PREFIX_AS		= _T("[AS]");

// �ڑ�������p�^�[��
const int CONN_PTN_1 = 1;	// user@server(version)
const int CONN_PTN_2 = 2;	// user/password/version/port/server
const int CONN_PTN_3 = 3;	// user/password/version/server
const int CONN_PTN_4 = 4;	// [AS]user@server(version)
const int CONN_PTN_5 = 5;	// [AS]user/password/version/port/server

namespace test_name
{
	TEST_CLASS(create_workspace)
	{
	private:
		// �ڑ��v���p�e�B���
		struct PROPERTY_INFO
		{
			CString csPropertieName;		// �v���p�e�B��
			CString csValue;				// �ݒ�l
		};

		/**
		* @brief excel�t�@�C���ǂݍ��݁B
		*        �J���āA�ŏ��̃V�[�g��ITabele�Ƃ��ēǂݍ��߂�ΐ���
		*/
		void openExcel( const CString& filePath )
		{
			// Excel�t�@�C���I�[�v��
			IWorkspacePtr ipExcelWorkspace = sindy::create_workspace(filePath);
			Assert::IsNotNull<IWorkspace>(ipExcelWorkspace, _T("�t�@�C���I�[�v�����s�F ") + filePath);

			// �V�[�g���擾����
			IEnumDatasetNamePtr ipEnumDatasetName;
			ipExcelWorkspace->get_DatasetNames(esriDTTable, &ipEnumDatasetName);

			// �ŏ��̃V�[�g���擾
			IDatasetNamePtr ipDatasetName;
			//auto hr = ipEnumDatasetName->Reset(); �� ��������Ɖ��̂�Next()�Ŏ��s����B�B�i��
			auto hr = ipEnumDatasetName->Next(&ipDatasetName);
			Assert::IsNotNull<IDatasetName>(ipDatasetName, _T("�V�[�g�擾���s") );
			
			// �V�[�g���擾
			CComBSTR bstr;
			ipDatasetName->get_Name(&bstr);

			// �V�[�g���J��
			ITablePtr ipTable;
			((IFeatureWorkspacePtr)ipExcelWorkspace)->OpenTable(bstr, &ipTable);
			Assert::IsNotNull<ITable>(ipTable, _T("�V�[�g�I�[�v�����s�F ") + CString(bstr) );
		}

		/**
		* @brief �T�[�o�ڑ����_�C���N�g�R�l�N�V�������A�v���p�e�B�����`�F�b�N����
		*/
		void checkConnection(std::vector<PROPERTY_INFO>& vecPropertyInfo)
		{
			bool bHasInstance = false;
			bool bHasDBClient = false;
			bool bHasDBConnProperties = false;

			auto itr = vecPropertyInfo.begin();
			for (; itr != vecPropertyInfo.end(); ++itr) {
				CString csItem = itr->csPropertieName;
				CString csVal = itr->csValue;
				if (ITEM_SERVER == csItem) {
					// SERVER �̏ꍇ�AAS�ڑ�
					Assert::Fail("�R�l�N�V������ʂ�DC�ł͂���܂��� [" + ITEM_SERVER + "�F" + csVal + "]");
				}
				else if (ITEM_INSTANCE == csItem) {
					// INSTANCE �̏ꍇ
					bHasInstance = true;

					CString cstmp = DC_INSTANCE + TEST_NORMAL_SERVER;
					Assert::AreEqual(cstmp.GetString(), csVal.GetString(), "�R�l�N�V������ʂ�DC�ł͂���܂��� [" + ITEM_INSTANCE + "]�F " + csVal);
				}
				else if (ITEM_DBCLIENT == csItem) {
					// DBCLIENT �̏ꍇ
					bHasDBClient = true;

					Assert::AreEqual(DC_DBCLIENT.GetString(), csVal.GetString(), "�_�C���N�g�R�l�N�V������" + ITEM_DBCLIENT + "�v���p�e�B���قȂ��Ă��܂� [" + ITEM_DBCLIENT + "]�F " + csVal);
				}
				else if (ITEM_DB_CONN_PROPERTIES == csItem) {
					// DB_CONNECTION_PROPERTIES �̏ꍇ
					bHasDBConnProperties = true;

					Assert::AreEqual(TEST_NORMAL_SERVER.GetString(), csVal.GetString(), "�_�C���N�g�R�l�N�V������" + ITEM_DB_CONN_PROPERTIES + "�v���p�e�B���قȂ��Ă��܂� [" + ITEM_DB_CONN_PROPERTIES + "]�F " + csVal);
				}
				else {
					// ���̍��ڂ̓`�F�b�N���Ȃ�
				}
			}

			if (!bHasInstance) {
				Assert::Fail(ITEM_INSTANCE + "�v���p�e�B�����݂��܂���");
			}
			if (!bHasDBClient) {
				Assert::Fail(ITEM_DBCLIENT + "�v���p�e�B�����݂��܂���");
			}
			if (!bHasDBConnProperties) {
				Assert::Fail(ITEM_DB_CONN_PROPERTIES + "�v���p�e�B�����݂��܂���");
			}
		}

		/**
		* @brief �T�[�o�ڑ����A�v���P�[�V�����ڑ��A�v���p�e�B�����`�F�b�N����
		*/
		void checkConnection_as(std::vector<PROPERTY_INFO>& vecPropertyInfo)
		{
			bool bHasServer   = false;
			bool bHasInstance = false;

			auto itr = vecPropertyInfo.begin();
			for (; itr != vecPropertyInfo.end(); ++itr) {
				CString csItem = itr->csPropertieName;
				CString csVal = itr->csValue;
				if (ITEM_SERVER == csItem) {
					// SERVER �̏ꍇ
					bHasServer = true;

					Assert::AreEqual(TEST_NORMAL_SERVER.GetString(), csVal.GetString(), "�A�v���P�[�V�����ڑ���" + ITEM_SERVER + "�v���p�e�B���قȂ��Ă��܂� [" + ITEM_SERVER + "]�F " + csVal);
				}
				else if (ITEM_INSTANCE == csItem) {
					// INSTANCE �̏ꍇ
					bHasInstance = true;

					Assert::AreEqual(AS_INSTANCE.GetString(), csVal.GetString(), "�R�l�N�V������ʂ�AS�ł͂���܂��� [" + ITEM_INSTANCE + "]�F " + csVal);
				}
				else if (ITEM_DB_CONN_PROPERTIES == csItem) {
					// DB_CONNECTION_PROPERTIES �̏ꍇ�A�_�C���N�g�R�l�N�V����

					Assert::Fail("�R�l�N�V������ʂ�AS�ł͂���܂��� [" + ITEM_DB_CONN_PROPERTIES + "�F" + csVal + "]");
				}
				else {
					// ���̍��ڂ̓`�F�b�N���Ȃ�
				}
			}

			if (!bHasServer) {
				Assert::Fail(ITEM_SERVER + "�v���p�e�B�����݂��܂���");
			}
			if (!bHasInstance) {
				Assert::Fail(ITEM_INSTANCE + "�v���p�e�B�����݂��܂���");
			}
		}

		/**
		* @brief �T�[�o�ڑ������ۂ̃v���p�e�B�����擾���A�R���e�i�ɋl�߂�
		*/
		void getPropertyInfo(IWorkspacePtr& ipSDEWorkspace, std::vector<PROPERTY_INFO>& vecPropertyInfo)
		{
			// �ڑ��v���p�e�B���擾
			IPropertySetPtr ipPropertySetAfterConn;
			ipSDEWorkspace->get_ConnectionProperties(&ipPropertySetAfterConn);
			CComVariant vNames, vValues;
			ipPropertySetAfterConn->GetAllProperties(&vNames, &vValues);
			long lPropertieCnt;
			ipPropertySetAfterConn->get_Count(&lPropertieCnt);
			SAFEARRAY *pSarName = vNames.parray;
			SAFEARRAY *pSarVal = vValues.parray;

			// �v���p�e�B�����R���e�i�ɋl�߂�
			for (long i = 0; i < lPropertieCnt; i++) {
				PROPERTY_INFO tmpProperty;
				CString csName;
				VARIANT vVal;
				SafeArrayGetElement(pSarName, &i, (void**)&csName);
				SafeArrayGetElement(pSarVal, &i, (void**)&vVal);

				tmpProperty.csPropertieName = csName.GetString();
				tmpProperty.csValue = vVal.bstrVal;
				vecPropertyInfo.push_back(tmpProperty);
			}
		}

		/**
		* @brief �ڑ�������ŃT�[�o�ڑ����s��
		*        �_�C���N�g�R�l�N�V�����Őڑ�����Ă���΃e�X�g����
		*/
		void connectServerTestNormal(const CString& csDBConnect, bool checkDC)
		{
			// �w��T�[�o�֐ڑ�
			IWorkspacePtr ipSDEWorkspace = sindy::create_workspace(csDBConnect);
			Assert::IsNotNull<IWorkspace>(ipSDEWorkspace, "�T�[�o�ڑ��Ɏ��s���܂��� [�ڑ�������F" + csDBConnect + "]");

			// �ڑ����擾
			std::vector<PROPERTY_INFO> vecPropertyInfo;
			getPropertyInfo(ipSDEWorkspace, vecPropertyInfo);

			// �ڑ����Ӑ}�ʂ�̕����ł��邩�m�F
			checkDC ? checkConnection(vecPropertyInfo) : checkConnection_as(vecPropertyInfo);
		}

		/**
		* @brief �s���̐ڑ�������ŃT�[�o�ڑ����s��
		*        �ڑ��ł��Ȃ���΃e�X�g����
		*/
		void connectServerTestError(const CString& csDBConnect)
		{
			// �w��T�[�o�֐ڑ�
			IWorkspacePtr ipSDEWorkspace = sindy::create_workspace(csDBConnect);
			Assert::IsNull<IWorkspace>(ipSDEWorkspace, "�ُ�Ȑڑ�������ŃT�[�o�ڑ�����܂��� [�ڑ�������F" + csDBConnect + "]");
		}

		/**
		* @brief �w��p�^�[���̃T�[�o�ڑ��p�������Ԃ�
		*/
		CString makeConnectString(const int iPtn,
								const CString& csServer,
								const CString& csUser,
								const CString& csPass,
								const CString& csVersion,
								const CString& csPort)
		{
			CString csReturn;
			switch (iPtn) {
				case CONN_PTN_1:
					// user@server(version)
					csReturn = csUser + TOKEN_AT + csServer + TOKEN_FRONT_BRACKET + csVersion + TOKEN_REAR_BRACKET;
					break;

				case CONN_PTN_2:
					// user/password/version/port/server
					csReturn = csUser + TOKEN_SLASH + csPass + TOKEN_SLASH + csVersion + TOKEN_SLASH + csPort + TOKEN_SLASH + csServer;
					break;

				case CONN_PTN_3:
					// user/password/version/server
					csReturn = csUser + TOKEN_SLASH + csPass + TOKEN_SLASH + csVersion + TOKEN_SLASH + csServer;
					break;
				case CONN_PTN_4:
					// [AS]user@server(version)
					csReturn = TOKEN_PREFIX_AS + csUser + TOKEN_AT + csServer + TOKEN_FRONT_BRACKET + csVersion + TOKEN_REAR_BRACKET;
					break;
				case CONN_PTN_5:
					// [AS]user/password/version/port/server
					csReturn = TOKEN_PREFIX_AS + csUser + TOKEN_SLASH + csPass + TOKEN_SLASH + csVersion + TOKEN_SLASH + csPort + TOKEN_SLASH + csServer;
					break;
				default:
					break;
			}
			return csReturn;
		}

	public:
		/**
		* @brief .xls�t�@�C���ǂݍ��݂̃e�X�g
		*/
		TEST_METHOD(create_workspace_excel_xls)
		{
			openExcel(_T("../data/_2003.xls"));
		}
		
		/**
		* @brief .xlsx�t�@�C���ǂݍ��݂̃e�X�g
		* @note ���̃e�X�g�Ńt�@�C���I�[�v���Ɏ��s�����ꍇ�A���L���Q�l�ɂ��čĎ��{���邱�ƁB
		*       http://confluence.ipc.pioneer.co.jp/confluence/pages/viewpage.action?pageId=35727413
		*/
		TEST_METHOD(create_workspace_excel_xlsx)
		{
			openExcel(_T("../data/2007_.xlsx"));
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� ����ڑ��@�ڑ�������p�^�[��1 [user@server(version)]
		* @note 
		*/
		TEST_METHOD(normal_ptn1)
		{
			// SDE�ڑ�������p�^�[��1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� ����ڑ��@�ڑ�������p�^�[��2 [user/password/version/port/server]
		* @note
		*/
		TEST_METHOD(normal_ptn2)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� ����ڑ��@�ڑ�������p�^�[��2 [user/password/version/port/server] �i������|�[�g���w��j
		* @note DC�ڑ��ł̓|�[�g�̒l�͖��������̂Ő���ڑ��ł���
		*/
		TEST_METHOD(normal_ptn2_port_miss)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_ERROR_PORT);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� ����ڑ��@�ڑ�������p�^�[��3 [user/password/version/server]
		* @note
		*/
		TEST_METHOD(normal_ptn3)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� ����ڑ��@�ڑ�������p�^�[��4 [[AS]user@server(version)]/
		* @note
		*/
		TEST_METHOD(normal_ptn4)
		{
			// SDE�ڑ�������p�^�[��4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestNormal(csConnStr, false);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ�  ����ڑ��@�ڑ�������p�^�[��5 [[AS]user/password/version/port/server]
		* @note
		*/
		TEST_METHOD(normal_ptn5)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestNormal(csConnStr, false);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��1�@(���݂��Ȃ��T�[�o�w��)
		* @note
		*/
		TEST_METHOD(error_ptn1_server_notexist)
		{
			// SDE�ڑ�������p�^�[��1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_ERROR_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��1�@(�T�[�o�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn1_server_notspecified)
		{
			// SDE�ڑ�������p�^�[��1
			CString csConnStr = makeConnectString(CONN_PTN_1, TOKEN_NONE, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��1�@(���݂��Ȃ����[�U�w��)
		* @note
		*/
		TEST_METHOD(error_ptn1_user_notexist)
		{
			// SDE�ڑ�������p�^�[��1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_ERROR_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��1�@(���[�U�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn1_user_notspecified)
		{
			// SDE�ڑ�������p�^�[��1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TOKEN_NONE, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��1�@(���݂��Ȃ��o�[�W����)
		* @note
		*/
		TEST_METHOD(error_ptn1_version_notexist)
		{
			// SDE�ڑ�������p�^�[��1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_ERROR_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��1�@(�o�[�W�����w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn1_version_notspecified)
		{
			// SDE�ڑ�������p�^�[��1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TOKEN_NONE, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(���݂��Ȃ��T�[�o�w��)
		* @note
		*/
		TEST_METHOD(error_ptn2_server_notexist)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_ERROR_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(�T�[�o�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn2_server_notspecified)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TOKEN_NONE, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(���݂��Ȃ����[�U�w��)
		* @note
		*/
		TEST_METHOD(error_ptn2_user_notexist)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_ERROR_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(���[�U�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn2_user_notspecified)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TOKEN_NONE, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(�p�X���[�h�w����)
		* @note
		*/
		TEST_METHOD(error_ptn2_pass_miss)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_ERROR_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(�p�X���[�h�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn2_pass_notspecified)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(���݂��Ȃ��o�[�W�����w��)
		* @note
		*/
		TEST_METHOD(error_ptn2_version_notexist)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_ERROR_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(�o�[�W�����w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn2_version_notspecified)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TOKEN_NONE, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��2�@(�|�[�g�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn2_port_notspecified)
		{
			// SDE�ڑ�������p�^�[��2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(���݂��Ȃ��T�[�o�w��)
		* @note
		*/
		TEST_METHOD(error_ptn3_server_notexist)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_ERROR_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(�T�[�o�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn3_server_notspecified)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TOKEN_NONE, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(���݂��Ȃ����[�U�w��)
		* @note
		*/
		TEST_METHOD(error_ptn3_user_notexist)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_ERROR_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(���[�U�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn3_user_notspecified)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TOKEN_NONE, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(�p�X���[�h�w����)
		* @note
		*/
		TEST_METHOD(error_ptn3_pass_notexist)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_ERROR_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(�p�X���[�h�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn3_pass_notspecified)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(���݂��Ȃ��o�[�W�����w��)
		* @note
		*/
		TEST_METHOD(error_ptn3_version_notexist)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_ERROR_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �_�C���N�g�R�l�N�V���� �ُ�ڑ��@�ڑ�������p�^�[��3�@(�o�[�W�����w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn3_version_notspecified)
		{
			// SDE�ڑ�������p�^�[��3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TOKEN_NONE, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��4�@(���݂��Ȃ��T�[�o�w��)
		* @note
		*/
		TEST_METHOD(error_ptn4_server_notexist)
		{
			// SDE�ڑ�������p�^�[��4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_ERROR_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��4�@(�T�[�o�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn4_server_notspecified)
		{
			// SDE�ڑ�������p�^�[��4
			CString csConnStr = makeConnectString(CONN_PTN_4, TOKEN_NONE, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��4�@(���݂��Ȃ����[�U�w��)
		* @note
		*/
		TEST_METHOD(error_ptn4_user_notexist)
		{
			// SDE�ڑ�������p�^�[��4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_ERROR_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��4�@(���[�U�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn4_user_notspecified)
		{
			// SDE�ڑ�������p�^�[��4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TOKEN_NONE, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��4�@(���݂��Ȃ��o�[�W����)
		* @note
		*/
		TEST_METHOD(error_ptn4_version_notexist)
		{
			// SDE�ڑ�������p�^�[��4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_ERROR_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��4�@(�o�[�W�����w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn4_version_notspecified)
		{
			// SDE�ڑ�������p�^�[��4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TOKEN_NONE, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(���݂��Ȃ��T�[�o�w��)
		* @note
		*/
		TEST_METHOD(error_ptn5_server_notexist)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_ERROR_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(�T�[�o�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn5_server_notspecified)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TOKEN_NONE, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(���݂��Ȃ����[�U�w��)
		* @note
		*/
		TEST_METHOD(error_ptn5_user_notexist)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_ERROR_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(���[�U�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn5_user_notspecified)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TOKEN_NONE, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(�p�X���[�h�w����)
		* @note
		*/
		TEST_METHOD(error_ptn5_pass_miss)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_ERROR_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(�p�X���[�h�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn5_pass_notspecified)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(���݂��Ȃ��o�[�W�����w��)
		* @note
		*/
		TEST_METHOD(error_ptn5_version_notexist)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_ERROR_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(�o�[�W�����w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn5_version_notspecified)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TOKEN_NONE, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief �A�v���P�[�V�����ڑ� �ُ�ڑ��@�ڑ�������p�^�[��5�@(�|�[�g�w��Ȃ�)
		* @note
		*/
		TEST_METHOD(error_ptn5_port_notspecified)
		{
			// SDE�ڑ�������p�^�[��5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}
	};
}