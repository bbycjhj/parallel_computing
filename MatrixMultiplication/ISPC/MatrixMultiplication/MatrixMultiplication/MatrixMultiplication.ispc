export void multiply(uniform int a[][2000], uniform int b[][2000], uniform int r[][2000], uniform int n) {
	foreach (i = 0...n) {
	    for (uniform int j = 0; j < n; j++) {
	        r[j][i] = 0;
	        for (uniform int k = 0; k < n; k++) {
	            r[j][i] += a[j][k] * b[k][i];
	        }
	    }
	}
}