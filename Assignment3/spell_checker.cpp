#include <iostream>
#include <string>
#include <vector>

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

bool search_string(Node* root, string input, int index){
	if(root){
		int n = input.size();
		if(index<n){
			int temp = input[index] - 'a';
			if(root->next[temp]){
				if(index == n-1 && root->next[temp]->word_ending)
					return true;
				return search_string(root->next[temp], input, index+1);
			}
			else{
				return false;
			}
		}
		return false;
	}
	return false;
}

void get_string(Node* root, vector<string>& result){
	if(root){
		if(root->word_ending)
			result.push_back(root->word);
		for(int i=0;i<26;i++)
			get_string(root->next[i], result);
	}
}

Node* match_node(Node* root, string input, int index){
	char ch = input[index];
	int temp = ch - 'a';
	int n = input.size();
	if(index<n){
		if(root->next[temp] != NULL)
			return match_node(root->next[temp], input, index+1);
		else
			return NULL;
	}
	else if(index == n){
		return root;
	}
	else{
		return NULL;
	}
}

int levenstein(string a, string b) {
	int n = a.size();
    int m = b.size();
    if(n == 0)
        return m;
    else if(m == 0)
        return n;
    else if(a[0] == b[0]){
        a.erase(a.begin());
        b.erase(b.begin());
        return levenstein(a, b);
    }
    else{
        string tempa = a;
        string tempb = b;
        tempa.erase(tempa.begin());
        tempb.erase(tempb.begin());
        return 1+min(levenstein(tempa,tempb), min(levenstein(a, tempb),levenstein(b, tempa)));
    }
}

void get_min_dist_strings(Node* root, string input, vector<string>& result){
	if(root){
		if(root->word_ending){
			if(levenstein(root->word, input)<=3)
				result.push_back(root->word);
		}
		for(int i=0;i<26;i++){
			get_min_dist_strings(root->next[i], input, result);
		}
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

	void viewstrings(){
		vector<string> result;
		get_string(root, result);
		for(auto w: result)
			cout << w << endl;
	}

	void autocomplete(string input){
		Node* temp = match_node(root, input, 0);
		vector<string> result;
		if(temp == NULL)
			get_string(root, result);
		else
			get_string(temp, result);
		cout << result.size() << endl;
		for(auto w: result)
			cout << w << endl;
	}

	void autocorrect(string input){
		vector<string> result;
		get_min_dist_strings(root, input, result);
		cout << result.size() << endl;
		for(auto w: result)
			cout << w << endl;	
	}

	bool search(string input){
		return search_string(root, input, 0);
	}
};

int main(){
	int n;
	cin >> n;
	trie t;
	for(int i=0;i<n;i++){
		string inp;
		cin >> inp;
		t.insert(inp);
	}
	int action;
	cin >> action;
	string check;
	cin >> check;
	if(action == 1){
		if(t.search(check))
			cout << 1 << endl;
		else
			cout << 0 << endl;
	}
	else if(action == 2){
		t.autocomplete(check);
	}
	else{
		t.autocorrect(check);
	}

	return 0;
} 