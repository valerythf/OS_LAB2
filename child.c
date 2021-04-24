#include <stdio.h>
#include<string.h>

char Inversion(char* string ,int i){
	char str;
	int n = 0;
	int k = i/2;
	while(i != k){
		str = string[n];
		string[n] = string[i-1];
		string[i-1] = str;
		i--;
		n++;
	}
}

int main() {
	char str[50];
	scanf("%s",str);
	Inversion(str, strlen(str));
	printf("%s\n",str);
}
