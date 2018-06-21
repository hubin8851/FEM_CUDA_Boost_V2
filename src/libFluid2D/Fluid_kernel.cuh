#pragma once

#include <HbxDefMacro.h>
#include <CuDefMacro.h>
#include <HbxCudaDef\SharedMem.cuh>

//�ú˺�����ӳ��������ٶ���v��ͨ��ŷ�����ֵ���ʽ
//v(x,t+1) = v(x,t) + dt * f.
//δ�������޸Ĳ��ö�׵���ʽ��ʹ�㷨���ȶ����ȸ���
//@v:�ٶ���
//@dx:x����������
//@dy:y����������
//@spx:ʩ��x������ʼ��������
//@spy:ʩ��y������ʼ��������
//@fx:x���������
//@fy:Y���������
//@r:�����µİ뾶,�൱��ͼ�����еġ��ˡ���С
//@pitch:�����ڴ����
__global__ void addForces_k( HBXDef::HbxCuDef::UserCalPrec2 *v, int dx, int dy, int spx, int spy, float fx, float fy, int r, size_t pitch );

//ģ���ٶ�����������ÿ�������ڵ��ٶ���������
//v(x,t+1) = v(p(x,-dt),t)�����ٶȿռ��ڲ���˫���Բ�ֵ
//@v:�ٶ���
//@vx:x�����ٶ�����
//@vy:y�����ٶ�����
//@dx:x����������
//@pdx:��ַ������x����������
//@dy��y����������
//@pdy:
//@dt:����
//@lb:��ʾÿ���߳���Ҫ������е���������tiley/tidsy��ʾ
__global__ void advectVelocity_k(	HBXDef::HbxCuDef::UserCalPrec2 *v, HBXDef::UserCalPrec *vx, HBXDef::UserCalPrec *vy,
									int dx, int pdx, int dy, float dt, int lb);
