#pragma once
class BaseMesh
{
public:
	BaseMesh(const IFeatureClassPtr& ipFeatureClass);
	~BaseMesh(void);


	/*
	 *	@brief	���b�V���`��擾
	 *	@param	secondMesh [in] �Ώۃ��b�V��
	 *	@retval	true ����
	 *	@retval false ���s
	 */
	bool SetGeometry(const long secondMesh);

	/*
	 *	@brief	���b�V���`��o��
	 *	@return	���b�V���`��
	 *
	 */
	IGeometryPtr GetGeometry(){
		return m_ipGeometry;
	}

private:
	IFeatureClassPtr m_ipFeatureClass;
	IGeometryPtr m_ipGeometry;

};

