/*----------------------------------------------
 * Author: Bridge
 * Date: Due: 2/8/23
 * Description:turn a phrase into ent-speech
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	// prompt pause length
	int pauseLen = 0;
	printf("Pause length: ");
	scanf("%d", &pauseLen);

	// allocate space for slow speech
	char buff[32];
	char* slower;
	printf("Text: ");
	scanf("%s", buff);
	int phraseLen = strlen(buff);
	slower = malloc(sizeof(char) * (phraseLen * (1 + pauseLen) + 1));

	// fill slower word w/ each letter of buff and a pause
	if (slower != NULL) {
		int tracker = 0;
		for (int i = 0; i < phraseLen; i++) {
			slower[tracker] = buff[i];
			// add the pause
			for (int j = 1; j <= pauseLen; j++) {
				slower[tracker + j] = '.';
			}
			tracker = tracker + 1 + pauseLen;
		}
		slower[tracker] = '\0';

		// print output
		printf("%s\n", slower);
		free(slower);
	}
	else {
		printf("Malloc failed.");
	}
  return 0;
}
