#pragma once
class BaseMesh
{
public:
	BaseMesh(const IFeatureClassPtr& ipFeatureClass);
	~BaseMesh(void);


	/*
	 *	@brief	メッシュ形状取得
	 *	@param	secondMesh [in] 対象メッシュ
	 *	@retval	true 成功
	 *	@retval false 失敗
	 */
	bool SetGeometry(const long secondMesh);

	/*
	 *	@brief	メッシュ形状出力
	 *	@return	メッシュ形状
	 *
	 */
	IGeometryPtr GetGeometry(){
		return m_ipGeometry;
	}

private:
	IFeatureClassPtr m_ipFeatureClass;
	IGeometryPtr m_ipGeometry;

};

