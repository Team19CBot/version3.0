#include <stdio.h>
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

#define MAXLEN 1024;

string website_address(string que);

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
                string que = cgi_str;
                string website = website_address(que);
                fprintf(stdout,"%s", website.c_str());
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

// given quesiton string and return relevant website
string website_address(string que) {
        
        if (que.find("array") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_arrays.htm";
        }
        
        if (que.find("point") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_pointers.htm";
        }
        
        if (que.find("file") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_file_io.htm";
        }
        
        if (que.find("struct") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_structures.htm";
        }
        
        if (que.find("function") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_functions.htm";
        }
        
        if (que.find("statement") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_decision_making.htm";
        }
        
        if (que.find("variable") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_variables.htm";
        }
        
        if (que.find("operat") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_operators.htm";
        }
        
        if (que.find("string") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_strings.htm";
        }
        
        if (que.find("loop") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_loops.htm";
        }
        
        if (que.find("list") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_structures.htm";
        }
        
        if (que.find("expression") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_decision_making.htm";
        }
        
        if (que.find("compile") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_command_line_arguments.htm";
        }
        
        if (que.find("graph") != -1)
        {
                return "https://www.tutorialspoint.com/graph_theory/index.htm";
        }
        
        if (que.find("tree") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_memory_management.htm";
        }
        
        if (que.find("stack") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_memory_management.htm";
        }
        
        if (que.find("queue") != -1)
        {
                return "https://www.tutorialspoint.com/cprogramming/c_memory_management.htm";
        }
        
        return "https://www.tutorialspoint.com/cprogramming/c_quick_guide.htm";
}
