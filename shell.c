#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void errorRoutine();
bool builtInFunctions(char *line);
bool binaryFile(char *line);
void semicolcommands(char* line);
bool redirection(char *line);
void redirectionclose();

struct command{
	char* commandst;
	struct command* next;
};
int temp=0, temp1=0, tempdfd = 0, tempdfd2 = 0;
struct command *head = NULL, *tail = NULL;
char *token;
bool rd = false, errorF = false;

int main(int argc, char *argv[]){
	char line[1024];
		if (argc == 2){
			FILE *fileptr = NULL;
			if(fileptr = fopen(argv[1], "r")){
				while(fgets(line, 1024, fileptr)){
					if (line[0] != '\n'){
					write(STDOUT_FILENO, line, strlen(line));
					semicolcommands(line);
					struct command *temp;
					temp = head;
					while(temp != NULL){
						strcpy(line, temp->commandst);
					if (strlen(line)>512)
						errorRoutine();
					else {
						if (!builtInFunctions(strdup(line)))
							binaryFile(line);
					}
					temp = temp->next;
				}
			}
		}
				fclose(fileptr);
			}
			else
				errorRoutine();
			exit(1);
		}
		else if (argc == 1){
			while(1){
			printf("myshell> ");		
			fgets(line, 1024, stdin);
			if (line[0] !='\n'){
			semicolcommands(line);
			struct command *temp;
			temp = head;
			while(temp != NULL){
				strcpy(line, temp->commandst);
			if (strlen(line)>512)
				errorRoutine();
			else {
				if (!builtInFunctions(strdup(line)))
					binaryFile(line);
			}
			temp = temp->next;
			}	
		}
		}
	}
		else
			errorRoutine();
		return 0;
}

bool builtInFunctions(char *line){
	errorF = false;
	char *token = strtok(strdup(line), " \n\t");
	if (token!=NULL){
	if (strstr(token, "cd")){
		if (strcmp(token, "cd")==0){
		if (!strchr(line,'>')){
		token = strtok(NULL, "\n");
		if(token == NULL)
			token = getenv("HOME");
			if (chdir(token))
				errorRoutine();
		}
		else{
			errorRoutine();
		}
	}
	else{
			errorRoutine();
		}
		return true;
	}
	else if (strstr(token, "pwd")){
		if (strcmp(token, "pwd")==0){
		if (!strchr(line,'>')){
			token = strtok(line, " \n\t");
			token = strtok(NULL, " \n\t");
			if (token == NULL)
				printf("%s\n",getcwd(line, 100));
			else
				errorRoutine();
		}
			else{
			errorRoutine();
			}
		}
		else{
			errorRoutine();
		}
			return true;
		}
	else if(strstr(token, "wait")){
		if (!strchr(line,'>')){
		wait(&temp);
		while(errno != 10)
			if(!wait(&temp))
				errorRoutine();
		}
			else{
			errorRoutine();
			}
			return true;
	}
	else if (strstr(token, "exit")){
		if (strcmp(token, "exit")==0){
		if (!strchr(line,'>')){
		token = strtok(line, " \n\t");
	    token = strtok(NULL, " \n\t");
	    if (token == NULL)
			exit(1);
		else
			errorRoutine();
		}
		else{
			errorRoutine();
			}
	}
		else{
			errorRoutine();
			}
			return true;
	}
}else{
	return true;
}
return false;
}

bool binaryFile(char *line) {
	errorF = false;
	pid_t childPid;
	temp = 0, temp1 = 0;
	char *token = strtok(strdup(line), " \n\t>");
	while(token != NULL){
		token = strtok(NULL, " \n\t>");
		temp++;
	}
	char **argv =malloc(temp+1);
	token = strtok(strdup(line), " \n\t>");
	while(token != NULL){
		argv[temp1] = malloc(strlen(token));
		strcpy(argv[temp1], token);
		if (temp-1 != temp1 && !strcmp(token, "&")){
			errorF=true;
		}
		token = strtok(NULL, " \n\t>");
		temp1++;
	}
	rd = redirection(strdup(line));
	argv[temp1] = NULL;
	if (!errorF){
	childPid = fork();
	if (childPid >= 0){
		if (childPid == 0){
			close(STDERR_FILENO);
			if (rd){
				argv[temp-1] = NULL;
			}
			if (strchr(line, '&')){
				argv[temp-1] = NULL;
			}
			if(execvp(argv[0], argv)<0){
				errorRoutine();
			}
			exit(1);
		}
		else {
			if (rd)
				redirectionclose();
			if (strchr(line, '&')){
				if(!waitpid(childPid, &temp, WNOHANG)){
					errorRoutine();
				}
			}
			else {
				if (!wait(&temp)){
					errorRoutine();
				}
			}
			}
		}
		else
			errorRoutine();
	}
else{
	if (rd)
		redirectionclose();
	errorRoutine();
}
return false;
}

void semicolcommands(char* line){
	head = NULL; 
	tail = NULL;
	token = strtok(line, ";");
	while (token !=NULL){
		struct command *node = malloc(sizeof(struct command));;
		node->commandst = malloc(strlen(token+1));
		strcpy(node->commandst, token);
		node->next = NULL;
		if (head == NULL){
				head = node;
				tail = node;
			}
		else{
			tail->next = node;
			tail = node;
			}
		token = strtok(NULL, ";");
	}
}

void errorRoutine() {
	char error_message[30] = "An error has occurred\n";	
	write(STDOUT_FILENO, error_message,
	strlen(error_message));	
}

bool redirection (char* line) {
   if (token = strrchr(line, '>')){
            strcpy(line, token);
            token = strtok(line, " >\n&\t");
            if (token != NULL){
            	tempdfd = dup(STDOUT_FILENO);
            	close(STDOUT_FILENO);
            	tempdfd2 = open(token, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
            	if(tempdfd2<=0)
            		errorF = true;
            	return true;
    	}
    	else{
    			errorF = true;
    	}
    }
return false;
}

void redirectionclose() {
	close(tempdfd2);
	dup2(tempdfd, STDOUT_FILENO);
}
