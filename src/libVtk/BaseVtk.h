#pragma once
#include <HBXPreDef.h>
#include <ExportDef.h>
#include <VtkPreDef.h>
#include <libCUFEM\InputRecord.h>
#include <libReader\DynamicInputRecord.h>
#include "..\libCUFEM\domain.h"



class CUFEM_API BaseVtk
{
	typedef	std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>> >::iterator _ElmtIter;
	typedef std::map< std::string, std::shared_ptr<std::vector<HBXFEMDef::Node>> >::iterator _NodeIter;

	typedef  std::map< std::string, vtkSmartPointer<vtkCellArray> >::iterator _ElementIter;
	typedef  std::map< std::string, vtkSmartPointer<vtkPolyData> >::iterator _PartIter;
private:
	//�����������ÿ�������Ӧһ��Actor
	int m_PartCount;
protected:
	bool bTopologyChange;	//���˽ṹһ���ı䣬��Ҫ�ػ�ڵ�͵�Ԫ
	bool bFreshing;			//ˢ�±�־λ
	bool bScalarLabel;		//�Ƿ���ʾ��ǩ��

	unsigned int m_wide;
	unsigned int m_height;
	vtkIdType m_SumNodeNum;//�ڵ�����
	vtkIdType m_SumElmtNum;//��Ԫ����
	int		m_LabelNum;//��ǩ�������

	double m_scalarRange[2];	//���Է�Χ

	std::map< std::string, vtkSmartPointer<vtkCellArray> > m_MElmt;
	std::map< std::string, vtkSmartPointer<vtkPolyData> > m_PartMap;

//	vtkSmartPointer<vtkPolyData> m_PolyCube;	//����μ��ϣ����õ㡢��Ԫ�����Ե�
	vtkSmartPointer<vtkPolyDataAlgorithm> m_AlgorithmFilter;	//filter������map��polydata֮����н顣���㷨���ڴ˻����ϵĽӿڸ�д����
	vtkSmartPointer<vtkFloatArray> m_Scalar;	//�����洢��
	vtkSmartPointer<vtkScalarBarActor> m_scalarBar;	//��ǩ��

	vtkSmartPointer<vtkLookupTable> m_ColorTable;	//��ɫ��

	std::vector < vtkSmartPointer<vtkPolyDataMapper> >  m_vMapper;	//ӳ����
	std::vector < vtkSmartPointer<vtkActor> > m_vActor;				//�ж���

	vtkSmartPointer<vtkInteractorStyleTrackballCamera>	m_Camera;//���
	vtkSmartPointer<vtkRenderer>   m_Renderer;	//�����࣬����?
	vtkSmartPointer<vtkRenderWindow> m_RenderWin;	//���ƴ��壬��һ��
	vtkSmartPointer<vtkRenderWindowInteractor> m_Interactor;//������Ⱦ����������һ��

//	vtkSmartPointer<vtkBoxWidget> m_BoxWidget;
	vtkSmartPointer<vtkCallbackCommand> m_MyCallback;	//yige callbackcommand�����൱�ڹ۲���/����ģʽ�Ĺ۲���

	std::vector< vtkSmartPointer<vtkTextMapper> >	m_vTextMapper;

	

protected:
	//ʵ�����㼯
	virtual UserStatusError_t InstanceNodes(vtkSmartPointer<vtkPoints> _Node, _NodeIter _NIter);
	//ʵ������Ԫ
	virtual UserStatusError_t InstanceElmts(vtkSmartPointer<vtkCellArray> _Elmt, _ElmtIter _EIter);
	//�����������ƻ�ȡ��������
	virtual vtkFloatArray* GetCurData(vtkPolyDataAlgorithm* _dataIn);


//	static void CallbackFunc(vtkObject* _caller, unsigned long _eventId, void* clientdata, void* _callData);
public:
	BaseVtk();
	BaseVtk( unsigned int _wide, unsigned int _height );
	static BaseVtk* New();
	~BaseVtk();

	//��ʼ������
	virtual UserStatusError_t Initial();

	//������ز���
	virtual void Reset();

	//����ģ������
	virtual UserStatusError_t SetData(HBXFEMDef::InputRecord* _ir);
	//ͨ��stl�ı�����ģ������
	virtual UserStatusError_t SetData(const char* _FileName, std::string  _path);

	//������������
//_intArray:��������Բ���ָ��
//_num ����
//_name �����벿�������ƣ�Ĭ��part0
	virtual void SetScalar(int* _intArray, int _lgth, int _colorCount, const char* _name = "part0");
	virtual void SetScalar(float* _fArray, int _num, const char* _name = "part0");
	virtual void SetScalar(double* _dArray, int _num, const char* _name = "part0");

//�봰����صĺ���
public:
	//ʵ����actor��renderer��renderwin��
	virtual UserStatusError_t Instance();

	//���¶��崰�ڳߴ�
	void ResizeWin(unsigned int _wide, unsigned int _height);

	//������ɫ��,������setdata������ʹ��
	void SetColorTable( HBXDef::ColorType_t _t = HBXDef::ColorType_t::RGB,  int _ColorCount = 0);

	//���ñ�ǩ��,������������ɫ������ʹ�ã�������Ч
	void SetScalarBar();

	//���ݽڵ���ɫ�����ûص��������Զ�ˢ��
	void AutoFreshNodeNum();
	//���ݽڵ�����ɫ,������setdata������ʹ��
	void SetNodeNum(vtkObject * _caller = nullptr,
							unsigned long _eventId = 0,
							void* clientdata = nullptr,
							void * _callData = nullptr);

	//���ݵ�Ԫ��ɫ�����ûص��������Զ�ˢ��
	void AutoFreshCellNum();
	//���ݵ�Ԫ�����ɫ,������setdata������ʹ��
	virtual void SetCellNum();

	//���ݷ�������ɫ�����ûص��������Զ�ˢ��
	void AutoFreshNormal();
	//���ݷ�������ɫ��������setdata������ʹ��
	virtual void SetNormal(HBXDef::NormalType_t _nt = HBXDef::NormalType_t::POINTNORMAL);

	//����������ɫ�����ûص��������Զ�ˢ��
	void AutoFreshCur();

	//����������ɫ,������setdata������ʹ��
	virtual void SetCur(HBXDef::CurvatureType_t _t = HBXDef::CurvatureType_t::MAX);

	//ģ��MFC���ڱ�׼���ڡ��Ի��������������ʹ��
	virtual UserStatusError_t Paint();

	//����ģ�ֵ������CView����ػ�
	virtual UserStatusError_t Draw();

	void Run();

	//���ñ�����ɫ
	void SetBackground( vtkSmartPointer<vtkRenderer> _render, double* _rgb);
};
