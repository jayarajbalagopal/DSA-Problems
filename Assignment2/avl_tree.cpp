#include <iostream>
#include <math.h>

using namespace std;

template <class T>
struct Node{
	T data;
	int count;
	int count_left;
	int count_right;
	Node* left;
	Node* right;
	int height;
};

template <class T>
int find_height(Node<T>* root){
	if(!root)
		return -1;
	else
		return 1+max(find_height(root->left), find_height(root->right));
}

template <class T>
int node_height(Node<T>* root){
	if(!root)
		return -1;
	int hl = root->left==NULL?-1:root->left->height;
	int hr = root->right==NULL?-1:root->right->height;
	return 1+max(hl,hr);
}

template <class T>
Node<T>* balance(Node<T>* root, char imbalance[2]){
	int lh = node_height(root->left);
	int rh = node_height(root->right);

	if(imbalance[0] == 'L' && imbalance[1] == 'L'){
		Node<T>* left = root->left;
		Node<T>* buffer = left->right;
		root->left = buffer;
		left->right = root;
		if(buffer)
			root->count_left = buffer->count_right+buffer->count_left+1;
		else
			root->count_left = 0;
		left->count_right = root->count_left+root->count_right+1;
		root->height = node_height(root);
		left->height = node_height(left);
		return left;
	}
	else if(imbalance[0] == 'L' && imbalance[1] == 'R'){
		Node<T>* lr = root->left->right;
		Node<T>* buffer = root->left;
		root->left = lr;
		Node<T>* temp = lr->left;
		lr->left = buffer;
		buffer->right = temp;
		if(temp)
			buffer->count_right = temp->count_left+temp->count_right+1;
		else
			buffer->count_right = 0;
		lr->count_left = buffer->count_right+buffer->count_left+1;
		buffer->height = node_height(buffer);
		lr->height = node_height(lr);
		Node<T>* left = root->left;
		buffer = left->right;
		root->left = buffer;
		left->right = root;
		if(buffer)
			root->count_left = buffer->count_right+buffer->count_left+1;
		else
			root->count_left = 0;
		left->count_right = root->count_left+root->count_right+1;
		root->height = node_height(root);
		left->height = node_height(left);
		return left;
	}
	else if(imbalance[0] == 'R' && imbalance[1] == 'L'){
		Node<T>* rl = root->right->left;
		Node<T>* buffer = root->right;
		root->right = rl;
		Node<T>* temp = rl->right;
		rl->right = buffer;
		buffer->left = temp;
		if(temp)
			buffer->count_left = temp->count_left+temp->count_right+1;
		else
			buffer->count_left = 0;
		rl->count_right = buffer->count_left+buffer->count_right+1;
			buffer->height = node_height(buffer);
		rl->height = node_height(rl);
		Node<T>* right = root->right;
		buffer = right->left;
		root->right = buffer;
		right->left = root;
		if(buffer)
			root->count_right = buffer->count_right+buffer->count_left+1;
		else
			root->count_right = 0;
		right->count_left = root->count_left+root->count_right+1;
		root->height = node_height(root);
		right->height = node_height(right);
		return right;
	}
	else{
		Node<T>* right = root->right;
		Node<T>* buffer = right->left;
		root->right = buffer;
		right->left = root;
		if(buffer)
			root->count_right = buffer->count_right+buffer->count_left+1;
		else
			root->count_right = 0;
		right->count_left = root->count_left+root->count_right+1;
		root->height = node_height(root);
		right->height = node_height(right);
		return right;
	}
}

template <class T>
Node<T>* balance_insert(Node<T>* root, T data){
	int lh = node_height(root->left);
	int rh = node_height(root->right);
	if(abs(lh-rh)<=1)
		return root;

	char imbalance[2];
	if(lh>rh){
		imbalance[0] = 'L';
		if(data > root->left->data)
			imbalance[1] = 'R';
		else
			imbalance[1] = 'L';

	}
	else{
		imbalance[0] = 'R';
		if(data < root->right->data)
			imbalance[1] = 'L';
		else
			imbalance[1] = 'R';
	}

	return balance(root, imbalance);
}

template <class T>
Node<T>* balance_remove(Node<T>* root){
	int lh = node_height(root->left);
	int rh = node_height(root->right);
	if(abs(lh-rh)<=1)
		return root;

	char imbalance[2];
	if(lh>rh){
		imbalance[0] = 'L';
		if(root->left->left)
			imbalance[1] = 'L';
		else
			imbalance[1] = 'R';

	}
	else{
		imbalance[0] = 'R';
		if(root->right->right)
			imbalance[1] = 'R';
		else
			imbalance[1] = 'L';
	}
	return balance(root, imbalance);
}

template <class T>
Node<T>* insert_node(Node<T>* root, T data){
	if(root == NULL){
		Node<T>* temp = new Node<T>();
		temp->data = data;
		temp->left = NULL;
		temp->right = NULL;
		temp->count = 1;
		temp->count_right = 0;
		temp->count_left = 0;
		temp->height = 0;
		return temp;
	}
	else if(data > root->data){
		root->right = insert_node(root->right, data);
		root->count_right++;
		root->height = node_height(root);
		return balance_insert(root, data);
	}
	else if(data < root->data){
		root->left = insert_node(root->left, data);
		root->count_left++;
		root->height = node_height(root);
		return balance_insert(root, data);
	}
	else{
		root->count++;
		return root;
	}
}

template <class T>
Node<T>* remove_node(Node<T>* root, T data, int freq){
	if(!root){
		return NULL;
	}
	if(root->data == data){
		if(root->left == NULL && root->right == NULL){
			delete(root);
			return NULL;
		}
		else if(root->left && !root->right){
			delete(root);
			return root->left; 	
		}
		else if(root->right && !root->left){
			delete(root);
			return root->right;
		}
		else{
			Node<T>* temp = root->left;
			while(temp->right){
				temp = temp->right;
			}
			T buffer = root->data;
			root->data = temp->data;
			temp->data = buffer;
			root->left = remove_node(root->left, data, freq);
			root->count_left =  root->count_left - freq;
			root->height = node_height(root);
			return balance_remove(root);
		}
	}
	else if(data > root->data){
		root->right = remove_node(root->right, data, freq);
		root->count_right = root->count_right - freq;
		root->height = node_height(root);
		return balance_remove(root);
	}
	else{
		root->left = remove_node(root->left, data, freq);
		root->count_left =  root->count_left - freq;
		root->height = node_height(root);
		return balance_remove(root);
	}
}

template <class T>
bool search_node(Node<T>* root, T data){
	if(!root)
		return false;
	else if(root->data == data)
		return true;
	else if(data > root->data)
		return search_node(root->right, data);
	else
		return search_node(root->left, data);
}

template <class T>
int get_frequency(Node<T>* root, T data){
	if(!root)
		return -1;
	else if(root->data == data)
		return root->count;
	else if(data > root->data)
		return get_frequency(root->right, data);
	else
		return get_frequency(root->left, data);
}

template <class T>
Node<T>* ub(Node<T>* root, T data){
	if(!root)
		return NULL;
	if(root->data <= data)
		return ub(root->right, data);
	Node<T>* temp = ub(root->left, data);
	if(temp && temp->data > data)
		return temp;
	return root;
}

template <class T>
Node<T>* lb(Node<T>* root, T data){
	if(!root)
		return NULL;
	if(root->data == data)
		return root;
	if(root->data < data)
		return lb(root->right, data);
	Node<T>* temp = lb(root->left, data);
	if(temp && temp->data >= data)
		return temp;
	return root;
}

template <class T>
void closest_element(Node<T>* root, T data, T *res, T* mindiff){
	if(root){
		if(root->data == data){
			*mindiff = 0;
			*res = root->data;
		}

		T diff = abs(root->data-data);
		if(diff < *mindiff){
			*mindiff = diff;
			*res = root->data;
		}
		if(data < root->data)
			closest_element(root->left, data, res, mindiff);
		else
			closest_element(root->right, data, res, mindiff);
	}
}

template <class T>
Node<T>* min_val(Node<T>* root){
	if(!root)
		return NULL;
	if(!root->left)
		return root;
	return min_val(root->left);
}

template <class T>
Node<T>* max_val(Node<T>* root){
	if(!root)
		return NULL;
	if(!root->right)
		return root;
	return max_val(root->right);
}

template <class T>
Node<T>* largest(Node<T>*root, int* k){
	if(!root)
		return NULL;
	Node<T>* temp = largest(root->right, k);
	if(*k<=0)
		return temp;
	*k = *k-root->count;
	if(*k<=0)
		return root;
	return largest(root->left, k);
}

template <class T>
Node<T>* lca(Node<T>* root ,T a ,T b){
    if(root){
        if(root->data == a || root->data == b)
            return root;
        else{
            Node<T>* left = lca(root->left, a, b);
            Node<T>* right = lca(root->right, a, b);
            if(left && right)
                return root;
            if(left)
                return left;
            else
                return right;
        }
    }
    else{
        return NULL;
    }
}

template <class T>
void inorder_b(Node<T>* root){
	if(root){
		inorder_b(root->left);
		for(int i=0;i<root->count;i++)
			cout << root->data << " "; 
		inorder_b(root->right);
	}
}

template <class T>
int rank_of(Node<T>* root, T data){
	if(!root)
		return 0;
	if(root->data == data)
		return root->count_left;
	else if(data > root->data)
		return root->count_left + root->count + rank_of(root->right, data);
	else
		return rank_of(root->left, data);
}

template <class T>
class avlTree
{
	Node<T>* root;
	int total;
public:
	avlTree(){
		root = NULL;
		total = 0;
	}

	void insert(T data){
		root = insert_node(root, data);
		total++;
	}

	bool remove(T data){
		if(!search_node(root, data))
			return false;
		int freq = get_frequency(root, data);
		root = remove_node(root, data, freq);
		total-=freq;
		return true;
	}

	void inorder(){
		inorder_b(root);
		cout << endl;
	}

	bool search(T data){
		return search_node(root, data);
	}

	int frequency(T data){
		return get_frequency(root, data);
	}

	T upperbound(T data){
		Node<T>* n = ub(root, data);
		if(n)
			return n->data;
		return T();
	}

	T lowerbound(T data){
		Node<T>* n = lb(root, data);
		if(n)
			return n->data;
		return T();
	}

	T kth_largest(int k){
		int count = k;
		Node<T>* temp = largest(root, &count);
		return temp->data;
	}

	T lowest_ancestor(T a, T b){
		return lca(root, a, b)->data;
	}

	T max(){
		Node<T>* temp = max_val(root);
		return temp->data;
	}

	T min(){
		Node<T>* temp = min_val(root);
		return temp->data;
	}

	T closest(T data){
		T min = min_val(root)->data;
		T max = max_val(root)->data;
		T diff = abs(max-min);
		T result;
		closest_element(root, data, &result, &diff);
		return result;
	}

	int range(T start, T end){
		int range_count = rank_of(root, end)-rank_of(root, start);
		if(search_node(root, end))
			range_count += get_frequency(root, end);
		return range_count;
	}

	int height(){ 
		return find_height(root);	
	}

	Node<T>* get_root(){
		return root;
	}
};

class data
{	int element;
public:
	data(){
		element = 0;
	}
	data(int element){
		this->element = element;
	}

	bool operator > (const data& b){
		return (this->element > b.element);
	}

	bool operator < (const data& b){
		return (this->element < b.element);
	}

	bool operator >= (const data& b){
		return (this->element >= b.element);
	}

	bool operator <= (const data& b){
		return (this->element <= b.element);
	}

	bool operator == (const data& b){
		return (this->element == b.element);
	}


	bool operator != (const data& b){
		return (this->element != b.element);
	}

	data operator - (const data& b){
		return data(this->element - b.element);
	}

	friend int abs(const data& a){
		if(a.element<0)
			return -(a.element);
		else
			return a.element;
	}

	friend ostream& operator << (ostream& out, data a){
		out << a.element;
		return out;
	}
};

int main(){
	avlTree<data> t;

	for(int i=0;i<40;i++){
		data n(i);
		t.insert(n);
	}
	for(int i=60;i<80;i++){
		data n(i);
		t.insert(n);
	}
	for(int i=100;i<140;i++){
		data n(i);
		t.insert(n);
	}
	for(int i=65;i<75;i++){
		data n(i);
		t.insert(n);
	}
	for(int i=65;i<75;i++){
		data n(i);
		t.remove(n);
	}

	data beg(70);
	data end(140);
	cout << t.range(beg, end) << endl;
	cout << t.closest(data(59)) << endl;
 	t.inorder();
	return 0;
}