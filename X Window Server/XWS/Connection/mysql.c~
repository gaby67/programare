#include <stdio.h>
void main(){
	FILE *file; 
	char buffer[100];
	file = popen("mysql -u root -p phplogin","r+");
	fwrite("amber",1,sizeof(const char),file);
	printf("%s",buffer);
	//fread(buffer,1,100,file);*/
	//file = fopen("apa","r+");
	//fread(buffer,1,100,file);
	//printf("%s",buffer);	
	fwrite(buffer,1,sizeof(buffer),file);
}
