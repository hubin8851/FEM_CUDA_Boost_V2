#pragma once
#include <ExportDef.h>
#include <HbxCudaDef\HbxCuDefStruct.h>

//所有单元刚度计算的接口
CUFEM_API void  EletStiffCal( HBXDef::HbxCuDef::EltMatCalworkload* _workload );