#include <iostream>
#include <string>

using namespace std;

template <class T>
class dequeue
{	
	T* dq;
	T def;
	T err = T();
	int front_p;
	int rear_p;
	int capacity;
	int size_current;
public:
	dequeue(){
		dq = new T[1];
		front_p = rear_p = -1;
		size_current = 0;
		capacity = 1;
	}

	dequeue(int n, T x){
		capacity = 2*n;
		def = x;
		front_p = 0;
		rear_p = n-1;
		size_current = n;
		dq = new T[capacity];
		for(int i=front_p;i<=rear_p;i++)
			dq[i] = def;
	}

	void extend_capacity(int c){
		T* temp = new T[c];
		int idx = 0;
		int count = 0;
		if(front_p <= rear_p){
			int i = front_p;
			while(idx<c && i<=rear_p){
				temp[idx] = dq[i];
				idx++;
				i++;
				count++;
			}
		}
		else{
			int i = front_p;
			while(idx<c && i<capacity){
				temp[idx] = dq[i];
				idx++;
				i++;
				count++;
			}
			i = 0;
			while(idx<c && i<=rear_p){
				temp[idx] = dq[i];
				idx++;
				i++;
				count++;
			}
		}
		dq = temp;
		front_p = 0;
		rear_p = count-1;
		capacity = c;
	}

	void push_back(T data){
		if(size_current == capacity){
			extend_capacity(2*capacity);
		}		
		if(front_p == -1){
			front_p = rear_p = 0;
		}
		else{
			rear_p = (rear_p+1)%capacity;
		}
		dq[rear_p] = data;
		size_current++;
	}

	void push_front(T data){
		if(size_current == capacity){
			extend_capacity(2*capacity);
		}
		if(front_p == -1){
			front_p = rear_p = 0;
		}
		else{
			front_p = front_p-1;
			if(front_p < 0)
				front_p = capacity + front_p;	
		}
		dq[front_p] = data;
		size_current++;
	}

	void pop_front(){
		if(size_current>0){
			front_p = (front_p+1)%capacity;
			size_current--;
		}
	}

	void pop_back(){
		if(size_current>0){
			rear_p = rear_p-1;
			if(rear_p < 0)
				rear_p = capacity+rear_p;
			size_current--;
		}
	}

	T front(){
		if(size_current == 0)
			return err;
		return dq[front_p];
	}

	T back(){
		if(size_current == 0)
			return err;
		return dq[rear_p];
	}

	void resize(int x, T def){
		extend_capacity(x);
		if(x>size_current){
			for(int i=size_current;i<capacity;i++)
				dq[i] = def;
		}
		front_p = 0;
		rear_p = x-1;
		size_current = x;
	}

	int size(){
		return size_current;
	}

	bool empty(){
		if(size_current == 0)
			return true;
		return false;
	}

	void clear(){
		front_p = -1;
		rear_p = -1;
		size_current = 0;
	}

	T &operator [] (int index){
		int pos = (front_p+index)%capacity;
		if(index >= size_current || index <0 || pos > rear_p)
			return err;
		return dq[pos];
	}

	int get_capacity(){
		return capacity;
	}

	void display(){
		if(size_current>0){
			if(front_p<=rear_p){
				for(int i=front_p;i<=rear_p;i++)
					cout << dq[i] << " ";
			}
			else{
				for(int i=front_p;i<capacity;i++)
					cout << dq[i] << " ";
				for(int i=0;i<=rear_p;i++)
					cout << dq[i] << " ";
			}
			cout << endl;
		}
	}

};

int main(){
	dequeue<string> d;
	d.push_back("1");
	d.push_front("2");
	d.push_front("3");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.push_front("4");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_front();
	d.pop_front();
	d.pop_front();
	d.pop_front();
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.push_back("1");
	d.push_back("2");
	d.push_back("3");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.push_back("4");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_back();
	d.pop_front();
	d.pop_back();
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_back();
	d.push_front("2");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.push_back("3");
	d.push_front("1");
	d.push_back("4");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.resize(5,"a");
	cout << d.front() << endl;
	cout << d.back() << endl;
	cout << d.size() << endl;
	d.pop_back();
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.resize(5, "b");
	d.resize(6,"c");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_back();
	d.pop_back();
	cout << d.back() << endl;
	d.pop_front();
	d.pop_back();
	d.push_front("1");
	d.push_back("4");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.resize(3,"0");
	cout << d.back() << endl;
	d.resize(4,"0");
	d.pop_back();
	cout << d.back() << endl;
	d.push_back("4");
	d.pop_front();
	d.pop_front();
	d.pop_front();
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.push_front("1");
	d.push_front("2");
	d.push_front("3");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_front();
	d.pop_front();
	d.push_back("5");
	d.push_back("6");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_front();
	d.pop_front();
	d.pop_front();
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.push_back("7");
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_back();
	cout << d.front() << endl;
	cout << d.back() << endl;
	d.pop_back();
	cout << d.front() << endl;
	cout << d.back() << endl;
	return 0;
}
