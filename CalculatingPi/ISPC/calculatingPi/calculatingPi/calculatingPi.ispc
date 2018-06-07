export void calculatingPi(uniform int n, uniform double &result) {
	uniform double pi;
	double x, sum = 0.0;
	uniform double step = 1.0 / (double)n;
	foreach (index = 0 ... n) {
		x = (index + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}
	pi = step * reduce_add(sum);
	result = pi;
}