#include <stdio.h>
#include <string.h>

int isPalindrome(char str[]) {
	int len = strlen(str);

	for (int i = 0; i < len/2; i++) {
		if (!(str[i] == str[len-1-i])) {
			return 0;
		}
	}
	return 1;
}

int main() {
	printf("%d %d\n", isPalindrome("racecar"), isPalindrome("apple"));
	return 0;
}
