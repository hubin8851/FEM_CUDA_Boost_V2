#pragma once
#include <HbxDefMacro.h>
#include <CudaPreDef.h>
#include <CuDefMacro.h>
#include <CuDefStruct.h.h>

//基础函数
void updateTexture(HBXDef::HbxCuDef::UserCalPrec2 *data, size_t w, size_t h, size_t pitch);


//添加外部力的函数
//@v:速度域
//@dx:x方向网格数
//@dy:y方向网格数
//@spx:施力x方向起始网格坐标
//@spy:施力y方向起始网格坐标
//@fx:x方向冲量？
//@fy:Y方向冲量？
//@r:力更新的半径,相当于图像处理中的“核”大小
void __declspec(dllexport) addForces(	HBXDef::HbxCuDef::UserCalPrec2 *v, 
										int dx, int dy, int spx, int spy, float fx, float fy, int r);

//添加速度增量
//@v:速度域
//@vx:
//@vy:
//@dx:x方向网格数
//@pdx:修正的实数->复数域的FFT变换宽度。
//@dy:y方向网格数
//@dt:时间步长
void __declspec(dllexport) advectVelocity(	HBXDef::HbxCuDef::UserCalPrec2 *v, 
											HBXDef::UserCalPrec *vx, HBXDef::UserCalPrec *vy, 
											int dx, int pdx, int dy, HBXDef::UserCalPrec dt,
											int xTile, int yTile,
											int xTidsPerTile, int yTidsPerTile,
											int _xDim, int _yDim);

//在频域中计算速度扩散和力守恒。
//输入vx和vy均为复数向量
//@vx:x方向速度
//@vy:y方向速度
//@dx:x方向长度
//@dy:y方向长度
//@dt:步长
//@visc:流体的黏性
void __declspec(dllexport) diffuseProject(	HBXDef::HbxCuDef::UserCalPrec2 *vx, 
											HBXDef::HbxCuDef::UserCalPrec2 *vy,
											int dx, int dy, float dt, float visc);

void __declspec(dllexport) updateVelocity(HBXDef::HbxCuDef::UserCalPrec2 *v, float *vx, float *vy, int dx, int pdx, int dy);
void __declspec(dllexport) advectParticles(int vbo, HBXDef::HbxCuDef::UserCalPrec2 *v, int dx, int dy, float dt);


