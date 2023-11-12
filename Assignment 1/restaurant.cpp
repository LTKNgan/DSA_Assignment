#include "main.h"

/////////////////////////////////////////////////////////////////// TACH LENH ///////////////////////////////////////////////////////////////////////////
int toNum (string s)
{
	int num = 0;
	for (unsigned i = 0; i < s.size(); i++)
	{
		num = num*10 + ((int)s[i] - '0');
	}
	return num;
}

void getcomm(string &comm, string &keyw, string &name, int &num)
{
	if (comm[0] == ' ' || comm.back() == ' ')
	{
		keyw = "error";
		return;
	}
	stringstream cmd(comm);
	cmd >> keyw;
	string check;
	if (keyw == "REG")
	{
		cmd >> name >> check;
		if (name == "" || check != "") keyw = "error";
		if (keyw != "error")
		{
			for (unsigned i = 0; i < name.size(); i++)
			{
				if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')))
				{
					keyw = "error";
					break;
				}
			}
		}
		num = 0;
	}
	else if (keyw == "CLE")
	{
		cmd >> check;
		if (check == "") keyw = "error";
		else 
		{
			for (unsigned i = 0; i < check.size(); i++)
			{
				if (!(check[i] == '+' || check [i] == '-' || (check[i] >= '0' && check[i] <= '9')))
				{
					keyw = "error";
					return;
				}
			}
		}
		if (keyw != "error")
		{
			num = toNum(check);
			check = "";
			cmd >> check;	
			if (cmd.str() == keyw || check != "") keyw = "error";
		}
	}
	else if (keyw == "PrintHT" || keyw == "PrintAVL" || keyw == "PrintMH")
	{
		cmd >> check;
		if (check != "") keyw = "error";
		num = 0;
	}
	else keyw = "error";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////// HAM REG - HUFFMAN TREE  //////////////////////////////////////////////////////////////////////

////////// Node cua huffman tree //////////
class Nodehuff 
{ 
	public:  
		int freq; 
		char key; 
		Nodehuff *left; 
		Nodehuff *right; 
    	Nodehuff(int freq, char key) : freq(freq), key(key), left(NULL), right(NULL) {} 
    	~Nodehuff() {} 
};

////////// ham sap xep node cua huffman tree //////////
class compareMin {
    public:
        bool operator()(Nodehuff* pre, Nodehuff* next)
		{
            if(pre -> freq < next -> freq) return true;          
            else if (pre -> freq == next -> freq) 
			{
				if (pre -> key < next -> key) return true;
				else return false;
			}
            else return false;
      	}
};

////////// Tim vi tri ki tu //////////
int search(deque<pair<char, string>> &dq, char x)
{
	for (unsigned i = 0; i < dq.size(); i++)
	{
		if (x == dq[i].first) return i;
	}
	return -1;
}

////////// Ma hoa ki tu tu array //////////
string getcode(bool arr[], int num)
{
	string s = "";
	for (int i = 0; i < num; i++)
	{
		s += to_string(arr[i]);
	}
	return s;
}

////////// Ma hoa ki tu tu cay huffman //////////
void enco(Nodehuff* root, bool arr[], int num, deque<pair<char, string>> &dq)
{
	if (root -> left) 
	{
		arr[num] = 0;
		enco(root -> left, arr, num+1, dq);
	}

	if (root -> right)
	{
		arr[num] = 1;
		enco(root -> right, arr, num+1, dq);
	}

	if (root -> left == NULL && root -> right == NULL)
	{
		dq[search(dq, root -> key)].second = getcode(arr, num);
	}
}

////////// binary to decimal //////////
int binToDec (string& s)
{
	int luythua = 1;
	int sum = 0;
	for (int i = s.size() - 1; i >= 0; i--)
	{
		if (s[i] == '1') sum += luythua;
		luythua *= 2;
	}
	return sum;
}

////////// delete huffman tree //////////
Nodehuff* deleteTree(Nodehuff*& root)
{
    if (root == NULL) return NULL; 
    if (root -> left) root -> left = deleteTree(root -> left);
    if (root -> right) root -> right = deleteTree(root -> right);
    delete root;
	return NULL;
}

////////// Ma hoa huffman, tra ve result //////////
int reg (string &name)
{
	//// tim tan so tung ki tu ////
	deque <Nodehuff*> dq;
	for (int count = name.size() - 1; count >= 0; count--)
	{
		unsigned i;
		for (i = 0; i < dq.size(); i++)
		{
			if (name[count] == dq[i] -> key) 
			{
				dq[i] -> freq++;
				break;
			}
		}
		if (i == dq.size()) dq.push_back(new Nodehuff(1, name[count]));
	}
	//// ten chi co 1 ki tu bieu dien bang bit 1 ////
	if (dq.size() == 1) return 1;
	//// tong so ki tu ////
	int size = dq.size();	
	//// luu du lieu vao vector v ////
	deque <pair<char, string>> dqcode;
	for (int i = 0; i < size; i++)
	{
		dqcode.push_back(make_pair(dq[i] -> key, ""));
	}
	//// sap xep lai theo tan so ////
	sort(dq.begin(), dq.end(), compareMin());
	//// build tree ////
	Nodehuff* root = NULL;
	while(dq.size() > 1)
	{
	    root = new Nodehuff((dq[0] -> freq + dq[1] -> freq), '|');
	    root -> left = dq[0];
	    root -> right = dq[1];
	    dq.erase(dq.begin(), dq.begin() + 2);
		if (dq.size() > 0)
		{
			if (root -> freq >= dq.back() -> freq) dq.push_back(root);
			else if (root -> freq < dq.front() -> freq) dq.push_front(root);
			else 
			{
				for (unsigned i = dq.size(); i > 0; i--)
				{
					if (dq[i - 1] -> freq <= root -> freq) 
					{
						dq.insert(dq.begin() + i, root);
						break;
					}
				}
			}
		}	
	}	
	//// lay ma tu huffman tree ////
	int arrsize = log2(size) + 1;
	bool arr[arrsize] = {0};
	enco(root, arr, 0, dqcode);
	//// ma hoa name ////
	string code = "";
	for (int  i = name.size() - 1; i >= 0 && code.size() <= 15; i--)
	{
		code = dqcode[search(dqcode, name[i])].second + code;
	}
	if (code.size() > 15) code = code.substr(code.size() - 15);
	//// delete huffman tree ////
	root = deleteTree(root);
	return binToDec(code);
}
///////////////////////////////////////////////////////////// HET HAM REG - HUFFMAN TREE ///////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////// BUILD AVL TREE /////////////////////////////////////////////////////////////////////////

enum BalanceValue
{
	LH = -1,
	EH = 0,
	RH = 1
};

class NodeAVL
{ 
	public:  
		int id;
		int result;
		int num;
		NodeAVL* left;
		NodeAVL* right;
		BalanceValue balance;

    	NodeAVL(int id, int result, int num) : id(id), result(result), num(num), left(NULL), right(NULL), balance(EH) {} 
    	~NodeAVL() {} 
};

//----------------------------------------------- INSERT --------------------------------------------- //
////////// Rotate left //////////
NodeAVL* rotateLeft(NodeAVL*& root)
{
	NodeAVL* temp = root -> right;
	root -> right = temp -> left;
	temp -> left = root;
	return temp;
}

////////// Rotate right //////////
NodeAVL* rotateRight(NodeAVL*& root)
{
	NodeAVL* temp = root -> left;
	root -> left = temp -> right;
	temp -> right = root;
	return temp;
}

////////// Left balance //////////
NodeAVL* leftbalance(NodeAVL*& root, bool& taller)
{
	NodeAVL* leftTree = root -> left;
	//// left of left - rotate right ////
	if (leftTree -> balance == LH)
	{
		root -> balance = EH;
		leftTree -> balance = EH;
		root = rotateRight(root);
		taller = false;
	}
	//// right of left - rotate left - rotate right ////
	else 
	{
		NodeAVL* rightTree = leftTree -> right;
		if (rightTree -> balance == LH)
		{
			root -> balance = RH;
			leftTree -> balance = EH;
		}
		else if (rightTree -> balance == EH)
		{
			root -> balance = EH;
			leftTree -> balance = EH;
		}
		else
		{
			root -> balance = EH;
			leftTree -> balance = LH; 
		}
		rightTree -> balance = EH;
		root -> left = rotateLeft(leftTree);
		root = rotateRight(root);
		taller = false;
	}
	return root;
}

////////// right balance /////////
NodeAVL* rightbalance(NodeAVL*& root, bool& taller)
{
	NodeAVL* rightTree = root -> right;
	if (rightTree -> balance == RH)
	{
		root -> balance = EH;
		rightTree -> balance = EH;
		root = rotateLeft(root);
		taller = false;
	}
	else
	{
		NodeAVL* leftTree = rightTree -> left;
		if (leftTree -> balance == RH)
		{
			root -> balance = LH;
			rightTree -> balance = EH;
		}
		else if (leftTree -> balance == EH)
		{
			root -> balance = EH;
			rightTree -> balance = EH;
		}
		else 
		{
			root -> balance = EH;
			rightTree -> balance = RH;
		}
		leftTree -> balance = EH;
		root -> right = rotateRight(rightTree);
		root = rotateLeft(root);
		taller = false;
	}
	return root;
}

////////// insert node //////////
NodeAVL* insertNode(NodeAVL*& root, NodeAVL*& newNode, bool& taller)
{
	if (root == NULL)
	{
		root = newNode;
		taller = true;
		return root;
	}
	if (newNode -> result < root -> result)
	{
		root -> left = insertNode(root -> left, newNode, taller);
		if (taller)
		{
			if (root -> balance == LH) root = leftbalance(root, taller);
			else if (root -> balance == EH) root -> balance = LH;
			else
			{
				root -> balance = EH;
				taller = false;
			}
		}
	}
	else
	{
		root -> right = insertNode(root -> right, newNode, taller);
		if (taller)
		{
			if (root -> balance == EH) root -> balance = RH;
			else if (root -> balance == RH) root = rightbalance(root, taller);
			else
			{
				root -> balance = EH;
				taller = false;
			}
		}
	}
	return root;
}

////////// insert node (ham duoc goi) //////////
void insert(NodeAVL*& root, int id, int result, int num)
{
	bool taller = false;
	NodeAVL* newNode = new NodeAVL(id, result, num);
	root = insertNode(root, newNode, taller);
}
//---------------------------------- END INSERT ------------------------------------------------//

//---------------------------------- DELETE ---------------------------------------------//
////////// delete right balance //////////
NodeAVL* delRightBalance(NodeAVL*& root, bool& shorter)
{
	if (root -> balance == LH) root -> balance = EH;
	else if (root -> balance == EH)
	{
		root -> balance = RH;
		shorter = false;
	}
	else
	{
		NodeAVL* rightTree = root -> right;
		if (rightTree -> balance == LH)
		{
			NodeAVL* leftTree = rightTree -> left;
			if (leftTree -> balance == LH)
			{
				rightTree -> balance = RH;
				root -> balance = EH;
			}
			else if (leftTree -> balance == EH)
			{
				root -> balance = EH;
				rightTree -> balance = EH;
			}
			else
			{
				root -> balance = LH;
				rightTree -> balance = EH;
			}
			leftTree -> balance = EH;
			root -> right = rotateRight(rightTree);
			root = rotateLeft(root);
		}
		else
		{
			if (rightTree -> balance != EH)
			{
				root -> balance = EH;
				rightTree -> balance = EH;
			}
			else
			{
				root -> balance = RH;
				rightTree -> balance = LH;
				shorter = false;
			}
			root = rotateLeft(root);
		}
	}
	return root;
}

////////// delete left balance //////////
NodeAVL* delLeftBalance(NodeAVL*& root, bool& shorter)
{
	if (root -> balance == RH) root -> balance = EH;
	else if (root -> balance == EH)
	{
		root -> balance = LH;
		shorter = false;
	} 
	else
	{
		NodeAVL* leftTree = root -> left;
		if (leftTree -> balance == RH)
		{
			NodeAVL* rightTree = leftTree -> right;
			if (rightTree -> balance == RH)
			{
				root -> balance = EH;
				leftTree -> balance = LH;				
			} 
			else if (rightTree -> balance == EH)
			{
				root -> balance = EH;
				leftTree -> balance = EH;
			}
			else 
			{
				root -> balance = RH;
				leftTree -> balance = EH;
			}
			rightTree -> balance = EH;
			root -> left = rotateLeft(leftTree);
			root = rotateRight(root);
		}
		else 
		{
			if (leftTree -> balance != EH)
			{
				root -> balance = EH;
				leftTree -> balance = EH;
			} 
			else
			{
				root -> balance = LH;
				leftTree -> balance = RH;
				shorter = false;
			}
			root = rotateRight(root);
		}
	}
	return root;
}

////////// delete //////////
NodeAVL* del(NodeAVL*& root, int id, int result, bool& shorter, bool& success)
{
	if (root == NULL)
	{
		shorter = false;
		success = false;
		return NULL;
	}
	if (result < root -> result)
	{
		root -> left = del(root -> left, id, result, shorter, success);
		if (shorter) root = delRightBalance(root, shorter);
	}
	else if (result > root -> result)
	{
		root -> right = del(root -> right, id, result, shorter, success);
		if (shorter) root = delLeftBalance(root, shorter);
	}
	else 
	{
		if (root -> id != id)
		{
			root -> right = del(root -> right, id, result, shorter, success);
			if (shorter) root = delLeftBalance(root, shorter);
			if (!success) 
			{
				root -> left = del(root -> left, id, result, shorter, success);
				if (shorter) root = delRightBalance(root, shorter);
			}
		}
		else
		{
			NodeAVL* delNode = root;
			if (root -> right == NULL)
			{
				NodeAVL* newRoot = root -> left;
				success = true;
				shorter = true;
				delete delNode;
				return newRoot;
			}
			else if (root -> left == NULL)
			{
				NodeAVL* newRoot = root -> right;
				success = true;
				shorter = true;
				delete delNode;
				return newRoot;
			}
			else
			{
				NodeAVL* temp = root -> right;
				while (temp -> left)
				{
					temp = temp -> left;
				}
				root -> id = temp -> id;
				root -> result = temp -> result;
				root -> num = temp -> num;
				root -> right = del(root -> right, temp -> id, temp -> result, shorter, success);
				if (shorter)
				{
					root = delLeftBalance(root, shorter);
				}
			}
		}
	}
	return root;
}

////////// remove (ham duoc goi) //////////
void remove(NodeAVL*& root, int result, int id)
{
	bool shorter = false;
	bool success = false;
	root = del(root, id, result, shorter, success);
}
//---------------------------------- END DELETE --------------------------//

////////// search //////////
NodeAVL* searchAVL(NodeAVL*& root, int result, int id)
{
	if (root == NULL) return NULL;
	if (root -> result == result) 
	{
		if (root -> id == id) return root;
		else 
		{
			NodeAVL* temp = searchAVL(root -> right, result, id);
			if (temp) return temp;
			return searchAVL(root -> left, result, id);
		}
	}
	else if (root -> result < result) return (searchAVL(root -> right, result, id));
	else return (searchAVL(root -> left, result, id));
}

////////// clear tree //////////
NodeAVL* cleartree(NodeAVL*& root)
{
	if (root == NULL) return NULL; 
    if (root -> left) root -> left = cleartree(root -> left);
    if (root -> right) root -> right = cleartree(root -> right);
    delete root;
	return NULL;
}

////////// print tree //////////
void printtree (NodeAVL*& root)
{
	if (root == NULL) return;
	queue <NodeAVL*> q;
	q.push(root);
	while (!q.empty())
	{
		NodeAVL* temp = q.front();
		cout << temp -> id << "-" << temp -> result << "-" << temp -> num << endl;
		q.pop();
		if (temp -> left) q.push(temp -> left);
		if (temp -> right) q.push(temp -> right);
	}
}

//////////////////////////////////////////////////////////////// END BUILD AVL TREE ///////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////// CO CHE DUOI KHACH //////////////////////////////////////////////////////////////////////////
////////// node area 1 //////////
class Nodearea1
{ 
	public:  
		int id;
		int result;
		int num;

		Nodearea1() : id(0), result(0), num(0) {}
    	Nodearea1(int id, int result, int num) : id(id), result(result), num(num) {} 
    	~Nodearea1() {} 
};

////////// node fifo //////////
class Nodefifo 
{ 
	public:  
		string name;
		int area;
		int id;
		int result;

    	Nodefifo(string name, int area, int id, int result) : name(name), area(area), id(id), result(result) {} 
    	~Nodefifo() {} 
};

////////// node lrco //////////
class Nodelrco
{ 
	public:  
		int area;
		int id;
		int result;

    	Nodelrco(int area, int id, int result) : area(area), id(id), result(result) {} 
    	~Nodelrco() {} 
};

////////// node lfco //////////
class Nodelfco
{ 
	public:  
		int area;
		int id;
		int result;
		int num;


    	Nodelfco(int area, int id, int result, int num) : area(area), id(id), result(result), num(num) {} 
    	~Nodelfco() {} 
};

////////// search xem co dang trong nha hang khong //////////
int searchfifo (string name, deque<Nodefifo>& fifo, int& id, int& result)
{
	//// search theo name ////
	if (name != "")
	{
		for (unsigned i = 0; i < fifo.size(); i++)
		{
			if (fifo[i].name == name) 
			{
				id = fifo[i].id;
				result = fifo[i].result;
				return fifo[i].area;
			}
		}
	}
	//// search theo id ////
	else 
	{
		for (unsigned i = 0; i < fifo.size(); i++)
		{
			if (fifo[i].id == id)
			{
				result = fifo[i].result;
				return fifo[i].area;
			}
		}
	}
	return 0;
}

////////// cap nhat khu vuc 1 //////////
void updateArea1(deque <Nodearea1>& area1, int address, int id, int result, bool check)
{
	//// chi them nguoi moi ////
	if (check)
	{
		area1[address].id = id;
		area1[address].num = 1;
		area1[address].result = result;
	}
	//// ghi de nguoi cu len nguoi moi ////
	else
	{
		for (unsigned  i = 0; i < area1.size(); i++)
		{
			if (id == area1[i].id) 
			{
				area1[i].result = result;
				area1[i].num = 1;
				break;
			}
		}
	}
}

////////// duoi khach va them khach moi (id, area cu, result moi) //////////
void capnhat(int id, int area, int oldRes, int newRes, deque <Nodearea1>& area1, NodeAVL*& area2)
{
	if (area == 1) updateArea1(area1, 0, id, newRes, 0);
	else
	{
		remove(area2, oldRes, id);
		insert(area2, id, newRes, 1);
	}
}

////////// duoi khach area 1 /////////
void duoiArea1 (deque <Nodearea1>& area1, int id)
{
	for (int i = 0; i < MAXSIZE/2; i++)
	{
		if (area1[i].id == id) 
		{
			area1[i].id = 0;
			area1[i].num = 0;
			area1[i].result = 0;
			return;
		}
	}
}

/////////////////////////////////////////////////////// HET CO CHE DUOI KHACH ////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// CAP NHAT FIFO, LRCO, LFCO ////////////////////////////////////////////////////////////////////
//------------------------- delete va add --------------------------//
void delFifo (deque <Nodefifo>& fifo, int id)
{
	for (unsigned i = 0; i < fifo.size(); i++)
	{
		if (fifo[i].id == id) 
		{
			fifo.erase(fifo.begin() + i);
			return;
		}
	}
}

void delLrco (deque <Nodelrco>& lrco, int id)
{
	for (unsigned i = 0; i < lrco.size(); i++)
	{
		if (lrco[i].id == id) 
		{
			lrco.erase(lrco.begin() + i);
			return;
		}
	}
}

bool whofirst(deque <Nodefifo>& fifo, int idleft, int idright)
{
	for (unsigned i = 0; i < fifo.size(); i++)
	{
		if (fifo[i].id == idleft) return true;
		if (fifo[i].id == idright) return false;
	}
	return false;
}

void heapUp (deque <Nodefifo>& fifo, deque <Nodelfco>& lfco, int child)
{
	int par = (child - 1)/2;
	while (child > 0 && lfco[par].num >= lfco[child].num)
	{
		if (lfco[par].num == lfco[child].num) 
		{
			if (!whofirst(fifo, lfco[child].id, lfco[par].id)) return;
		}
		swap(lfco[par], lfco[child]);
		child = par;
		par = (child - 1)/2;
	}
}

void heapDown(deque <Nodefifo>& fifo, deque<Nodelfco>& lfco, int par)
{
	int lChild = par*2 + 1;
	int rChild = par*2 + 2;
	while (lChild < (int)lfco.size())
	{
		int smallChild = lChild;
		if (rChild < (int)lfco.size())
		{
			if (lfco[rChild].num < lfco[lChild].num) smallChild = rChild;
			else if (lfco[rChild].num == lfco[lChild].num)
			{
				if (lfco[lChild].num > lfco[par].num) return;
				if (!whofirst(fifo, lfco[lChild].id, lfco[rChild].id)) smallChild = rChild;
			}	
		}
		if (lfco[smallChild].num == lfco[par].num)
		{
			if (!whofirst(fifo, lfco[smallChild].id, lfco[par].id)) return;
		}
		else if (lfco[smallChild].num > lfco[par].num) return;
		swap(lfco[smallChild], lfco[par]);
		par = smallChild;
		lChild = par*2 + 1;
		rChild = par*2 + 2;
	}
}

void delLfco (deque <Nodefifo>& fifo, deque <Nodelfco>& lfco, int id)
{
	for (unsigned i = 0; i < lfco.size(); i++)
	{
		if (lfco[i].id == id)
		{
			swap(lfco[i], lfco.back());
			lfco.pop_back();
			heapDown(fifo, lfco, i);
			heapUp(fifo, lfco, i);
			break;
		}
	}
}

void addLfco (deque <Nodefifo>& fifo, deque <Nodelfco>& lfco, int area, int id, int result)
{
	lfco.push_back(Nodelfco(area, id, result, 1));
	heapUp(fifo, lfco, lfco.size() - 1);
}
//-------------------------- het delete va add ---------------------------------//

//-------------------------- update khi khach goi them -------------------------//
void themmonArea1 (deque <Nodearea1>& area1, int id)
{
	for (int i = 0; i < MAXSIZE/2; i++)
	{
		if (area1[i].id == id)
		{
			area1[i].num++;
			return;
		}
	}
}

void themmonArea2 (NodeAVL*& area2, int result, int id)
{
	NodeAVL* temp = searchAVL(area2, result, id);
	if (temp != NULL) (temp -> num)++;
}
////////// cap nhat lrco //////////
void updateLrco (deque <Nodelrco> lrco, int id)
{
	for (unsigned i = 0; i < lrco.size(); i++)
	{
		if (lrco[i]. id == id) 
		{
			lrco.push_back(lrco[i]);
			lrco.erase(lrco.begin() + i);
			return;
		}
	}
}
////////// cap nhat lfco //////////
void updateLfco (deque <Nodefifo>& fifo, deque <Nodelfco>& lfco, int id)
{
	for (unsigned i = 0; i < lfco.size(); i++)
	{
		if (lfco[i].id == id)
		{
			int num = lfco[i].num + 1;
			int area = lfco[i].area;
			int result = lfco[i].result;
			delLfco(fifo, lfco, id);
			lfco.push_back(Nodelfco(area, id, result, num));
			heapUp(fifo, lfco, lfco.size() - 1);
			return;
		}
	}
}

////////// print preorder lfco //////////
void printMH(deque <Nodelfco>& lfco, unsigned index)
{
	if (index < lfco.size())
	{
		cout << lfco[index].id << "-" << lfco[index].num << endl;
		printMH(lfco, index*2 + 1);
		printMH(lfco, index*2 + 2);
	}
}

///////////////////////////////////////////////////////// HET CAP NHAT /////////////////////////////////////////////////////////////////////////

void simulate(string filename)
{
	ifstream myfile(filename);
    if (myfile.is_open())
	{
		//// dem so nguoi ////
		int count[2] = {0};
		//// check xem ban o id do co nguoi lay chua ////
		vector <bool> check (MAXSIZE, 0); 
		//// khu vuc 1 ////
		deque <Nodearea1> area1;
		for (int i = 0; i < MAXSIZE/2; i++)
		{
			area1.push_back(Nodearea1());
		} 
		//// khu vuc 2 ////
		NodeAVL* area2 = NULL;

		//// co che fifo luu thu tu den nha hang ////
		deque <Nodefifo> fifo;
		//// co che lrco luu nguoi goi mon tre nhat ////
		deque <Nodelrco> lrco;
		//// co che lfco luu nguoi goi mon it nhat ////
		deque <Nodelfco> lfco;

		while (!myfile.eof())
		{
			string comm;
			getline(myfile,comm);
			string name, keyw;
			int num;
			int id, result, area;
			getcomm(comm, keyw, name, num);	
			// cout << keyw << " " << name << " " << num << endl;	
			if (keyw == "REG")
			{
				area = searchfifo(name, fifo, id, result);
				//// khach moi toi ////
				if (area == 0)
				{
					//// chuan hoa ten khach hang ////
					result = reg(name);
					//// nha hang het cho ////
					if (count[0] + count[1] == MAXSIZE) 
					{
						int oldRes;
						int OPT = result % 3;
						//// duoi khach theo fifo ////
						if (OPT == 0) 			
						{
							// cout << "duoi theo fifo" << endl;
							id = fifo.front().id;
							area = fifo.front().area;
							oldRes = fifo.front().result;
							capnhat(id, area, oldRes, result, area1, area2);
							//// cap nhat fifo ////
							fifo.pop_front();
							fifo.push_back(Nodefifo(name, area, id, result));
							//// cap nhat lrco ////
							delLrco(lrco, id);
							lrco.push_back(Nodelrco(area, id, result));
							//// cap nhat lfco ////
							delLfco(fifo, lfco, id);
							addLfco(fifo, lfco, area, id, result);
						}
						//// duoi khach theo lrco ////
						else if (OPT == 1)
						{							
							id = lrco.front().id;
							area = lrco.front().area;
							oldRes = lrco.front().result;
							capnhat(id, area, oldRes, result, area1, area2);
							//// cap nhat fifo ////
							delFifo(fifo, id);
							fifo.push_back(Nodefifo(name, area, id, result));
							//// cap nhat lrco ////
							lrco.pop_front();
							lrco.push_back(Nodelrco(area, id, result));
							//// cap nhat lfco ////
							delLfco(fifo, lfco, id);
							addLfco(fifo, lfco, area, id, result);
						}
						else
						{
							id = lfco.front().id;
							area = lfco.front().area;
							oldRes = lfco.front().result;
							capnhat(id, area, oldRes, result, area1, area2);
							//// cap nhat fifo ////
							delFifo(fifo, id);
							fifo.push_back(Nodefifo(name, area, id, result));
							//// cap nhat lrco ////
							delLrco(lrco, id);
							lrco.push_back(Nodelrco(area, id, result));
							//// cap nhat lfco ////
							delLfco(fifo, lfco, id);
							addLfco(fifo, lfco, area, id, result);
						}
					}						
					//// nha hang con cho ////
					else
					{

						
						//// chon khu vuc ////
						area = (result % 2 == 0) ? 2 : 1;
						if (count[area - 1] == MAXSIZE/2)
						{
							area = (area == 1) ? 2 : 1;
						} 
						//// chon ban ////
						id = result % MAXSIZE + 1;
						while (check[id - 1]) 
						{
							if (id == MAXSIZE) id = 0;
							id++;
						}
						//// cap nhat check ////
						check[id - 1] = 1;									
						//// bo tri ban tai khu vuc 1: hashtable ////
						if (area == 1)
						{
							int address = result % (MAXSIZE / 2);
							while (area1[address].id != 0)
							{
								if (address == MAXSIZE/2 - 1) address = -1;
								address++;
							}
							updateArea1(area1, address, id, result, 1);
						}
						//// bo tri ban tai khu vuc 2: AVL tree ////
						else 
						{
							insert(area2, id, result, 1);							
						}
						count[area - 1]++;											////////// cap nhat count //////////
						fifo.push_back(Nodefifo(name, area, id, result));			////////// cap nhat fifo //////////
						lrco.push_back(Nodelrco(area, id, result));					////////// cap nhat lrco //////////
						////////// cap nhat lfco //////////
						addLfco(fifo, lfco, area, id, result);	
					}
					
				}
				//// khach goi them mon - cap nhat lrco, lfco, 2 area ////
				else
				{
					//// cap nhat 2 area ////
					if (area == 1) themmonArea1(area1, id);
					else themmonArea2(area2, result, id);
					//// cap nhat lrco ////
					updateLrco(lrco, id);
					//// cap nhat lfco ////
					updateLfco(fifo, lfco, id);		
				}	
							
			}
			else if (keyw == "CLE")
			{
				//// duoi het khu 1 ////
				if (num < 1)
				{
					unsigned index = 0;
					for (; index < fifo.size(); index++)
					{
						if (fifo[index].area == 1) 
						{
							id = fifo[index].id;
							duoiArea1(area1, id);
							delFifo(fifo, id);
							delLrco(lrco, id);
							delLfco(fifo, lfco, id);
							check[id - 1] = 0;
							count[0] -= 1;
							index--;
						}
						if (count[0] == 0) break;
					}
				}
				else if (num > MAXSIZE)
				{
					unsigned index = 0;
					for (; index < fifo.size(); index++)
					{
						if (fifo[index].area == 2)
						{
							id = fifo[index].id;
							result = fifo[index].result;
							remove(area2, result, id);
							delFifo(fifo, id);
							delLrco(lrco, id);
							delLfco(fifo, lfco, id);
							check[id - 1] = 0;
							count[1] -= 1;
							index--;
						}
						if (count[1] == 0) break;
					}
				}
				else
				{
					if (check[num - 1])
					{
						id = num;
						area = searchfifo("", fifo, id, result);
						if (area == 1)
						{
							duoiArea1(area1, id);
							delFifo(fifo, id);
							delLrco(lrco, id);
							delLfco(fifo, lfco, id);
							check[id - 1] = 0;
							count[0] -= 1;
						}
						else
						{
							remove(area2, result, id);
							delFifo(fifo, id);
							delLrco(lrco, id);
							delLfco(fifo, lfco, id);
							check[id - 1] = 0;
							count[1] -= 1;
						}
					}

				}
			}
			else if (keyw == "PrintHT")			
			{
				for (unsigned i = 0; i < area1.size(); i++)
				{
					if (area1[i].id != 0) cout << area1[i].id << "-" << area1[i].result << "-" << area1[i].num << endl;
				}
			}
			else if (keyw == "PrintAVL")
			{
				printtree(area2);
			}
			else if (keyw == "PrintMH")
			{
				
				printMH (lfco, 0);
			}
		}
		area2 = cleartree(area2);
	}
	else cout << "Cannot read input file";  
}