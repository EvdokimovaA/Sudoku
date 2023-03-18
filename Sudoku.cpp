#include <iostream>
#include <windows.h>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>

using namespace std;

void advice(int** box, int n, int** solvetable);
void statistics(int tips, bool finality, int n, int lvl);
void printbox(int** box, int** firstbox, int n, int* ch_symb);
bool checkequal(int** box, int x, int y, int n, int k);
int OneSymbTranslat(char symb);
int SimpleCheckSIn();
int CheckCin(int* ch_symb, int N);
void translattoint(int N, char** boxchar, int** box);
bool isemptyel(int** box, int n, int& i, int& j);
int findemptycnt(int** box, int n);
void fillsolvet(int** box, int n, int** solvetable);
bool solvesudoku19(int** box, int n);
bool solvesudoku91(int** box, int n);
int countsolves(int** box, int n, int** solvetable);
int getchoice(int max);
int entry(int* ch_size, int* ch_lvl, int* ch_symb, int* ch_file);
int** zero(int n, int** box, int* ch_symb, int* ch_lvl);
void exchbig3vert(int** box, int n);
void exchbig3hor(int** box, int n);
void exchrowsin3(int** box, int n);
void exchcolsin3(int** box, int n);
void generatebox(int n, int** box, int* ch_symb, int* ch_lvl);
void put(int** box, int** firstbox, int n, int* ch_symb, int* ch_temp, int** solvetable);
void remove(int** box, int** firstbox, int n, int* ch_symb, int* ch_temp, int** solvetable);
bool iswin(int** box, int n);
void menu(int** box, int** firstbox, int n, int* ch_symb, int* ch_temp, int** solvetable, int* tips, bool* win);
int checkequalstr(int** box, int x, int y, int n, vector<vector<int>>* conflict);
int checkequalcol(int** box, int x, int y, int n, vector<vector<int>>* conflict);
int checkequalbox(int** box, int x, int y, int n, vector<vector<int>>* conflict);


int main()
{
    setlocale(LC_ALL, "rus");
    int ch_size, ch_lvl, ch_symb, ch_file, tips = 0, ch_temp = 1;
    int work = 2;
    bool win;
    // пока пользователь желает играть
    while (work == 2)
    {
        // вызываем начальное меню, считываем характеристики поля
        entry(&ch_size, &ch_lvl, &ch_symb, &ch_file);
        // инициализируем n - размер блока поля, N - размер поля
        int n = ch_size + 1, N = n * n;
        // выделяем память под матрицу для поля
        int** box = new int* [N];
        for (int i = 0; i < N; i++)
        {
            box[i] = new int[N];
        }
        // если пользователь желает сгенерировать матрицу, вызываем функцию генерации
        if (ch_file == 1)
        {
            system("cls");
            generatebox(n, box, &ch_symb, &ch_lvl);

        }
        // если вводит матрицу самостоятельно
        else
        {
            // если вводит из файла и символы - цифры
            if (ch_symb == 1)
            {
                // открывает файл на чтение
                ifstream file("matrix.txt");
                if (!file.is_open())
                {
                    cout << "\tФайл не открыт!" << endl;
                    return -1;
                }

                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        // считываем элементы и проверяем их на корректность
                        file >> box[i][j];
                        if (file.fail() || box[i][j] < 0)
                        {
                            cout << "\tВ файле присутствуют некорректные данные, исправьте их и запустите программу ещё раз." << endl;
                            return 0;
                        }
                    }
                }
                // зарываем файл
                file.close();
            }
            // если вводит из файла и символы - буквы
            else
            {
                // открываем файл на чтение
                ifstream filech("matrix.txt");
                if (!filech.is_open())
                {
                    cout << "\tФайл не открыт!" << endl;
                    return -1;
                }
                // выделяем память под массив символов
                char** boxchar = new char* [N];
                for (int i = 0; i < N; i++)
                {
                    boxchar[i] = new char[N];
                }
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        // считываем значения в массив boxchar и проверяем их на корректность
                        filech >> boxchar[i][j];
                        if (filech.fail() || boxchar[i][j] < 0 || (OneSymbTranslat(boxchar[i][j]) < 0 || OneSymbTranslat(boxchar[i][j]) > N))
                        {
                            cout << "\tВ файле присутствуют некорректные данные, исправьте их и запустите программу ещё раз." << endl;
                            return 0;
                        }
                    }
                }
                // закрываем файл
                filech.close();
                // переводим массив символов в массив чисел
                translattoint(N, boxchar, box);
                // освобождаем память
                delete[] boxchar;
            }
        }
        // выделяем память под массив изначального поля
        int** firstbox = new int* [N];
        for (int i = 0; i < N; i++)
        {
            firstbox[i] = new int[N];
        }
        // заполняем массив изначального поля
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                firstbox[i][j] = box[i][j];
            }
        }

        /*Создание массива решений solvetable*/
        int emptyel = findemptycnt(box, n);
        int** solvetable = new int* [4];
        for (int i = 0; i < 4; i++) {
            solvetable[i] = new int[emptyel];
        }

        do
        {
            // вызываем меню навигации по действиям в игре
            menu(box, firstbox, n, &ch_symb, &ch_temp, solvetable, &tips, &win);
            system("cls");
            // если пользователь заполнил судоку верно
            if (win)
            {
                // печатаем поле
                cout << endl;
                printbox(box, firstbox, n, &ch_symb);
                // выводим сообщение о победе зеленого цвета
                HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(handle, 10);
                cout << "\n\tПоздравляем! Вы решили судоку" << endl;
                SetConsoleTextAttribute(handle, 15);
                cout << "\n\tХотите выйти?" << endl;
            }
            // предлагаем выйти / начать заново / вернуться в игру
            else cout << "\n\n\n\tУверены, что хотите выйти?" << endl;
            cout << "\n\t1 - Да" << endl;
            cout << "\n\t2 - Нет, начать заново" << endl;
            if (!win) cout << "\n\t3 - Вернуться обратно" << endl;
            work = SimpleCheckSIn();
            while (work < 1 || work > 3)
            {
                cout << "\n\tВы ввели неверное значение.";
                work = SimpleCheckSIn();
            }
        } while (work == 3);
        // записываем статистику партии в файл
        statistics(tips, win, ch_size + 1, ch_lvl);
        cout << "\n\n\n\tСпасибо за игру! Статистика находится в файле stat.txt.\n";
        Sleep(3000);
    }
    return 0;
}

// Записывает статистику в файл stat.txt после каждой партии
void statistics(int tips, bool finality, int n, int lvl)
{
    // открываем файл в режиме дозаписи
    ofstream file("stat.txt", ios_base::out | ios_base::app);
    if (!file.is_open())
    {
        cout << "Файл не открыт!" << endl;
        Sleep(2000);
    }

    // запоминаем текущие дату и время
    char buffer[80];
    time_t seconds = time(NULL);
    tm timeinfo;
    localtime_s(&timeinfo, &seconds);
    const char* format = "%B %d, %Y %H:%M:%S";
    strftime(buffer, 80, format, &timeinfo);

    // инициализируем fin в зависимости от finality
    string fin;
    if (finality) fin = "Да"; else fin = "Нет";

    // записываем в файл данные
    file << "\n----------------------------------------------------\n";
    file << "\tДата партии: " << buffer << endl;
    file << "\tРазмер поля: " << n << "x" << n << endl;
    file << "\tСложность: " << lvl << endl;
    file << "\tКоличество использованных подсказок: " << tips << endl;
    file << "\tБыла ли партия завершена: " << fin;
    file << "\n----------------------------------------------------";

    // закрываем файл
    file.close();
}

//Проверка на совпадение цифр в строке
int checkequalstr(int** box, int x, int y, int n, vector<vector<int>>* conflict)
{
    vector<int> chelp; // вспомогательный вектор для добавления информации в вектор conflict
    if (box[y][x] != 0)
    {
        for (int j = 0; j < n * n; j++)
        {
            if (box[y][x] == box[y][j] && j != x) // при совпадении элементов в строке
            {
                chelp.push_back(y);// добавляем координаты во вспомогательный вектор
                chelp.push_back(x);
                conflict->push_back(chelp);// добавляем вспомогательный вектор в conflict
                chelp.clear(); // очищаем вспомогательный вектор
                chelp.push_back(y);// добавляем второй совпавший элемент
                chelp.push_back(j);
                conflict->push_back(chelp);
                chelp.clear();// очищаем
                return 1;
            }
        }
    }
    return 0;
}

// Проверка на совпадение цифр в столбце
int checkequalcol(int** box, int x, int y, int n, vector<vector<int>>* conflict)
{
    vector<int> chelp;// вспомогательный вектор для добавления информации в вектор conflict
    if (box[y][x] != 0)
    {
        for (int i = 0; i < n * n; i++)
        {
            if (box[y][x] == box[i][x] && i != y)// при совпадении элементов в столбце
            {
                chelp.push_back(y);// добавляем информацию о первом элементе
                chelp.push_back(x);
                conflict->push_back(chelp);
                chelp.clear();
                chelp.push_back(i);// добавляем информацию о втором элементе
                chelp.push_back(x);
                conflict->push_back(chelp);
                chelp.clear();
                return 1;
            }
        }
    }
    return 0;
}

// Проверка на совпадение элементов внутри одного блока n*n
int checkequalbox(int** box, int x, int y, int n, vector<vector<int>>* conflict)
{
    vector<int> chelp;
    int k = x / n;
    int p = y / n;
    int X = k * n;// определяем минимальное значение по иксу в блоке
    int Y = p * n;// определяем минимальное значение по игреку в блоке
    if (box[y][x] != 0)
    {
        for (int i = Y; i < n + Y; i++)
        {
            for (int j = X; j < n + X; j++)
            {
                if (box[i][j] == box[y][x] && (i != y || j != x))
                {
                    chelp.push_back(i);// добавляем информацию о первом элементе
                    chelp.push_back(j);
                    conflict->push_back(chelp);
                    chelp.clear();
                    chelp.push_back(y);// добавляем информацию о втором элементе
                    chelp.push_back(x);
                    conflict->push_back(chelp);
                    chelp.clear();
                    return 1;
                }
            }
        }
    }
    return 0;
}


// Печатает матрицу поля в виде судоку, раскрашивая при этом конфликтующие элементы и поставленные изначально
void printbox(int** box, int** firstbox, int n, int* ch_symb)
{
    int N = n * n;
    // вектор, хранящий координаты конфликтующих элементов
    vector<vector<int>> conflict;
    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            // проверяем конфликты в строке с элементом
            int str = checkequalstr(box, x, y, n, &conflict);
            // проверяем конфликты в столбце с элементом
            int colmn = checkequalcol(box, x, y, n, &conflict);
            // проверяем конфликты в блоке с элементом
            int block = checkequalbox(box, x, y, n, &conflict);
        }
    }
    // извлечение дескриптора устройства стандартного вывода
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int row = 0; row < N; row++)
    {
        cout << "\t";
        for (int col = 0; col < N; col++)
        {
            // выводим вертикальную линию
            if (col % n == 0 && col != 0)
                cout << "| ";
            // делаем символ серым, если он был поставлен изначально
            if (firstbox[row][col] != 0) SetConsoleTextAttribute(handle, 7);
            for (int c = 0; c < conflict.size(); c++)
            {
                if (row == conflict[c][0] && col == conflict[c][1])
                {
                    // делаем символ красным, если он конфликтующий и был поставлен изначально
                    if (firstbox[row][col] != 0) SetConsoleTextAttribute(handle, 4);
                    // делаем символ светло-красным, если он конфликтующий и был поставлен пользователем
                    else SetConsoleTextAttribute(handle, 12);
                    break;
                }
            }
            // если пользователь играет буквами и на ячейке стоит символ
            if (*ch_symb == 2 && box[row][col] != 0)
            {
                cout.setf(ios::left);
                cout.width(2);
                // переводим этот символ в букву по коду ASCII и выводим
                cout << static_cast<char>(box[row][col] + 64);
            }
            else {
                cout.setf(ios::left);
                cout.width(2);
                // иначе просто выводим эту цифру
                cout << box[row][col];
            }
            cout << " ";
            // возвращаем символам стандартный белый цвет
            SetConsoleTextAttribute(handle, 15);
        }
        // выводим горизонтальную линию
        if ((row + 1) % n == 0 && row != N - 1)
        {
            cout << endl << "\t";
            for (int i = 0; i < N + n - 1; i++)
            {
                cout.setf(ios::left);
                cout.width(2);
                cout << "---";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Объединяет все функции проверки совпадения, проверяет корректность постановки элементов на поле
bool checkequal(int** box, int x, int y, int n, int k)
{
    vector<vector<int>> conflict;
    box[y][x] = k;
    int str = checkequalstr(box, x, y, n, &conflict);// если есть совпадение по строкам str = 1
    int col = checkequalcol(box, x, y, n, &conflict);//если есть совпадение по столбцам col = 1
    int block = checkequalbox(box, x, y, n, &conflict);// если есть совпадение по блокам block = 1
    box[y][x] = 0;
    return str + col + block != 0;
}

// Переводит массив символов в масссив int
void translattoint(int N, char** boxchar, int** box)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (boxchar[i][j] == '0') box[i][j] = 0;
            else box[i][j] = static_cast<int>(boxchar[i][j]) - 64;
        }
    }
}

// Переводит один символ в int, чтобы проверять корректность считывания поля из файла
int OneSymbTranslat(char symb)
{
    int sym;
    if (symb == '0') sym = 0;// ноль равняется нулю и в int, и в char
    else sym = static_cast<int>(symb) - 64;
    return sym;
}


int SimpleCheckSIn()
{
    while (true)
    {
        int n;
        cin >> n;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(32767, '\n');
        }
        else
        {
            cin.ignore(32767, '\n');
            return n;
        }
    }
}

// Проверяет корректность постановки на поле символов, букв и номеров ячеек, в которые производится постановка
int CheckCin(int* ch_symb, int N)
{
    if (*ch_symb == 1)
    {
        while (true)
        { // цикл продолжается до тех пор, пока пользователь не введет корректное значение
            int n;
            cin >> n;
            if (cin.fail() || n < 1 || n > N)
            {
                cin.clear();
                cin.ignore(32767, '\n');
                cout << "\tНеверно введены данные. Попробуйте ещё раз." << endl;
            }
            else
            {
                cin.ignore(32767, '\n');
                return n;
            }
        }
    }
    else
    {
        while (true)
        { // цикл продолжается до тех пор, пока пользователь не введет корректное значение
            int sym;
            char symb;
            cin >> symb;
            if (symb == '0')
            {
                sym = 0;
            }
            else sym = static_cast<int>(symb) - 64;// перевод char в int

            if (cin.fail() || sym < 1 || sym > N)// проверка ввода уже на цифрах
            {
                cin.clear();
                cin.ignore(32767, '\n');
                cout << "\tНеверно введены данные. Попробуйте ещё раз." << endl;
            }
            else
            {
                cin.ignore(32767, '\n');
                return sym;
            }
        }
    }
}

/*Функция isemptyel проверяет, остались ли в таблице судоку незаполненные ячейки*/
bool isemptyel(int** box, int n, int& i, int& j)
{
    for (i = 0; i < n * n; i++)
        for (j = 0; j < n * n; j++)
            if (box[i][j] == 0) return true;
    return false;
}

/*Функция findemptycnt cчитает количество пустых ячеек в таблице судоку*/
int findemptycnt(int** box, int n)
{
    int i, j;
    int empt = 0;
    for (i = 0; i < n * n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            if (box[i][j] == 0)
            {
                empt++;
            }
        }
    }
    return empt;
}

/*Функция fillsolvet запоняет первые 2 строки массива решений solvetable,
содержащие координаты незаполненных ячеек (в строке с индексом 0 хранятся номера строк
пустых ячеек, а в строке с индексом 1 - соответствующие номера столбцов)*/
void fillsolvet(int** box, int n, int** solvetable)
{
    int i, j, k = 0;
    for (i = 0; i < n * n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            if (box[i][j] == 0)
            {
                solvetable[0][k] = i;
                solvetable[1][k] = j;
                k++;
            }
        }
    }
}

/*Рекурсивная функция solvesudoku19 решает судоку, пытаясь подставить
в незаполненные ячейки значения от 1 до n*n, если это возможно*/
bool solvesudoku19(int** box, int n)
{
    int i, j;
    /*Если свободных ячеек не осталось, функция завершает свою работу*/
    if (!isemptyel(box, n, i, j)) return true;
    for (int k = 1; k <= n * n; k++)
    {
        /*Если при подстановке числа в ячейку выполняются правила судоку, программа
        ставит это число на поле и рекурсивно решает судоку уже с получившейся расстановкой элементов*/
        if (!checkequal(box, j, i, n, k))
        {
            box[i][j] = k;
            if (solvesudoku19(box, n)) return true;
            box[i][j] = 0;
        }
    }
    return false;
}


/*Рекурсивная функция solvesudoku91 решает судоку, пытаясь подставить
в незаполненные ячейки значения от n*n до 1, если это возможно*/
bool solvesudoku91(int** box, int n)
{
    int i, j;
    /*Если свободных ячеек не осталось, функция завершает свою работу*/
    if (!isemptyel(box, n, i, j)) return true;
    for (int k = n * n; k >= 1; k--)
    {
        /*Если при подстановке числа в ячейку выполняются правила судоку, программа
        ставит это число на поле и рекурсивно решает судоку уже с получившейся расстановкой элементов*/
        if (!checkequal(box, j, i, n, k)) {
            box[i][j] = k;
            if (solvesudoku91(box, n)) return true;
            box[i][j] = 0;
        }
    }
    return false;
}


/*Функция countsolves заполняет массив решений solvetable
и возвращает количество решений судоку*/
int countsolves(int** box, int n, int** solvetable)
{
    int i, j, k;
    int cntsame = 0;
    /*Создание и заполнение массива firstbox, в котором хранится
    переданный в функцию массив, в который ещё не внесены изменения*/
    int** firstbox = new int* [n * n];
    for (i = 0; i < n * n; i++)
    {
        firstbox[i] = new int[n * n];
    }
    for (i = 0; i < n * n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            firstbox[i][j] = box[i][j];
        }
    }

    /*Обнуление строк с индексами 2 и 3 массива решений solvetable, в которых
    будут храниться значения решённого судоку, соответствующие изначально пустым ячейкам*/
    int emptyel = findemptycnt(box, n);
    fillsolvet(box, n, solvetable);
    for (i = 0; i < emptyel; i++) {
        solvetable[2][i] = 0;
        solvetable[3][i] = 0;
    }

    /*Заполнение строки с индексом 2 массива решений solvetable,
если судоку решено с помощью функции solvesudoku19. В противном случае,
если функция не решила судоку, возвращается индикатор того, что у данного судоку нет решения*/
    if (solvesudoku19(box, n))
    {
        for (i = 0; i < n * n; i++)
        {
            for (j = 0; j < n * n; j++)
            {
                for (k = 0; k < emptyel; k++)
                {
                    if ((solvetable[0][k] == i) && (solvetable[1][k] == j))
                    {
                        solvetable[2][k] = box[i][j];
                    }
                }
            }
        }
    }
    else { return 0; }
    /*Возврат таблицы судоку к исходному - нерешённому виду,
    чтобы попробовать решить её другим способом*/
    for (i = 0; i < n * n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            box[i][j] = firstbox[i][j];
        }
    }
    /*Заполнение строки с индексом 3 массива решений solvetable,
    если судоку решено с помощью функции solvesudoku91. В противном случае,
    если функция не решила судоку, возвращается индикатор того, что у данного судоку нет решения*/
    if (solvesudoku91(box, n))
    {
        for (i = 0; i < n * n; i++)
        {
            for (j = 0; j < n * n; j++)
            {
                for (k = 0; k < emptyel; k++)
                {
                    if ((solvetable[0][k] == i) && (solvetable[1][k] == j))
                    {
                        solvetable[3][k] = box[i][j];
                    }
                }
            }
        }
    }
    else { return 0; }

    /*Возврат таблицы судоку к исходному - нерешённому виду*/
    for (i = 0; i < n * n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            box[i][j] = firstbox[i][j];
        }
    }
    /*Сравнение двух решений, получившихся разными способами.
Если они совпадают, значит у данного судоку единственное решение,
если не совпадают, значит судоку имеет неколько (2 или больше) решений*/
    for (j = 0; j < emptyel; j++)
    {
        if ((solvetable[2][j] == solvetable[3][j]) && (solvetable[2][j] != 0))  cntsame++;
    }
    if (cntsame == emptyel) return 1;
    else return 2;
}



// Проверяет на корректность введенное значение и сравнивает его с максимальным возможным
int getchoice(int max)
{
    string temp;
    // считываем значение
    getline(cin, temp);
    unsigned len = temp.length();
    // если длина строки пустая, возвращаем EOF
    if (len == 0) return EOF;
    const char* num = "123456789";
    for (int i = 0; i < len; i++) {
        // если цифры нет в num или это пробел, возвращаем EOF
        if (!strchr(num, temp[i]) or temp[i] == ' ') return EOF;
    }
    // если введенное значение больше максимального, возвращаем EOF
    if (stoi(temp) > max) return EOF;
    // если число удовлетворяет всем критериям, возвращаем его в типе int
    else return stoi(temp);
}

// Выводит начальное меню игры, считывает характеристики поля
int entry(int* ch_size, int* ch_lvl, int* ch_symb, int* ch_file)
{
    system("cls");
    cout << "\n\tДобро пожаловать в судоку!\n" << endl;
    cout << "\tВыберите размер одного блока поля для игры:\n" << endl;
    cout << "\t\t1 - 2x2" << endl;
    cout << "\t\t2 - 3x3" << endl;
    cout << "\t\t3 - 4x4" << endl;
    cout << "\t\t4 - 5x5" << endl;
    // считывание ch_size - размера матрицы
    *ch_size = getchoice(4);
    while (*ch_size == EOF)
    {
        cout << "\n\n\n";
        cout << "\tНеверный ввод! Выберите размер одного блока поля для игры:" << endl;
        *ch_size = getchoice(4);
    }

    system("cls");
    cout << "\n\n\n";
    cout << "\tКакими символами хотите играть на поле?\n" << endl;
    cout << "\t\t1 - Цифрами" << endl;
    cout << "\t\t2 - Буквами" << endl;
    // считывание ch_symb - формата символов
    *ch_symb = getchoice(2);
    while (*ch_symb == EOF)
    {
        cout << "\n\n\n";
        cout << "\tНеверный ввод! Какими символами хотите играть на поле?" << endl;
        *ch_symb = getchoice(2);
    }

    system("cls");
    cout << "\n\n\n";
    cout << "\tХотите, чтобы программа сгенерировала начальное поле, или введёте поле сами из файла?\n" << endl;
    cout << "\t\t1 - Программа" << endl;
    cout << "\t\t2 - Самостоятельный ввод" << endl;
    // считывание ch_file - выбора задания матрицы
    *ch_file = getchoice(2);
    while (*ch_file == EOF)
    {
        cout << "\n\n\n";
        cout << "\t\nНеверный ввод! Введите правильные данные:" << endl;
        *ch_file = getchoice(2);
    }
    // если пользователь хочет сам ввести матрицу - выходим
    if (*ch_file == 2)
        return 0;

    system("cls");
    cout << "\n\n\n";
    cout << "\tВыберите уровень сложности:\n" << endl;
    cout << "\t\t1 - Легкий" << endl;
    cout << "\t\t2 - Средний" << endl;
    cout << "\t\t3 - Сложный" << endl;
    cout << "\t\t4 - Экспертный" << endl;
    cout << "\t\t5 - Безумный" << endl;
    // считываем ch_lvl - уровень сложности
    *ch_lvl = getchoice(5);
    while (*ch_lvl == EOF)
    {
        cout << "\n\n\n";
        cout << "\t\nНеверный ввод! Выберите уровень сложности:" << endl;
        *ch_lvl = getchoice(5);
    }
    return 0;
}

// Убирает некоторое количество элементов с поля при генерации
int** zero(int n, int** box, int* ch_symb, int* ch_lvl)
{
    srand(time(NULL));
    int N = n * n * n * n;// количество всех ячеек
    int del;
    /* В зависимости от размера поля и уровня сложности определяем количество элементов,
       которые стоит удалить*/
    if (*ch_lvl == 1)
    {
        if (n == 4) del = N - (N * 82 / 100);
        else if (n == 5) del = N - (N * 88 / 100);
        else del = N - (N * 48 / 100);
    }
    else if (*ch_lvl == 2)
    {
        if (n == 4) del = N - (N * 78 / 100);
        else if (n == 5) del = N - (N * 86 / 100);
        else del = N - (N * 43 / 100);
    }
    else if (*ch_lvl == 3)
    {
        if (n == 4) del = N - (N * 74 / 100);
        else if (n == 5) del = N - (N * 84 / 100);
        else del = N - (N * 38 / 100);
    }
    else if (*ch_lvl == 4)
    {
        if (n == 4) del = N - (N * 68 / 100);
        else if (n == 5) del = N - (N * 82 / 100);
        else del = N - (N * 33 / 100);
    }
    else
    {
        if (n == 4) del = N - (N * 64 / 100);
        else if (n == 5) del = N - (N * 80 / 100);
        else del = N - (N * 28 / 100);
    }

    /*Рандомно выбираем координаты ячеек, которые хотим обнулить*/
    int x, y;
    while (del > 0)
    {
        x = 0 + rand() % (n * n);
        y = 0 + rand() % (n * n);
        /*Если в выбранной ячейке стоит 0, перевыбираем ячейку до тех пор, пока не найдём ещё заполненную*/
        while (box[y][x] == 0)
        {
            x = 0 + rand() % (n * n);
            y = 0 + rand() % (n * n);
        }
        box[y][x] = 0;
        del--;// уменьшаем количество элементов, котрые надо удалить
    }

    return box;
}

/*Функция exchbig3vert меняет местами 2 больших
вертикальных блока таблицы судоку*/
void exchbig3vert(int** box, int n)
{
    srand(time(NULL));
    int randres1, randres2;
    int b1, b2;
    int i, j;
    /*Случайный выбор двух больших вертикальных блоков,
    которые будут меняться местами (отдельно рассмотрен
    случай для случая матрицы 4х4 (n=2))*/
    if (n == 2)
    {
        randres1 = 1;
        randres2 = 2;
    }
    else
    {
        do
        {
            randres1 = 1 + rand() % n;
            randres2 = 1 + rand() % n;
        } while (randres1 == randres2);
    }
    /*Нахождение границ больших вертикальных блоков*/
    b1 = (randres1 - 1) * n;
    b2 = (randres2 - 1) * n;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            swap(box[j][b1], box[j][b2]);
        }
        b1++;
        b2++;
    }
}

/*Функция exchbig3hor меняет местами 2 больших
горизонтальных блока таблицы судоку*/
void exchbig3hor(int** box, int n)
{
    srand(time(NULL));
    int randres1, randres2;
    int b1, b2;
    int i, j;
    /*Случайный выбор двух больших горизонтальных блоков,
    которые будут меняться местами (отдельно рассмотрен
    случай для случая матрицы 4х4 (у неё n=2))*/
    if (n == 2)
    {
        randres1 = 1;
        randres2 = 2;
    }
    else
    {
        do
        {
            randres1 = 1 + rand() % n;
            randres2 = 1 + rand() % n;
        } while (randres1 == randres2);
    }
    /*Нахождение границ больших горизонтальных блоков*/
    b1 = (randres1 - 1) * n;
    b2 = (randres2 - 1) * n;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            swap(box[b1][j], box[b2][j]);
        }
        b1++;
        b2++;
    }
}

// Меняет местами радомно выбранные строки внутри блока
void exchrowsin3(int** box, int n)
{
    srand(time(NULL));
    vector <int> range;
    vector <int> result;
    for (int i = 0; i < n * n; i++)
    {
        range.push_back(i);
    }
    int chosestr1 = 0 + rand() % (n * n - 1); // рандомный выбор 1 строки
    int x1 = chosestr1 / n;
    int X = x1 * n; // определяем нижнюю границу блока
    int x2 = X + n - 1;

    for (int i = 0; i < n * n; i++)// добавляем в вектор все возможные номера строк, которые можно выбрать при втором рандоме
    {
        if ((range[i] >= X && range[i] <= x2) && i != chosestr1)
        {
            result.push_back(range[i]);
        }
    }
    range.clear();
    int chose;
    int chosestr2;
    if (n == 2)
    {
        chosestr2 = result[0];
    }
    else
    {
        chose = 0 + rand() % (result.size() - 1);
        chosestr2 = result[chose]; //рандомный выбор 2 строки
    }
    for (int i = 0; i < n * n; i++)
    {
        swap(box[chosestr1][i], box[chosestr2][i]);
    }
}

// Меняет местами радомно выбранные столбцы внутри блока
void exchcolsin3(int** box, int n)
{
    srand(time(NULL));
    vector <int> range;
    vector <int> result;
    for (int i = 0; i < n * n; i++)
    {
        range.push_back(i);
    }
    int chosecol1 = 0 + rand() % (n * n - 1); // рандомный выбор 1 столбца
    int x1 = chosecol1 / n;
    int X = x1 * n; // определяем нижнюю границу блока
    int x2 = X + n - 1;

    for (int i = 0; i < n * n; i++)// добавляем в вектор все возможные номера строк, которые можно выбрать при втором рандоме
    {
        if ((range[i] >= X && range[i] <= x2) && i != chosecol1)
        {
            result.push_back(range[i]);
        }
    }
    range.clear();
    int chose;
    int chosecol2;
    if (n == 2)
    {
        chosecol2 = result[0];
    }
    else
    {
        chose = 0 + rand() % (result.size() - 1);
        chosecol2 = result[chose]; //рандомный выбор 2 строки
    }
    for (int i = 0; i < n * n; i++)
    {
        swap(box[chosecol1][i], box[chosecol2][i]);
    }
}

void generatebox(int n, int** box, int* ch_symb, int* ch_lvl)
{
    /*Формирование базовой таблицы судоку*/
    srand(time(NULL));
    int i, j;
    for (i = 0; i < n * n; i++)
    {
        for (j = 0; j < n * n; j++)
        {
            box[i][j] = (i * n + i / n + j) % (n * n) + 1;
        }
    }
    int random2 = 3 + rand() % 5;

    /*"Перемешивание" изначальной таблицы судоку различными методами,
    не влияющими на удовлетворение этой таблицы правилам игры*/
    while (random2 > 0)
    {
        exchrowsin3(box, n);
        exchcolsin3(box, n);
        exchbig3hor(box, n);
        exchbig3vert(box, n);
        exchcolsin3(box, n);
        exchbig3vert(box, n);
        exchbig3hor(box, n);
        exchrowsin3(box, n);
        exchbig3vert(box, n);
        random2--;
    }
    zero(n, box, ch_symb, ch_lvl);
}

// Дает пользователю возможность поставить символ на поле
void put(int** box, int** firstbox, int n, int* ch_symb, int* ch_temp, int** solvetable)
{
    // считываем строку ячейки
    int row, col, value, N = n * n;
    cout << "\n\tВведите строку ячейки, на которую хотите поставить элемент: ";
    row = CheckCin(ch_temp, N);
    row--;

    // считываем столбец ячейки
    cout << "\n\tВведите столбец ячейки, на которую хотите поставить элемент: ";
    col = CheckCin(ch_temp, N);
    col--;

    // считываем значение ячейки
    cout << "\n\tВведите значение элемента, который хотите поставить: ";
    value = CheckCin(ch_symb, N);
    // пока ячейка на поле не пустая, повторяем манипуляции
    while (firstbox[row][col] != 0)
    {
        cout << "\n\tНа выбранную ячейку нельзя поставить значение. Введите другую.";
        Sleep(3000);
        system("cls");
        cout << endl;
        printbox(box, firstbox, n, ch_symb);
        cout << "\n\tВведите строку ячейки, на которую хотите поставить элемент: ";
        row = CheckCin(ch_temp, N);
        row--;

        cout << "\n\tВведите столбец ячейки, на которую хотите поставить элемент: ";
        col = CheckCin(ch_temp, N);
        col--;

        cout << "\n\tВведите значение элемента, который хотите поставить: ";
        value = CheckCin(ch_symb, N);
    }

    // ставим значение в матрицу
    box[row][col] = value;
}

// Дает пользователю возможность удалить символ на поле
void remove(int** box, int** firstbox, int n, int* ch_symb, int* ch_temp, int** solvetable)
{
    int row, col, temp, N = n * n, c = 0;
    int i, j;
    // подсчет изначальных символов на поле
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (box[i][j] == firstbox[i][j]) c++;
        }
    }
    // если изначальные символы в каждой ячейке поля, удаление невозможно
    if (c == N * N) {
        cout << "\n\tУдаление элементов в иcходной матрице невозможно" << endl;
        Sleep(3000);
        return;
    }
    // считываем строку ячейки
    cout << "\n\tВведите строку элемента, который хотите удалить: ";
    row = CheckCin(ch_temp, N);
    row--;

    // считываем столбец ячейки
    cout << "\n\tВведите столбец элемента, который хотите удалить: ";
    col = CheckCin(ch_temp, N);
    col--;
    // запоминаем элемент на ячейке, удаляем его
    temp = box[row][col];
    box[row][col] = 0;
    // пока на месте введенной ячейки стоит начальный элемент, повторяем манипуляции
    while (firstbox[row][col] != 0)
    {
        // ставим удаленный элемент обратно
        box[row][col] = temp;
        cout << "\n\tВведенный элемент нельзя удалить. Введите другой.";
        Sleep(3000);
        system("cls");
        cout << endl;
        printbox(box, firstbox, n, ch_symb);
        cout << "\n\tВведите строку элемента, который хотите удалить: ";
        row = CheckCin(ch_temp, N);
        row--;

        cout << "\n\tВведите столбец элемента, который хотите удалить: ";
        col = CheckCin(ch_temp, N);
        col--;

        temp = box[row][col];
        box[row][col] = 0;
    }
}

// Возвращает true если пользователь заполнил поле правильно, иначе - false
bool iswin(int** box, int n) 
{
    vector<vector<int>> conflict;
    for (int y = 0; y < n * n; y++) 
    {
        for (int x = 0; x < n * n; x++) 
        {
            // проверяем конфликты в строке с элементом
            int str = checkequalstr(box, x, y, n, &conflict);
            // проверяем конфликты в столбце с элементом
            int col = checkequalcol(box, x, y, n, &conflict);
            // проверяем конфликты в блоке с элементом
            int block = checkequalbox(box, x, y, n, &conflict);
            // если есть конфликт, возвращаем false
            if (str + col + block != 0) return false;
        }
    }
    // если не осталось пустых ячеек, возвращаем true
    return findemptycnt(box, n) == 0;
}


/*С помощью функции menu осуществляется навигация по действиям, которые можно выполнять в программе*/
void menu(int** box, int** firstbox, int n, int* ch_symb, int* ch_temp, int** solvetable, int* tips, bool* win)
{
    int choice, res, tip = 0;
    /*Заполнение первых 2 строк массива решений solvetable, содержащих координаты пустых ячеек */
    fillsolvet(box, n, solvetable);
    do
    {
        system("cls");
        cout << endl;
        /*Вывод исходной матрицы и меню игры*/
        printbox(box, firstbox, n, ch_symb);
        cout << "\tВыберите операцию:" << endl;
        cout << "\n\t1 - Поставить символ  2 - Удалить символ  3 - Подсказка  4 - Проверка наличия решения  0 - Выход"
            << endl;
        choice = SimpleCheckSIn();
        /*При вводе пользователем цифры, которой не соответствует никакое действие из меню,
        программа попросит пользователя ввести корректное значение*/
        while (choice < 0 || choice > 4)
        {
            cout << "\tВы ввели неверное значение. Попробуйте ещё раз" << endl;
            choice = SimpleCheckSIn();
        }
        system("cls");
        /*При выборе пользователем пункта "1 - Поставить символ" программа поставит
        выбранный пользователем символ на определённое пользователем место на поле, если это возможно*/
        if (choice == 1)
        {
            cout << endl;
            printbox(box, firstbox, n, ch_symb);
            put(box, firstbox, n, ch_symb, ch_temp, solvetable);
        }
        /*При выборе пользователем пункта "2 - Удалить символ" программа удалит
        выбранный пользователем символ с определённого пользователем места на поле, если это возможно*/
        else if (choice == 2)
        {
            cout << endl;
            printbox(box, firstbox, n, ch_symb);
            remove(box, firstbox, n, ch_symb, ch_temp, solvetable);
        }
        /*При выборе пользователем пункта "3 - Подсказка" программа покажет ему следующий ход
        и добавит в статистику игры информацию о том, что пользователь взял подсказку*/
        else if (choice == 3) {
            tip++;
            advice(box, n, solvetable);
            Sleep(3000);
        }
        /*При выборе пользователем пункта "4 - Проверка наличия решения" программа покажет,
        есть ли на данном этапе решения судоку и сколько их*/
        else if (choice == 4)
        {
            cout << endl;
            printbox(box, firstbox, n, ch_symb);
            res = countsolves(box, n, solvetable);
            if (res == 1)
            {
                cout << "\n\tСуществует 1 способ решить судоку на данном этапе" << endl;
                Sleep(3000);
            }
            else if (res == 2)
            {
                cout << "\n\tСуществует несколько способов решить судоку на данном этапе" << endl;
                Sleep(3000);
            }
            else if (res == 0)
            {
                cout << "\n\tНе существует способов решить судоку на данном этапе" << endl;
                Sleep(3000);
            }
        }
        /*Программа продолжает свою работу, пока судоку не решено успешно
        либо пока пользователь сам не захочет выйти из игры, выбрав в меню 0 - Выход*/
    } while (choice != 0 && !iswin(box, n));
    *win = iswin(box, n);
    *tips = tip;
}


/* Функция advice подсказывает пользователю следующих ход
и заполняет одну клетку на поле, приближая пользователя к победе */
void advice(int** box, int n, int** solvetable) 
{
    int res;
    res = countsolves(box, n, solvetable);  //В res сохраняется количество решений на данном этапе игры
    /*Если существует одно или несколько решений, программа подскажет  пользователю
    следующий ход из массива, в котором хранится решение(я) судоку на данном этапе*/
    if ((res == 1) || (res == 2)) 
    {
        box[solvetable[0][0]][solvetable[1][0]] = solvetable[2][0];
        cout << "\n\tИщите подсказку в ячейке с координатами: " << solvetable[0][0] + 1 << " " << solvetable[1][0] + 1 << endl;
    }
    /*Если решений не существует, программа не сможет дать подсказку
    и выведет сообщение о том, что при данной расстановке дальнейшее решение невозможно*/
    else if (res == 0) {
        cout << "\n\tПри данной комбинации на поле дальнейшее решение невозможно" << endl;
    }
}
