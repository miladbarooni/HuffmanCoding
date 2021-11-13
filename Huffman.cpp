#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>
using namespace std;

class Node{
public:
// it is not safe. but for now ... (needs getter and setter)
    Node* right;
    Node* left;
    pair<char, int> data;// for the data for each node
};

int to_integer(string s){
    int res = 0;
    for (int i=7; i>=0; i--){
        res += (s[i] - 48) * pow(2,-(i-7));
    }
    return res;
}

void swap(Node **x, Node **y) 
{ 
    Node *temp = *x; 
    *x = *y; 
    *y = temp; 
}




// impelement an minheap by array
class MinHeap 
{ 
    Node* harr[1000];  
    int capacity; 
    int heap_size; 
public: 
    MinHeap(){
        heap_size = 0; 
        capacity = 1000;  
    }
    void MinHeapify(int i){
        int l = left(i);
        int r = right(i); 
        int smallest = i;
        // first we have to find smallest element
        if (l < heap_size && harr[l]->data.second < harr[i]->data.second) 
            smallest = l; 
        if (r < heap_size && harr[r]->data.second < harr[smallest]->data.second) 
            smallest = r; 
        if (smallest != i){ 
            swap(&harr[i], &harr[smallest]); 
            MinHeapify(smallest); 
        } 
    }
    
    int parent(int i) { return (i-1)/2; } 
  
    int left(int i) { return (2*i + 1); } 
  
    int right(int i) { return (2*i + 2); } 
    // now to remove minimume element
    Node* extractMin(){         
        if (heap_size == 1){ 
            heap_size--; 
            return harr[0]; 
        } 
        Node* root = harr[0]; 
        harr[0] = harr[heap_size-1]; 
        heap_size--; 
        MinHeapify(0); 
        return root; 
    }
  
  
    Node* getMin() { return harr[0]; } 
  
    void deleteKey(int i){
        // int root = harr[0]
        harr[i] = harr[heap_size-1];
        heap_size--;
        MinHeapify(i);
    }
  
    void insertKey(Node* p){
        if (heap_size == capacity){ 
            cout << "\nOverflow: Could not insertKey\n"; 
            return; 
        } 
        heap_size++; 
        int i = heap_size - 1; 
        harr[i] = p; 
        while (i != 0 && harr[parent(i)]->data.second > harr[i]->data.second) { 
            // cout << "in the while" << endl;
            swap(&harr[i], &harr[parent(i)]); 
            i = parent(i); 
        } 
    }

    int getHeapSize(){
        return heap_size;
    }

    void print(){
        for (int i=0; i<heap_size ; i++){
            cout << harr[i]->data.first << "   :" << harr[i]->data.second << endl;
        }
    } 
}; 
  


class Huffman{
    ifstream file;
    fstream ofile;
    int extra_bits;
    vector<pair<char, int>> chars;
    pair<char,int> arr[100];
    MinHeap mheap;
    Node* root; 
    vector<pair<char, string>> path;
    string file_string;
public:
    void getInputFile(string filename){
        cout << endl << "I'm going to open the file " << filename << " as a input file to zip "<<endl;
        file.open(filename);
    }

    void calculateRepeats(){
        // It will add all the existing charachters to a vector of pairs as defined.
        cout << endl;
        getline (file, file_string, (char) file.eof());
        cout << "the content of the file is :" << endl << file_string << endl;
        cout << endl << endl << "Now I'm going to calculate the number of repeats for each charachter!!!! " << endl;
        for (int i=0; i<file_string.size(); i++){
            int flag = 0 ;
            // if (file_string[i] == '\n') continue;
            for (int j=0; j<chars.size(); j++){
                if (chars[j].first == file_string[i]){
                    chars[j].second ++;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){
                pair<char, int> p;
                p.first = file_string[i];
                p.second = 1;
                chars.push_back(p);
            }
        }
        // for EOF
        pair <char, int> p1;
        p1.first = '\0';
        p1.second = 1;
        chars.push_back(p1);
        // by the way we can impelement the pair. but we use STL library.
    }

    void printRepeats(){
        cout << "Number of repeats for each charachter is : " << endl;
        for (int i=0; i<chars.size(); i++){
            cout << chars[i].first << " :" << chars[i].second << endl; 
        }
    }

    void makeMinHeap(){
        cout << endl << endl << endl << "Now I'm going to make a min-heap using its impelemented class!!! " << endl ;
        mheap = MinHeap();
        for (int i=0; i<chars.size(); i++){
            Node* node = new Node();
            node->data.first = chars[i].first;
            node->data.second = chars[i].second;
            node->right = NULL;
            node->left = NULL;
            mheap.insertKey(node);
        }
        cout << endl << "I'd made the min-heap" << endl << endl;
        // mheap.print(); // just for print the heap that the program made.
    }

    void makeTree(){
        cout << endl << endl << "I'm going to make the tree using the min-heap as its regular algorithm!!!!" << endl << endl;
        int counter = 1;
        while (mheap.getHeapSize() != 1){
            Node* t1 = mheap.extractMin();
            Node* t2 = mheap.extractMin();
            Node* node = new Node();
            node->data.second = t1->data.second + t2->data.second;
            node->data.first = t1->data.first + t2->data.first;
            node->right = t2;
            node->left = t1;
            mheap.insertKey(node);
            root = node;
            // a Cout for printing the node, its right and left values :
            // cout << "now i'm making the node" << endl << "my node is " << node->data.first << "   and repeats of that is :"  << node->data.second
            //                                 << endl << "It's right node is : " << node->right->data.first << "  and repeats of that is : " << node->right->data.second
            //                                 << endl << "It's left node is : " << node->left->data.first << "  and repeats of that is : " << node->left->data.second << endl;

        }
        cout << endl <<"The tree was made and I have a pointer to its root!! " << endl;
        // now root points to the root of our tree.
        string s = "";
        simiDFS(root, s);
    }

    void simiDFS(Node* root, string s){
        // cout << "I'm now at " << root->data.second << endl;
        if (root->left == NULL && root->right == NULL){
            pair <char, string> p;
            p.first = root->data.first;
            p.second = s;
            path.push_back(p);
            return;
        }
        string s1 = s + "1"; 
        simiDFS(root->right, s1);
        string s2 = s + "0";
        simiDFS(root->left, s2);

    }

    void printPaths(){
        cout << endl << endl << "The path for each charachter is like this :" << endl;
        for (int i=0; i<path.size(); i++){
            cout << "for the charachter : " << path[i].first << "  : " << path[i].second << endl;
        }
    }


    void makeHuffmanFile(){
        cout << endl << endl << "I'm going to make the file huffman.txt " << endl;
        ofstream ofile ;
        ofile.open("Huffman.txt");
        for (int i=0; i<path.size(); i++){
            string s;
            s = s + path[i].first;
            s = s + '\t';
            s = s +to_string(path[i].second.size()) ;
            s = s + '\t';
            s = s + path[i].second;
            s = s + '\n' ;
            cout << s ;
            ofile << s;
        }
        cout << "The huffman file was created on the working directory " << endl << endl << endl ;
    }

    void makeZipFile(){
        cout << endl << "I'm going to make the zip.txt file";
        extra_bits  = 0;
        ofile.open("Zip.txt" ,ios::out);
        string temp = "";
        for (int i=0; i<file_string.size(); i++){
            for (int j=0; j<path.size(); j++){
                if (path[j].first == file_string[i]){
                    temp += path[j].second;
                }
            }
        }
        cout << endl << temp.size();
        while (temp.size() %8 != 0){
            temp+= "0";
            // extra_bits++;
        }
        cout << endl << temp.size();
        cout << endl << "Binary string that I'm going to write it on zip file : "<< endl << temp << endl << endl << endl;
        string s ="";
        string write_into_zip_file = "";
        for (int i=0; i<temp.size(); i++){
            s += temp[i]; 
            if ((i+1)%8 == 0){
                write_into_zip_file += (char) to_integer(s);
                s = "";
            }
        }
        
        ofile << write_into_zip_file;
        ofile.close();
    }
    void extractZipFile(){
        cout << "I'm going to extract the zip.txt " << endl;
        ofile.open("Zip.txt", ios::in);
        string myString ;
        ofile >> myString;
        string binary_string;
        for (std::size_t i = 0; i < myString.size(); ++i){
            binary_string += (bitset<8>(myString.c_str()[i])).to_string();
        }

        cout << "The extracted binary string is: " << endl <<binary_string << endl;
        makeTreeAgain();
        // after this functin call root will be made again.
        Node* node = root;
        string compressString = "";
        cout << endl << endl << endl << endl;
        cout << "Now time to use tree and decode the binary string "  << endl;
        for (int i=0; i<binary_string.size(); i++){
            
            cout << "The charachter is: " << node->data.first << "  the repeat is: " << node->data.second <<"  and I saw the " << binary_string[i] << endl;
            if (binary_string[i] == '1'){
                node = node->right;
            }
            else if (binary_string[i] == '0'){
                node = node->left;
            }

            if (node->left == NULL){
                if (node->data.first == '\0') break;
                if (node->data.first == '\n') cout <<"Yess I was found a backslash n" << endl; 
                compressString += node->data.first;
                
                node = root;
                continue;
            }
        }

        cout << endl<<endl<<endl<<"The Mined string from the zip file is : " << endl <<  compressString << endl;
        cout << "Hint: " << "Should be similar to the Input.txt" << endl;

    }
    void makeTreeAgain(){
        cout << "I'm making the tree again from Huffman.txt" << endl;
        fstream huffman_file;
        huffman_file.open("Huffman.txt");
        string line;
        Node* root_node = new Node();
        root_node->left = NULL;
        root_node->right = NULL;
        Node * copy_node = new Node();
        copy_node = root_node;
        bool newlineflag;
        while (getline(huffman_file, line)){
            // cout <<  line.size() << endl;
            newlineflag = false;
            if (line.size() == 0){
                cout << "yeeee I was found a backslash n" << endl;
                getline(huffman_file, line);
                newlineflag = true;
            }
            char data = line[0];
            // cout << line << endl;
            int tab_counter = 0;
            int start_of_string;
            for (int i=0; i<line.size(); i++){
                if (line[i] == '\t'){
                    tab_counter ++;
                }
                if (tab_counter == 2){
                    start_of_string = i;
                    break;
                }
            }
            // cout << start_of_string << endl;
            start_of_string++;
            string str;
            int counter = 0;
            for (int i=start_of_string ; i<line.size(); i++){
                if (line[i] == '1' && root_node->right == NULL){
                    Node* rnode = new Node();
                    root_node->right = rnode;
                    root_node = rnode;
                }
                else if (line[i] == '0' && root_node->left == NULL){
                    Node* lnode = new Node();
                    root_node->left = lnode;
                    root_node = lnode;
                }
                else if (line[i] == '1' && root_node->right !=NULL){
                    root_node = root_node->right;
                }
                else if (line[i] == '0' && root_node->left != NULL){
                    root_node = root_node->left;
                }
                if (i == line.size()-1){
                    if (newlineflag == true){
                        data = '\n' ;
                    }
                    if (root_node->data.first == '\n') cout << "Yess I was found a backslash n" << endl;
                    root_node->data.first = data;
                    root_node->data.second = 1;
                }
                
            }
          
            root_node = copy_node;
        }
        root = root_node; // very important part of code............................
        huffman_file.close();
    }

    ~Huffman(){
        file.close();
    }
};

int main(){
    Huffman* huffman = new Huffman();
    huffman->getInputFile("Input.txt"); // for get input file.
    huffman->calculateRepeats();        // it will calculate all characters with their repeats and store them in a vector of pairs.
    huffman->printRepeats();         // it will print the calculated repeats of charachters in the last fucntion. (can be comment)
    huffman->makeMinHeap();             // it will make the heap form the charachters in order to find huffman tree.
    huffman->makeTree();
    huffman->printPaths();
    huffman->makeHuffmanFile();         // it is fo section 5 of document
    // huffmaroot_noden->makeZipFile();             // it is for section 6 of document
    huffman->extractZipFile();           
    return 0;
}