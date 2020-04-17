
/**  func_main.h **/
using namespace std;

/** function for matching **/
string match(string que, topic_list* head);
string match_solution(int match_val, string topic_name);
int match_special_word(string word);
int match_keyword(string word, Topic *topic, int level_id);
int match_symbol(string que);
string get_main_keyword(string topic_name, int level_id);

/** function for string operation **/
string function_usage_question(vector<string> que_arr);
string asking_differ_question(vector<string> que_arr);
string symbol_usage_question(string que);
string view_topic();
string view_statistics();
void split(const string& s,vector<string>& sv, char flag);
int split_match(string package, Topic *head);

/** function for log  **/
void failure_log(string topic_name, string question);
void modify_log(string topic_name);
void add_log(string topic_name, string que_word, string keyword1, string keyword2);
void delete_log(string topic_name, string target);

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
void modify_regular_answer(string que, string new_answer, string name);
void modify_database(string package, topic_list* head);
void modify_pattern(string new_answer, string topic_name);
void modify_answer(string new_answer, string topic_name, string pattern);
int check_keyword_is_topic(string k1, string k2);
int translation(Topic *head, string filepath);
int cons_database(string topic);
string add_answer(string pacakge);
int check_keyword_exist(string path, string keyword);
void add_keyword(string path, string keyword);
int specify_add_type(string topic_name, string keyword1, string keyword2);
void add_solution_file(int type_val, string topic_name, string que_word, string keyword1, string keyword2, string new_answer);
void add_que_pattern(string path, string que_pattern, string new_answer);
string delete_answer(string package);
void delete_solution_file(string topic_name, string target);

/** function for monitor statistics **/
void monitor_statistics(string topic_name);
void reset_statistics();
string view_error_rate();
string failure_statistics(string topic_name);
void reset_error_rate();

/** function for communication between front-end and back-end **/
char* getcgidata(FILE* fp, char* requestmethod);

/** end of func_main.h **/
