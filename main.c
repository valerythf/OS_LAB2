#include <stdio.h>
#include <string.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h>
#define MAX_SIZE 100

int main(){
	int pipe1[2], pipe2[2];
  	int  file1, file2, size;
	char str[MAX_SIZE];
	char filename1[15], filename2[15];
	if (pipe(pipe1)==-1 || pipe(pipe2)==-1 ) {
		fprintf(stderr, "Pipe Failed" );
        	return 1;
        }

        printf("file1:\n");
        scanf("%s",filename1);
        printf("file2:\n");
        scanf("%s",filename2);

        file1 = open(filename1, O_CREAT|O_RDWR|O_TRUNC, S_IREAD|S_IWRITE);
        file2 = open(filename2, O_CREAT|O_RDWR|O_TRUNC, S_IREAD|S_IWRITE);
        int child1 = fork();
        int child2 = fork();

        if (child1 < 0 || child2 < 0 ){
                perror("error");
                exit(1);
        } else if (child1 > 0 && child2 > 0){
		printf("Enter a line\n");
		__fpurge(stdin);
		fgets(str, MAX_SIZE, stdin);
		fflush(stdout);
		close(pipe1[0]);
		close(pipe2[0]);
		size = strlen(str);
		if( size <= 10) {
			close(pipe2[1]);
			write(pipe1[1], &size, sizeof(int));	
			write(pipe1[1], &str, strlen(str));
		} else {
			close(pipe1[1]);
			write(pipe2[1], &size, sizeof(int));
			write(pipe2[1], &str, strlen(str));
		}
		close(pipe1[1]);
		close(pipe2[1]);
	} else if(child1 == 0 && child2 != 0){
		read(pipe1[0],&size,sizeof(int));
		if (size > 10)
			exit(0);
		printf("child1 is running %s\n", filename1);
		dup2(pipe1[0],0);
		dup2(file1, 1);
		close(file1);
		close(file2);
		close(pipe1[0]);
		close(pipe1[1]);
		execl("Child1", "child", NULL);
	} else if(child1 != 0 && child2 == 0){
		read(pipe2[0],&size,sizeof(int));
		if (size <= 10)
			exit(0);
		printf("Child2 is running %s\n", filename2);
		dup2(pipe2[0],0);
		dup2(file2, 1);
		close(file1);
		close(file2);
		close(pipe2[0]);
		close(pipe2[1]);
		execl("child2", "child", NULL);
	}
	return 0;
}
