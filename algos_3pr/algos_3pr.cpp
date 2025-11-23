#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Узел бинарного дерева
struct BinaryTree {
    int data;
    BinaryTree* left;
    BinaryTree* right;
    BinaryTree(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Узел АВЛ дерева
struct AVLTree {
    int data;
    AVLTree* left;
    AVLTree* right;
    int height;
    AVLTree(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

// Стек для итеративных обходов
template<typename T>
struct Stack {
    T** data;
    int capacity;
    int top;

    Stack(int size) {
        capacity = size;
        data = new T * [capacity];
        top = -1;
    }
    ~Stack() {
        delete[] data;
    }
    void push(T* node) {
        if (top < capacity - 1) {
            data[++top] = node;
        }
    }
    T* pop() {
        if (top >= 0) {
            return data[top--];
        }
        return nullptr;
    }
    bool isEmpty() {
        return top == -1;
    }
};

// Очередь для обхода в ширину
template<typename T>
struct Queue {
    T** data;
    int capacity;
    int front;
    int rear;
    int count;

    Queue(int size) {
        capacity = size;
        data = new T * [capacity];
        front = 0;
        rear = -1;
        count = 0;
    }
    ~Queue() {
        delete[] data;
    }
    void enqueue(T* node) {
        if (count < capacity) {
            rear = (rear + 1) % capacity;
            data[rear] = node;
            count++;
        }
    }
    T* dequeue() {
        if (count > 0) {
            T* node = data[front];
            front = (front + 1) % capacity;
            count--;
            return node;
        }
        return nullptr;
    }
    bool isEmpty() {
        return count == 0;
    }
};

// Рекурсивные обходы в глубину:
// Прямой обход
void preOrderTravers(BinaryTree* root) {
    if (!root) return;
    cout << root->data << " ";
    preOrderTravers(root->left);
    preOrderTravers(root->right);
}
// Симметричный обход
void inOrderTravers(BinaryTree* root) {
    if (!root) return;
    inOrderTravers(root->left);
    cout << root->data << " ";
    inOrderTravers(root->right);
}
// Обратный обход
void postOrderTravers(BinaryTree* root) {
    if (!root) return;
    postOrderTravers(root->left);
    postOrderTravers(root->right);
    cout << root->data << " ";
}

// Проверка корректности скобочной записи
bool isValidTreeString(const string& str) {
    int balance = 0;
    for (char c : str) {
        if (c == '(') balance++;
        else if (c == ')') balance--;
        if (balance < 0) return false;
    }
    return balance == 0;
}

// Парсинг скобочной записи
BinaryTree* parseTree(const string& str, size_t& pos) {
    while (pos < str.length() && str[pos] == ' ') pos++;
    if (pos >= str.length() || str[pos] != '(') return nullptr;

    pos++; // Пропускаем '('
    while (pos < str.length() && str[pos] == ' ') pos++;

    // Читаем число
    string numStr = "";
    while (pos < str.length() && isdigit(str[pos])) {
        numStr += str[pos++];
    }

    if (numStr.empty()) return nullptr;

    BinaryTree* node = new BinaryTree(stoi(numStr));

    // Рекурсивно парсим потомков
    node->left = parseTree(str, pos);
    node->right = parseTree(str, pos);

    while (pos < str.length() && str[pos] == ' ') pos++;
    if (pos < str.length() && str[pos] == ')') pos++;

    return node;
}

// Загрузка дерева из файла
BinaryTree* loadTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "File opening error!" << endl;
        return nullptr;
    }

    string line;
    getline(file, line);
    file.close();

    if (!isValidTreeString(line)) {
        cout << "Incorrect bracket format!" << endl;
        return nullptr;
    }

    size_t pos = 0;
    BinaryTree* root = parseTree(line, pos);

    if (root) cout << "The tree has been uploaded successfully!" << endl;
    else cout << "Parsing error!" << endl;
    return root;
}

// Вспомогательные функции
int height(AVLTree* node) { return node ? node->height : 0; }
int max(int a, int b) { return (a > b) ? a : b; }
int balanceFactor(AVLTree* node) { return node ? height(node->left) - height(node->right) : 0; }

// Повороты для балансировки:
// правый поворот
AVLTree* rotateRight(AVLTree* y) {
    AVLTree* x = y->left;
    AVLTree* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}
// левый поворот
AVLTree* rotateLeft(AVLTree* x) {
    AVLTree* y = x->right;
    AVLTree* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Обход в ширину (итеративный)
void breadthFirstAVL(AVLTree* root) {
    if (!root) return;
    Queue<AVLTree> queue(100);
    queue.enqueue(root);

    while (!queue.isEmpty()) {
        AVLTree* curr = queue.dequeue();
        cout << curr->data << " ";
        if (curr->left) queue.enqueue(curr->left);
        if (curr->right) queue.enqueue(curr->right);
    }
}
// Прямой обход (итеративный)
void preOrderAVL(AVLTree* root) {
    if (!root) return;
    Stack<AVLTree> stack(100);
    stack.push(root);

    while (!stack.isEmpty()) {
        AVLTree* curr = stack.pop();
        cout << curr->data << " ";
        if (curr->right) stack.push(curr->right);
        if (curr->left) stack.push(curr->left);
    }
}
// Симметричный обход (итеративный)
void inOrderAVL(AVLTree* root) {
    Stack<AVLTree> stack(100);
    AVLTree* curr = root;

    while (curr || !stack.isEmpty()) {
        while (curr) {
            stack.push(curr);
            curr = curr->left;
        }
        curr = stack.pop();
        cout << curr->data << " ";
        curr = curr->right;
    }
}
// Обратный обход (итеративный)
void postOrderAVL(AVLTree* root) {
    if (!root) return;
    Stack<AVLTree> st1(100), st2(100);
    st1.push(root);

    while (!st1.isEmpty()) {
        AVLTree* curr = st1.pop();
        st2.push(curr);
        if (curr->left) st1.push(curr->left);
        if (curr->right) st1.push(curr->right);
    }
    while (!st2.isEmpty()) {
        cout << st2.pop()->data << " ";
    }
}

// Поиск минимального узла
AVLTree* findMin(AVLTree* node) {
    while (node->left) node = node->left;
    return node;
}

// Вставка в АВЛ дерево
AVLTree* insertAVL(AVLTree* root, int key) {
    if (!root) return new AVLTree(key);

    if (key < root->data) root->left = insertAVL(root->left, key);
    else if (key > root->data) root->right = insertAVL(root->right, key);
    else return root; // Дубликаты не разрешены

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = balanceFactor(root);

    if (balance > 1 && key < root->left->data) return rotateRight(root);
    if (balance < -1 && key > root->right->data) return rotateLeft(root);
    if (balance > 1 && key > root->left->data) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && key < root->right->data) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

// Удаление из АВЛ дерева
AVLTree* deleteAVL(AVLTree* root, int key) {
    if (!root) return root;

    if (key < root->data) root->left = deleteAVL(root->left, key);
    else if (key > root->data) root->right = deleteAVL(root->right, key);
    else {
        if (!root->left || !root->right) {
            AVLTree* temp = root->left ? root->left : root->right;
            if (!temp) {
                delete root;
                return nullptr;
            }
            *root = *temp;
            delete temp;
        }
        else {
            AVLTree* temp = findMin(root->right);
            root->data = temp->data;
            root->right = deleteAVL(root->right, temp->data);
        }
    }
    if (!root) return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = balanceFactor(root);
  
    if (balance > 1 && balanceFactor(root->left) >= 0) return rotateRight(root);
    if (balance > 1 && balanceFactor(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && balanceFactor(root->right) <= 0) return rotateLeft(root);
    if (balance < -1 && balanceFactor(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

// Поиск в АВЛ дереве
bool searchAVL(AVLTree* root, int key) {
    if (!root) return false;
    if (key == root->data) return true;
    return key < root->data ? searchAVL(root->left, key) : searchAVL(root->right, key);
}

// Вывод деревьев
void print_Tree(BinaryTree* root, int level = 0) {
    if (!root) return;
    print_Tree(root->right, level + 1);
    cout << string(level * 4, ' ') << root->data << endl;
    print_Tree(root->left, level + 1);
}
void print_AVL(AVLTree* root, int level = 0) {
    if (!root) return;
    print_AVL(root->right, level + 1);
    cout << string(level * 4, ' ') << root->data << " (h:" << root->height << ")" << endl;
    print_AVL(root->left, level + 1);
}

// Освобождение памяти
void delete_Tree(BinaryTree* root) {
    if (!root) return;
    delete_Tree(root->left);
    delete_Tree(root->right);
    delete root;
}
void delete_AVLTree(AVLTree* root) {
    if (!root) return;
    delete_AVLTree(root->left);
    delete_AVLTree(root->right);
    delete root;
}

void convert_AVLTree(BinaryTree* binaryRoot, AVLTree*& avlRoot) {
    if (!binaryRoot) return;
    Queue<BinaryTree> queue(100);
    queue.enqueue(binaryRoot);

    while (!queue.isEmpty()) {
        BinaryTree* curr = queue.dequeue();
        avlRoot = insertAVL(avlRoot, curr->data);
        if (curr->left) queue.enqueue(curr->left);
        if (curr->right) queue.enqueue(curr->right);
    }
}

void show_menu() {
    cout << "1. Load tree from file" << endl;
    cout << "2. Show binary tree" << endl;
    cout << "3. Binary tree traversals (recursive)" << endl;
    cout << "4. Create AVL tree" << endl;
    cout << "5. Show AVL tree" << endl;
    cout << "6. AVL tree traversals (4 methods)" << endl;
    cout << "7. Insert into AVL" << endl;
    cout << "8. Delete from AVL" << endl;
    cout << "9. Search in AVL" << endl;
    cout << "0. Exit" << endl;
    cout << "Choose action: ";
}

int main() {
    BinaryTree* binaryTree = nullptr;
    AVLTree* avlTree = nullptr;
    string filename;
    int choice, value;

    while (true) {
        show_menu();
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Filename: ";
            cin >> filename;
            if (binaryTree) delete_Tree(binaryTree);
            binaryTree = loadTreeFromFile(filename);
            break;
        }
        case 2: {
            if (binaryTree) print_Tree(binaryTree);
            else cout << "Tree not loaded!" << endl;
            break;
        }
        case 3: {
            if (binaryTree) {
                cout << "Pre-order: "; preOrderTravers(binaryTree); cout << endl;
                cout << "In-order: "; inOrderTravers(binaryTree); cout << endl;
                cout << "Post-order: "; postOrderTravers(binaryTree); cout << endl;
            }
            else cout << "Tree not loaded!" << endl;
            break;
        }
        case 4: {
            if (binaryTree) {
                if (avlTree) delete_AVLTree(avlTree);
                convert_AVLTree(binaryTree, avlTree);
                cout << "AVL tree created!" << endl;
            }
            else cout << "Load tree first!" << endl;
            break;
        }
        case 5: {
            if (avlTree) print_AVL(avlTree);
            else cout << "AVL tree not created!" << endl;
            break;
        }
        case 6: {
            if (avlTree) {
                cout << "Breadth-first: "; breadthFirstAVL(avlTree); cout << endl;
                cout << "Pre-order: "; preOrderAVL(avlTree); cout << endl;
                cout << "In-order: "; inOrderAVL(avlTree); cout << endl;
                cout << "Post-order: "; postOrderAVL(avlTree); cout << endl;
            }
            else cout << "AVL tree not created!" << endl;
            break;
        }
        case 7: {
            if (avlTree) {
                cout << "Value: ";
                cin >> value;
                avlTree = insertAVL(avlTree, value);
                cout << "Inserted!" << endl;
            }
            else cout << "AVL tree not created!" << endl;
            break;
        }
        case 8: {
            if (avlTree) {
                cout << "Value: ";
                cin >> value;
                avlTree = deleteAVL(avlTree, value);
                cout << "Deleted!" << endl;
            }
            else cout << "AVL tree not created!" << endl;
            break;
        }
        case 9: {
            if (avlTree) {
                cout << "Value: ";
                cin >> value;
                cout << (searchAVL(avlTree, value) ? "Found!" : "Not found!") << endl;
            }
            else cout << "AVL tree not created!" << endl;
            break;
        }
        case 0: {
            if (binaryTree) delete_Tree(binaryTree);
            if (avlTree) delete_AVLTree(avlTree);
            cout << "Exit!" << endl;
            return 0;
        }
        default: cout << "Invalid choice!" << endl;
        }
    }
}
