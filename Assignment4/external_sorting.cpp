#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <math.h>
#include <vector>
#include <unistd.h>

using namespace std;

//#define CHUNK_SIZE 10485760
#define CHUNK_SIZE 100

#define ll long long int

class number{
    ll num;
    ll chunk;
public:
    number(){
        num = -1;
        chunk = -1;
    }

    number(ll n, ll c){
        num = n;
        chunk = c;
    }

    ll get_chunk(){
        return chunk;
    }

    ll get_num(){
        return num;
    }

    bool operator < (const number& b){
        return num < b.num;
    }

    bool operator > (const number& b){
        return num > b.num;
    }

    bool operator != (const number& b){
        return (num!=b.num && chunk!=b.chunk);
    }

    friend ostream& operator << (ostream& os, const number& cur_num){
        os << cur_num.num;
        return os;
    }
};

template <class T>
void heapify_down(vector<T>& arr, ll size, ll root_index){
    ll left = (2*root_index)+1;
    ll right = (2*root_index)+2;
    ll min = root_index;
    
    if(left < size && arr[left]<arr[min])
        min = left;
    if(right < size && arr[right]<arr[min])
        min = right;

    if(min != root_index){
        T temp = arr[min];
        arr[min] = arr[root_index];
        arr[root_index] = temp;
        heapify_down(arr, size, min);
    }
}

template <class T>
void heapify_up(vector<T>& arr, ll size, ll element_index){
    ll parent = floor((element_index+1)/2)-1;

    if(parent >=0 && arr[parent] > arr[element_index]){
        T temp = arr[parent];
        arr[parent] = arr[element_index];
        arr[element_index] = temp;
        heapify_up(arr, size, parent);
    }
}

template <class T>
void build_heap(T arr[], ll size){
    for(ll i=(size/2)-1;i>=0;i--){
        heapify_up(arr, size, i);
    }
}

template <class T>
class heap{
    vector<T> arr;
    ll size;
    ll cur_size; 
public:
    heap(){
        size = 0;
    }

    heap(ll n){
        arr = vector<T>(n);
        size = n;
        cur_size = 0;
    }

    heap(ll n, vector<T> inp){
        arr.resize(n);
        for(int i=0;i<inp.size();i++)
            arr[i] = inp[i];
        size = n;
        cur_size = inp.size();
        build_heap(arr, cur_size);
    }

    int insert(T inp){
        if(size == cur_size){
            return -1;
        }
        else{
            arr[cur_size] = inp;
            cur_size++;
            ll parent = floor((cur_size)/2)-1;
            heapify_up(arr, cur_size, cur_size-1);
            return 1;
        }
    }

    T extract_min(){
        if(cur_size == 0){
            return number(-1,-1);
        }
        else{
            T min = arr[0];
            arr[0] = arr[cur_size-1];
            cur_size--;
            heapify_down(arr, cur_size, 0);
            return min; 
        }
    }

    void display_heap(){
        for(ll i=0;i<cur_size;i++)
            cout << arr[i] << " ";
        cout << endl;
    }
};

string dir = "temp";

void create_dir(){
    struct stat st = {0};

    if (stat(dir.c_str(), &st) == -1) {
        mkdir(dir.c_str(), 0700);
    }
}

void merge(ll* arr, ll l, ll m, ll r)
{
    ll n = m-l+1;
    ll s = r-m; 
    ll* a1 = new ll[n];
    ll* a2 = new ll[s];
    
    for(ll i=0;i<n;i++){
        a1[i] = arr[l+i];
    }
    for(ll i=0;i<s;i++){
        a2[i] = arr[m+1+i];
    }
        
    ll k = l;
    ll i = 0;
    ll j = 0;
    
    while(i<n && j<s){
        if(a1[i]<a2[j]){
            arr[k]=a1[i];
            i++;
        }
        else{
            arr[k]=a2[j];;
            j++;
        }
        k++;
    }
    while(i<n){
        arr[k] = a1[i];
        k++;
        i++;
    }
    while(j<s){
        arr[k] = a2[j];
        k++;
        j++;
    }

    delete[] a1;
    delete[] a2;
}

void merge_sort(ll* arr, ll l, ll r)
{
    if(l<r){
        ll mid = (l+r)/2;
        merge_sort(arr,l,mid);
        merge_sort(arr,mid+1,r);
        merge(arr, l, mid, r);
    }
}

void k_way_sort(ll no_of_chunks, string outfile){
    ifstream fd[no_of_chunks];
    for(ll i=0;i<no_of_chunks;i++){
        string filepath = "temp/sorted_chunk_" + to_string(i) + ".txt";
        fd[i].open(filepath);
    }

    string filename = outfile;
    ofstream out(filename, std::ios::out);

    heap<number> min_heap(no_of_chunks);
    string line;
    for(int i=0;i<no_of_chunks;i++){
        if(getline(fd[i],line,' ')){
           min_heap.insert(number(stoll(line),i));
        }
    }

    number m = min_heap.extract_min();
    bool first = true;
    while(m != number(-1,-1)){
        ll chunk = m.get_chunk();
        if(getline(fd[chunk], line, ' ')){
             min_heap.insert(number(stoll(line),chunk));
        }
        if(first){
            out << m.get_num();
            first = false;
        }
        else{
            out << "," << m.get_num();
        }
        m = min_heap.extract_min();
    }
}

void sort_and_store(ll* arr, ll s, int chunk_number){
    merge_sort(arr, 0, s-1);

    string filename = "temp/sorted_chunk_";
    filename = filename + to_string(chunk_number) + ".txt";
    ofstream fd(filename, std::ios::out);

    for(int i=0;i<s;i++){
        fd << arr[i] << " ";
    }

    fd.close();
}

void external_sort(string filepath, string outfile){
	ifstream fd(filepath);
	string line;
	ll* arr = (ll*)malloc(CHUNK_SIZE*sizeof(ll));
	ll index = 0;

    create_dir();

    int chunk = 0;

 	while(getline(fd,line,',')){
 		ll num = stoll(line);
 		arr[index%CHUNK_SIZE] = num; 
        if((index+1)%CHUNK_SIZE == 0){
            sort_and_store(arr, CHUNK_SIZE, chunk);
            index = 0;
            chunk++;
        }
        else{
            index++;
        }
    }
    if(index != 0){
        sort_and_store(arr, index, chunk);
        chunk++;
    }
    k_way_sort(chunk, outfile);
    for(int i=0;i<chunk;i++){
        string filename = "temp/sorted_chunk_"+to_string(i)+".txt";
        remove(filename.c_str());
    }
    rmdir("temp");
}

int main(int argc, char* argv[]){

	string filepath = argv[1];
    string outfile = argv[2];
	external_sort(filepath, outfile);
 	return 0;
}
