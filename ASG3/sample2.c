#include <stdio.h>
int main() {
	int a;
	int fact = 1;
	scanf("%d", &a);
	while (a > 0) {
		fact = fact * a;
		a = a - 1;
	}
	printf("%d\n", fact);
	return 0;
}
