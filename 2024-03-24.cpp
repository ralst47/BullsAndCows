﻿#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <cmath>

using namespace::std;

class SimpleTimer//таймер
{
    const float precision = 0.001f;//точность
    std::chrono::time_point<std::chrono::steady_clock> start, end;
public:
    SimpleTimer()
    {
        start = std::chrono::high_resolution_clock::now();//старт секундомера
    }
    ~SimpleTimer()
    {
        end = std::chrono::high_resolution_clock::now();//стоп секундомера
        std::chrono::duration<float> duration = end - start;//время работы
        //cout << "DURATION: " << duration.count() << " s" << endl;
        cout << "\t" << round(duration.count() / precision) * precision << endl;
    }
};

size_t Placements(const int& k, const int& n)
{
    size_t result = 1;
    for (int i = (n - k + 1); i <= n; i++)
    {
        result *= i;
    }
    return result;
}

class TStep //ход + быки и коровы
{
public:
    string code = "";//ход
    int bulls = 0;//быки
    int cows = 0;//коровы
};

class Count_BC//автоматический подсчёт быков и коров
{
    int bulls, cows;
public:
    int Bulls(const string& variant, const string& other)
    {
        bulls = 0;
        for (int i = 0; i < other.size(); ++i)//подсчёт быков
            if (variant[i] == other[i]) ++bulls;
        return bulls;
    }
    int Cows(const string& variant, const string& other)
    {
        //string str = other;
        int cows = -Bulls(variant, other);
        for (const auto& symbol : other)//подсчёт коров
            if (variant.find(symbol) != string::npos) ++cows;
        return cows;
    }
};

bool operator ==(const TStep& variant, const string& other)
{
    int bulls = 0;
    for (int i = 0; i < other.size(); ++i)//подсчёт быков
        if (variant.code[i] == other[i]) ++bulls;
    if (variant.bulls != bulls) return false;
    int cows = -bulls;
    for (const auto& symbol : other)//подсчёт коров
        if (variant.code.find(symbol) != string::npos) ++cows;
    if (variant.cows != cows) return false;
    else return true;
}

class Bull_Cow//родительский класс
{
public:
    string symbols;//строка вариантов символов
    int listSize, count;//кол-во всех символов, разрядность
    TStep mStep;
protected:
    //vector<TStep>container;//контейнер ходов + быков и коров
    static vector<TStep>container;//контейнер ходов + быков и коров
    //void Header()
    //{
    //    cout << "n = " << symbols.size() << " (" << symbols << ")" << endl;
    //}
public:
    bool is_Ok(const string& step)//проверка хода на соответствие предыдущим ходам (в контейнере)
    {
        for (const auto& variant : container)//перебор имеющегося контейнера с ходами
            if (!(variant == step)) return false;//тут перегрузка оператора ==
        return true;
    }
    //Bull_Cow() {}//конструктор по умолчанию
    Bull_Cow(const int& COUNT, const string& LIST)//конструктор
    {
        srand((int)time(NULL));
        symbols = LIST;
        count = COUNT;
        //Header();
    }

    void SetStep(const string& step, const int& bulls, const int& cows)//заполнение контейнера ответами
    {
        if (bulls == 0 && cows == 0)//исключение символов из symbols, если bulls = 0 и cows = 0
            for (char ch : step)
                symbols.erase(remove(symbols.begin(), symbols.end(), ch), symbols.end());
        else//заполнение контейнера ответами
        {
            mStep.code = step;
            mStep.bulls = bulls;
            mStep.cows = cows;
            container.push_back(mStep);
        }
    }
};

vector<TStep>Bull_Cow::container;//для static vector<TStep>container: контейнер ходов + быков и коров

class Bull_Cow_Random :public Bull_Cow
{
    bool is_Same(const string& version, const char& x);//проверка на повтор символа
    char GenerateSymbol() { return symbols[rand() % symbols.size()]; }//генерация символа
    string GenerateStep();//генерация хода
//public:
//    bool is_Ok(const string& step)//проверка хода на соответствие предыдущим ходам (в контейнере)
//    {
//        for (const auto& variant : container)//перебор имеющегося контейнера с ходами
//            if (!(variant == step)) return false;//тут перегрузка оператора ==
//        return true;
//    }
public:
    Bull_Cow_Random(const int& COUNT, const string& LIST) :Bull_Cow(COUNT, LIST) {}//конструктор

    //void SetStep(const string& step, const int& bulls, const int& cows)//заполнение контейнера ответами
    //{
    //    if (bulls == 0 && cows == 0)//удаление символов из symbols, если bulls = 0 и cows = 0
    //        for (char ch : step)
    //            symbols.erase(remove(symbols.begin(), symbols.end(), ch), symbols.end());
    //    else//заполнение контейнера ответами
    //    {
    //        TStep mStep;
    //        mStep.code = step;
    //        mStep.bulls = bulls;
    //        mStep.cows = cows;
    //        container.push_back(mStep);
    //    }
    //}

    string GetStep()//ход
    {
        string step;
        if (container.empty()) step = GenerateStep();
        else do step = GenerateStep(); while (!is_Ok(step));
        return step;
    }
};

class Bull_Cow_Array :public Bull_Cow
{
    //SimpleTimer st;//запуск таймера

    //list<string>array;//контейнер
    list<string>array;//контейнер
    void Permutation_Ref(const int& rk, string& rnumber);
    void Permutation(const int& k, string& symbols);//лексикографическая перестановка
    //{
    //    //auto sBegin = symbols.begin() + k;
    //    do
    //    {
    //        array.push_front(symbols.substr(0, k));//добавление комбинации в контейнер list
    //        //array.insert(symbols.substr(0, k));//добавление комбинации в контейнер set
    //        reverse(symbols.begin() + k, symbols.end());//реверс символов начиная с k-го элемента
    //    } while (next_permutation(symbols.begin(), symbols.end()));//следующая комбинация - лексикографическая перестановка
    //}

    //void Permutation(const int& k, string& symbols)//лексикографическая перестановка
    //{
    //    int i = 0; size_t pm = Placements(k, symbols.size());
    //    bool flag;
    //    do
    //    {
    //        array.push_front(symbols.substr(0, k));//добавление комбинации в контейнер list
    //        //array.insert(symbols.substr(0, k));//добавление комбинации в контейнер set
    //        reverse(symbols.begin() + k, symbols.end());//реверс символов начиная с k-го элемента
    //        flag = next_permutation(symbols.begin(), symbols.end());
    //        //if (i % (pm / 8) == 0 || !flag) { auto it = array.begin(); advance(it, i); cout << i << "\t" << *it << endl; }//вывод через итератор
    //        //if (i % (pm / 8) == 0 || !flag) cout << i << "\t" << array[i] << endl;
    //        //++i;
    //    } while (flag);//следующая комбинация - лексикографическая перестановка

    //    //cout << endl << "Всего: " << i << endl;
    //}
public:
    Bull_Cow_Array(const int& COUNT, string& LIST) :Bull_Cow(COUNT, LIST)//конструктор
    {
        string &symbols = LIST;
        Permutation(COUNT, symbols);
        //Permutation_Ref(COUNT, symbols);
    }
    void SetStep(const string& step, const int& bulls, const int& cows)//заполнение контейнера ответами
    {
        //SimpleTimer st;//запуск таймера
        mStep.code = step;
        mStep.bulls = bulls;
        mStep.cows = cows;
        //array.remove_if([&](string step) {return!(mStep == step); });//удаление элемента по условию лямбды функции для контейнера list

        for (auto it = array.cbegin(); it != array.cend();)
            if (!(mStep == *it))
                it = array.erase(it);//list
            else 
                ++it;//удаление элемента для set || list

        //for (auto it = array.cbegin(), it_prev = array.cbefore_begin(); it != array.cend();)
        //    if (!(mStep == *it))
        //        it = array.erase_after(it_prev);//forward_list
        //    else
        //        it_prev = it++;//удаление элемента для forward_list

    }

    //auto operator [](int index)//перегрузка оператора оператора []
    //{
    //    auto it = array.cbegin();
    //    advance(it, index);
    //    return *it;
    //}

    string GetStep()
    {
        auto it = array.cbegin();
        int size = 0; for (const auto& element : array) ++size; advance(it, rand() % size);//случайный элемент, в т.ч. для forward_list
        //advance(it, rand() % array.size());//случайный элемент, кроме forward_list
        return *it;
    }
    friend class Bull_Cow_Random;

};

int main()
{
    setlocale(LC_ALL, "ru");
    string numUser = "012345";
    int count = (int)numUser.size();
    string list = "0123456789ab";
    //string list = "0123456789abcd";
    sort(list.begin(), list.end());
    for (const auto& element : numUser)//проверка символов
        if (list.find(element) == string::npos)
        {
            cout << "Нет символа: " << element << endl;
            return 0;
        }

    int b = 0, c = 0, i = 0;
    string x;
    Count_BC cbc;

    cout << "n = " << list.size() << " (" << list << ")" << endl;
    cout << "k = " << count << endl;
    cout << "A (" << count << "/" << list.size() << ") = " << Placements(count, list.size()) << endl << endl;
    cout << "N" << "\t" << numUser << "\tB C" << "\tTime, s" << endl << endl;
    SimpleTimer stO;//запуск таймера

    const float precision = 0.001f;//точность
    chrono::time_point<std::chrono::steady_clock> start, end;
    start = chrono::high_resolution_clock::now();//старт секундомера

    //Bull_Cow bc(count, list);
    Bull_Cow_Random bcRnd(count, list);
    //Bull_Cow_Array bcArr(count, list);
    //bool bool_arr = false;
    do
    {
        //SimpleTimer st;//запуск таймера
        x = bcRnd.GetStep();

        chrono::duration<float> duration = chrono::high_resolution_clock::now() - start;//время работы
        start = chrono::high_resolution_clock::now();//старт секундомера

        //cout << ++i << "\t" << x <<  "\t";
        //cin >> b >> c;
        b = cbc.Bulls(x, numUser);
        c = cbc.Cows(x, numUser);
        cout << ++i << "\t" << x << "\t" << b << " " << c << "\t" << round(duration.count() / precision) * precision << endl;
        //bc.SetStep(x, b, c);
        bcRnd.SetStep(x, b, c);
    } while (i < 6);

    Bull_Cow_Array bcArr(count, list);
    do
    {
        //SimpleTimer st;//запуск таймера
        x = bcArr.GetStep();

        chrono::duration<float> duration = chrono::high_resolution_clock::now() - start;//время работы
        start = chrono::high_resolution_clock::now();//старт секундомера

        //cout << ++i << "\t" << x <<  "\t";
        //cin >> b >> c;
        b = cbc.Bulls(x, numUser);
        c = cbc.Cows(x, numUser);
        cout << ++i << "\t" << x << "\t" << b << " " << c << "\t" << round(duration.count() / precision) * precision << endl;
        //bc.SetStep(x, b, c);
        bcArr.SetStep(x, b, c);
    } while (b < count || c > 0);

    cout << endl << "I Winner!" << (numUser.size() > 7 ? "\t" : "") << "\t";
}

//Bull_Cow_Random::Bull_Cow_Random(const int& COUNT, const string& LIST)
//{
//    srand(time(NULL));
//    count = COUNT;
//    symbols = LIST;
//}

bool Bull_Cow_Random::is_Same(const string& version, const char& x)
{
    for (const auto& symbol : version) if (symbol == x) return true;
    return false;
}

string Bull_Cow_Random::GenerateStep()//генерация хода
{
    char x;
    string step = "";//изначально символов в ходе нет
    for (int i = 0; i < count; i++)
    {
        do x = GenerateSymbol();
        while (is_Same(step, x));
        step.push_back(x);//добавление символа в ход
    }
    return step;
}

void Bull_Cow_Array::Permutation(const int& k, string& symbols)//лексикографическая перестановка
{
    //auto sBegin = symbols.begin() + k;
    string step = "";
    do
    {
        step = symbols.substr(0, k);
        if (is_Ok(step)) array.push_front(step);//добавление комбинации в контейнер list
        //array.insert(symbols.substr(0, k));//добавление комбинации в контейнер set
        std::reverse(symbols.begin() + k, symbols.end());//реверс символов начиная с k-го элемента
    } while (next_permutation(symbols.begin(), symbols.end()));//следующая комбинация - лексикографическая перестановка
}

void Bull_Cow_Array::Permutation_Ref(const int& rk, string& rnumber)
{
    string perm = (rnumber).substr(0, rk);
    string& rperm = perm;
    int j = 0; int& rj = j;
    for (; !(rperm).empty();)
    {
        array.push_back(rperm);
        bool flag = false; bool& rflag = flag;
        //********** начало ******************************************
        int n = rnumber.length(); int& rn = n;
        int i = rk; int& ri = i;
        for (; ri < rn && rnumber[ri] < rnumber[rk - 1];) ++ri;

        if (ri < rn) std::swap(rnumber[ri], rnumber[rk - 1]);
        else
        {
            std::reverse(rnumber.begin() + rk, rnumber.end());
            rj = rk - 2;
            for (; rj >= 0 && rnumber[rj] > rnumber[rj + 1];) --rj;

            if (rj < 0)
            {
                rperm = "";
                rflag = true;
            }
            else
            {
                ri = rn - 1;
                for (; ri > rj;)
                {
                    if (rnumber[ri] > rnumber[rj]) break;
                    --ri;
                }
                std::swap(rnumber[ri], rnumber[rj]);
                std::reverse(rnumber.begin() + rj + 1, rnumber.end());
            }
        }
        if (!rflag) rperm = rnumber.substr(0, rk);
        else rflag = false;
    }
}