#include <iostream>

using namespace std;

//reverse a string
void sreverse(string::iterator beg, string::iterator end){
	end--;
	while(beg<end){
		char temp = *beg;
		*beg = *end;
		*end = temp;
		beg++;
		end--;
	}
}

// Takes input as reverse strings and return result in reverse
string subtract(string numa, string numb){
	int borrow = 0;
	int i = 0;
	int n = numa.size();
	int m = numb.size();
	string result;
	while(i<n && i<m){
		int x = numa[i]-'0';
		int y = numb[i]-'0';
		int intres = x-y-borrow;
		if(intres<0){
			intres = 10+x-y-borrow;
			borrow = 1;
		}
		else{
			borrow = 0;
		}
		result.push_back(intres+'0');
		i++;
	}

	while(i<n){
		int x = numa[i] - '0';
		int intres = x-borrow;
		if(intres < 0){
			intres = x+10-borrow;
			borrow = 1;
		}
		else{
			borrow = 0;
		}
		result.push_back(intres+'0');
		i++;
	}
	//i = result.size()-1;
	//while(!result.empty() && result[i] == '0')
	//	result.erase(result.begin()+i);
	for(int i=result.size()-1; i>=0 && result[i] == '0';i--)
		result.pop_back();
	if(result.empty())
		return "0";
	return result;
}


// Takes input as reverse strings and return result in reverse
string add(string numa, string numb){
	int carry = 0;
	int n = numa.size();
	int m = numb.size();
	int i = 0;
	string result;
	result.reserve(max(n,m) + 2);
	while(i<n && i<m){
		int x = numa[i]-'0';
		int y = numb[i]-'0';
		int intres = x+y+carry;
		result += (intres%10+'0');
		carry = intres/10;
		i++;
	}
	while(i<m){
		int x = numb[i] - '0';
		int intres = x+carry;
		result += (intres%10+'0');
		carry = intres/10;
		i++;
	}
	while(i<n){
		int x = numa[i] - '0';
		int intres = x+carry;
		result += (intres%10+'0');
		carry = intres/10;
		i++;
	}
	if(carry == 1){
		result += (carry+'0');
	}
	return result;
}


// Takes input as reverse string and return result in reverse
string multiply(string numa, char c){
	string res="0";
	c -= '0';
	for(int i=0;i<c;i++)
		res = add(res, numa);
	return res;
}


// Takes input as reverse strings and return result in reverse
string repeatedadd(string numa, string numb){
	string res;
	int n = numa.size();
	int m = numb.size();
	if(n > m)
		numa.swap(numb);
	string& multiplier = numa;
	string& multiplicant = numb;
	
	string buffer = "";
	for(int i=0;i<multiplier.size();i++){
		string partialsum = multiply(multiplicant, multiplier[i]);
		partialsum = buffer+partialsum;
		buffer += "0";
		res = add(res, partialsum);
	}
	return res;
}


// Takes base as a reverse string and return result in reverse
string binaryexp(string base, unsigned long long e){
	string res="1";
	while(e){
		if(e&1){
			res = repeatedadd(res, base);
		}
		base = repeatedadd(base, base);
		e = e>>1;
	}
	return res;
}

// Takes input as a reverse string and return result in reverse
string divide_by_two(string s){
	int index = s.size()-1;
	int temp = s[index]-'0';
	string result;
	if(temp<2){
		index--;
		temp = temp*10+(s[index]-'0');
	}
	while(index>=0){
		result += (temp/2)+'0';
		index--;
		temp = (temp%2)*10+(s[index]-'0');
	}
	sreverse(result.begin(), result.end());
	return result;
}

class BigInt{
	string number;
public:
	BigInt(){
		number = "0";
	}

	BigInt(string s) : number(s.rbegin(), s.rend()) {
	}

	friend BigInt operator + (const BigInt& a, const BigInt& b){
		string numa = a.number;
		string numb = b.number;
		string sum = add(numa, numb);
		BigInt result;
		result.number.swap(sum);
		return result;
	}

	friend BigInt operator - (const BigInt& a, const BigInt& b){
		string numa = a.number;
		string numb = b.number;
		string diff = subtract(numa , numb);
		BigInt result;
		result.number.swap(diff);
		return result;
	}

	friend BigInt operator * (const BigInt& a, const BigInt& b){
		string numa = a.number;
		string numb = b.number;
		string product = repeatedadd(numa, numb);
		BigInt result;
		result.number.swap(product);
		return result;
	}

	friend bool operator < (const BigInt& a, const BigInt& b){
		string numa = a.number;
		string numb = b.number;
		if(numa.size() < numb.size())
			return true;
		else if(numa.size() > numb.size())
			return false;
		else{
			sreverse(numa.begin(), numa.end());
			sreverse(numb.begin(), numb.end());
			return numa<numb;
		}
	}

	friend bool operator > (const BigInt& a, const BigInt& b){
		string numa = a.number;
		string numb = b.number;
		if(numa.size() > numb.size())
			return true;
		else if(numa.size() < numb.size())
			return false;
		else{
			sreverse(numa.begin(), numa.end());
			sreverse(numb.begin(), numb.end());
			return numa>numb;
		}
	}

	friend bool operator == (const BigInt& a, const BigInt& b){
		string numa = a.number;
		string numb = b.number;
		return numa == numb;
	}

	friend BigInt power(const BigInt& a, unsigned long long e){
		string numa = a.number;
		string res = binaryexp(numa, e);
		BigInt result;
		result.number.swap(res);
		return result;
	}

	friend bool even(const BigInt& a){
		return !((a.number.front()-'0')&1);
	}

	friend BigInt gcd(BigInt& a, BigInt& b){
		if(a == BigInt("0"))
			return b;
		else if(b == BigInt("0"))
			return a;

		BigInt mult = BigInt("1");
		while(even(a) && even(b)){
			mult = mult*BigInt("2");
			a.number = divide_by_two(a.number);
			b.number = divide_by_two(b.number);
		}

		while(even(a)){
			a.number = divide_by_two(a.number);	
		}
		while(!(b == BigInt("0"))){
			while(even(b)){
				b.number = divide_by_two(b.number);
			}
			if(a>b){
				BigInt t = a;
				a = b;
				b = t;
			}
			b = b-a;
		}
		return a*mult;
	}

	friend ostream& operator << (ostream& out, BigInt a){
		sreverse(a.number.begin(), a.number.end());
		out << a.number;
		return out;
	}

};

template <class T>
struct Node{
	T data;
	struct Node* next;
};

template <class T>
class stack{
	Node<T>* top;
public:
	stack(){
		top = NULL;
	}

	void push(T s){
		Node<T>* temp = new Node<T>();
		temp->data = s;
		if(top == NULL){
			top = temp;
		}
		else{
			temp->next = top;
			top = temp;
		}
	}

	T pop(){
		if(top == NULL){
			return T(NULL);
		}
		else{
			T temp = top->data;
			Node<T>* del = top;
			top = top->next;
			free(del);
			return temp;
		}
	}

	T peek(){
		return top->data;
	}

	bool empty(){
		if(top == NULL)
			return true;
		return false;
	}
};

bool isoperator(char c){
	if(c == '+' || c == '-' || c == 'x')
		return true;
	return false;
}

int precence(char a){
	if(a == 'x')
		return 2;
	else
		return 1;
}

BigInt evaluate(string s){
	int n = s.size();
	stack<char> opr;
	stack<BigInt> opn;

	string num;
	for(int i=0;i<n;i++){
		if(isoperator(s[i])){
			opn.push(BigInt(num));
			num = "";
			if(opr.empty()){
				opr.push(s[i]);
			}
			else{
				while(!opr.empty() && precence(opr.peek())>=precence(s[i])){
					BigInt opr2 = opn.pop();
					BigInt opr1 = opn.pop();
					if(opr.peek() == '+')
						opn.push(opr1+opr2);
					else if(opr.peek() == '-')
						opn.push(opr1-opr2);
					else
						opn.push(opr1*opr2);
					opr.pop();
				}
				opr.push(s[i]);
			}
		}
		else{
			num+=s[i];	
		}
	}
	if(num != "")
		opn.push(BigInt(num));

	while(!opr.empty()){
		BigInt opr2 = opn.pop();
		BigInt opr1 = opn.pop();
		if(opr.peek() == '+')
			opn.push(opr1+opr2);
		else if(opr.peek() == '-')
			opn.push(opr1-opr2);
		else
			opn.push(opr1*opr2);
		opr.pop();
	}
	BigInt res = opn.peek();
	return res;
}

BigInt fact(unsigned long long e){
	if(e==0)
		return BigInt("1");
	return fact(e-1)*BigInt(to_string(e));
}

int main(){
	int action;
	cin >> action;
	switch(action){
		case 1:{string base;
				unsigned long long exp;
				cin >> base >> exp;
				BigInt b(base);
				cout << power(b, exp) << endl;
				break;
			}
		case 2:{string numa;
				string numb;
				cin >> numa >> numb;
				BigInt g1(numa); BigInt g2(numb);
				cout << gcd(g1, g2) << endl;
				break;
			}
		case 3:{unsigned long long f;
				cin >> f;
				cout << fact(f) << endl;
				break;
			}
		case 4:{string expression;
				cin >> expression;
				cout << evaluate(expression) << endl;
				break;
			}
		default: break;
	}
	return 0;
}