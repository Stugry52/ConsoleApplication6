#include <iostream>
using namespace std;

struct DamageRecord
{
    string attackerName;
    string targetName;
    int damage;
};

class DamageHistory
{
private: 
    DamageRecord* records;
    int capacity;
    int count;
    
public: 
    // Эта строчка запрещает копирование
    // Зачем его запрещать? Все потому что класс владеет динамической памятью (DamageRecord* records)
    // Если с++ скопирует такой объект автоматически, то он скопирует не весь массив а только нго адрес 
    // DamageHistory(const DamageHistory&) = delete;
    // DamageHistory& operator = (const DamageHistory&) = delete;
    
    DamageHistory(int maxRecords)
    {
        if (maxRecords <= 0)
        {
            maxRecords = 1;
        }
        
        records = new DamageRecord[maxRecords];
        capacity = maxRecords;
        count = 0;
    }
    
    ~DamageHistory()
    {
        delete[] records;
        records = nullptr;
        capacity = 0;
        count = 0;
    }
    
    void Add(const string& attacker, const string& target, int damage)
    {
        if (damage < 0)
        {
            damage = 0;
        }
        if (count < capacity)
        {
            records[count] = DamageRecord{attacker, target, damage}; // Записываем в конце массива
            count++; 
            return;
        }

        for (int i = 0; i < capacity; ++i) // Если места нет, сдвигаем массив влево
        {
            records[i - 1] = records[i]; // Переносим запись
        }
        // После этого новую запись кладем в конец
        records[capacity - 1] = DamageRecord{attacker, target, damage};
    }
    
    void Print() const
    {
        cout << "История урона: " <<  endl;
        
        for (int i = 0; i < count; ++i)
        {
            cout << i + 1 << ". ";
            cout << records[i].attackerName << " -> ";
            cout << records[i].targetName << ": ";
            cout << records[i].damage << " урона";
            cout << endl;
        }
    }
    
    DamageHistory(const DamageHistory& other)
    {
        capacity = other.capacity; // Берем capacity из другого объекта
        count = other.count;    // Берем count из другого объекта
        records = new DamageRecord[capacity]; // Создаем новый массив
    
        for (int i = 0; i < count; i++) // Копируем каждую запись по одной
        {
            records[i] = other.records[i];
        }
    }
    
    DamageHistory& operator=(const DamageHistory& other){
        if (this == &other) // Проверка не присваивается ли самому себе
        {
            return *this; // Просто возвращаем текущий объект
        }
        
        delete[] records; // Освобождаем старый массив текущего объета
        
        capacity = other.capacity; // Копируем вместимость
        count = other.count;   // Копируем ко-во
        records = new DamageRecord[capacity];  // Выделяем память под новый массив


        for (int i = 0; i < count; ++i)
        {
            records[i] = other.records[i]; // Копируем каждую запись по одной из прошлого массива
        }
        
        return *this; // Возвращаем скопированный объект
    }
};

class EventLog{
private: // Закрытая часть класса
    string* lines;
    int capacity;   // макс. кол-во в коде
    int count;      // текущее кол-во
    
public: // Открытая часть кода
    EventLog(int maxLines){   // Конструктор принимает макс. число строк
        if (maxLines <= 0)
        {
            maxLines = 1;
        }
        lines = new string[maxLines];  // Выделяем память под строки
        capacity = maxLines;
        count = 0;
    }
    
    ~EventLog(){   // Автовызов деконструктора
        delete[] lines;
        lines = nullptr;
        capacity = 0;
        count = 0;
    }
    
    void Add(const string& line){
        if (count < capacity){
            lines[count] = line;   // Кладем строку в свободное место, если оно есть
            count++;
            return;
        }

        for (int i = 0; i < count; i++){
            lines[i] = lines[i + 1];
        }
        lines[capacity - 1] = line;   // Кладем новую строку в конец списка
    }
    
    void Print() const{    // Печатаем лог и не изменяем объект
        cout << "Event Log: " << endl;
        for (int i = 0; i < count; i++){
            cout << "- " << lines[i] << endl;
        }
    }
    
    EventLog(const EventLog& other)
    {
        capacity = other.capacity; 
        count = other.count;    
        lines = new string[capacity]; 
    
        for (int i = 0; i < count; i++) 
        {
            lines[i] = other.lines[i];
        }
    }
    
    EventLog& operator=(const EventLog& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        delete[] lines;
        
        capacity = other.capacity; 
        count = other.count;   
        lines = new string[capacity];
        
        for (int i = 0; i < count; i++)
        {
            lines[i] = other.lines[i];
        }
        
        return *this;
    }
};


// Допустим есть объект
//DamageHistory a(3);
// Внутри него a.records -> [record][record1][record2]
// Теперь, если напишем:
//DamageHistory b = a;
// Это не ручное копирование, с++ сделает обычное копирование полей
// b.records = a.records
// b.capacity = a.capacity
// b.count = a.count
// В итоге получится что b и a это один и тот же массив
// Это называет shallow copy - поверхностное копирование
// Копирпуется только указатель, а не данные на которые он указал
////////////////////////////////////////////////////
// Почему shallow copy опасен
// когда программа завершится выполнив деструктор ~DamageHistory()
// Деструктор выполнится для а
// он сделает delete[] records
// Но это тот же самый массив
// Поучится double delete - двойное освобождение памяти
// Это Баг - программа может упасть даже не там, где ошибка была сделана
//////////////////////////////////////////////////////
// Deep Copy - правильное копирование объекта сдинамической памятью
// a.records -> [A0][A1][A2]
// b.records -> [B0][B1][B2]
// Значение внутри будут одинаковые, но память выделена разная
// ///////////////////////////////////////////////
// Copy constructor
// Вызывается когда новый объект создается как копия старого
//DamageHistory b = a;
//DamageHistory b(a);
// Сигнатура:
//DamageHistory(const DamageHistory& other)
// other - и есть объект который мы копируем
////////////////////////////////////////////////////
// Плохой пример поверхностного копирования

// copy assigment operator
DamageHistory a(3);
DamageHistory b(5);

// b = a;
// b как объект уже существует 
// То есть это не copy constructor, а оператор
//DamageHistory& operator=(const DamageHistory& other);
// Он вызывается, когда мы присваиваем один существующий объект, другому
///////////////////////
// Чем они отличаются?
// copy constructor создает новый объект DamageHistory b = a; (тут b еще не существовал)
// a copy assigment - перезаписывает уже существующий объект b = a; (b уже существовал и возможно имел свою память)
// Поэтому assigment copy должен:
// Проверить не присваиваем ли объет самому себе
// удалить старую память 
// выделить новую память
// скопировать данные
// и вернуть *this


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russia");
    DamageHistory original(3); // Создаем пустой журнал
    
    original.Add("Oleg", "Stas", 10);
    original.Add("Oleg", "Kirill", 12);
    
    DamageHistory copy = original; // Вызваем copy constructor и копируем
    
    copy.Add("Kirill", "Oleg", 5);
    
    DamageHistory c(1);
    c = b; // copy assigment operator
    
    cout << "Original: " << endl;
    original.Print();
    
    cout << "Copy: " << endl;
    copy.Print();
    
    cout << "C: " << endl;
    copy.Print();
    
    //DamageHistory b = a; // ОПАСНО - будет ошибка shallow copy
    
    return 0;
}
