#pragma once

namespace HBXDef
{
	//待插值点的坐标结构体
	template< unsigned int T>
	struct Interpos
	{
		unsigned int	m_numperdim[T];		//插值坐标维度
		unsigned int	m_lag_cordinate[T];	//插值坐标标位置
	};


}
