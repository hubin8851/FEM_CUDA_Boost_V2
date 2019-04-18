#include "ElementStiffCal.h"

__device__ EltMatFunc_t EltMatFunc_table[2];

//主机端的单元矩阵计算列表，可拷贝至设备端
EltMatFunc_t h_EltMatFunc_table[2];


void EletStiffCal(HBXDef::HbxCuDef::EltMatCalworkload * _workload)
{
//	EltMatFunc_t pOption = h_EltMatFunc_table[_workload->id];

}
