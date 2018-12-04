#pragma once

#include <ExportDef.h>
#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <libCUFEM\BaseReader.h>
#include <matlab\mat.h>


#pragma warning(disable: 4996)

namespace HBXFEMDef
{


	class CUFEM_API UFgeMatReader :
		public BaseReader
	{
	private:
		typedef std::map<std::string, mxArray*>::iterator	mx_Map_itr;

		std::map<std::string, mxArray*>	_mxArray_Map;

		MATFile*	m_pMatFile;	//文件指针
		mxArray*	m_pMxArray;	//数组向量指针

		mwSize m_RowNum, m_ColNum;	//矩阵的行列数
		size_t m_id;				//矩阵在标准库中的ID号

		HBXDef::_CSRInput<HBXDef::UserCalPrec> m_MatMsg;

	protected:
		//用于读取单个矩阵的相关信息
		::ImpMatError_t ReadObject(const mxArray *const _Array);
		//用于读取UF_Index内的信息
		::ImpMatError_t ReadMsg(const mxArray* const _Array);
		::ImpMatError_t ReadSparseMat(const mxArray *const _Array);
		::ImpMatError_t ReadRhs(const mxArray *const _Array);
		::ImpMatError_t ReadCell(const mxArray* const _Array);

	public:
		UFgeMatReader(const char* _matname, boost::filesystem::path _searchpath = "C:\\Users\\hbx\\Desktop\\SuiteSparse-2.1.1\\UFget");
		~UFgeMatReader(void);

		//完成inp\xml\PW文件的导入，填充各容器并计算总刚矩阵的维度等参数
		bool SetInputData();

		//强制断开所有数据连接
		void terminate();

		//获取稀疏矩阵模板函数
		void* GetStiffMat( bool _bSv = false );

		//输出系统方程右端向量
		void* GetRhsVec( bool _bSv = false );
		
		//输出nnA的长度
		int& GetNoneZeroLgt();
		//输出rowsort的长度，即rownum+1
		int& GetnALgt();
		//获取当前矩阵的ID号，因为某些矩阵的CSR格式其rowsort有问题
		int& GetID();

	}
}
