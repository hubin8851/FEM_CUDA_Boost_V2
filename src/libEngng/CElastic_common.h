//���ļ���Ȼ������Engng�ڣ����ǿ��ܲ�����������Engng����

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
#define nsMax	400	//վ����Ŀ
#endif // !nsMax


	struct TrajInput	//����ģ������
	{
		HBXDef::UserCalPrec nx1;		//�������
		HBXDef::UserCalPrec V0;		//��������ٶ�
		HBXDef::UserCalPrec Q;			//�ٶ�ͷ,��ѹ��
		HBXDef::UserCalPrec Cna;		//cna
		HBXDef::UserCalPrec alfa0;		//����
	};

	void GenTraj(TrajInput &_this);

	struct d_TrajInput	//����ģ������
	{
		HBXDef::UserCalPrec nx1;		//�������
		HBXDef::UserCalPrec V0;			//��������ٶ�
		HBXDef::UserCalPrec Q;			//�ٶ�ͷ
		HBXDef::UserCalPrec *Cna;		//cna
		HBXDef::UserCalPrec *Aero_Ni;	//�ֲ�������������
		HBXDef::UserCalPrec alfa0;		//����

// 		~d_TrajInput()
// 		{
// 			delete[] Cna;
// 			delete[] Aero_Ni;
// 		}
	};

	struct MonitorInput	//������ģ������
	{
		HBXDef::UserCalPrec P[3];//����������
		HBXDef::UserCalPrec Fei[3];	//�������ڶ�������
		HBXDef::UserCalPrec Mx_P;//������������
		HBXDef::UserCalPrec Mx_Fei;//�������ڶ���������
	};

	struct d_MonitorInput	//������ģ������
	{
		HBXDef::UserCalPrec P[3];//����������
		HBXDef::UserCalPrec Fei[3];	//�������ڶ�������
		HBXDef::UserCalPrec Mx_P;//������������
		HBXDef::UserCalPrec Mx_Fei;//�������ڶ���������
	};

	void GenMonitor(MonitorInput& _this);

	struct ShakeInput	//����ģ������
	{
		HBXDef::UserCalPrec Fslosh[3];//�ζ�������
		HBXDef::UserCalPrec Mx_Fslosh;//�ζ���������
	};

	struct d_ShakeInput	//����ģ������
	{
		HBXDef::UserCalPrec Fslosh[3];//�ζ�������
		HBXDef::UserCalPrec Mx_Fslosh;//�ζ���������
	};

	void GenShake(ShakeInput& _this);

	struct  SepInput	//����ģ��
	{
		double Fd[3];//���������
		double Fdx;//�����������
	};

	struct MassInput	//����ģ��
	{
		HBXDef::UserCalPrec* StationMass;	//�ֲ�����
		HBXDef::UserCalPrec* xStation;//վ������
		HBXDef::UserCalPrec* mx;		//վ�㴦����ϵ����ǫ���ע��
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

	struct d_MassInput	//����ģ��
	{
		HBXDef::UserCalPrec* StationMass;	//�ֲ�����
		HBXDef::UserCalPrec* xStation;//վ������
		HBXDef::UserCalPrec* mx;		//վ�㴦����ϵ����ǫ���ע��
	};

	//���Ϸ���ռ䣬�ٶȽ�����
	void GenMassInput(MassInput& _this, size_t _lght);

	template<class T = HBXDef::UserCalPrec>
	struct OriginData	//ԭʼ����ʼ������������
	{
		T ShapeStationElastic[3][numElastic][nsMax];//����,������ǰ����Ϊ������ά�ȵ��ڴ����������ںϲ�����
		T DShapeStationElastic[3][numElastic][nsMax];//����б��
		//�˴��漰����8����Ҫ��ֵ������
		T xGyroS;	//�������ݰ�װλ��
		T xAcce;	//�ӱ�װλ��
		T xIns;	//�������鰲װλ��
		T xEr;		//�������ڶ����λ��
		T xServo_B[4];//������1234�ŷ�����λ��
		//end �˴��漰����8����Ҫ��ֵ������
		T Omiga[numElastic];//��Ƶ��
		T ksi[numElastic];	//����ϵ��
		T L;//�ο�����
		T S;//�ο�����
		T qi[numElastic];	//����λ��
		T Dqi[numElastic];	//�����ٶ�
	};

	template< class T = HBXDef::UserCalPrec >
	struct d_OriginData	//ԭʼ����ʼ������������
	{
		T* ShapeStationElastic;//����:����վ�㡢����
		T* DShapeStationElastic;//����б��
		T xGyroS;	//�������ݰ�װλ��
		T xAcce;	//�ӱ�װλ��
		T xIns;	//�������鰲װλ��
		T xEr;		//�������ڶ����λ��
		T xServo_B[4];	//������1234�ŷ�����λ��
		T Omiga[numElastic];//��Ƶ��
		T ksi[numElastic];	//����ϵ��
		T L;//�ο�����
		T *S;//�ο�����
		T qi[numElastic];	//����λ��
		T Dqi[numElastic];	//�����ٶ�
	};

	void GenElasticData(OriginData<>& _this, size_t _lght);
}	//end namespace

#ifdef __cplusplus
extern "C" {
#endif

	//��ֵ����Xpos������
	//@k:�ᷨ��ĳ����
	//@xj:����ֵ��λ��
	//@iE:����
	//@_dataIn:�������ģ����������
	//@_massIn:�����������Ϣ

	double cuGetXposShape(int k, double xj, int iE, HBXDef::OriginData<>* _dataIn, HBXDef::MassInput& _massIn);

	double cuGetXposDShape(int k, double xj, int iE, HBXDef::OriginData<>* _dataIn, HBXDef::MassInput& _massIn);

	//���Էֲ�����������
	//@k:�ᷨ��ĳ����
	//@_TrajIn:����ĵ���ģ���������
	//@_dataIn:�������ģ����������
	//@_massIn:�����������Ϣ
//	void cuElastic_Aero_Force(int k, HBXDef::d_TrajInput _TrajIn, HBXDef::d_OriginData<>& d_ElasticIn, HBXDef::d_MassInput& _massIn, unsigned int _stationNum);

	//����������

//	void cuGen_Force_Cal(int k, HBXDef::MonitorInput A, HBXDef::OriginData<> B, HBXDef::MonitorInput C, HBXDef::ShakeInput D);

#ifdef __cplusplus
}
#endif


#endif // _CELASTIC_COMMON_H_