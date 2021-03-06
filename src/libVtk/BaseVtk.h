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
	//零件计数器，每个零件对应一个Actor
	int m_PartCount;
protected:
	bool bTopologyChange;	//拓扑结构一旦改变，需要重绘节点和单元
	bool bFreshing;			//刷新标志位
	bool bScalarLabel;		//是否显示标签栏

	unsigned int m_wide;
	unsigned int m_height;
	vtkIdType m_SumNodeNum;//节点总数
	vtkIdType m_SumElmtNum;//单元总数
	int		m_LabelNum;//标签分类个数

	double m_scalarRange[2];	//属性范围

	std::map< std::string, vtkSmartPointer<vtkCellArray> > m_MElmt;
	std::map< std::string, vtkSmartPointer<vtkPolyData> > m_PartMap;

//	vtkSmartPointer<vtkPolyData> m_PolyCube;	//多边形集合，放置点、单元、属性等
	vtkSmartPointer<vtkPolyDataAlgorithm> m_AlgorithmFilter;	//filter器，是map与polydata之间的中介。新算法皆在此基础上的接口复写函数
	vtkSmartPointer<vtkFloatArray> m_Scalar;	//标量存储器
	vtkSmartPointer<vtkScalarBarActor> m_scalarBar;	//标签栏

	vtkSmartPointer<vtkLookupTable> m_ColorTable;	//颜色表

	std::vector < vtkSmartPointer<vtkPolyDataMapper> >  m_vMapper;	//映射器
	std::vector < vtkSmartPointer<vtkActor> > m_vActor;				//行动器

	vtkSmartPointer<vtkInteractorStyleTrackballCamera>	m_Camera;//相机
	vtkSmartPointer<vtkRenderer>   m_Renderer;	//绘制类，几个?
	vtkSmartPointer<vtkRenderWindow> m_RenderWin;	//绘制窗体，就一个
	vtkSmartPointer<vtkRenderWindowInteractor> m_Interactor;//窗口渲染交互器，就一个

//	vtkSmartPointer<vtkBoxWidget> m_BoxWidget;
	vtkSmartPointer<vtkCallbackCommand> m_MyCallback;	//yige callbackcommand对象，相当于观察者/命令模式的观察者

	std::vector< vtkSmartPointer<vtkTextMapper> >	m_vTextMapper;

	

protected:
	//实例化点集
	virtual UserStatusError_t InstanceNodes(vtkSmartPointer<vtkPoints> _Node, _NodeIter _NIter);
	//实例化单元
	virtual UserStatusError_t InstanceElmts(vtkSmartPointer<vtkCellArray> _Elmt, _ElmtIter _EIter);
	//依据属性名称获取曲率数据
	virtual vtkFloatArray* GetCurData(vtkPolyDataAlgorithm* _dataIn);


//	static void CallbackFunc(vtkObject* _caller, unsigned long _eventId, void* clientdata, void* _callData);
public:
	BaseVtk();
	BaseVtk( unsigned int _wide, unsigned int _height );
	static BaseVtk* New();
	~BaseVtk();

	//初始化参数
	virtual UserStatusError_t Initial();

	//重置相关参数
	virtual void Reset();

	//导入模型数据
	virtual UserStatusError_t SetData(HBXFEMDef::InputRecord* _ir);
	//通过stl文本导入模型数据
	virtual UserStatusError_t SetData(const char* _FileName, std::string  _path);

	//设置属性数组
//_intArray:传入的属性参数指针
//_num 长度
//_name 待放入部件的名称，默认part0
	virtual void SetScalar(int* _intArray, int _lgth, int _colorCount, const char* _name = "part0");
	virtual void SetScalar(float* _fArray, int _num, const char* _name = "part0");
	virtual void SetScalar(double* _dArray, int _num, const char* _name = "part0");

//与窗口相关的函数
public:
	//实例化actor、renderer和renderwin等
	virtual UserStatusError_t Instance();

	//重新定义窗口尺寸
	void ResizeWin(unsigned int _wide, unsigned int _height);

	//设置颜色表,必须在setdata函数后使用
	void SetColorTable( HBXDef::ColorType_t _t = HBXDef::ColorType_t::RGB,  int _ColorCount = 0);

	//设置标签栏,必须在设置颜色表函数后使用，否则无效
	void SetScalarBar();

	//根据节点着色，设置回调函数并自动刷新
	void AutoFreshNodeNum();
	//根据节点编号着色,必须在setdata函数后使用
	void SetNodeNum(vtkObject * _caller = nullptr,
							unsigned long _eventId = 0,
							void* clientdata = nullptr,
							void * _callData = nullptr);

	//根据单元着色，设置回调函数并自动刷新
	void AutoFreshCellNum();
	//根据单元编号着色,必须在setdata函数后使用
	virtual void SetCellNum();

	//根据法向量着色，设置回调函数并自动刷新
	void AutoFreshNormal();
	//根据法向量着色，必须在setdata函数后使用
	virtual void SetNormal(HBXDef::NormalType_t _nt = HBXDef::NormalType_t::POINTNORMAL);

	//根据曲率着色，设置回调函数并自动刷新
	void AutoFreshCur();

	//根据曲率着色,必须在setdata函数后使用
	virtual void SetCur(HBXDef::CurvatureType_t _t = HBXDef::CurvatureType_t::MAX);

	//模仿MFC，在标准窗口、对话框等派生窗口中使用
	virtual UserStatusError_t Paint();

	//特殊的，值适用于CView类的重绘
	virtual UserStatusError_t Draw();

	void Run();

	//设置背景颜色
	void SetBackground( vtkSmartPointer<vtkRenderer> _render, double* _rgb);
};
