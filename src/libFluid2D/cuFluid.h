#pragma once
#include <HbxDefMacro.h>
#include <CudaPreDef.h>
#include <CuDefMacro.h>
#include <CuDefStruct.h.h>

//��������
void updateTexture(HBXDef::HbxCuDef::UserCalPrec2 *data, size_t w, size_t h, size_t pitch);


//����ⲿ���ĺ���
//@v:�ٶ���
//@dx:x����������
//@dy:y����������
//@spx:ʩ��x������ʼ��������
//@spy:ʩ��y������ʼ��������
//@fx:x���������
//@fy:Y���������
//@r:�����µİ뾶,�൱��ͼ�����еġ��ˡ���С
void __declspec(dllexport) addForces(	HBXDef::HbxCuDef::UserCalPrec2 *v, 
										int dx, int dy, int spx, int spy, float fx, float fy, int r);

//����ٶ�����
//@v:�ٶ���
//@vx:
//@vy:
//@dx:x����������
//@pdx:������ʵ��->�������FFT�任��ȡ�
//@dy:y����������
//@dt:ʱ�䲽��
void __declspec(dllexport) advectVelocity(	HBXDef::HbxCuDef::UserCalPrec2 *v, 
											HBXDef::UserCalPrec *vx, HBXDef::UserCalPrec *vy, 
											int dx, int pdx, int dy, HBXDef::UserCalPrec dt,
											int xTile, int yTile,
											int xTidsPerTile, int yTidsPerTile,
											int _xDim, int _yDim);

//��Ƶ���м����ٶ���ɢ�����غ㡣
//����vx��vy��Ϊ��������
//@vx:x�����ٶ�
//@vy:y�����ٶ�
//@dx:x���򳤶�
//@dy:y���򳤶�
//@dt:����
//@visc:��������
void __declspec(dllexport) diffuseProject(	HBXDef::HbxCuDef::UserCalPrec2 *vx, 
											HBXDef::HbxCuDef::UserCalPrec2 *vy,
											int dx, int dy, float dt, float visc);

void __declspec(dllexport) updateVelocity(HBXDef::HbxCuDef::UserCalPrec2 *v, float *vx, float *vy, int dx, int pdx, int dy);
void __declspec(dllexport) advectParticles(int vbo, HBXDef::HbxCuDef::UserCalPrec2 *v, int dx, int dy, float dt);


