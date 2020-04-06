
/**  func_main.h **/
using namespace std;

/** function for matching **/
string match(string que, topic_list* head);
string match_solution(int match_val, string topic_name);
int match_special_word(string word);
int match_keyword(string word, Topic *topic, int level_id);
int match_symbol(string que);
int get_match_val(string path, string word);


/** function for string operation **/
string function_usage_question(vector<string> que_arr);
string asking_differ_question(vector<string> que_arr);
string symbol_usage_question(string que);
string view_answer(string str_package);
string view_topic();
void split(const string& s,vector<string>& sv, char flag);
int split_match(string package, Topic *head);
string get_answer(string directory, int match_val);
int exist_string(string words, string target);


/** function for file operation **/
void failure_log(string topic_name, string question);


/** function for structure construction **/
void cons_topic_list(topic_list **head, topic_list **tail, string topic_name);
void cons_keyword_BST(string keyword, BST ** head, int match_val);
Topic * set_up_topic(string topic_name);
topic_list *cons_topic_from_file();
BST* cons_keyword_from_file(string filename, int base_weight);


/** function for printing structure **/
void printTopicList(topic_list *head);


/** function for memory management **/
void free_list(topic_list *head);
void free_keyword_tree(BST *head);
void free_topic(Topic *head);

/** function for database construction **/
int cons_database(string topic);
int translation(Topic *head, string filepath);




/** end of func_main.h **/
