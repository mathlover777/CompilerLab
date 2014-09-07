#include <stdio.h>
int main() {
	int a, b;
	a = 5;
	b = 6;
	a = b - a;
	b = b - a;
	a = a + b;
	printf("%d %d\n", a, b);
	// return 0;
}
