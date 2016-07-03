def f_cy(int I, int J):
	cdef double res = 0
	for i in range(I):
		for j in range(J * I):
			res += 1
	return res