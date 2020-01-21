/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

// MrxLoad.cpp: CMrxLoad �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MrxLoad.h"


/**
 * �}�b�v���l�q�w�f�[�^�N���X�̍\�z
 * @note �}�b�v���l�q�w�f�[�^�N���X���\�z���܂��B
 * @param	void
 * @retval	void
 */
CMrxLoad::CMrxLoad(){}
CMdxLoad::CMdxLoad(){}
/**
 * �}�b�v���l�q�w�f�[�^�N���X�̏���
 * @note �}�b�v���l�q�w�f�[�^�N���X�����ł��܂��B
 * @param	void
 * @retval	void
 */
CMrxLoad::~CMrxLoad()
{
	close();
}
CMdxLoad::~CMdxLoad()
{
	CMdxLoad::close();
}
/**
 * �}�b�v���l�q�w�f�[�^�J��
 * @note �}�b�v���l�q�w�f�[�^�J�����܂�
 * @param	void
 * @retval	void
 */
void CMrxLoad::close(void)
{
	for(vector<MRXNode>::iterator it1=m_MrxInfo.m_Nodes.begin(); it1!=m_MrxInfo.m_Nodes.end(); it1++ ){
		(*it1).vLinkID.clear();
	}
	m_MrxInfo.m_Nodes.clear();

	for(vector<MRXLink>::iterator it2=m_MrxInfo.m_Links.begin(); it2!=m_MrxInfo.m_Links.end(); it2++ ){
		(*it2).vLinkPos.clear();
	}
	m_MrxInfo.m_Links.clear();

	for(vector<MRXGroup>::iterator it3=m_MrxInfo.m_Group.begin(); it3!=m_MrxInfo.m_Group.end(); it3++ ){
		(*it3).vGroupID.clear();
	}
	m_MrxInfo.m_Group.clear();

	for(vector<MRXLQLink>::iterator it4=m_MrxInfo.m_LQ.begin(); it4!=m_MrxInfo.m_LQ.end(); it4++ ){
		(*it4).vLinkPos.clear();
	}
	m_MrxInfo.m_LQ.clear();
}

void CMdxLoad::close()
{
	for(vector<MDXPolygon>::iterator it=m_MdxInfo.m_Poly.begin(); it!=m_MdxInfo.m_Poly.end(); ++it ){
		(*it).vPolygonPos.clear();
	}
	m_MdxInfo.m_Poly.clear();
}
/**
 * �}�b�v���l�q�w�f�[�^�f�B�o�O�����o��
 * @note �}�b�v���l�q�w�f�[�^�f�B�o�O�����o��
 * @param	void
 * @retval	void
 */
void CMrxLoad::print(void)
{
	for(vector<MRXNode>::iterator it1=m_MrxInfo.m_Nodes.begin(); it1!=m_MrxInfo.m_Nodes.end(); it1++ ){
		for(vector<U_LONG>::iterator it=(*it1).vLinkID.begin(); it!=(*it1).vLinkID.begin(); it++ ){
			cout << "NODE "
				 << (*it) << " : "						///< �ڑ������N�ԍ�
				 << (*it1).eNodes.eLon		<< "-"		///< �o�x[1/1000�b]
				 << (*it1).eNodes.eLat		<< endl;	///< �ܓx[1/1000�b]
		}
	}

	for(vector<MRXLink>::iterator it2=m_MrxInfo.m_Links.begin(); it2!=m_MrxInfo.m_Links.end(); it2++ ){
		for(vector<MRX_Pos>:: iterator it=(*it2).vLinkPos.begin(); it!=(*it2).vLinkPos.end(); it++ ){
			cout << "LINK "
				 << (*it2).eLinks.eSnodeID  << "-"		///< �n�_�m�[�h�ԍ�
				 << (*it2).eLinks.eEnodeID  << " "		///< �I�_�m�[�h�ԍ�
				 << (*it).ePos[0] << "-" << (*it).ePos[1] << endl;
		}
	}

	for(vector<MRXGroup>::iterator it3=m_MrxInfo.m_Group.begin(); it3!=m_MrxInfo.m_Group.end(); it3++ ){
		for( vector<MRX_GroupId>::iterator it=(*it3).vGroupID.begin(); it!=(*it3).vGroupID.end(); it++ ){
			cout << "GRP "
				 << (*it).eID << " : "					///< �\�����̎��ʔԍ�
				 << (*it3).eGroup.eKindCode << "-"		///< �}�����ރR�[�h
				 << (*it3).eGroup.eAttrCode << endl;	///< �������ރR�[�h
		}
	}
}
/**
 * �w�菺���Ђl�q�w�iMapple Routing eXchange�j�m�[�h�f�[�^�̎擾
 * @note	�w�菺���Ђl�q�w�m�[�h�f�[�^���擾���܂�
 * @param	cFileHDL	(I)	MRX�f�[�^�t�@�C���n���h��
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMrxLoad::loadNodes(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	for( U_LONG i=0; i<m_MrxInfo.m_Header.eNodeNs; i++ ){
		MRXNode		aNode;
		ifp.read((char*)&aNode.eNodes,sizeof(MRX_Node));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.m_Nodes (" << i << ")" << endl;
			continue;
		}
		//�ڑ������N�ԍ����擾
		for( short j=0; j<aNode.eNodes.eLinkCount ; j++ ){
			unsigned long aLinkNo;
			ifp.read((char*)&aLinkNo,sizeof(unsigned long));
			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eNode (" << i << "/" << aNode.eNodes.eLinkCount << ")  " << endl;
				continue;
			}
			aNode.vLinkID.push_back(aLinkNo);
			if( bDBGMode ) cerr << "\r read NODE.. " <<  i+1 << "/" << m_MrxInfo.m_Header.eNodeNs << " " << aLinkNo;
		}		
		short	aRegCodeNs;
		//��ʋK���������擾
		ifp.read((char*)&aRegCodeNs,sizeof(short));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.m_Nodes.eNodeNs (" << i << ") " << endl;
			continue;
		}
		m_MrxInfo.m_Nodes.push_back(aNode);
	}
	if( bDBGMode ) cerr << endl;
	return true;
}
/**
 * �w�菺���Ђl�q�w�iMapple Routing eXchange�j�����N�f�[�^�̎擾
 * @note	�w�菺���Ђl�q�w�����N�f�[�^���擾���܂�
 * @param	cFileHDL	(I)	MRX�f�[�^�t�@�C���n���h��
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMrxLoad::loadLinks(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//�����N���I�t�Z�b�g����
	ifp.seekg(m_MrxInfo.m_Header.eLinkOffset, ios::beg);

	// �����N�v�f���̎擾
	for( U_LONG i=0; i<m_MrxInfo.m_Header.eLinkNs; i++ ){
		MRXLink		aLink;
		ifp.read((char*)&aLink.eLinks,sizeof(MRX_Link));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eLinkNs (" << i << ")" << endl;
			continue;
		}
		//�`��_���擾
		for( short j=0; j<aLink.eLinks.eCount; j++ ){
			MRX_Pos	aPos;
			ifp.read((char*)&aPos,sizeof(MRX_Pos));
			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount (" << i << "/" << m_MrxInfo.m_Header.eLinkNs << ")" << endl;
				continue;
			}
			aLink.vLinkPos.push_back(aPos);
			if( bDBGMode ){
				cerr << "\r read LINK.. " << i+1 << "/" << m_MrxInfo.m_Header.eLinkNs << " "
												 << aLink.eLinks.eSnodeID << "��"
												 << aLink.eLinks.eEnodeID << " : ("
												 << aPos.ePos[0] << " - " << aPos.ePos[1] << ")";
			}
		}
		m_MrxInfo.m_Links.push_back(aLink);
	}
	if( bDBGMode )	cerr << endl;
	return true;
}
/**
 * �w�菺���Ђl�q�w�iMapple Routing eXchange�j�O���[�v�f�[�^�̎擾
 * @note	�w�菺���Ђl�q�w�O���[�v�f�[�^���擾���܂�
 * @param	cFileHDL	(I)	MRX�f�[�^�t�@�C���n���h��
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMrxLoad::loadGroups(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//�O���[�v���I�t�Z�b�g����
	ifp.seekg(m_MrxInfo.m_Header.eGroupLineOffset, ios::beg);

	// �O���[�v���v�f���̎擾
	for( U_LONG i=0; i<m_MrxInfo.m_Header.eGroupLineNs; i++ ){
		MRXGroup	aGroup;

		ifp.read((char*)&aGroup.eGroup,sizeof(MRX_Group));


		if( !ifp ){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eGroupLineNs (" << i << ")" << endl;
			continue;
		}
		//�\�������N����擾
		for( short j=0; j<aGroup.eGroup.eCount; j++ ){
			MRX_GroupId	aGID;
			ifp.read((char*)&aGID,sizeof(MRX_GroupId));

			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount ("
					  << i << "/" << m_MrxInfo.m_Header.eGroupLineNs << ")" << endl;
				continue;
			}
			aGroup.vGroupID.push_back(aGID);
			if( bDBGMode ){
				cerr << "\r road GRP..  "
					 << i+1 << "/" << m_MrxInfo.m_Header.eGroupLineNs << " "
					 << aGroup.eGroup.eGroupID << " : ("
					 << aGID.eID << "��" << aGID.eDir << ")";
			}
		}
		m_MrxInfo.m_Group.push_back(aGroup);
	}
	if( bDBGMode )	cerr << endl;

	return true;
}
/**
 * �w�菺���Ђl�q�w�iMapple Routing eXchange�j�f�[�^��蓹�H������
 * @note	�w�菺���Ђl�q�w�O���[�v�f�[�^��蓹�H���������܂�
 * @param	void
 * @return	true�F���������Afalse�F�������s
 */
bool CMrxLoad::bindGroups(void)
{
	//�O���[�v���ƃ����N�����A�\���_��̃R���e�i���쐬����
	long aOID=1;
	int aMeshcode = atoi(m_MrxInfo.m_Header.eMeshcode);			///< �����Ѓ��b�V���R�[�h
	int aType = 0 ;
	for( aType=1; aType<=3; aType++ )						///< (1:�ʂ��݂��A2:����݂��A3:�a��)
	{
		for(vector<MRXGroup>::iterator it1=m_MrxInfo.m_Group.begin(); it1!=m_MrxInfo.m_Group.end(); it1++ )
		{
			int	aKind=0;
				 if( !strcmp((*it1).eGroup.eKindCode,"mga@@@@")){ aKind=1; }//�ʂ��݂�
			else if( !strcmp((*it1).eGroup.eKindCode,"mgb@@@@")){ aKind=2; }//����݂�
			else if( !strcmp((*it1).eGroup.eKindCode,"mgc@@@@")){ aKind=3; }//�a�ؓ��H
			else { ; }
			if( aType!=aKind ){	continue; }							///< ���C���ʂɃR���e�i�o�^

			MRXLQLink	aLQ;										///< �����N��Ǘ��R���e�i
						aLQ.eType	  = aType;						///< �����N���ʃR�[�h
						aLQ.eMeshcode = aMeshcode;					///< ���b�V���R�[�h
						aLQ.ePrimId	  = aOID++;						///< �����N��OBJECTID
						aLQ.eCount	  = 0;							///< �\���v�f��(init)
						aLQ.eGroupID  = (*it1).eGroup.eGroupID;		///< ���ʔԍ�
			for( vector<MRX_GroupId>::iterator it2=(*it1).vGroupID.begin(); it2!=(*it1).vGroupID.end(); it2++ )
			{				
				MRX_Pos	aPos;
				MRXLink	*pMrxLink;									///< ���H�����N�擾
				pMrxLink = m_MrxInfo.FindLink((*it2).eID);
				if( (*it2).eDir==1 ){								///< ������
					for(long i=0; i<pMrxLink->eLinks.eCount; i++ ){
						aPos.ePos[0] = pMrxLink->vLinkPos[i].ePos[0];
						aPos.ePos[1] = pMrxLink->vLinkPos[i].ePos[1];
						aLQ.vLinkPos.push_back(aPos);
						aLQ.eCount ++;
					}
				}
				else{												///< �t����
					for(long i=pMrxLink->eLinks.eCount-1; i>=0; i-- ){
						aPos.ePos[0] = pMrxLink->vLinkPos[i].ePos[0];
						aPos.ePos[1] = pMrxLink->vLinkPos[i].ePos[1];
						aLQ.vLinkPos.push_back(aPos);
						aLQ.eCount ++;
					}
				}
			}
			m_MrxInfo.m_LQ.push_back(aLQ);
		}
	}
#ifdef	ADDNW
//		aOID=1;
		aType=4;												///< (4:���H�����N)
		for(vector<MRXLink>::iterator it3=m_MrxInfo.m_Links.begin(); it3!=m_MrxInfo.m_Links.end(); it3++ ){
			MRXLQLink	aLQ;									///< �����N��Ǘ��R���e�i(�K�������N�P�ō\��)
						aLQ.eType	  = aType;					///< �����N���ʃR�[�h
						aLQ.eMeshcode = aMeshcode;				///< ���b�V���R�[�h
						aLQ.ePrimId	  = aOID++;					///< �����N��OBJECTID
						aLQ.eCount	  = 0;						///< �\���v�f��(init)
						aLQ.eGroupID  = it3->eLinks.eLinkNo;	///< ���H��ʃR�[�h
			for(long i=0; i<it3->eLinks.eCount; i++ ){
				MRX_Pos	aPos;
						aPos.ePos[0] = it3->vLinkPos[i].ePos[0];
						aPos.ePos[1] = it3->vLinkPos[i].ePos[1];
				aLQ.vLinkPos.push_back(aPos);
				aLQ.eCount ++;
			}
			m_MrxInfo.m_LQ.push_back(aLQ);
		}
//		aOID=1;
		aType=5;												///< (5:���H�m�[�h)
		for(vector<MRXNode>::iterator it4=m_MrxInfo.m_Nodes.begin(); it4!=m_MrxInfo.m_Nodes.end(); it4++ ){
			MRX_Pos		aPos;
						aPos.ePos[0] = it4->eNodes.eLon;
						aPos.ePos[1] = it4->eNodes.eLat;
			MRXLQLink	aLQ;									///< �����N��Ǘ��R���e�i(�K���m�[�h�P�ō\��)
						aLQ.eType	  = aType;					///< �����N���ʃR�[�h
						aLQ.eMeshcode = aMeshcode;				///< ���b�V���R�[�h
						aLQ.ePrimId	  = aOID++;					///< �����N��OBJECTID
						aLQ.eCount	  = 1;						///< �\���v�f��
						aLQ.eGroupID  = it4->eNodes.eNodeNo;	///< �m�[�h�ԍ�
						aLQ.vLinkPos.push_back(aPos);
			m_MrxInfo.m_LQ.push_back(aLQ);
		}
#endif
	
	return true;
}
/**
 * �w�菺���Ђl�q�w�iMapple Routing eXchange�j�f�[�^�̎擾
 * @note	�w�菺���Ђl�q�w�f�[�^���擾���܂�
 * @param	cFileStr	(I)	MRX�f�[�^�ւ̃p�X
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMrxLoad::load(char *cFileStr)
{
	bool	bDBGMode=false;

#ifdef	_DEBUG
	cerr << "Open [" << cFileStr << "]" << endl;	bDBGMode = true;
#endif
	//�w��l�q�w�t�@�C�����I�[�v��
	ifstream aFileStream(CA2CT(cFileStr), ios::in | ios::binary);

	if(!aFileStream){
		cerr << "ERROR CMrxLoad::OpenFile() [" << cFileStr << "]" << endl;
		return false;
	}

	//�w�b�_�[���̎擾
	aFileStream.read((char*)&m_MrxInfo.m_Header,sizeof(MRXHead));
	if(!aFileStream){
		cerr << "ERROR CMrxLoad::read() m_MrxInfo.m_Header " << endl;
		return false;
	}
	// �m�[�h�v�f���̎擾
	if( !loadNodes(aFileStream)){
		return false;
	}
	// �����N�v�f���̎擾
	if( !loadLinks(aFileStream)){
		return false;
	}
	// �O���[�v���v�f���̎擾
	if( !loadGroups(aFileStream)){
		return false;
	}
	if( !bindGroups()){	///< �O���[�v��񖈂ɓ��H��ڑ�
		return false;
	}
	//�w��l�q�w�t�@�C�����N���[�Y
	aFileStream.close();

	return true;
}

/**
 * �w�菺���Ђl�c�w�iMapple Drawing eXchange�j�f�[�^�̎擾
 * @note	�w�菺���Ђl�c�w�f�[�^���擾���܂�
 * @param	cFileStr	(I)	MDX�f�[�^�ւ̃p�X
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMdxLoad::load(char *cFileStr)
{
	bool	bDBGMode=false;

#ifdef	_DEBUG
	cerr << "Open [" << cFileStr << "]" << endl;	bDBGMode = true;
#endif

	ifstream aFileStream(CA2CT(cFileStr), ios::in | ios::binary);

	//�w��l�c�w�t�@�C�����I�[�v��
	if(!aFileStream)
	{
		cerr << "ERROR CMrxLoad::OpenFile() [" << cFileStr << "]" << endl;
		return false;
	}
	//�w�b�_�[���̎擾
	aFileStream.read((char*)&m_MdxInfo.m_Header,sizeof(MDXHead));
	if( !aFileStream )
	{
		cerr << "ERROR CMdxLoad::read() m_MdxInfo.m_Header " << endl;
		return false;
	}

	// �����N���v�f���̎擾
	if( !loadLinks(aFileStream)){
		return false;
	}

	// �|�����C�����v�f���̎擾
	if( !loadGroups(aFileStream)){
		return false;
	}

	// �|���S�����v�f���̎擾
	if( !loadPoly(aFileStream)){
		return false;
	}
	//�w��l�c�w�t�@�C�����N���[�Y
	aFileStream.close();

	return true;
}

/**
 * �w�菺���Ђl�c�w�|���S���f�[�^�̎擾
 * @note	�w�菺���Ђl�c�w�|���S���f�[�^���擾���܂�
 * @param	cFileHDL	(I)	MDX�f�[�^�t�@�C���n���h��
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMdxLoad::loadPoly(ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif

	ifp.seekg(m_MdxInfo.m_Header.ePolyOffset, ios::beg);
	//�����N���I�t�Z�b�g����

	// �|���S���v�f���̎擾
	for( U_LONG i=0; i<m_MdxInfo.m_Header.ePolyNs; ++i )
	{
		MDXPolygon		aPoly;
		ifp.read((char*)&aPoly.eMdxPoly,sizeof(MDX_Polygon));

		if(!ifp )
		{
			cerr << "ERROR CMdxLoad::read() m_MdxInfo.ePolyNs (" << i << ")" << endl;
			continue;
		}
		//�`��_���擾
		for( short j=0; j<aPoly.eMdxPoly.eCount; j++ )
		{
			MRX_Pos	aPos;
			ifp.read((char*)&aPos,sizeof(MRX_Pos));

			 if(!ifp )
			{
				cerr << "ERROR CMdxLoad::read() m_MdxInfo.eCount (" << i << "/" << m_MdxInfo.m_Header.ePolyNs << ")" << endl;
				continue;
			}
			aPoly.vPolygonPos.push_back(aPos);
			if( bDBGMode )
			{
				cerr << "\r read Polygon.. " << i+1 << "/" << m_MdxInfo.m_Header.ePolyNs << " " 
					<< aPoly.eMdxPoly.eLavel << " : (" << aPos.ePos[0] << " - " << aPos.ePos[1] << ")";
			}
		}
		m_MdxInfo.m_Poly.push_back(aPoly);
	}
	if( bDBGMode )	cerr << endl;
	return true;
}

/**
 * �w�菺���Ђl�c�w�iMapple Drawing eXchange�j�O���[�v���f�[�^�̎擾
 * @note	�w�菺���Ђl�c�w�O���[�v���f�[�^���擾���܂�
 * @param	cFileHDL	(I)	MDX�f�[�^�t�@�C���n���h��
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMdxLoad::loadGroups(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//�O���[�v���I�t�Z�b�g����
	ifp.seekg(m_MdxInfo.m_Header.eGroupLineOffset, ios::beg);

	// �O���[�v���v�f���̎擾
	for( U_LONG i=0; i<m_MdxInfo.m_Header.eGroupLineNs; i++ ){
		MRXGroup	aGroup;

		ifp.read((char*)&aGroup.eGroup,sizeof(MRX_Group));


		if( !ifp ){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eGroupLineNs (" << i << ")" << endl;
			continue;
		}
		//�\�������N����擾
		for( short j=0; j<aGroup.eGroup.eCount; j++ ){
			MRX_GroupId	aGID;
			ifp.read((char*)&aGID,sizeof(MRX_GroupId));

			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount ("
					  << i << "/" << m_MdxInfo.m_Header.eGroupLineNs << ")" << endl;
				continue;
			}
			aGroup.vGroupID.push_back(aGID);
			if( bDBGMode ){
				cerr << "\r road GRP..  "
					 << i+1 << "/" << m_MdxInfo.m_Header.eGroupLineNs << " "
					 << aGroup.eGroup.eGroupID << " : ("
					 << aGID.eID << "��" << aGID.eDir << ")";
			}
		}
		m_MdxInfo.m_Group.push_back(aGroup);
	}
	if( bDBGMode )	cerr << endl;

	return true;
}

/**
 * �w�菺���Ђl�c�w�iMapple Drawing eXchange�j���f�[�^�̎擾
 * @note	�w�菺���Ђl�c�w���f�[�^���擾���܂�
 * @param	cFileHDL	(I)	MDX�f�[�^�t�@�C���n���h��
 * @return	true�F�擾�����Afalse�F�擾���s
 */
bool CMdxLoad::loadLinks(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//�����N���I�t�Z�b�g����
	ifp.seekg(m_MdxInfo.m_Header.eLineOffset, ios::beg);

	// �����N�v�f���̎擾
	for( U_LONG i=0; i<m_MdxInfo.m_Header.eLineNs; i++ ){
		MDXLink		aLink;
		ifp.read((char*)&aLink.eMdxLink,sizeof(MDX_Link));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eLinkNs (" << i << ")" << endl;
			continue;
		}
		//�`��_���擾
		for( short j=0; j<aLink.eMdxLink.eCount; j++ ){
			MRX_Pos	aPos;
			ifp.read((char*)&aPos,sizeof(MRX_Pos));
			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount (" << i << "/" << m_MdxInfo.m_Header.eLineNs << ")" << endl;
				continue;
			}
			aLink.vLinkPos.push_back(aPos);
			if( bDBGMode ){
				cerr << "\r read LINK.. " << i+1 << "/" << m_MdxInfo.m_Header.eLineNs << " "
												 << aPos.ePos[0] << " - " << aPos.ePos[1] << ")";
			}
		}
		m_MdxInfo.m_Links.push_back(aLink);
	}
	if( bDBGMode )	cerr << endl;
	return true;
}
