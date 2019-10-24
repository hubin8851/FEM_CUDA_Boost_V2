#pragma once

#include <ExportDef.h>
#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <libCUFEM\BaseReader.h>


#pragma warning(disable: 4996)

namespace HBXFEMDef
{

	//Matrix Market里面的SparseMat的读取
	class CUFEM_API MMSpMatReader :
		public BaseReader
	{
	private:
		char m_ElemetType;
		bool MatSaveFormat;
		HBXDef::_CSRInput<HBXDef::UserCalPrec> m_MatMsg;

	protected:
	public:
		MMSpMatReader(	const char* _matname = "bcsstk01.mtx", 
						const char* _searchpath = "E:\\Phd_Hu_Data\\matrix market",
						bool _csrFormat = true);
		virtual ~MMSpMatReader(void);

		//完成mtx文件的导入，对矩阵维度进行校验并放入稀疏矩阵结构体中
		InputFileResult SetInputData();

		//强制断开所有数据连接
		void terminate() {};

		//获取CSR格式稀疏矩阵
		HBXDef::_CSRInput<HBXDef::UserCalPrec>* GetStiffMat(bool _bSv = false);

		//输出nnA的长度
		int& GetNoneZeroLgt();
		//输出rowsort的长度，即rownum+1
		int& GetnALgt();
	};

	CUFEM_API BaseReader* InstanceMMSpMatReader(const char* _name = "", const char* _path = "");



}
