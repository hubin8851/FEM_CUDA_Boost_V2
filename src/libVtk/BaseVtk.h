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
private:
	bool bTopologyChange;	//���˽ṹһ���ı䣬��Ҫ�ػ�ڵ�͵�Ԫ
	bool bFreshing;			//ˢ�±�־λ
	static bool bScalarLabel;		//�Ƿ���ʾ��ǩ��

	unsigned int m_wide;
	unsigned int m_height;
	static vtkIdType m_SumNodeNum;	//��Ԫ����
	static vtkIdType m_SumElmtNum;	//��Ԫ����
	static int		m_LabelNum;	//��ǩ�������

	static double m_scalarRange[2];	//���Է�Χ

	std::map< std::string, vtkSmartPointer<vtkCellArray> > m_MElmt;

//	vtkSmartPointer<vtkPolyData> m_PolyCube;	//����μ��ϣ����õ㡢��Ԫ�����Ե�
	static vtkSmartPointer<vtkPolyDataAlgorithm> m_AlgorithmFilter;	//filter������map��polydata֮����н顣���㷨���ڴ˻����ϵĽӿڸ�д����
	static vtkSmartPointer<vtkFloatArray> m_Scalar;	//�����洢��
	static vtkSmartPointer<vtkScalarBarActor> m_scalarBar;	//��ǩ��

	vtkSmartPointer<vtkLookupTable> m_ColorTable;	//��ɫ��

	static vtkSmartPointer<vtkPolyDataMapper>  m_Mapper;	//ӳ����
	vtkSmartPointer<vtkActor> m_Actor;				//�ж���
	static vtkSmartPointer<vtkRenderer>  m_Renderer;	//������
	vtkSmartPointer<vtkRenderWindow> m_RenderWin;	//���ƴ���
	static vtkSmartPointer<vtkRenderWindowInteractor> m_Interactor;//������

	std::vector< vtkSmartPointer<vtkTextMapper> >	m_vTextMapper;

	vtkSmartPointer<vtkCallbackCommand> m_MyCallback;	//yige callbackcommand�����൱�ڹ۲���/����ģʽ�Ĺ۲���

private:
	//ʵ�����㼯
	virtual UserStatusError_t InstanceNodes(vtkSmartPointer<vtkPoints> _Node, _NodeIter _NIter);
	//ʵ������Ԫ
	virtual UserStatusError_t InstanceElmts(vtkSmartPointer<vtkCellArray> _Elmt, _ElmtIter _EIter);
	//�����������ƻ�ȡ��������
	virtual vtkFloatArray* GetCurData(vtkPolyDataAlgorithm* _dataIn);
	//������������
	virtual void SetScalar(vtkDataArray* _inArray);

	static void CallbackFunc(vtkObject* _caller, unsigned long _eventId, void* clientdata, void* _callData);
public:
	BaseVtk();
	BaseVtk( unsigned int _wide, unsigned int _height );
	static BaseVtk* New();
	~BaseVtk();
	virtual UserStatusError_t Initial();

	//ʵ����actor��renderer��renderwin��
	virtual UserStatusError_t Instance();

	//������ز���
	virtual void Reset();

	//���¶��崰�ڳߴ�
	void ResizeWin(unsigned int _wide, unsigned int _height);

	//����ģ������
	virtual UserStatusError_t SetData( HBXFEMDef::InputRecord* _ir );
	//ͨ��stl�ı�����ģ������
	virtual UserStatusError_t SetData( const char* _FileName, std::string  _path);

	//������ɫ��,������setdata������ʹ��
	void SetColorTable();

	//���ñ�ǩ��,������������ɫ������ʹ�ã�������Ч
	static void SetScalarBar();

	//���ݽڵ���ɫ�����ûص��������Զ�ˢ��
	void AutoFreshNodeNum();
	//���ݽڵ�����ɫ,������setdata������ʹ��
	static void SetNodeNum(vtkObject * _caller = nullptr,
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
	static UserStatusError_t Paint();

	//����ģ�ֵ������CView����ػ�
	virtual UserStatusError_t Draw();

	void Run();

	//���ñ�����ɫ
	void SetBackground( vtkSmartPointer<vtkRenderer> _render, double* _rgb);
};
