#include <stdio.h>
#include <string.h>

void removeL(char* str, char letter) {
	int str_len = strlen(str);
	char buff[32] = "";
	int buff_len = 0;
	for (int i = 0; i < str_len; i++) {
		if (str[i] != letter) {
			buff[buff_len] = str[i];
			buff_len++;
		}
	}
	buff[buff_len] = '\0';
	strcpy(str, buff);
	return;
}

int main() {
	char word[32] = "lasagna";
	removeL(word, 'a');
	printf("%s\n", word);

	return 0;
}
