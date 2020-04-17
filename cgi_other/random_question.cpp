// @author: Zhengrui Xue
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <ctime>
#include <math.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../lib/abstractDataType.h"
#include "../lib/func_main.h"
using namespace std;

#define MAXLEN 100;

string random_gen_question(string topic_name);

int main(){
        
        printf( "Content-type: text/html;charset=utf-8\n\n" );
        
        char *req_method = getenv("REQUEST_METHOD");
        
        char *cgi_str = getcgidata(stdin, req_method);
        if (cgi_str == NULL)
        {
                fprintf(stdout, "WARNNING: REQUEST_METHOD ERROR");
        }
        else
        {
                string topic = cgi_str;
                string que = random_gen_question(topic);
                fprintf(stdout,"%s",que.c_str());
                free(cgi_str);
        }
        
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

// given topic name and generate relevant question
string random_gen_question(string topic_name) {
        
        vector<string> que;
        
        if (topic_name == "pointer")
        {
                que.push_back("how to dereference a pointer?");
                que.push_back("how to change a value of variable through pointer?");
                que.push_back("how to compare two pointers?");
        }
        else if (topic_name == "array")
        {
                que.push_back("how to use array as a pointer?");
                que.push_back("what is the memory layout of one array?");
                que.push_back("how to pass array as an argument?");
        }
        else if (topic_name == "memory management")
        {
                que.push_back("how to allocate memory in C?");
                que.push_back("how to free memory?");
                que.push_back("what the segmentation fault means?");
        }
        
        srand(time(0));
        int i = rand()%3+1;
        return que[i-1];
}
