#pragma once
#include <vector>
#include <boost/array.hpp>
#include "libOdeMethod/container_algebra.hpp"
#include "libOdeMethod/default_operations.hpp"

//一个常态类，有着模板函数
class runge_kutta4 {
	public:
	typedef std::vector<double> state_type;
	runge_kutta4(size_t N)
		: N(N), x_tmp(N), k1(N), k2(N), k3(N), k4(N) { }
	template < typename System>
		 void do_step(System system, state_type &x, double t, double dt)
		 {
		 const double dt2 = dt / 2;
		 const double dt3 = dt / 3;
		 const double dt6 = dt / 6;
		 system(x, k1, t);
		 for (size_t i = 0; i < N; ++i)
		 x_tmp[i] = x[i] + dt2 * k1[i];
		 system(x_tmp, k2, t + dt2);
		 for (size_t i = 0; i < N; ++i)
		 x_tmp[i] = x[i] + dt2 * k2[i];
		 system(x_tmp, k3, t + dt2);
		 for (size_t i = 0; i < N; ++i)
			 x_tmp[i] = x[i] + dt * k3[i];
		 system(x_tmp, k4, t + dt);
		 for (size_t i = 0; i < N; ++i)
			 x[i] += dt6 * k1[i] + dt3 * k2[i] + dt3 * k3[i] + dt6 * k4[i];
		 }
private:
	const size_t N;
	state_type x_tmp, k1, k2, k3, k4;
};


struct container_algebra {
	template < class S1, class S2, class S3, class Op>
		static void for_each3(S1 &s1, S2 &s2, S3 &s3, Op op) {
		const size_t dim = s1.size();
		for (size_t n = 0; n < dim; ++n)
			op(s1[n], s2[n], s3[n]);
	}
};



template < class state_type>
void resize(const state_type &in, state_type &out) {
	// 标准容器，若自定义state_type需重载resize
	out.resize(in.size());
}

// boost::array定制版
template < class T, size_t N>
void resize(const boost::array<T, N> &, boost::array<T, N>&) {
	/* arrays don’t need resizing */
	
}


template <	class state_type, class value_type = double,
			class time_type = value_type,
			class algebra = container_algebra,
			class operations = default_operations>
	class runge_kutta4_gpu
{
public:
	template < typename System>
	void do_step(System &system, state_type &x,
		time_type t, time_type dt)
	{
		if (ture)
		{
			adjust_size(x);
		}
		const value_type one = 1;
		const time_type dt2 = dt / 2, dt3 = dt / 3, dt6 = dt / 6;
		typedef typename operations::template scale_sum2 <value_type, time_type> scale_sum2;
		typedef typename operations::template scale_sum5 < value_type, time_type, time_type, time_type, time_type> scale_sum5;
	}
private:
	state_type x_tmp, k1, k2, k3, k4;
	void adjust_size(const state_type &x) 
	{
		resize(x, x_tmp);
		resize(x, k1); 
		resize(x, k2);
		resize(x, k3); 
		resize(x, k4);
	}
}