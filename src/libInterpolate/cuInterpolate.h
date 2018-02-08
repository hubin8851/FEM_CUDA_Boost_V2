#pragma once


template<unsigned int _T = 2, HBXDef::HbxCuDef::CudaMalloc_t _M = HBXDef::HbxCuDef::CudaMalloc_t::NORMAL>
class cuCElastic
{
private:
	typedef cuCElastic<_T, _M>	_sameCElastic;
}

#ifdef __cplusplus
extern "C" {
#endif

	void cuInterpolate( HBXDef::_AEROTABLE& tableIn, int _streamNum,  );


#ifdef __cplusplus
}
#endif