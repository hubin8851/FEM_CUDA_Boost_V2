#pragma once

struct default_operations {
	template < class Fac1 = double, class Fac2 = Fac1>
		struct scale_sum2 {
		typedef void result_type;
		const Fac1 alpha1;
		const Fac2 alpha2;
		scale_sum2(Fac1 alpha1, Fac2 alpha2)
			: alpha1(alpha1), alpha2(alpha2) { }
		template < class T0, class T1, class T2>
			void operator()(T0 &t0, const T1 &t1, const T2 &t2) const {
			t0 = alpha1 * t1 + alpha2 * t2;
		}
	};
};