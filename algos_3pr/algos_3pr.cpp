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
        cout << "Ошибка открытия файла!" << endl;
        return nullptr;
    }

    string line;
    getline(file, line);
    file.close();

    if (!isValidTreeString(line)) {
        cout << "Неверный формат скобочной записи!" << endl;
        return nullptr;
    }

    size_t pos = 0;
    BinaryTree* root = parseTree(line, pos);

    if (root) cout << "Дерево успешно загружено!" << endl;
    else cout << "Ошибка парсинга!" << endl;
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
    cout << "1. Загрузить дерево из файла" << endl;
    cout << "2. Показать обычное дерево" << endl;
    cout << "3. Обходы обычного дерева (рекурсивные)" << endl;
    cout << "4. Создать АВЛ дерево" << endl;
    cout << "5. Показать АВЛ дерево" << endl;
    cout << "6. Обходы АВЛ дерева (4 способа)" << endl;
    cout << "7. Вставить в АВЛ" << endl;
    cout << "8. Удалить из АВЛ" << endl;
    cout << "9. Поиск в АВЛ" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    setlocale(LC_ALL, "Russian");
    BinaryTree* BinaryTree = nullptr;
    AVLTree* AVLTree = nullptr;
    string filename;
    int choice, value;

    while (true) {
        show_menu();
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Имя файла: ";
            cin >> filename;
            if (BinaryTree) delete_Tree(BinaryTree);
            BinaryTree = loadTreeFromFile(filename);
            break;
        }
        case 2: {
            if (BinaryTree) print_Tree(BinaryTree);
            else cout << "Дерево не загружено!" << endl;
            break;
        }
        case 3: {
            if (BinaryTree) {
                cout << "Прямой обход: "; preOrderTravers(BinaryTree); cout << endl;
                cout << "Симметричный обход: "; inOrderTravers(BinaryTree); cout << endl;
                cout << "Обратный обход: "; postOrderTravers(BinaryTree); cout << endl;
            }
            else cout << "Дерево не загружено!" << endl;
            break;
        }
        case 4: {
            if (BinaryTree) {
                if (AVLTree) delete_AVLTree(AVLTree);
                convert_AVLTree(BinaryTree, AVLTree);
                cout << "АВЛ дерево создано!" << endl;
            }
            else cout << "Сначала загрузите дерево!" << endl;
            break;
        }
        case 5: {
            if (AVLTree) print_AVL(AVLTree);
            else cout << "АВЛ дерево не создано!" << endl;
            break;
        }
        case 6: {
            if (AVLTree) {
                cout << "Обход в ширину: "; breadthFirstAVL(AVLTree); cout << endl;
                cout << "Прямой обход: "; preOrderAVL(AVLTree); cout << endl;
                cout << "Симметричный обход: "; inOrderAVL(AVLTree); cout << endl;
                cout << "Обратный обход: "; postOrderAVL(AVLTree); cout << endl;
            }
            else cout << "АВЛ дерево не создано!" << endl;
            break;
        }
        case 7: {
            if (AVLTree) {
                cout << "Значение: ";
                cin >> value;
                AVLTree = insertAVL(AVLTree, value);
                cout << "Вставлено!" << endl;
            }
            else cout << "АВЛ дерево не создано!" << endl;
            break;
        }
        case 8: {
            if (AVLTree) {
                cout << "Значение: ";
                cin >> value;
                AVLTree = deleteAVL(AVLTree, value);
                cout << "Удалено!" << endl;
            }
            else cout << "АВЛ дерево не создано!" << endl;
            break;
        }
        case 9: {
            if (AVLTree) {
                cout << "Значение: ";
                cin >> value;
                cout << (searchAVL(AVLTree, value) ? "Найдено!" : "Не найдено!") << endl;
            }
            else cout << "АВЛ дерево не создано!" << endl;
            break;
        }
        case 0: {
            if (BinaryTree) delete_Tree(BinaryTree);
            if (AVLTree) delete_AVLTree(AVLTree);
            cout << "Выход!" << endl;
            return 0;
        }
        default: cout << "Неверный выбор!" << endl;
        }
    }
}
