#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#define ALPHA 0.75

template <class T, class S>
struct Node{
	T key;
	S data;
	Node* next;
};

template <class T, class S>
class unordered_map
{
	Node<T,S> **hash_table;
	int total_size;
	int bucket_size;
	S def = S();

	string convert(T key){
		stringstream res;
		res << key;
		return res.str();
	}

	int hashvalue(T key){
		int p = 53;
		string temp = convert(key);
		int s = temp.size();
		long long int hashv = 0;
		long long pv = 53;
		for(int i=0;i<s;i++){
			hashv = (hashv + (temp[i]*pv%bucket_size)%bucket_size)%bucket_size;
			pv = (pv*53)%bucket_size;
		}
		return hashv%bucket_size;
	}

	void rehash(){
		int old_size = bucket_size;
		bucket_size = 2*bucket_size;
		Node<T,S> **temp = new Node<T,S>*[bucket_size];
		for(int i=0;i<old_size;i++){
			Node<T,S>* list = hash_table[i];
			while(list){
				int hashv = hashvalue(list->key);
				Node<T,S>* node = new Node<T,S>();
				node->key = list->key;
				node->data = list->data;
				node->next = temp[hashv];
				temp[hashv] = node;
				list = list->next;
			}
		}
		delete(hash_table);
		hash_table = temp;
	}
public:
	unordered_map(){
		hash_table = new Node<T,S>*[10];
		bucket_size = 10;
		total_size = 0;
	}

	unordered_map(int n){
		hash_table = new Node<T, S>[n];
		bucket_size = n;
		total_size = 0;
	}

	bool find(T key){
		int hashv = hashvalue(key);
		Node<T,S>* list = hash_table[hashv];
		while(list){
			if(list->key == key)
				return true;
			list = list->next;
		}
		return false;
	}

	void insert(T key, S data){
		int hashv = hashvalue(key);
		if(find(key)){
			Node<T,S>* list = hash_table[hashv];
			while(list){
				if(list->key == key){
					list->data = data;
					break;
				}
				list = list->next;
			}
		}
		else{
			Node<T,S>* node = new Node<T,S>();
			node->data = data;
			node->key = key;
			node->next = hash_table[hashv];
			hash_table[hashv] = node;
			total_size++;
			float lf = float(total_size)/float(bucket_size);
			if(lf > ALPHA)
				rehash();
		}
	}

	S &operator [] (T key){
		int hashv = hashvalue(key);
		Node<T,S>* list = hash_table[hashv];
		while(list){
			if(list->key == key)
				return list->data;
			list = list->next;
		}
		return def;
	}

	int size(){
		return total_size;
	}

	void viewTable(){
		for(int i=0;i<bucket_size;i++){
			cout << i << " : ";
			Node<T,S>* list = hash_table[i];
			while(list){
				cout << "(" << list->key << ":" << list->data << ")" << " ";
				list = list->next;
			}
			cout << endl;
		}
	}
};
