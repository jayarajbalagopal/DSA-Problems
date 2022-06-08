#include <iostream>
#include <string>

using namespace std;

struct Node{
	char data;
	Node* next[26];
	string word;
	bool word_ending = false;
};

void insert_string(Node* root, string input, int index){
	if(root){
		int n = input.size();
		if(index < n){
			int temp = input[index] - 'a';
			if(root->next[temp] == NULL){
				Node* char_node = new Node();
				char_node->data = input[index];
				for(int i=0;i<26;i++)
					char_node->next[i] = NULL;
				root->next[temp] = char_node;
			}
			insert_string(root->next[temp], input, index+1);
		}
		else{
			root->word = input;
			root->word_ending = true;
		}
	}
}

void view(Node* root){
	if(root){
		if(root->word_ending)
			cout << root->word << endl;
		for(int i=0;i<26;i++)
			view(root->next[i]);
	}
}

class trie
{
	Node* root;
public:
	trie(){
		root = new Node();
		root->data = '\0';
		for(int i=0;i<26;i++)
			root->next[i] = NULL;	
	}

	void insert(string input){
		insert_string(root, input, 0);
	}

	Node* getroot(){
		return root;
	}

	void viewstrings(){
		view(root);
	}
};

struct ListItem{
	string data;
	ListItem* next;
};

ListItem* merge_list(ListItem* list1, ListItem* list2){
    if(!list1)
        return list2;
    if(!list2)
        return list1;
    ListItem* head = NULL;
    ListItem* iter = NULL;
    while(list1 && list2){
        string temp_val;
        if(list1->data < list2->data){
            temp_val = list1->data;
            list1 = list1->next;
        }
        else{
            temp_val = list2->data;
            list2 = list2->next;
        }
        ListItem* temp_node = new ListItem();
        temp_node->data = temp_val;
        if(iter == NULL){
            iter = temp_node;
            head = temp_node;
        }
        else{
            iter->next = temp_node;
            iter = temp_node;
        }
    }
    if(list1)
        iter->next = list1;
    if(list2)
        iter->next = list2;
    return head;
}

ListItem* sort_list(ListItem* head){
	if(head == NULL || head->next == NULL)
        return head;
    int size = 0;
    ListItem* temp = head;
    while(temp){
        size++;
        temp = temp->next;
    }
    ListItem* list1 = head;
    temp = head;
    int count = 0;
    while(count < (size/2)-1){
        temp = temp->next;
        count++;
    }
    ListItem* list2 = temp->next;
    temp->next = NULL;
    ListItem* left = sort_list(list1);
    ListItem* right = sort_list(list2);
    return merge_list(left, right);
}

class list{
	ListItem* head;
	ListItem* tail;
public:
	list(){
		head = NULL;
		tail = NULL;
	}

	void insert(string input){
		ListItem* node = new ListItem();
		node->data = input;
		node->next = NULL;
		if(head == NULL){
			head = tail = node;
		}
		else{
			tail->next = node;
			tail = node;
		}
	}

	void sortlist(){
		head = sort_list(head);
	}

	void viewList(){
		ListItem* temp = head;
		while(temp){
			cout << temp->data << endl;
			temp = temp->next;
		}
	}
};


void dfs(char puzzle_grid[][100], list& result, Node* root, int i, int j, int row, int column){
    char ch = puzzle_grid[i][j];
    int temp = ch-'a';

    if(ch != '\0' && root->next[temp] != NULL){
        puzzle_grid[i][j] = '\0';
        if(i-1>=0){
            dfs(puzzle_grid, result, root->next[temp], i-1, j, row, column);
        }
        if(i+1<row){
            dfs(puzzle_grid, result, root->next[temp], i+1, j, row, column);
        }
        if(j-1>=0){
            dfs(puzzle_grid, result, root->next[temp], i, j-1, row, column);
        }
        if(j+1<column){
            dfs(puzzle_grid, result, root->next[temp], i, j+1, row, column);
        }
        if(root->next[temp]->word_ending){
            result.insert(root->next[temp]->word);
            root->next[temp]->word_ending = false;
        }
    }
    puzzle_grid[i][j] = ch;
}

void identify_words(char puzzle_grid[][100], trie t, int row, int column){
	list result;
    Node* root = t.getroot();
    for(int i=0;i<row;i++)
        for(int j=0;j<column;j++)
            dfs(puzzle_grid, result, root, i, j, row, column);
    result.sortlist();
    result.viewList();
}

int main(){
	int row, column;
	cin >> row >> column;
	char puzzle_grid[100][100];
	for(int i=0;i<row;i++){
		for(int j=0;j<column;j++){
			cin >> puzzle_grid[i][j];
		}
	}
	int x;
	cin >> x;
	trie t;
	for(int i=0;i<x;i++){
		string temp;
		cin >> temp;
		t.insert(temp);
	}
	identify_words(puzzle_grid, t, row, column);
	return 0;
}
