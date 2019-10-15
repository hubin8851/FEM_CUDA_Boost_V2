#pragma once

#include <ExportDef.h>
#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <libCUFEM\BaseReader.h>
#include <matlab\mat.h>


#pragma warning(disable: 4996)

namespace HBXFEMDef
{

	//UFget的读取类，适用于UFget文件中matlab的*.mat文件
	//UFgetMatReader的数据在m_pMatFile内，所以CSR格式的矩阵结构体仅提供指针，不额外分配内存
	class CUFEM_API UFgetMatReader :
		public BaseReader
	{
	private:
		typedef std::map<std::string, mxArray*>::iterator	mx_Map_itr;

		//mat文件中，项目-数据 的映射表
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
		UFgetMatReader(const char* _matname = "662_bus.mat", const char* _searchpath = "C:\\Users\\hbx\\Desktop\\SuiteSparse-2.1.1\\UFget");
		virtual ~UFgetMatReader(void);

		//完成inp\xml\PW文件的导入，填充各容器并计算总刚矩阵的维度等参数
		InputFileResult SetInputData();

		//强制断开所有数据连接
		void terminate() {};

		//获取CSR格式稀疏矩阵
		HBXDef::_CSRInput<HBXDef::UserCalPrec>* GetStiffMat( bool _bSv = false );

		//输出系统方程右端向量
		void* GetRhsVec( bool _bSv = false );
		
		//输出nnA的长度
		int& GetNoneZeroLgt();
		//输出rowsort的长度，即rownum+1
		int& GetnALgt();
		//获取当前矩阵的ID号，因为某些矩阵的CSR格式其rowsort有问题
		size_t& GetID();

	};


	CUFEM_API BaseReader* InstanceUFgetReader(const char* _name = "", const char* _path = "");
}
