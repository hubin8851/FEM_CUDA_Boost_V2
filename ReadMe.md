# ReadMe.txt #

##**configuration instructions**##

该程序可由任意C++编译器编译，测试时在visual studio下经C++11标准及以上编译器编译成功。cmakelist未写。待以后编写。


##**operating instructions**##


## **project manifest** ##
FEM\_CUDA\_boost\_v2	主程序  
libElement	单元拓展项目，目前设计仅包含结构单元  
libEngng	引擎拓展项目，内置引擎基类的派生类  
libFEMWithCUDA	所有派生类的基类，囊括了所有必要的接口信息，最为基础  
libNumericalMethod 数值算法的派生类，包括CG,PCG等大型稀疏矩阵的迭代方法      
libReader	数据读取派生类，包括xml，mysql，excle，数据文件读取派生类

**《libFEMWithCUDA》筛选器**   
	BaseComponent.h	基础组件基类   
	BaseMaterial.h	基础材料类   
	BaseSection.h	基础截面类  
	BaseBoundary.h	基础边界类    
	Dispatch.h		消息队列管理类    
	EntityManager.h	组件管理类    
	Domain.h		计算域类   
	...

## **contact information for the programmer** ##

 [鄙人的github，无CSDN博客。保密单位很少发帖很少传代码，但自己会总结 ~](https://github.com/hubin8851)    
<font color=green>工厂模式、C组件、状态机、继承-派生均在项目FEM_CUDA _ _BOOST_ _V2项目中有所体现。   
<font color=red>联系电话：<font color=#0099ff>**18109239467**    


## <font color=black>**troubleshooting** ##
 

## log ##
1/26/2018 10:34:58 PM    
本程序的第一个log。thoughtworks已挂=。=，但好歹学了个markdownpad对日志文件的简单编辑。   
修改了数据接口，采用了标准的正则表达式，在某些编译器上可能会不支持。

