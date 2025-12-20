  //=======================================================================
//	Лабораторная №3. Шаблоны функций. Шаблоны классов. Стандартные шаблоны С++.
//				Обработка исключений.
//=======================================================================
//Используйте недостающие файлы из лабораторной 2
#include "../../lab2/include/MyVector.h"
#include "../../lab2/include/MyString.h"
#include "../include/func.h"
#include "../include/MyStack.h"

#include <iostream>
#include <vector>
#include <string>
#include <list>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::array;
using std::list;

//============= Шаблон функции для вывода с помощью итератора
template <class T> 
void pr(T& v, string s)
{
	cout<<"\n\n\t"<<s<<"  # Sequence:\n";
	
	// Итератор любого контейнера
	typename T::iterator p; // add typename
	int i;

	for (p = v.begin(), i=0;  p != v.end();  p++, i++)
		cout << endl << i+1 <<". "<< *p;
	cout << '\n';
}

int main()
{

	//===========================================================
	// Шаблоны функций
	//===========================================================
	// Создайте шаблон функции перестановки двух параметров - Swap().
	// Проверьте работоспособность созданного шаблона с помощью
	// приведенного ниже фрагмента.
	{
		int i = 1, j = -1;
		Swap (i, j);

		double a = 0.5, b = -5.5;
		Swap (a, b);

		Vector u(1,2), w(-3,-4);
		Swap(u, w);

		// Если вы достаточно развили класс MyString в предыдущей работе,
		// то следующий фрагмент тоже должен работать корректно.
		
		MyString s1 ("Your fault"), s2 ("My forgiveness");
		Swap (s1, s2);
	}
	//===========================================================
	// Шаблоны классов
	//===========================================================
	// Создайте шаблон класса MyStack для хранения элементов любого типа T.
	// В качестве основы для стека может быть выбран массив.
	// Для задания максимального размера стека может быть использован
	// параметр-константа шаблона
	// Обязательными операциями со стеком являются "Push" и "Pop","GetSize" и "Capacity"
	// Необязательной - может быть выбор по индексу (operator[]).
	// Для того, чтобы гарантировать корректное выполнение этих операций 
	// следует генерировать исключительные ситуации.
	
	// С помощью шаблона MyStack создайте стек переменных типа int, затем
	// стек переменных типа double и, наконец, стек из переменных типа Vector 
	// Если вы подготовите три класса для обработки исключений,
	// то следующий фрагмент должен работать
	try
	{
		cout << "\tTest MyStack\n";
		MyStack <int, 3> stack;

		cout << "\nInteger Stack capacity: " << stack.Capacity();

		stack.Push(1);
		stack.Push(2);
		stack.Push(3);
		
		cout << "\nInteger Stack has: " << stack.GetSize() << " elements";
		try{
			stack.Push(4);	// Здесь должно быть "выброшено" исключение
		}
		catch (const StackOverflow& e){
			cout << e.what() << endl;
		}

		cout << "\nInteger Stack pops: " << stack.Pop();
		cout << "\nInteger Stack pops: " << stack.Pop();
		
		cout << "\nInteger Stack has: " << stack.GetSize() << " elements";
		stack.Pop();
		try {
			stack.Pop();		// Здесь должно быть "выброшено" исключение
		}
		catch (const StackUnderflow& e){
			cout << e.what() << endl;
		}
		stack.Push(2);
		try {
			int i = stack[3];	// Здесь должно быть "выброшено" исключение
		}
		catch (const StackOutOfRange& e) {
			e.Out();
		}
		MyStack<Vector, 5> ptStack;
		
		cout << "\nVector Stack capacity: " << ptStack.Capacity();
		
		ptStack.Push(Vector(1,1));
		ptStack.Push(Vector(2,2));
		
		cout << "\nVector Stack pops: ";
		// Используйте метод класса Vector для вывода элемента
		ptStack.Pop().Out();
		
		cout << "\nVector Stack has: " << ptStack.GetSize() << " elements";
	}
	catch (const StackOverflow& e)
	{
		cout << e.what() << endl;;
	}
	catch (const StackUnderflow& e)
	{
		cout << e.what() << endl;
	}
	catch (const StackOutOfRange& o)
	{
		o.Out();
	}

	//=======================================================================
	// Контейнеры стандартной библиотеки. Последовательности типа vector
	//=======================================================================
	
	// Создайте пустой вектор целых чисел. Узнайте его размер с помощью метода size(),
	// С помощью метода push_back() заполните вектор какими-либо значениями.
	// Получите новый размер вектора и выведите значения его элементов.
	// В процессе работы с вектором вы можете кроме количества реально заполненных
	// элементов (size()) узнать максимально возможное количество элементов (max_size()),
	// а также зарезервированную память (capacity()).

	vector<int> v;
	int n = v.size();
	v.push_back(-1);
	v.push_back(-2);
		v.push_back(-2);
	n = v.size();
	int q = v.capacity();
	cout << "!!!!!" << n << " " << q << endl;
	n = v.max_size();
		
	// Так как мы часто будем выводить последовательности, то целесообразно
	// создать шаблон функции для вывода любого контейнера.
	// Проанализируйте коды такого шабдлона (pr), который приведен выше
	// Используйте его для вывода вашего вектора

	pr (v, "Vector of ints");
	
	// Используем другой конструктор для создания вектора вещественных
	// с начальным размером в 2 элемента и заполнением (222.).
	// Проверим параметры вектора. Затем изменим размер вектора и его заполнение
	// (метод - resize()) и вновь проверим параметры.
	
	vector<double> vd;
	pr (vd, "Vector of doubles");
	n = vd.size();
	n = vd.capacity();
	n = vd.max_size();

	vd.resize(6, 666.6);
	pr (vd, "After resize");
	n = vd.size();
	n = vd.capacity();
	n = vd.max_size();
	
	// Используя метод at(), а также операцию выбора [], измените значения
	// некоторых элементов вектора и проверьте результат.
	vd.at(0) = 333.3; 
    vd[3] = 999.9;  
	pr (vd, "After at");
	
	// Создайте вектор вещественных, который является копией существующего.
	vector<double> wd = vd;
	pr (wd, "Copy");
	
	// Создайте вектор, который копирует часть существующей последовательности
	vector<double> ud(vd.begin() + 2, vd.begin() + 5);
	pr (ud, "Copy part");

	// Создайте вектор вещественных, который является копией части обычного массива.
	double ar[] = { 0., 1., 2., 3., 4., 5. };
	vector<double> va(ar, ar + 3);

	pr (va, "Copy part of array");
	
	// Создайте вектор символов, который является копией части обычной строки
	char s[] = "Array is a succession of chars";
	vector<char> vc(s + 7, s + 13);
	
	//vector<char> vc 
	pr (vc, "Copy part of c-style string");

	// Создайте вектор элементов типа Vector и инициализируйте
	// его вектором с координатами (1,1).
	vector<Vector> vv(7, Vector(1,1));
	
	cout << "\n\nvector of Vectors\n";
	for (size_t i=0;  i < vv.size();  i++)
		vv[i].Out();

	// Создайте вектор указателей на Vector и инициализируйте его адресами
	// объектов класса Vector
	
	vector<Vector*> vp;
	vp.push_back(new Vector(11, 11));
	vp.push_back(new Vector(21, 12));
	vp.push_back(new Vector(33, 33));
	
	cout << "\n\nvector of pointers to Vector\n";
	
	for (size_t i=0;  i < vp.size();  i++)
		vp[i]->Out();

	// Научитесь пользоваться методом assign и операцией
	// присваивания = для контейнеров типа vector.
	vp.assign(7, new Vector(77, 77)); 

	cout << "\n\nAfter assign\n";
	for (size_t i=0;  i < vp.size();  i++)
		vp[i]->Out();
	
	// Декларируйте новый вектор указателей на Vector и инициализируйте его 
	// с помощью второй версии assign
	vector<Vector*> vpNew;
	vpNew.assign(vp.begin(), vp.end());
	
	cout << "\n\nNew vector after assign\n";
	for (const auto& el : vpNew)
		el->Out();

	// На базе шаблона vector создание двухмерный массив и
	// заполните его значениями разными способами.
	// Первый вариант - прямоугольная матрица
	// Второй вариант - ступенчатая матрица
	const double val = 3.33;
	const int rows = 3, cols = 2;
	vector<vector<double>> vdd(rows, vector<double>(cols, val));

	
	//========= Ступенчатая матрица
	//vector <vector
	for (size_t i=0;  i < vdd.size();  i++)
		vdd[i] = vector<double>(i+1, double(i));
	
	cout << "\n\n\tTest vector of vector<double>\n";
	for (size_t i=0;  i < vdd.size();  i++)
	{
		cout << endl;
		for (size_t j=0;  j < vdd[i].size();  j++)
			cout << vdd[i][j] << "  ";
	}	
	cout << endl;

	//===================================
	// Простейшие действия с контейнерами
	//===================================
	//3б. Получение значения первого и последнего элементов последовательности.
	//Получение размера последовательности. Присваивание значений
	//элементов одной последовательности элементам другой - assign().

	//Создайте и проинициализируйте вектор из элементов char. Размер -
	//по желанию.

	vector<char> charVector = {'A', 'L', 'G', 'E', 'B', 'R', 'A'};

	//Создайте и проинициализируйте массив из элементов char. Размер -
	//по желанию.
	const size_t sz = 6;
	array<char, sz> ca;
	for (size_t i = 0; i < sz; ++i){
		ca[i] = static_cast<char>('c' + i);
	} 

	//Получите значение первого элемента вектора ( front() )

	cout << charVector.front() << endl;

	//Получите значение последнего элемента вектора ( back() )

	cout << charVector.back() << endl;

	//Получите размер вектора

	cout << charVector.size() << endl;

	//Присвойте вектору любой диапазон из значений массива cMas.

	cout << "старый размер: " << charVector.size() << endl;

	charVector.assign(ca.begin() + 1, ca.begin() + 3);

	//Проверьте размер вектора, первый и последний элементы.

	cout << "новый размер: " << charVector.size() << endl;
    cout << "новый первый: " << charVector.front() << endl;
    cout << "новый послед: " << charVector.back() << endl;

	//3в. Доступ к произвольным элементам вектора с проверкой - at()
	//и без проверки - []
	//Создайте неинициализированный вектор из 8 элементов char - vChar2.
	//С помощью at() присвойте четным элементам вектора значения
	//элементов vChar1 из предыдущего задания,
	//а с помощью [] присвойте нечетным элементам вектора vChar2 значения
	//массива {'K','U','K','U'}.

	vector<char> vChar1 = {'A', 'B', 'C', 'D', 'E', 'F'};
    vector<char> vChar2(8);
    array<char, 4> charArray = {'K', 'U', 'K', 'U'};

	for (size_t i = 0; i < vChar1.size(); ++i) {
        if (i < 4) { 
            vChar2.at(i * 2) = vChar1.at(i);
        }
    }

	for (size_t i = 0; i < charArray.size(); ++i) {
        vChar2[i * 2 + 1] = charArray[i];
    }

	//Попробуйте "выйти" за границы вектора с помощью at() и
	//с помощью []. Обратите внимание: что происходит при
	//попытке обращения к несуществующему элементу в обоих случаях

	try {
        vChar2.at(66) = 'e'; 
    } catch (const std::out_of_range& e) {
        std::cerr << "except catched: " << e.what() << std::endl;
    }

	char c = vChar2[66]; // мусор
	// [] -> *(arr + 66)

	//3г.Добавьте в конец вектора vChar2  - букву Z (push_back()). Для
	//расширения кругозора можете ее сразу же и выкинуть (pop_back())

	vChar2.push_back('Z');
	vChar2.pop_back();

	//3д. Вставка-удаление элемента последовательности insert() - erase()
	//Очистка последовательности - clear()

	//Вставьте перед каждым элементом вектора vChar2 букву 'W'

	for (size_t i = 0; i < vChar2.size(); i += 2) {
        vChar2.insert(vChar2.begin() + i, 'W');
    }	

	//Вставьте перед 5-ым элементом вектора vChar2 3 буквы 'X'

	vChar2.insert(vChar2.begin() + 4, 3, 'X');

	//Вставьте перед 2-ым элементом вектора vChar2 с третьего по
	//шестой элементы массива "aaabbbccc"

	vector<char> letters = {'a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c'};
	vChar2.insert(vChar2.begin() + 1, letters.begin() + 2, letters.begin() + 6);
  
	//Сотрите c первого по десятый элементы vChar2

	if (vChar2.size() >= 10) 
        vChar2.erase(vChar2.begin(), vChar2.begin() + 10);

	//Уничтожьте все элементы последовательности - clear()

	vChar2.clear();

	//Создание двухмерного массива

	vector<vector<int>> mtx(3, vector<int>(6, 3));

	//Задание 4. Списки. Операции, характерные для списков.
	//Создайте два пустых списка из элементов Vector - ptList1 и
	//ptList2

	list<Vector> ptList1;
	list<Vector> ptList2;

	//Наполните оба списка значениями с помощью методов push_back(),
	//push_front, insrert()

	ptList1.push_back(Vector(3.3, 4.4));
    ptList1.push_front(Vector(1.1, 2.2));
    ptList1.push_back(Vector(5.5, 6.6));
    ptList1.insert(ptList1.begin(), Vector(0.0, 0.0));

	ptList2.push_back(Vector(2.0, 2.0));
    ptList2.push_front(Vector(6.0, 8.0));
    ptList2.push_back(Vector(4.0, 3.0));


	//Отсортируйте списки - sort().
	//Подсказка: для того, чтобы работала сортировка, в классе Vector
	//должен быть переопределен оператор "<"

	printList(ptList1, "ptList1 before sort");
    printList(ptList2, "ptList2 before sort");

    ptList1.sort();
    ptList2.sort();

    printList(ptList1, "ptList1 after sort");
    printList(ptList2, "ptList2 after sort");


	//Объедините отсортированные списки - merge(). Посмотрите: что
	//при этом происходит со вторым списком.

	ptList1.merge(ptList2);
    printList(ptList1, "ptList1 after merge");
    printList(ptList2, "ptList2 after merge (now empty)");

	//Исключение элемента из списка - remove()
	//Исключите из списка элемент с определенным значением.
	//Подсказка: для этого необходимо также переопределить
	//в классе Vector оператор "=="

	ptList1.remove(Vector(1.1, 2.2));
    printList(ptList1, "ptList1 after removing (1.1, 2.2)");

	//Задание 5. Стандартные алгоритмы.Подключите заголовочный файл
	// <algorithm>
	//5а. Выведите на экран элементы ptList1 из предыдущего
	//задания с помощью алгоритма for_each()

	std::for_each(ptList1.begin(), ptList1.end(), [](const Vector& v) {
    	v.Out();
	});

	//5б.С помощью алгоритма find() найдите итератор на элемент Vector с
	//определенным значением. С помощью алгоритма find_if() найдите
	//итератор на элемент, удовлетворяющий определенному условию, 
	//например, обе координаты точки должны быть больше 2.
	//Подсказка: напишите функцию-предикат, которая проверяет условие
	//и возвращает boolean-значение (предикат может быть как глобальной
	//функцией, так и методом класса)

	auto it1 = std::find(ptList1.begin(), ptList1.end(), Vector(3.3, 4.4));
    if (it1 != ptList1.end()) {
        std::cout << "\nfound Vector (3.3., 4.4)!\n";
    }

	auto it2 = std::find_if(ptList1.begin(), ptList1.end(), bothCoordsGreaterThanTwo);
    if (it2 != ptList1.end()) {
        std::cout << "\nfound Vector with both coords > 2:";
        it2->Out();
    }

	//Создайте список из указателей на элеметы Vector. С помощью 
	//алгоритма find_if() и предиката (можно использовать предикат - 
	//метод класса Vector, определенный в предыдущем задании) найдите в
	//последовательности элемент, удовлетворяющий условию

	list<Vector*> ptrList;

	ptrList.push_back(new Vector(3.3, 4.4));
    ptrList.push_front(new Vector(1.1, 2.2));
    ptrList.push_back(new Vector(5.5, 6.6));

    auto it_ptr = std::find_if(
		ptrList.begin(), ptrList.end(), 
		[](const Vector* v){ 
			return v && bothCoordsGreaterThanTwo(*v); 
		}
	);
    
    if (it_ptr != ptrList.end()) {
        cout << "\n\nfound Vector with both coords > 2: ";
        (*it_ptr)->Out();
    } else {
        cout << "\n\nnot found. \n";
    }

	for (Vector* v_ptr : ptrList) delete v_ptr; 
    ptrList.clear();

	//5в. Создайте список элементов Vector. Наполните список
	//значениями. С помощью алгоритма replace() замените элемент
	//с определенным значением новым значением. С помощью алгоритма
	//replace_if() замените элемент, удовлетворяющий какому-либо
	//условию на определенное значение. Подсказка: условие
	//задается предикатом.

	printList(ptList1, "исходный список");

    std::replace(
		ptList1.begin(), ptList1.end(), Vector(5.5, 6.6), Vector(11.1, 11.1)
	);
    printList(ptList1, "после replace()");

    // используем лямбда-функцию как предикат
    std::replace_if(ptList1.begin(), ptList1.end(),
        [](const Vector& v) { return v.GetX() > 3 && v.GetY() > 3; },
        Vector(-3.3, -3)
    );
    printList(ptList1, "После replace_if()");

	//5г. Создайте вектор строк (string). С помощью алгоритма count()
	//сосчитайте количество одинаковых строк. С помощью алгоритма
	//count_if() сосчитайте количество строк, начинающихся с заданной
	//буквы

	vector<string> strVect;

	strVect.push_back("Math");
	strVect.push_back("Math");
	strVect.push_back("Math");
	strVect.push_back("Math");
	strVect.push_back("Alrebra");

	cout << std::count(strVect.begin(), strVect.end(), "Math") << endl;

	char startChar = 'M';
    int startsWithM = std::count_if(strVect.begin(), strVect.end(),
        [startChar](const string& s) {
            return !s.empty() && s[0] == startChar;
        }
    );

	cout << startsWithM << endl;

	//5д. С помощью алгоритма count_if() сосчитайте количество строк,
	//которые совпадают с заданной строкой. Подсказка: смотри тему
	//объекты-функции

	const string target = "Algebra"; 
	int countWithFunctor = std::count_if(strVect.begin(), strVect.end(),
        [&target](const string& s) {
            return std::strcmp(s.c_str(), target.c_str()) == 0;
        }
    );

	cout << countWithFunctor << endl;
	cout <<"\n\n";
}