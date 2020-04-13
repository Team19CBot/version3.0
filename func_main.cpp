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
#include "../lib/staticValue.h"

using namespace std;

// construct topic list from topic.txt file
// if failure, return NULL
// otherwise, return list head pointer
topic_list *cons_topic_from_file(){
        topic_list *head = NULL;
        topic_list *tail = NULL;
        string str = "";
        
        ifstream in("./src/topic/topic.txt");
        if (in.is_open())
        {
                while (!in.eof())
                {
                        getline(in,str);
                        // ignore empty line
                        if (str != "")
                        {
                                cons_topic_list(&head, &tail, str);
                        }
                }
        }
        else
        {
                cout<< "Error: Topic File Not Exist"<< endl;
        }
        in.close();
        
        return head;
}

// construct topic name list
void cons_topic_list(topic_list **head, topic_list **tail, string topic_name){
        // empty list
        if (*head == NULL)
        {
                topic_list *list = new topic_list();
                list->name = topic_name;
                list->next = NULL;
                *head = list;
                *tail = list;
        }
        // non-empty list
        else
        {
                topic_list *temp = *tail;
                topic_list *list = new topic_list();
                list->name = topic_name;
                list->next = NULL;
                temp->next = list;
                *tail = list;
        }
}

// print out topic name
void printTopicList(topic_list *head) {
        topic_list *temp = head;
        if (temp == NULL)
        {
                return;
        }
        else
        {
                cout << temp->name << endl;
                printTopicList(temp->next);
        }
}

// release topic list
void free_list(topic_list *head){
        if (head == NULL)
        {
                return;
        }
        else
        {
                topic_list *temp = head;
                topic_list *next = NULL;
                
                do {
                        next = temp->next;
                        delete temp;
                        temp = next;
                } while (temp != NULL);
                
        }
}

// input question, matching with database
// return corresponding answer string
// NOTE: question string must separate each word using space
string match(string que, topic_list* head){
        
        string copy = que;
        // remove punctuation
        que.erase(remove_if (que.begin(), que.end(), static_cast<int(*)(int)>(&ispunct)),que.end());
        // transform to lower case
        transform(que.begin(),que.end(),que.begin(),::tolower);
        
        // split question string
        vector<string> que_arr;
        string result;
        
        stringstream input(que);
        while(input>>result)
        {
                // filter trivial words
                if (result == "c" || result.length() > 2)
                {
                        que_arr.push_back(result);
                }
        }
        
        topic_list *temp = head;
        string topic_name = "";
        Topic *current_topic = NULL;
        int match_val = 0;
        int index_keyword1 = -1;
        int index_keyword2 = -1;
        int index_special_word = -1;
        int size = que_arr.size();
        
        // check whether difference-related question
        if (que.find("differ") != -1)
        {
                string answer = asking_differ_question(que_arr);
                if (answer == "")
                {
                        failure_log("differ",copy);
                        return "Sorry, I cannot differ them now";
                }
                return answer;
        }
        
        // match topic words
        while (temp != NULL)
        {
                // !!!
                // find() can be replaced by KMP algorithm to improve effiency
                // !!!!
                if (que.find(temp->name) != -1)
                {
                        topic_name = temp->name;
                        break;
                }
                temp = temp->next;
        }
        
        // check whether asking common function usage
        if (topic_name == "" || topic_name == "function")
        {
                string answer = function_usage_question(que_arr);
                if (answer != "")
                {
                        return answer;
                }
                else if (answer == "" && topic_name == "")
                {
                        // no topic matched and not ask usage
                        answer = symbol_usage_question(copy);
                        if (answer == "")
                        {
                                failure_log("general",copy);
                                return "Please ask questions on C programming";
                        }
                        return answer;
                }
        }
        
        // monitor topic statistics
        monitor_statistics(topic_name);
        
        // asking operator question
        if (topic_name == "operat")
        {
                string answer = symbol_usage_question(copy);
                if (answer != "")
                {
                        return answer;
                }
        }
        
        // construct keywords tree
        current_topic = set_up_topic(topic_name);
        if (current_topic == NULL)
        {
                // empty topic
                failure_log(topic_name,que);
                return "Sorry, I haven't learned it yet";
        }
        
        // match special words
        for (int i=0; i<size; i++)
        {
                int value = match_special_word(que_arr[i]);
                if (value != 0)
                {
                        index_special_word = i;
                        match_val += value;
                        break;
                }
        }
        
        // match keyword1
        for(int i=0; i<size; i++)
        {
                if (i == index_special_word)
                {
                        continue;
                }
                else
                {
                        int val = match_keyword(que_arr[i], current_topic, 1);
                        if (val != 0)
                        {
                                index_keyword1 = i;
                                match_val += val;
                                break;
                        }
                }
        }
        
        // match keyword2
        for(int i=0; i<size; i++){
                if (i == index_keyword1 || i == index_special_word)
                {
                        continue;
                }
                else
                {
                        int val = match_keyword(que_arr[i], current_topic, 2);
                        if (val != 0)
                        {
                                index_keyword2 = i;
                                match_val += val;
                                break;
                        }
                }
        }
        // delete topic object
        free_topic(current_topic);
        
        if (match_val == 0)
        {
                // no keywords matched
                failure_log(topic_name,copy);
                string answer = "Can you clarify your question about " + topic_name + "?";
                return answer;
        }
        else
        {
                string answer = match_solution(match_val, topic_name);
                if (answer == "")
                {
                        // record matching failure question
                        failure_log(topic_name,copy);
                        // no solution can be matched
                        if (index_keyword1 != -1)
                        {
                                answer = "What do you mean '" + que_arr[index_keyword1] + "' for " + topic_name + "?";
                        }
                        else if (index_keyword2 != -1)
                        {
                                answer = "What do you mean '" + que_arr[index_keyword2] + "' for " + topic_name + "?";
                        }
                        else {
                                failure_log(topic_name,copy);
                                answer = "Can you adapt your question in more clear way?";
                        }
                }
                return answer;
        }
}

// given match value and topic name
// search in database file and return answer
string match_solution(int match_val, string topic_name) {
        string answer = "";
        string filename = "./src/" + topic_name + "/database.txt";
        ifstream in(filename);
        int target_match = 0;
        int temp_val = 0;
        string str = "";
        
        if (in.is_open())
        {
                while(!in.eof())
                {
                        getline(in, str);
                        if (str == "#")
                        {
                                // get target answer, stop searching
                                if (target_match == 1)
                                {
                                        in.close();
                                        return answer;
                                }
                                getline(in, str);
                                // ensure valid value
                                while ((temp_val = atoi(str.c_str())) == 0)
                                {
                                        getline(in, str);
                                }
                                if (temp_val == match_val)
                                {
                                        target_match = 1;
                                }
                        }
                        else if (target_match == 1)
                        {
                                answer = answer + str + "\n";
                        }
                }
                in.close();
        }
        return answer;
}

// given word and check whether is special words
// return match value if succeed, otherwise return 0
int match_special_word(string word) {
        
        if (word.length() == 3)
        {
                if (word == special_words[0])
                {
                        return SPECIAL_BASE_WEIGHT*1;
                }
                else if (word == special_words[1])
                {
                        return SPECIAL_BASE_WEIGHT*2;
                }
        }
        
        if (word.length() == 4)
        {
                if (word == special_words[2])
                {
                        return SPECIAL_BASE_WEIGHT*3;
                }
                else if (word == special_words[3])
                {
                        return SPECIAL_BASE_WEIGHT*4;
                }
        }
        
        if(word.length() == 5)
        {
                if (word == special_words[4])
                {
                        return SPECIAL_BASE_WEIGHT*5;
                }
        }
        
        return 0;
}

// given word, Topic object and level tree id
// return match value in level tree if succeed,
// otherwise, return 0
int match_keyword(string word, Topic *topic, int level_id){
        BST *temp = NULL;
        int match_val = 0;
        
        if (level_id == 1)
        {
                temp = topic->level1_head;
        }
        else if (level_id == 2)
        {
                temp = topic->level2_head;
        }
        
        while (temp != NULL)
        {
                int val = (temp->keyword).compare(word);
                if (val == 0)
                {
                        match_val += temp->match_val;
                        break;
                }
                else if (val < 0)
                {
                        temp = temp->right;
                }
                else
                {
                        temp = temp->left;
                }
        }
        
        return match_val;
}

// store matching failure question in log file
void failure_log(string topic_name, string question) {
        string filename = "./src/log/" + topic_name + ".txt";
        
        ofstream out;
        out.open(filename,ios::out|ios::app);
        out << question << endl;
        out << endl;
        out.close();
}

// set up all relevant keyword and answer tree for each topic
// return pointer of topic class if succeed, otherwise return NULL
Topic * set_up_topic(string topic_name){
        Topic *topic = new Topic();
        
        // specify file path
        string path = "./src/"+topic_name;
        string level1_filepath = path + "/first_level.txt";
        string level2_filepath = path + "/second_level.txt";
        string solution_filepath = path + "/solution.txt";
        
        // construct keyword tree
        topic->level1_head = cons_keyword_from_file(level1_filepath, LEVEL1_BASE_WEIGHT);
        topic->level2_head = cons_keyword_from_file(level2_filepath, LEVEL2_BASE_WEIGHT);
        
        if (topic->level2_head == NULL && topic->level1_head == NULL)
        {
                // check whether solution file is empty
                ifstream in(solution_filepath);
                if (in.is_open()) {
                        int i = in.get();
                        if (in.eof()) {
                                delete topic;
                                in.close();
                                return NULL;
                        }
                        in.close();
                }
        }
        return topic;
}

// construct keyword tree
BST* cons_keyword_from_file(string filename, int base_weight){
        
        string keyword_str ="";
        BST *head = NULL;
        int num_keyword = 0;
        
        ifstream in(filename);
        
        // open info file...
        if (in.is_open())
        {
                while (!in.eof())
                {
                        num_keyword++;
                        // read keyword line from file
                        getline(in,keyword_str);
                        
                        // split synonym keyword string
                        vector<string> res;
                        string result;
                        stringstream input(keyword_str);
                        // remove space and split
                        while(input>>result)
                        {
                                res.push_back(result);
                        }
                        // construct tree using each keyword
                        for(int i=0;i<res.size();i++)
                        {
                                // synonym has same match value
                                cons_keyword_BST(res[i].c_str(), &head, num_keyword*base_weight);
                        }
                }
                in.close();
        }
        
        return head;
}

// construct binary search tree for keywords according to alphabet
void cons_keyword_BST(string keyword, BST ** head, int match_val) {
        
        if (*head == NULL)
        {
                BST *bst = new BST();
                bst->keyword = keyword;
                bst->match_val = match_val;
                bst->left = NULL;
                bst->right = NULL;
                *head = bst;
        }
        else
        {
                BST *temp = *head;
                do {
                        // check left
                        if ((temp->keyword).compare(keyword) > 0)
                        {
                                if (temp->left == NULL){
                                        BST *bst = new BST();
                                        bst->keyword = keyword;
                                        bst->match_val = match_val;
                                        bst->left = NULL;
                                        bst->right = NULL;
                                        temp->left = bst;
                                        break;
                                }
                                else
                                {
                                        temp = temp->left;
                                }
                        }
                        // check right
                        else
                        {
                                if (temp->right == NULL){
                                        BST *bst = new BST();
                                        bst->keyword = keyword;
                                        bst->match_val = match_val;
                                        bst->left = NULL;
                                        bst->right = NULL;
                                        temp->right = bst;
                                        break;
                                }
                                else
                                {
                                        temp = temp->right;
                                }
                        }
                } while (temp != NULL);
        }
}

// relaese memory of topic object
void free_topic(Topic *head) {
        if (head == NULL)
        {
                return ;
        }
        else
        {
                free_keyword_tree(head->level1_head);
                free_keyword_tree(head->level2_head);
                delete head;
        }
}

// release memory of keywords tree
void free_keyword_tree(BST *head) {
        if (head == NULL)
        {
                return;
        }
        else
        {
                free_keyword_tree(head->left);
                free_keyword_tree(head->right);
                delete head;
        }
}

// given array of string
// this function is used to match function usage question only
// return answer if succeed, otherwise return empty string
string function_usage_question(vector<string> que_arr) {
        string filename = "./src/common/second_level.txt";
        
        int match_val = 0;
        Topic *topic = new Topic();
        
        // construct keyword tree
        topic->level1_head = NULL;
        topic->level2_head = cons_keyword_from_file(filename, 1);
        
        if (topic->level2_head == NULL)
        {
                return "Error: function_usage_question failure";
        }
        
        for (int i=0; i<que_arr.size(); i++)
        {
                int val = match_keyword(que_arr[i], topic, 2);
                if (val != 0)
                {
                        match_val = val;
                        break;
                }
        }
        if (match_val == 0)
        {
                return "";
        }
        else
        {
                return match_solution(match_val, "common");
        }
}


// given array of string
// this function is used to match difference question only
// return answer if succeed, otherwise return empty string
string asking_differ_question(vector<string> que_arr) {
        
        Topic *current_topic = set_up_topic("differ");
        int index_keyword1 = -1;
        int match_val = 0;
        int size = que_arr.size();
        
        // match keyword1
        for(int i=0; i<size; i++)
        {
                int val = match_keyword(que_arr[i], current_topic, 1);
                if (val != 0)
                {
                        index_keyword1 = i;
                        match_val += val;
                        break;
                }
        }
        
        // match keyword2
        for(int i=0; i<size; i++){
                if (i == index_keyword1)
                {
                        continue;
                }
                else
                {
                        int val = match_keyword(que_arr[i], current_topic, 2);
                        if (val != 0)
                        {
                                match_val += val;
                                break;
                        }
                }
        }
        
        // release keyword tree
        free_keyword_tree(current_topic->level1_head);
        free_keyword_tree(current_topic->level2_head);
        delete current_topic;
        
        if (match_val == 0)
        {
                return "";
        }
        else
        {
                string answer = match_solution(match_val, "differ");
                return answer;
        }
}

// given question string without removing punctuation
// this function is used to match symbol question only
// return answer if succeed, otherwise return empty string
string symbol_usage_question(string que) {
        string answer ="";
        int match_value = match_symbol(que);
        if (match_value != 0) {
                answer = match_solution(match_value, "symbol");
        }
        return answer;
}

// given question string
// check whether exist symbol */&..
// return match value if exists, otherwise, return 0
int match_symbol(string que) {
        int count = 0;
        int match_value = 0;
        string str = "";
        int base_weight = LEVEL2_BASE_WEIGHT;
        string path = "./src/symbol/second_level.txt";
        ifstream in(path);
        
        if (in.is_open()) {
                while(!in.eof())
                {
                        count++;
                        getline(in,str);
                        if (que.find(str) != -1)
                        {
                                match_value = count*base_weight;
                                break;
                        }
                }
                in.close();
        }
        return match_value;
}

// split string package into each string
// store each string in vector
void split(const string& s,vector<string>& sv, char flag) {
        sv.clear();
        istringstream iss(s);
        string temp;
        
        while (getline(iss, temp, flag)) {
                sv.push_back(temp);
        }
        return;
}

// given topic name and update statistics file
void monitor_statistics(string topic_name) {
        string path_statistics = "./src/topic/statistics.txt";
        string path_copy = "./src/topic/copy.txt";
        
        ifstream in(path_statistics);
        ofstream out(path_copy);
        
        string str = "";
        vector<string> target;
        int find = 0;
        
        if (in.is_open() && out.is_open())
        {
                while (!in.eof())
                {
                        getline(in,str);
                        if (find == 1)
                        {
                                out << str << endl;
                        }
                        else if (str.find(topic_name) != -1)
                        {
                                split(str,target,',');
                                int frequency = stoi(target[1]);
                                frequency++;
                                target[1] = to_string(frequency);
                                string new_statistics = target[0] + "," + target[1];
                                out << new_statistics << endl;
                                find = 1;
                        }
                        else {
                                out << str << endl;
                        }
                }
                in.close();
                out.close();
                remove(path_statistics.c_str());
                rename(path_copy.c_str(),path_statistics.c_str());
        }
}
