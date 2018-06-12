#include <HbxDefMacro.h>
#include <libInterpolate\interval_lib.h>
//#include <libInterpolate\cuInterval_kernel.cuh>
#include <libInterpolate\spBaseLag.h>
#include <libInterpolate\interval_lib.h>

#include <cooperative_groups.h>

//CUDA��ֵ����У�����
template< unsigned int T>
bool check_interpolate( HBXDef::cuTable<T>* _table,
	HBXDef::cuLocation<T>* h_loc,
	HBXDef::UserCalPrec* h_result,
	unsigned int _num)
{
	//�ڴ˵���sp��CPU���ֵ��

	
	baselag<T>* thelag = new baselag<T>(_table, 0);

	for (unsigned int i = 0; i < _num; i++)
	{
		h_result[i] = thelag->get(h_loc[i].m_Loc);
	}



}