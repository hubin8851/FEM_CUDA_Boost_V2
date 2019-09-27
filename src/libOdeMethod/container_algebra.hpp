#pragma once


struct container_algebra {
	template < class S1, class S2, class S3, class Op>
		static void for_each3(S1 &s1, S2 &s2, S3 &s3, Op op) {
		const size_t dim = s1.size();
		for (size_t n = 0; n < dim; ++n)
			op(s1[n], s2[n], s3[n]);
	}
};
