#include <iostream>

using namespace std;

#define ROW_MAX 500
#define COLUMN_MAX 500

template <class T>
struct Node{
	T data;
	int i;
	int j;
	struct Node* next;
	struct Node* prev;
};

template <class T>
class LinkedSparseMat{
	Node<T>* head;
	Node<T>* tail;
	int row, column;
public:
	LinkedSparseMat(){
		head = NULL;
		tail = NULL;
		row = 0;
		column = 0;
	}

	LinkedSparseMat(int n, int m){
		row = n;
		column = m;
		head = NULL;
		tail =  NULL;
	}

	LinkedSparseMat(T mat[][COLUMN_MAX], int n, int m){
		head = NULL;
		tail = NULL;
		row  = n;
		column = m;
		for(int i=0;i<n;i++)
			for(int j=0;j<m;j++){
				if(mat[i][j]!=0){
					Node<T>* temp = new Node<T>();
					temp->data = mat[i][j];
					temp->i = i;
					temp->j = j;
					if(head == NULL){
						head = tail = temp;
					}
					else{
						tail->next = temp;
						temp->prev = tail;
						tail = temp;
					}
				}
			}
	}


	void push(T data, int i, int j){
		Node<T>* temp = new Node<T>();
		temp->data = data;
		temp->i = i;
		temp->j = j;
		if(head == NULL){
			head = tail = temp;
		}
		else{
			tail->next = temp;
			tail = temp;
		}
	}

	void remove(Node<T>* node){
		Node<T>* next = node->next;
		Node<T>* prev = node->prev;
		if(next)
			next->prev = prev;
		if(prev)
			prev->next = next;
		if(node == head)
			head = next;
		if(node == tail)
			tail = prev;
	}

	void take_input(int n, int m){
		for(int i=0;i<n;i++){
			for(int j=0;j<m;j++){
				T temp;
				cin >> temp;
				if(temp != 0)
					push(temp, i, j);
			}
		}
		row = n;
		column = m;
	}

	void transpose(){
		Node<T>* list = head;
		int* count = new int[column]{0};
		int size = 0;
		while(list){
			size++;
			count[list->j]++;
			list = list->next;
		}
		int* index = new int[column]{0};
		int stn = 0;
		for(int i=0;i<column;i++){
			index[i] = stn;
			stn+= count[i];
		}
		Node<T>** order = new Node<T>*[size];
		list = head;
		while(list){
			int j = list->j;
			int temp = list->i;
			list->i = list->j;
			list->j = temp;
			order[index[j]] = list;
			index[j]++;
			list = list->next;
		}
		for(int i=0;i<size-1;i++){
			order[i]->next = order[i+1];
			order[i+1]->prev = order[i];
		}
		tail = order[size-1];
		tail->next = NULL;
		head = order[0];
		int temp = row;
		row = column;
		column = temp;
	}

	friend LinkedSparseMat operator + (const LinkedSparseMat& a, const LinkedSparseMat& b){
		Node<T>* mat1 = a.head;
		Node<T>* mat2 = b.head;
		LinkedSparseMat<T> result(a.row, a.column);
		while(mat1 && mat2){
			if(mat1->i == mat2->i){
				if(mat1->j == mat2->j){
					if(mat1->data+mat2->data != 0){
						result.push(mat1->data+mat2->data, mat1->i, mat1->j);
					}
					mat1 = mat1->next;
					mat2 = mat2->next;
				}
				else if(mat1->j < mat2->j){
					result.push(mat1->data, mat1->i, mat1->j);
					mat1 = mat1->next;
				}
				else{
					result.push(mat2->data, mat2->i, mat2->j);
					mat2 = mat2->next;
				}
			}
			else if(mat1->i < mat2->i){
				while(mat1 && mat1->i < mat2->i){
					result.push(mat1->data, mat1->i, mat1->j);
					mat1 = mat1->next;
				}
			}
			else{
				while(mat2 && mat2->i < mat1->i){
					result.push(mat2->data, mat2->i, mat2->j);
					mat2 = mat2->next;
				}
			}
		}
		while(mat1){
			result.push(mat1->data, mat1->i, mat1->j);
			mat1 = mat1->next;
		}
		while(mat2){
			result.push(mat2->data, mat2->i, mat2->j);
			mat2 = mat2->next;
		}
		return result;
	}

	friend LinkedSparseMat operator * (LinkedSparseMat a, LinkedSparseMat b){
		int r = a.row;
		int c = b.column;
		Node<T>* mat1 = a.head;
		Node<T>* mat2 = b.head;
		LinkedSparseMat<T> intermiedate(r, c);
		b.transpose();
		while(mat1){
			while(mat2){
				if(mat1->j ==mat2->j){
					intermiedate.push(mat1->data*mat2->data, mat1->i, mat2->i);
				}
				mat2 = mat2->next;
			}
			mat2 = b.head;
			mat1 = mat1->next;
		}

		LinkedSparseMat<T> result(r, c);
		Node<T>* list = intermiedate.head;
		while(list){
			bool found = false;
			Node<T>* temp_list = result.head;
			while(temp_list){
				if(temp_list->i == list->i && temp_list->j == list->j){
					temp_list->data += list->data;
					found = true;
				}
				temp_list = temp_list->next;
			}
			if(!found){
				result.push(list->data, list->i, list->j);
			}
			list = list->next;
		}
		list = result.head;
		while(list){
			if(list->data == 0)
				result.remove(list);
			list = list->next;
		}
		b.transpose();
	 	return result;
	}

	void viewList(){
		Node<T>* list = head;
		while(list){
			cout << list->data << " ";
			list = list->next;
		}
		if(head)
			cout << endl;
	}

	void viewmatrix(){
		T **matrix = new T*[row];
		for(int i=0;i<row;i++)
			matrix[i] = new T[column]{0};
		Node<T>* temp = head;
		while(temp){
			matrix[temp->i][temp->j] = temp->data;
			temp = temp->next;
		}
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++)
				cout << matrix[i][j] << " ";
			cout << endl;
		}
	}
};

/*
SAMPLE INPUT
1 // No of Testcases

2 // Multiply
2 2 // Dimensions of Matrix 1
2.1 2.3
4.1 2.2
2 2 // Dimensions of Matrix 2
2 1.7
4.1 2.2

SAMPLE OUTPUT
13.63 8.63 
17.22 11.81
*/

int main(){
	int t;
	cin >> t;
	while(t--){
		int action;
		cin >> action; // 1 is ADD, 2 is MULTIPLY, 3 is Transpose
		if(action == 1){
			int n,m;
			cin >> n >> m;
			LinkedSparseMat<double> M;
			M.take_input(n,m);
			cin >> n >> m;
			LinkedSparseMat<double> N;
			N.take_input(n,m);
			(M+N).viewmatrix();
		}
		else if(action == 2){
			int n,m;
			cin >> n >> m;
			LinkedSparseMat<double> M;
			M.take_input(n,m);
			cin >> n >> m;
			LinkedSparseMat<double> N;
			N.take_input(n,m);
			(M*N).viewmatrix();
		}
		else if(action == 3){
			int n,m;
			cin >> n >> m;
			LinkedSparseMat<double> M;
			M.take_input(n,m);
			M.transpose();
			M.viewmatrix();
		}
		cout << endl;
	}
	return 0;
}