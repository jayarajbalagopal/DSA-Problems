#include <iostream>
#include <unordered_map>
#include <map>

using namespace std;

struct Node{
	int data;
	Node* next;
	Node* prev;
};

class doublyLinkedQueue{
	Node* head;
	Node* rear;
public:
	doublyLinkedQueue(){
		head = NULL;
		rear = NULL;
	}

	Node* push(int data){
		struct Node* temp = new Node();
		temp->data = data;
		if(head == NULL){
			head = rear = temp;
		}
		else{
			rear->next = temp;
			temp->prev = rear;
			rear = rear->next;
		}
		return temp;
	}

	int pop(){
		if(head == NULL){
			return -1;
		}
		else{
			int data = head->data;
			struct Node* temp = head;
			head = head->next;
			if(head == NULL)
				rear = NULL;
			else
				head->prev = NULL;
			free(temp);
			return data;
		}
	}

	void remove(Node* node){
		Node* prev = node->prev;
		Node* next = node->next;
        if(prev)
            prev->next = next;
        if(next)
            next->prev = prev;
        if(node == head)
            head = next;
        if(node == rear)
            rear = prev;
        free(node);
	}

	Node* get_head(){
		return head;
	}

	Node* get_rear(){
		return rear;
	}

	void view(){
		struct Node* list = head;
		while(list){
			cout << list->data << " ";
			list = list->next;
		}
	}
};

class LFU{
	int capacity;
	unordered_map<int, int> cache;
	unordered_map<int, int> key_freq_map;
	map<int, doublyLinkedQueue> freq_queue_map;
	unordered_map<int, Node*> key_node_map;
public:
	LFU(int c){
		capacity = c;
	}

	int get(int key){
		if(cache.find(key) != cache.end()){
			int old_freq = key_freq_map[key];
			key_freq_map[key]++;
			int new_freq = key_freq_map[key];
			Node* old_node = key_node_map[key];
			freq_queue_map[old_freq].remove(old_node);
			if(freq_queue_map[old_freq].get_head() == NULL)
				freq_queue_map.erase(old_freq);
			Node* new_node = freq_queue_map[new_freq].push(key);
			key_node_map[key] = new_node;
			return cache[key];
		}
		return -1;
	}

	void set(int key, int value){
		if(cache.find(key) != cache.end()){
			int old_freq = key_freq_map[key];
			key_freq_map[key]++;
			int new_freq = key_freq_map[key];
			Node* old_node = key_node_map[key];
			freq_queue_map[old_freq].remove(old_node);
			if(freq_queue_map[old_freq].get_head() == NULL)
				freq_queue_map.erase(old_freq);
			Node* new_node = freq_queue_map[new_freq].push(key);
			key_node_map[key] = new_node;
			cache[key] = value;
		}
		else{
			if(cache.size()<capacity){
				cache[key] = value;
				key_freq_map[key] = 1;
				if(freq_queue_map.find(1) == freq_queue_map.end()){
					doublyLinkedQueue q;
					Node* new_node = q.push(key);
					freq_queue_map[1] = q;
					key_node_map[key] = new_node;
				}
				else{
					Node* new_node = freq_queue_map[1].push(key);
					key_node_map[key] = new_node;
				}
			}
			else if(capacity != 0){
				int min_freq = freq_queue_map.begin()->first;
				int del_key = freq_queue_map[min_freq].pop();
				key_freq_map.erase(del_key);
				cache.erase(del_key);
				key_node_map.erase(del_key);
				cache[key] = value;
				key_freq_map[key] = 1;
				if(freq_queue_map.find(1) == freq_queue_map.end()){
					doublyLinkedQueue q;
					Node* new_node = q.push(key);
					freq_queue_map[1] = q;
					key_node_map[key] = new_node;
				}
				else{
					Node* new_node = freq_queue_map[1].push(key);
					key_node_map[key] = new_node;
				}
			}
		}
	}

	void viewcache(){
		for(auto c:cache)
			cout << c.first << ":" << c.second << ":" << key_freq_map[c.first] << endl;
	}
};

/*
SAMPLE INPUT
3 // Capacity

1 // Set
2 3 // Key, Value

1 // Set
3 4 // Key, Value

2 // Get
2 // Key

SAMPLE OUTPUT
3 // Only Gets will be displayed
*/

int main(){
	int capacity;
	cin >> capacity;
	LFU l(capacity);
	//1 is set 2 is get
	int no_of_operations;
	cin >> no_of_operations;
	for(int i=0;i<no_of_operations;i++){
		int action;
		cin >> action;
		if(action == 1){
			int key,value;
			cin >> key >>value;
			l.set(key,value);
		}
		else if(action == 2){
			int key;
			cin >> key;
			cout << l.get(key);
		}
	}
	return 0;	
}