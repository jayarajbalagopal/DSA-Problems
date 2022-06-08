#include <iostream>

using namespace std;

#define ROW_MAX 500
#define COLUMN_MAX 500

template <class T>
class sparse{
	int row, column;
	int size;
	T** triplet;
public:
	sparse(){
		row = 0;
		column = 0;
		size = 0;
	}
	sparse(int n, int m){
		row = n;
		column = m;
		size = 0;
	}

	sparse(int n, int m, int s){
		row = n;
		column = m;
		size = s;
		triplet = new T*[s];
		for(int i=0;i<s;i++)
			triplet[i] = new T[3]{0};
	}

	void take_input(int n, int m){
		row = n;
		column = m;
		int c = 0;
		T **input = new T*[row];
		for(int i=0;i<row;i++)
			input[i] = new T[column]{0};

		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
				T temp;
				cin >> temp;
				input[i][j] = temp;
				if(temp != 0)
					c++;
			}
		}

		triplet = new T*[c];
		for(int i=0;i<c;i++)
			triplet[i] = new T[3]{0};

		int idx = 0;
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
				if(input[i][j] != 0){
					triplet[idx][0] = i;
					triplet[idx][1] = j;
					triplet[idx][2] = input[i][j];
					idx++;
				}
			}
		}
		size = c;
	}

	friend sparse operator + (const sparse& a, const sparse& b){
		int i = 0;
		int j = 0;
		sparse c(a.row, a.column, a.size+b.size);
		int count = 0;
		int idx = 0;
		while(i<a.size && j<b.size){
			if(a.triplet[i][0] == b.triplet[j][0]){
				if(a.triplet[i][1] == b.triplet[j][1]){
					if(a.triplet[i][2] + b.triplet[j][2] != 0){
						c.triplet[idx][0] = a.triplet[i][0];
						c.triplet[idx][1] = a.triplet[i][1];
						c.triplet[idx][2] = a.triplet[i][2] + b.triplet[j][2];
						i++;
						j++;
						idx++;
						count++;
					}
					else{
						i++;
						j++;
					}
				}
				else if(a.triplet[i][1]<b.triplet[j][1]){
					c.triplet[idx][0] = a.triplet[i][0];
					c.triplet[idx][1] = a.triplet[i][1];
					c.triplet[idx][2] = a.triplet[i][2];
					i++;
					idx++;
					count++;
				}
				else{
					c.triplet[idx][0] = b.triplet[j][0];
					c.triplet[idx][1] = b.triplet[j][1];
					c.triplet[idx][2] = b.triplet[j][2];
					j++;
					idx++;
					count++;
				}
			}
			else if( a.triplet[i][0] < b.triplet[j][0]){
				while(a.triplet[i][0] < b.triplet[j][0]){
					c.triplet[idx][0] = a.triplet[i][0];
					c.triplet[idx][1] = a.triplet[i][1];
					c.triplet[idx][2] = a.triplet[i][2];
					i++;
					idx++;
					count++;
				}
			}
			else{
				while(b.triplet[j][0] < a.triplet[i][0]){
					c.triplet[idx][0] = b.triplet[j][0];
					c.triplet[idx][1] = b.triplet[j][1];
					c.triplet[idx][2] = b.triplet[j][2];
					j++;
					idx++;
					count++;
				}
			}
		}
		while(i<a.size){
			c.triplet[idx][0] = a.triplet[i][0];
			c.triplet[idx][1] = a.triplet[i][1];
			c.triplet[idx][2] = a.triplet[i][2];
			i++;
			idx++;
			count++;
		}
		while(j<b.size){
			c.triplet[idx][0] = b.triplet[j][0];
			c.triplet[idx][1] = b.triplet[j][1];
			c.triplet[idx][2] = b.triplet[j][2];
			j++;
			idx++;
			count++;
		}
		c.size = count;
		return c;
	}

	void transpose(){
		int* count = new int[column]{0};
		for(int i=0;i<size;i++)
			count[int(triplet[i][1])]++;
		int* index = new int[column]{0};
		int stn = 0;
		for(int i=0;i<column;i++){
			index[i] = stn;
			stn+= count[i];
		}

		T** trans_triplet = new T*[size];
		for(int i=0;i<size;i++)
			trans_triplet[i] = new T[3]{0};

		for(int i=0;i<size;i++){
			trans_triplet[index[int(triplet[i][1])]][0] = triplet[i][1];
			trans_triplet[index[int(triplet[i][1])]][1] = triplet[i][0];
			trans_triplet[index[int(triplet[i][1])]][2] = triplet[i][2];
			index[int(triplet[i][1])]++;
		}

		int temp = row;
		row = column;
		column = temp;
		triplet = trans_triplet;
	}

	friend sparse operator * (sparse a, sparse b){
		int r = a.row;
		int c = b.column;
		b.transpose();
		int new_size = min(r*c, a.size*b.size);

		int idx = 0;
		int msize = 0;
		sparse<T> result(r,c,new_size);
		for(int i=0;i<a.size;i++){
			for(int j=0;j<b.size;j++){
				if(a.triplet[i][1] == b.triplet[j][1]){
					T prod = a.triplet[i][2]*b.triplet[j][2];
					bool found = false;
					for(int k=0;k<msize;k++){
						if(a.triplet[i][0]  == result.triplet[k][0] && b.triplet[j][0] == result.triplet[k][1]){
							result.triplet[k][2] += prod;
							found = true;
						}
					}
					if(!found){
						result.triplet[idx][0] = a.triplet[i][0];
						result.triplet[idx][1] = b.triplet[j][0];
						result.triplet[idx][2] = prod;
						msize++;
						idx++;
					}
				}
			}
		}

		int ssize = msize;
		for(int i=0;i<msize;i++)
			if(result.triplet[i][2] == 0)
				ssize--;

		sparse<T> final_result(r,c, ssize);
		idx  = 0;
		for(int i=0;i<msize;i++){
			if(result.triplet[i][2] != 0){
				final_result.triplet[idx][0] = result.triplet[i][0];
				final_result.triplet[idx][1] = result.triplet[i][1];
				final_result.triplet[idx][2] = result.triplet[i][2];
				idx++;
			}
		}
		b.transpose();
		return final_result;
	}

	void viewmatrix(){
		T** mat = new T*[row];
		for(int i=0;i<row;i++)
			mat[i] = new T[column]{0};
		for(int i=0;i<size;i++)
			mat[int(triplet[i][0])][int(triplet[i][1])] = triplet[i][2];
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++)
				cout << mat[i][j] << " ";
			cout << endl;
		}
	}

	void viewtriplet(){
		for(int i=0;i<size;i++)
			cout << triplet[i][0] << " " <<triplet[i][1] << " : " << triplet[i][2] << endl;
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
			sparse<double> M;
			M.take_input(n,m);
			cin >> n >> m;
			sparse<double> N;
			N.take_input(n,m);
			(M+N).viewmatrix();
		}
		else if(action == 2){
			int n,m;
			cin >> n >> m;
			sparse<double> M;
			M.take_input(n,m);
			cin >> n >> m;
			sparse<double> N;
			N.take_input(n,m);
			(M*N).viewmatrix();
		}
		else if(action == 3){
			int n,m;
			cin >> n >> m;
			sparse<double> M;
			M.take_input(n,m);
			M.transpose();
			M.viewmatrix();
		}
		cout << endl;
	}
	return 0;
}

