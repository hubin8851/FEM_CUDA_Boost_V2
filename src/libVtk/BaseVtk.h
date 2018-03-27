#pragma once
#include <HBXPreDef.h>
#include <VtkPreDef.h>
#include <libCUFEM\InputRecord.h>
#include <libReader\DynamicInputRecord.h>
#include "..\libCUFEM\domain.h"



class BOOST_SYMBOL_EXPORT BaseVtk
{
	typedef	std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>> >::iterator _ElmtIter;
	typedef std::map< std::string, std::shared_ptr<std::vector<HBXFEMDef::Node>> >::iterator _NodeIter;
private:
	bool bTopologyChange;	//���˽ṹһ���ı䣬��Ҫ�ػ�ڵ�͵�Ԫ
	bool bFreshing;			//ˢ�±�־λ
	bool bScalarLabel;		//�Ƿ���ʾ��ǩ��

	unsigned int m_wide;
	unsigned int m_height;
	vtkIdType m_SumNodeNum;	//��Ԫ����
	vtkIdType m_SumElmtNum;	//��Ԫ����
	int		m_LabelNum;	//��ǩ�������

	double m_scalarRange[2];	//���Է�Χ

	std::map< std::string, vtkSmartPointer<vtkCellArray> > m_MElmt;

	vtkSmartPointer<vtkPolyData> m_PolyCube;	//����μ��ϣ����õ㡢��Ԫ�����Ե�
	vtkSmartPointer<vtkFloatArray> m_Scalar;	//�����洢��	
	vtkSmartPointer<vtkCurvatures> m_curvaturesFilter;	//����filter
	vtkSmartPointer<vtkLookupTable> m_ColorTable;	//��ɫ��
	vtkSmartPointer<vtkPolyDataMapper>  m_Mapper;	//ӳ����
	vtkSmartPointer<vtkActor> m_Actor;				//�ж���
	vtkSmartPointer<vtkScalarBarActor> m_scalarBar;	//��ǩ��
	vtkSmartPointer<vtkRenderer>  m_Renderer;	//������
	vtkSmartPointer<vtkRenderWindow> m_RenderWin;	//���ƴ���
	vtkSmartPointer<vtkRenderWindowInteractor> m_Interactor;//������

	std::vector< vtkSmartPointer<vtkPolyDataAlgorithm> > m_vGeometricObjectSources;
	std::vector< vtkSmartPointer<vtkTextMapper> >	m_vTextMapper;

private:
	//ʵ�����㼯
	virtual UserStatusError_t InstanceNodes(vtkSmartPointer<vtkPoints> _Node, _NodeIter _NIter);
	//ʵ������Ԫ
	virtual UserStatusError_t InstanceElmts(vtkSmartPointer<vtkCellArray> _Elmt, _ElmtIter _EIter);
	//�����������ƻ�ȡ��������
	virtual vtkFloatArray* GetCurData();
	//������������
	virtual void SetScalar(vtkDataArray* _inArray);
public:
	BaseVtk();
	BaseVtk( unsigned int _wide, unsigned int _height );
	~BaseVtk();

	//ʵ����actor��renderer��renderwin��
	virtual UserStatusError_t Instance();

	//������ز���
	virtual void Reset();

	//���¶��崰�ڳߴ�
	void ResizeWin(unsigned int _wide, unsigned int _height);

	//����ģ������
	virtual UserStatusError_t SetData( HBXFEMDef::InputRecord* _ir );

	//������ɫ��,������setdata������ʹ��
	void SetColorTable();

	//���ñ�ǩ��,������������ɫ������ʹ�ã�������Ч
	void SetScalarBar();

	//���ݵ�Ԫ�����ɫ,������setdata������ʹ��
	virtual void FreshWithNum();

	//����������ɫ,������setdata������ʹ��
	virtual void FreshWithCur();

	//ģ��MFC���ڱ�׼���ڡ��Ի��������������ʹ��
	virtual UserStatusError_t Paint();

	//����ģ�ֵ������CView����ػ�
	virtual UserStatusError_t Draw();

	//���ñ�����ɫ
	void SetBackground( vtkSmartPointer<vtkRenderer> _render, double* _rgb);
};
