export void transpose(uniform int a[][15000], uniform int b[][15000], uniform int n) {
	foreach (i = 0...n) {
	    for (uniform int j = 0; j < n; j++) {
	        b[j][i] = a[i][j];
	    }
	}
}