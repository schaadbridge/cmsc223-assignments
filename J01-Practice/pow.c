#include <stdio.h>

int power(int base, int exp) {
	int ret = 1;
	for (int i = 0; i < exp; i++) {
		ret = ret * base;
	}
	return ret;
}


int main() {
	int base = 2;
	int exponent = 4;
	int result = power(base, exponent);

	printf("%d^%d = %d\n", base, exponent, result);
  return 0;
}
