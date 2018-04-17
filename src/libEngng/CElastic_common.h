//该文件虽然放置于Engng内，但是可能并不与其他的Engng并用

#ifndef  _CELASTIC_COMMON_H_
#define  _CELASTIC_COMMON_H_

#pragma warning(disable:4786) 

#include <ctime>
#include <boost/random.hpp>

#include "HbxDefMacro.h"

namespace HBXDef
{

#ifndef numElastic
#define numElastic 5
#endif // !numElastic

#ifndef nsMax
#define nsMax	400	//站点数目
#endif // !nsMax


	struct TrajInput	//弹道模块输入
	{
		HBXDef::UserCalPrec nx1;		//轴向过载
		HBXDef::UserCalPrec V0;		//火箭飞行速度
		HBXDef::UserCalPrec Q;			//速度头,动压？
		HBXDef::UserCalPrec Cna;		//cna
		HBXDef::UserCalPrec alfa0;		//攻角
	};

	void GenTraj(TrajInput &_this);

	struct d_TrajInput	//弹道模块输入
	{
		HBXDef::UserCalPrec nx1;		//轴向过载
		HBXDef::UserCalPrec V0;			//火箭飞行速度
		HBXDef::UserCalPrec Q;			//速度头
		HBXDef::UserCalPrec *Cna;		//cna
		HBXDef::UserCalPrec *Aero_Ni;	//分布气动力、力矩
		HBXDef::UserCalPrec alfa0;		//攻角

// 		~d_TrajInput()
// 		{
// 			delete[] Cna;
// 			delete[] Aero_Ni;
// 		}
	};

	struct MonitorInput	//发动机模块输入
	{
		HBXDef::UserCalPrec P[3];//发动机推力
		HBXDef::UserCalPrec Fei[3];	//发动机摆动惯性力
		HBXDef::UserCalPrec Mx_P;//发动机推力矩
		HBXDef::UserCalPrec Mx_Fei;//发动机摆动惯性力矩
	};

	struct d_MonitorInput	//发动机模块输入
	{
		HBXDef::UserCalPrec P[3];//发动机推力
		HBXDef::UserCalPrec Fei[3];	//发动机摆动惯性力
		HBXDef::UserCalPrec Mx_P;//发动机推力矩
		HBXDef::UserCalPrec Mx_Fei;//发动机摆动惯性力矩
	};

	void GenMonitor(MonitorInput& _this);

	struct ShakeInput	//贮箱模块输入
	{
		HBXDef::UserCalPrec Fslosh[3];//晃动惯性力
		HBXDef::UserCalPrec Mx_Fslosh;//晃动惯性力矩
	};

	struct d_ShakeInput	//贮箱模块输入
	{
		HBXDef::UserCalPrec Fslosh[3];//晃动惯性力
		HBXDef::UserCalPrec Mx_Fslosh;//晃动惯性力矩
	};

	void GenShake(ShakeInput& _this);

	struct  SepInput	//分离模块
	{
		double Fd[3];//分离干扰力
		double Fdx;//分离干扰力矩
	};

	struct MassInput	//质量模块
	{
		HBXDef::UserCalPrec* StationMass;	//分布质量
		HBXDef::UserCalPrec* xStation;//站点坐标
		HBXDef::UserCalPrec* mx;		//站点处力矩系数（谦哥标注）
		~MassInput()
		{
			delete[] StationMass;
			StationMass = nullptr;
			delete[] xStation;
			xStation = nullptr;
			delete[] mx;
			mx = nullptr;
		}
	};

	struct d_MassInput	//质量模块
	{
		HBXDef::UserCalPrec* StationMass;	//分布质量
		HBXDef::UserCalPrec* xStation;//站点坐标
		HBXDef::UserCalPrec* mx;		//站点处力矩系数（谦哥标注）
	};

	//堆上分配空间，速度较慢。
	void GenMassInput(MassInput& _this, size_t _lght);

	template<class T = HBXDef::UserCalPrec>
	struct OriginData	//原始（初始化）输入数据
	{
		T ShapeStationElastic[3][numElastic][nsMax];//振型,阶数在前，因为第三个维度的内存连续，便于合并访问
		T DShapeStationElastic[3][numElastic][nsMax];//振型斜率
		//此处涉及共计8个需要插值的数据
		T xGyroS;	//速率陀螺安装位置
		T xAcce;	//加表安装位置
		T xIns;	//捷联惯组安装位置
		T xEr;		//发动机摆动喷管位置
		T xServo_B[4];//助推器1234伺服机构位置
		//end 此处涉及共计8个需要插值的数据
		T Omiga[numElastic];//振动频率
		T ksi[numElastic];	//阻尼系数
		T L;//参考长度
		T S;//参考截面
		T qi[numElastic];	//广义位移
		T Dqi[numElastic];	//广义速度
	};

	template< class T = HBXDef::UserCalPrec >
	struct d_OriginData	//原始（初始化）输入数据
	{
		T* ShapeStationElastic;//振型:方向、站点、阶数
		T* DShapeStationElastic;//振型斜率
		T xGyroS;	//速率陀螺安装位置
		T xAcce;	//加表安装位置
		T xIns;	//捷联惯组安装位置
		T xEr;		//发动机摆动喷管位置
		T xServo_B[4];	//助推器1234伺服机构位置
		T Omiga[numElastic];//振动频率
		T ksi[numElastic];	//阻尼系数
		T L;//参考长度
		T *S;//参考截面
		T qi[numElastic];	//广义位移
		T Dqi[numElastic];	//广义速度
	};

	void GenElasticData(OriginData<>& _this, size_t _lght);
}	//end namespace

#ifdef __cplusplus
extern "C" {
#endif

	//插值计算Xpos处振型
	//@k:轴法横某方向
	//@xj:待插值点位置
	//@iE:阶数
	//@_dataIn:传入的振动模块的相关数据
	//@_massIn:传入的质量信息

	double cuGetXposShape(int k, double xj, int iE, HBXDef::OriginData<>* _dataIn, HBXDef::MassInput& _massIn);

	double cuGetXposDShape(int k, double xj, int iE, HBXDef::OriginData<>* _dataIn, HBXDef::MassInput& _massIn);

	//弹性分布气动力计算
	//@k:轴法横某方向
	//@_TrajIn:传入的弹道模块相关数据
	//@_dataIn:传入的振动模块的相关数据
	//@_massIn:传入的质量信息
//	void cuElastic_Aero_Force(int k, HBXDef::d_TrajInput _TrajIn, HBXDef::d_OriginData<>& d_ElasticIn, HBXDef::d_MassInput& _massIn, unsigned int _stationNum);

	//广义力计算

//	void cuGen_Force_Cal(int k, HBXDef::MonitorInput A, HBXDef::OriginData<> B, HBXDef::MonitorInput C, HBXDef::ShakeInput D);

#ifdef __cplusplus
}
#endif


#endif // _CELASTIC_COMMON_H_