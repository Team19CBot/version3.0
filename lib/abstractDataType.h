
/** abstractDataType.h **/
using namespace std;
struct binary_search_tree {
        binary_search_tree *left;
        binary_search_tree *right;
        int match_val;
        string keyword;
};
typedef binary_search_tree BST;

struct Topic {
        BST *level1_head;
        BST *level2_head;
};

struct topic_list {
        string name;
        topic_list *next;
};


/** end of abstractDataType.h **/
