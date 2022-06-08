#include <iostream>

using namespace std;

#define ll long long int

struct Node{
	int data;
	Node* next[2];
	ll num = 0;
	bool num_ending = false;
};

void insert_num(Node* root, ll num, int pos){
	if(pos>=0){
		int bit = ((num>>pos)&1);
		if(root->next[bit] == NULL){
			Node* node = new Node();
			node->data = bit;
			if(pos == 0){
				node->num_ending = true;
				node->num = num;
			}
			for(int i=0;i<2;i++)
				node->next[i] = NULL;
			root->next[bit] = node;
			insert_num(root->next[bit], num, pos-1);	
		}
		else{
			if(pos == 0){
				root->next[bit]->num_ending = true;
				root->next[bit]->num = num;
			}
			insert_num(root->next[bit], num, pos-1);
		}
	}
}

ll find_max(Node* root, ll num, int pos){
	int bit = ((num>>pos)&1);
	if(root->next[!bit] != NULL){
		return find_max(root->next[!bit], num, pos-1);
	}
	if(root->next[bit] != NULL){
		return find_max(root->next[bit], num, pos-1);
	}
	return (root->num^num);
}

void view_numbers(Node* root){
	if(root){
		if(root->num_ending)
			cout << root->num << endl;
		for(int i=0;i<2;i++)
			view_numbers(root->next[i]);
	}
}

class trie
{
	Node* root;
public:
	trie(){
		root = new Node();
		root->data = '\0';
		for(int i=0;i<2;i++)
			root->next[i] = NULL;	
	}

	void insert(ll input){
		insert_num(root, input, 39);
	}

	ll findmax(ll input){
		return find_max(root, input, 39);
	}

	void viewnumbers(){
		view_numbers(root);
	}
};

int main(){
	int n,q;
	cin >> n >> q;
	trie t;
	for(int i=0;i<n;i++){
		ll temp;
		cin >> temp;
		t.insert(temp);
	}
	for(int i=0;i<q;i++){
		ll temp;
		cin >> temp;
		cout << t.findmax(temp) << endl;
	}
	return 0;
}
