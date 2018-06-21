#pragma once

#include <HbxDefMacro.h>
#include <CuDefMacro.h>
#include <HbxCudaDef\SharedMem.cuh>

//该核函数添加常量力至速度域v，通过欧拉积分的形式
//v(x,t+1) = v(x,t) + dt * f.
//未来可能修改采用多阶的形式，使算法更稳定精度更高
//@v:速度域
//@dx:x方向网格数
//@dy:y方向网格数
//@spx:施力x方向起始网格坐标
//@spy:施力y方向起始网格坐标
//@fx:x方向冲量？
//@fy:Y方向冲量？
//@r:力更新的半径,相当于图像处理中的“核”大小
//@pitch:纹理内存对齐
__global__ void addForces_k( HBXDef::HbxCuDef::UserCalPrec2 *v, int dx, int dy, int spx, int spy, float fx, float fy, int r, size_t pitch );

//模拟速度流动，回溯每个网格内的速度向量，有
//v(x,t+1) = v(p(x,-dt),t)，在速度空间内采用双线性插值
//@v:速度域
//@vx:x方向速度向量
//@vy:y方向速度向量
//@dx:x方向网格数
//@pdx:地址对齐后的x方向网格数
//@dy：y方向网格数
//@pdy:
//@dt:步长
//@lb:表示每个线程需要处理的列的数量，用tiley/tidsy表示
__global__ void advectVelocity_k(	HBXDef::HbxCuDef::UserCalPrec2 *v, HBXDef::UserCalPrec *vx, HBXDef::UserCalPrec *vy,
									int dx, int pdx, int dy, float dt, int lb);
