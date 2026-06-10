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
};


// Допустим есть объект
DamageHistory a(3);
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



int main(int argc, char* argv[])
{
    DamageHistory a(3); // Создаем пустой журнал
    
    a.Add("Oleg", "Stas", 10);
    
    DamageHistory b = a; // ОПАСНО - будет ошибка shallow copy
    
    return 0;
}
