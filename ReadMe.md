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
5/7/2019 10:45:38 PM 
release下面的配置已全部更新

1/18/2019 5:30:01 PM 
长期没更新啊...惊闻老板下个月发500工资，震惊...
修改了inp读取器在错误下仍然能运行的bug，对部分头文件添加了CUDA宏以做判断

7/13/2018 9:19:51 PM 
添加着色类，尚未完成。
完成有限元模型的无向图模型，通过着色实现并行装配

6/19/2018 6:22:50 PM 
修改libreader里的小bug
完善structengng

4/27/2018 10:32:14 PM 
完成与CPU版本性能测试对比

4/26/2018 10:35:55 PM 
插值模块验证完毕


4/19/2018 4:41:37 PM 
添加了插值cuda库
完善了CUDA的回调
添加索引八叉树、四叉树ING

3/6/2018 9:08:14 PM 
沿用boost——MPI的设计思路选用shared_ptr存放，均放在dynrecord类中。文本读取工作基本完成。

3/5/2018 4:27:19 PM 
inp文件的set尚未正确配置，届时放在后面处理。

2/27/2018 5:33:33 PM 
弃用boost的ptree,在inp读取类中完成修改；
BaseRead的接口改为仅有一个path，通过path的自带api完成各种文本操作；

1/26/2018 10:34:58 PM    
本程序的第一个log。thoughtworks已挂=。=，但好歹学了个markdownpad对日志文件的简单编辑。   
修改了数据接口，采用了标准的正则表达式，在某些编译器上可能会不支持。
修改单元注册宏，完成单元注册接口

