// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

/*
 TO DO
 
 -rewrite clear using queue
 -understand mal allocation error in clear
 
 */

#include <queue>
using namespace std;

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap();
    
	~MyMap();
    
	void clear();
    
	int size() const;
    
    void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    struct Node {
        KeyType key;
        ValueType value;
        Node* left;
        Node* right;
    };
    
    Node* root;
    int m_size;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap(){
    root = nullptr;
    m_size = 0;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap(){
    clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear(){
    
    //clear empty tree
    if (root == nullptr)
        return;
    
    queue<Node*> q;
    Node* current = root;
    q.push(current);
    
    while (!q.empty()){
        current = q.front();
        q.pop();
        
        if (current->left != nullptr){
            q.push(current->left);
        }
        if (current->right != nullptr){
            q.push(current->right);
        }
        
        delete current;
        m_size--;
    }
    root = nullptr;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const {
    return m_size;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value){
    //if empty tree
    if (root == nullptr){
        Node* n = new Node;
        n->key = key;
        n->value = value;
        n->left = nullptr;
        n->right = nullptr;
        root = n;
        m_size++;
        return;
    }
    
    //attempt to find key
    
    //if already found
    if (find(key) != nullptr){
        ValueType* v = find(key);
        *v = value;
    }
    //if not found
    else{
        Node* n = root;
        while(true){
            if (key < n->key){
                if (n->left != nullptr){
                    n = n->left;
                }
                else{
                    Node* newNode = new Node;
                    newNode->key = key;
                    newNode->value = value;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    n->left = newNode;
                    m_size++;
                    return;
                }
            }
            else{
                if (n->right != nullptr){
                    n = n->right;
                }
                else{
                    Node* newNode = new Node;
                    newNode->key = key;
                    newNode->value = value;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    n->right = newNode;
                    m_size++;
                    return;
                }
            }
        }
    }
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const {
    //if empty
    
    if (root == nullptr)
        return nullptr;
    
    Node* ptr = root;
    while (ptr != nullptr){
        if (!(ptr->key < key) && !(key < ptr->key)){
            return &(ptr->value);
            break;
        }
        else if (key < ptr->key){
            ptr = ptr->left;
        }
        else{
            ptr = ptr->right;
        }
    }
    
    return nullptr;

}








