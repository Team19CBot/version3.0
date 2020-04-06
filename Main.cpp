#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../lib/abstractDataType.h"
#include "../lib/func_main.h"
using namespace std;

#define MAXLEN 1024

char* getcgidata(FILE* fp, char* requestmethod);

int main(){
        
        topic_list *head = cons_topic_from_file();
        printf( "Content-type: text/html;charset=utf-8\n\n" );
        
        char *req_method = getenv("REQUEST_METHOD");
        
        char *cgi_str = getcgidata(stdin, req_method);
        if (cgi_str == NULL)
        {
                fprintf(stdout, "WARNNING: REQUEST_METHOD ERROR");
        }
        else
        {
                string question = cgi_str;
                string answer = match(question, head);
                fprintf(stdout,"%s",answer.c_str());
                free(cgi_str);
        }

        // release memory
        free_list(head);
        
}

char* getcgidata(FILE* fp, char *requestmethod)
{
        char* input;
        int len;
        int size = MAXLEN;
        int i = 0;
        
        if (!strcmp(requestmethod, "GET"))
        {
                input = getenv("QUERY_STRING");
                return input;
        }
        else if (!strcmp(requestmethod, "POST"))
        {
                len = atoi(getenv("CONTENT_LENGTH"));
                input = (char*)malloc(sizeof(char)*(size + 1));
                
                if (len == 0)
                {
                        input[0] = '\0';
                        return input;
                }
                
                while(1)
                {
                        input[i] = (char)fgetc(fp);
                        if (i == size)
                        {
                                input[i+1] = '\0';
                                return input;
                        }
                        
                        --len;
                        if (feof(fp) || (!(len)))
                        {
                                i++;
                                input[i] = '\0';
                                return input;
                        }
                        i++;
                        
                }
        }
        return NULL;
}
