#include <iostream>
#include <fstream>
using namespace std;

struct edge {
    int vertex1, vertex2;//вершины
    int weight;//вес ребра
    edge(){

    }
    edge(int vertex1, int vertex2, int weight) {
        this->vertex1 = vertex1;
        this->vertex2 = vertex2;
        this->weight = weight;
    }
};

//Для хранения динамический массив и односвязный список
template <typename T>
class ArrayList {
private:
    T* ptr;
    unsigned int size;//размер массива, задаваемый при создании
    unsigned int capacity;//размер массива с учетом дополнительного места

public:
    ArrayList(unsigned int size = 0){//конструктор
        this->size = size;
        capacity = 1;
        while (capacity <= size){
            capacity *= 2;
        }
        ptr = (T*)malloc(capacity * sizeof(T));
    }
    ~ArrayList(){//деструктор
        size = 0;
        capacity = 0;
        free(ptr);
        ptr = nullptr;
    }
    unsigned int GetSize(){//геттер для размера массива
        return size;
    }
    unsigned int GetCapacity(){//геттер для размера массива с учётом дополнительного места
        return capacity;
    }
    void push_back(T data){//вставка в конец
        ++size;
        if (size > capacity){
            capacity *= 2;
            ptr = (T*)realloc(ptr, capacity * sizeof(T));
        }
        *(ptr + size - 1) = data;
    }
    void insert(unsigned int index, T data){//вставка со сдвигом элементов
        if (index < GetSize()){
            ++size;
            if (size > capacity){
                capacity *= 2;
                ptr = (T*)realloc(ptr, capacity * sizeof(T));
            }
            for (unsigned int i = size - 1; i >= index; --i){//обратный цикл до индекса
                if (i > index)
                    *(ptr + i) = *(ptr + i - 1);
                if (i == index)
                    *(ptr + i) = data;
            }
        }
        else
            push_back(data);
    }
    void removeAt(unsigned int index){//удаление по индексу
        for (unsigned int i = 0; i < size; ++i){
            if (i > index){
                *(ptr + i - 1) = *(ptr + i);
            }
            if (i == size - 1)
                *(ptr + i) = T();
        }
        --size;
        if (size < capacity / 4){
            capacity /= 2;
            ptr = (T*)realloc(ptr, capacity * sizeof(T));
        }
    }
    void pop_back(){//удаление последнего элемента массива
        removeAt(size - 1);
    }
    T& operator[](const unsigned int index){//перегрузка оператора
        if (index < size)
            return (*(ptr + index));
    }
};
template <typename T>
class Node{//узел односвязного списка
public:
    Node* pNextAddress;//указатель на следующий элемент
    T data;//данные элемента

    //конструктор
    Node(T data = T(), Node* pNextAddress = nullptr){
        this->data = data;
        this->pNextAddress = pNextAddress;
    }
};
template <typename T>
class LinkedList{
private:
    unsigned int size;//количество элементов в списке
    Node<T>* head;//указатель на первый элемент списка

public:
    T& operator[](const unsigned int index){//перегрузка оператора
        Node<T> *currentNode = head;
        int i = 0;//счётчик
        while (currentNode != nullptr){
            if (i == index){
                return currentNode->data;
            }
            currentNode = currentNode->pNextAddress;
            ++i;
        }
    }
    int GetSize(){//геттер для размера списка
        return size;
    }
    LinkedList(){//конструктор по умолчанию
        size = 0;
        head = nullptr;
    }
    ~LinkedList(){//деструктор
        //clear();
    }
    void push_back(T data){//добавить данные в конец списка
        if (head == nullptr){//если первого элемента нет, то создаём его
            head = new Node<T>(data);
        }
        else{
            Node<T>* currentNode = head;
            while (currentNode->pNextAddress != nullptr){
                currentNode = currentNode->pNextAddress;//присваиваем каждый следующий адрес текущему элементу, пока не nullptr
            }
            currentNode->pNextAddress = new Node<T>(data);
        }
        ++size;
    }
    void push_front(T data){//добавить данные в начало списка
        head = new Node<T>(data, head);
        ++size;
    }
    void pop_front(){//удаление первого элемента списка
        Node<T>* buffer = head;//буффер, который удаляем сразу после присваивания значения следующего элемента
        head = head->pNextAddress;
        delete buffer;
        --size;
    }
    void pop_back(){//удаление последнего эл-а списка
        removeAt(size - 1);
    }
    void clear(){//удаление списка и очистка памяти
        while (size != 0)
            pop_front();
    }
    void insert(T data, int index){//вставка нового элемента
        if (index == 0){
            push_front(data);
        }
        else{
            Node<T>* previous = head;
            for (int i = 0; i < index - 1; ++i){
                previous = previous->pNextAddress;
            }
            Node<T>* newNode = new Node<T>(data, previous->pNextAddress);//новый элемент указывает на тот, который шёл после предыдущего
            previous->pNextAddress = newNode;//предыдущий элемент указывает на новый
            ++size;
        }
    }
    void removeAt(int index){//удаление элемента по индексу
        if (index == 0){
            pop_front();
        }
        else {
            Node<T>* previous = head;
            for (int i = 0; i < index - 1; ++i){
                previous = previous->pNextAddress;
            }
            Node<T>* removable = previous->pNextAddress;
            previous->pNextAddress = removable->pNextAddress;
            delete removable;
            --size;
        }
    }
};
//Функция для сортировки рёбер по их весу(основано на сортировке вставками)
void sortEdges(LinkedList<edge> graph){
    int i, j;
    edge key;
    int n = graph.GetSize();
    for (i = 1; i < n; ++i){
        key = graph[i];
        j = i - 1;
        while (j >= 0 && graph[j].weight > key.weight){
            graph[j + 1] = graph[j];
            --j;
        }
        graph[j + 1] = key;
    }
}

int main(){
    ifstream input;
    input.open("input.txt");
    if (input.is_open())
    {
        LinkedList<edge> graph;//граф представляет собой множество рёбер
        LinkedList<edge> tree;//остовное дерево

        int n, m;//число вершин и рёбер
        input >> n >> m;
        for (int i = 0; i < m; i++) {
            int vertex1, vertex2, weight;
            input >> vertex1 >> vertex2 >> weight;
            edge buff(vertex1, vertex2, weight);
            graph.push_back(buff);
        }
        input.close();
        sortEdges(graph);//сортируем рёбра в графе по их весу(по возрастанию)

        int totalWeight = 0;//общий вес рёбер в дереве
        ArrayList<int> set(n + 1);//множество всех вершин
        for (int i = 0; i <= n; i++){
            set[i] = i;//set[i(вершина)] = i(множество);
        }
        /*
        Проверяем вершины каждого ребра.
        Если вершины не принадлежат одному и тому же множеству,	то такое ребро добавляем в наше дерево, а вершины помещаем в одно множество.
        */
        for (int i = 0; i < m; ++i){
            int a = graph[i].vertex1;
            int b = graph[i].vertex2;
            if (set[a] != set[b]){
                tree.push_back(graph[i]);
                totalWeight += graph[i].weight;
                int oldSet = set[b], newSet = set[a];
                for (int j = 0; j <= n; ++j){
                    if (set[j] == oldSet){
                        set[j] = newSet;
                    }
                }
            }
        }
        //Вывод всех рёбер остовного дерева
        ofstream output;
        output.open("output.txt");
        for (int i = 0; i < tree.GetSize(); ++i){
            cout << tree[i].vertex1 << ' ' << tree[i].vertex2 << ' '  << endl;
            output << tree[i].vertex1 << ' ' << tree[i].vertex2 << ' ' << endl;
        }
        cout << totalWeight;
        output << totalWeight;
        output.close();
    }
    else
        cout << "Check the path." << endl;
    return 0;
}

