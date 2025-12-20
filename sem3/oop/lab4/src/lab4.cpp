// Контейнеры STL: 
//deque, stack, queue, priority_queue
//set, multiset, map, multimap
//Итераторы. Стандартные алгоритмы. Предикаты.

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>

#include <cctype>

#include "../../lab2/include/MyString.h"
#include "../../lab2/include/Rect.h"
#include "../include/func.hpp"

using namespace std;	

int main()
{

	//Очередь с двумя концами - контейнер deque

	//Создайте пустой deque с элементами типа Point. С помощью
	//assign заполните deque копиями элементов вектора. С помощью
	//разработанного Вами в предыдущем задании универсального шаблона
	//выведите значения элементов на печать

	vector<Point> pntV = {{1, 7}, {3, 2}, {3, 6}};
	deque<Point> pntD; 
	/*
	assign аналог op=
	void assign( size_type count, const T& value );
	template< class InputIt >
	void assign( InputIt first, InputIt last );
	void assign( std::initializer_list<T> ilist );
	*/
	pntD.assign(pntV.begin(), pntV.end());
	printContainer(pntD);

	//Создайте deque с элементами типа MyString. Заполните его значениями
	//с помощью push_back(), push_front(), insert()
	//С помощью erase удалите из deque все элементы, в которых строчки
	//начинаются с 'A' или 'a'

	// deque не адаптер. принимает как доп параметр лишь аллокатор
	deque<MyString> myStrings;

    myStrings.push_back(MyString("aaa"));		// требует явного вызова кон-ра
    myStrings.emplace_back("333"); 				// не требует
    myStrings.push_front(MyString("math"));
    myStrings.emplace_front("Algebra");
    myStrings.insert(myStrings.begin() + 2, MyString("dimL"));

    printContainer(myStrings);
	/*
	template< class ForwardIt, class UnaryPred >
	ForwardIt remove_if( ForwardIt first, ForwardIt last, UnaryPred p );
	*/
	auto newEnd = remove_if(myStrings.begin(), myStrings.end(), 
                                 [](const MyString& s) {
                                     return  s.GetLength() != 0 && 
                                            (s.GetString()[0] == 'A' || 
											 s.GetString()[0] == 'a');
                                 });
    myStrings.erase(newEnd, myStrings.end());
    printContainer(myStrings);

	//Напишите шаблон функции для вывода значений stack, queue, priority_queue
	//Подумайте, как "получать" данное "с верхушки"? top() | front()
	//Что происходит с контейнерами после вывода значений? ничего тк копияы

	/*
	template<
		class T,
		class Container = std::deque<T>
	> class stack;
	*/
	//Создайте стек таким образом, чтобы
	//а) элементы стека стали копиями элементов вектора
	{
		vector<int> nums = {10, 20, 30, 40, 50};
		vector<int> rev_nums = nums;
		reverse(rev_nums.begin(), rev_nums.end());

		stack<int, vector<int>> st (rev_nums); // cont adapter 
		//б) при выводе значений как вектора, так и стека порядок значений был одинаковым 
		printContainer(nums);
		printPop(st);
	}
	//Сравнение и копирование стеков
	//а) создайте стек и любым способом задайте значения элементов
	//б) создайте новый стек таким образом, чтобы он стал копией первого
	//в) сравните стеки на равенство
	//г) модифицируйте любой из стеком любым образом (push, pop, top)
	//д) проверьте, какой из стеков больше (подумайте, какой смысл вкладывается в такое сравнение)
	{
		stack<int> stA;
		stA.push(1);
		stA.push(2);
		stA.push(3);

		stack<int> stB = stA;

		if (stA == stB) cout << "stacks are the same" << endl;
		else cout << "not same" << endl;

		stA.push(6);
		
		if (stA > stB) cout << 1 << endl;
		else cout << 0 << endl;

		if (stA.size() > stB.size()) cout << 1 << endl;
		else cout << 0 << endl;
	}

	////////////////////////////////////////////////////////////////////////////////////
	/*
	template<
		class T,
		class Container = std::deque<T>
	> class queue;
	*/

	//Создайте очередь, которая содержит указатели на объекты типа Point,
	//при этом явно задайте базовый контейнер.
	//Измените значения первого и последнего элементов посредством front() и back()
	//Подумайте, что требуется сделать при уничтожении такой очереди?
	{
		deque<Point*> dq;
		dq.push_back(new Point(0, 0));
		dq.push_back(new Point(0, 0));
		dq.push_back(new Point(0, 0));
		
		queue<Point*, deque<Point*>> q(dq);

		q.front() -> x = 11;
		q.back() -> y = 11;
		cout << "PRINT Q" << endl;
		printPop(q);

		// чистим память
		while (!q.empty()){
			delete q.front();
			q.pop();
		}

		// или прибегнуть к более современному подходу
		queue<unique_ptr<Point>> smartQ;

		smartQ.push(make_unique<Point>(0, 0));
		smartQ.push(make_unique<Point>(0, 0));
		smartQ.push(make_unique<Point>(0, 0));

	}
	////////////////////////////////////////////////////////////////////////////////////
	/*
	template<
		class T,
		class Container = std::vector<T>,
		class Compare = std::less<typename Container::value_type>
	> class priority_queue;
	*/
	//а) создайте очередь с приоритетами, которая будет хранить адреса строковых литералов - const char*
	//б) проинициализируйте очередь при создании с помощью вспомогательного массива с элементами const char*
	//в) проверьте "упорядоченность" значений (с помощью pop() ) - если они оказываются не упорядоченными, подумайте:
	//		что сравнивается при вставке?


	{
		const char* lit[] = {"ccc", "bbb", "aaa"};

		// неверно т.к const char* (в случае с int || double было бы все ок)
		// в случае строк < сравнивает адреса в памяти а не строки
		priority_queue<const char*> pq_0(lit, lit + 3); 

		printPop(pq_0);

		while (!pq_0.empty()) {
        cout << pq_0.top() << " ";
        pq_0.pop();
    	}

		auto cmp = [](const char* a,const char* b){
			return strcmp(a, b) < 0; // true : if(a < b)
		};

		priority_queue<const char*, vector<const char*>, decltype(cmp)> pq(
			lit, lit + 3, cmp
		);
		cout << "!!!!" <<
		printPop(pq);


		while (!pq.empty()) {
        cout << pq.top() << " ";
        pq.pop();
    	}		
		
	}
	
	
	////////////////////////////////////////////////////////////////////////////////////
	/*
	template<
		class Key,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<Key>
	> class set;
	*/
	//a) создайте множество с элементами типа Point - подумайте, что необходимо определить
	//		в классе Point (и каким образом) bool op <
	//б) распечатайте значения элементов с помощью шаблона, реализованного в предыдущей лаб. работе
	//в) попробуйте изменить любое значение...
	//г) Создайте два множества, которые будут содержать одинаковые значения
	//		типа int, но занесенные в разном порядке
	//д) Вставьте в любое множество диапазон элементов из любого другого
	//	контейнера, например, элементов массива	(что происходит, если в массиве имеются дубли?)
	{
		set<Point> p_set;
		p_set.insert({0, 0}); // в первую итерацию будет дубль и он игнор
		for (int i = 0; i < 4; ++i) p_set.insert({1*i, 1*i});

		// p_set.begin() = {6, 6}; // нельзя ошибка комп

		printContainer(p_set);

		set<int> setA = {13, 2, -2, 1};
		vector<int> nums = {10, 5, 1, 10, 20};

		setA.insert(nums.begin(), nums.end());

		printContainer(setA);
	}
	////////////////////////////////////////////////////////////////////////////////////
	/*
	template<
		class Key,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<Key>
	> class multiset;
	*/
	{
		multiset<int> nums = {10, 5, 20, 10, 30, 5};
		nums.insert(10);
		
		printContainer(nums);
	}

	////////////////////////////////////////////////////////////////////////////////////
	/*
	template<
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<std::pair<const Key, T>>
	> class map;
	*/	
	//а) создайте map, который хранит пары "фамилия, зарплата" - pair<const char*, int>,
	//	при этом строки задаются строковыми литералами
	//б) заполните контейнер значениями посредством operator[] и insert()
	//в) распечатайте содержимое

	//е) замените один из КЛЮЧЕЙ на новый (была "Иванова", вышла замуж => стала "Петрова")
	{
		map<const char*, int> mp; 	// красно черное двоичное дерево поиска 

		mp["Иванова"] = 33333;
		mp.insert(make_pair("qwe", 123123));	
		mp.insert({"eee", 333});

		int temp = mp["Иванова"];
		mp["Петрова"] = temp;
		mp.erase("Иванова");

		printMap(mp);
	}
	////////////////////////////////////////////////////////////////////////////////////
	/*
	template<
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<std::pair<const Key, T>>
	> class multimap;
	*/
	//а) создайте "англо-русский" словарь, где одному и тому же ключу будут соответствовать
	//		несколько русских значений - pair<string,string>, например: strange: чужой, странный...
	//б) Заполните словарь парами с помощью метода insert или проинициализируйте с помощью 
	//		вспомогательного массива пара (пары можно конструировать или создавать с помощью шаблона make_pair)
	//в) Выведите все содержимое словаря на экран
	//г) Выведите на axэкран только варианты "переводов" для заданного ключа. Подсказка: для нахождения диапазона
	//		итераторов можно использовать методы lower_bound() и upper_bound()

	multimap<string, string> dict = {
        {"strange", "чужой"},
        {"strange", "странный"},
        {"strange", "необычный"},
    };

	dict.insert({"book", "книга"});
    dict.insert(make_pair("book", "бронировать"));

	for (const auto& pair : dict) cout << pair.first << " -> " << pair.second << endl;

	string key = "strange";
    auto range = dict.equal_range(key); // pair<it, it>  замена lower ^ upper_bound

    for (auto it = range.first; it != range.second; ++it) {
        cout << "  -> " << it->second << endl;
    }


///////////////////////////////////////////////////////////////////

	//Итераторы

	//Реверсивные итераторы. Сформируйте set<Point>. Подумайте, что
	//нужно перегрузить в классе Point. Создайте вектор, элементы которого 
	//являются копиями элементов set, но упорядочены по убыванию

	// необ-мо перегрузить bool op==
	set<Point> p_set = {{1, 7}, {3, 2}, {3, 6}, {0, 0}};
    vector<Point> p_vec(p_set.rbegin(), p_set.rend());

	printContainer(p_vec); 

	//Потоковые итераторы. С помощью ostream_iterator выведите содержимое
	//vector и set из предыдущего задания на экран.

	// TODO: доделать не понял про ostream
	ostream_iterator<Point> out(cout, " ");

    copy(p_vec.begin(), p_vec.end(), out);
	cout << endl;
    copy(p_set.begin(), p_set.end(), out);
	cout << endl;

	//Итераторы вставки. С помощью возвращаемых функциями:

	vector<int> src = {1, 2, 3};
    deque<int> d;
    list<int> l;
	vector<int> v;

	/*
	template< class InputIt, class OutputIt >
	OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
	*/

	//back_inserter() - deque, vector, list
	copy(src.begin(), src.end(), back_inserter(d));
	//front_inserter() - deque, list
	copy(src.begin(), src.end(), front_inserter(l));
	//inserter() - w all
	copy(src.begin(), src.end(), inserter(v, v.begin()));
	//итераторов вставки добавьте элементы в любой из созданных контейнеров. Подумайте:
	//какие из итераторов вставки можно использовать с каждым контейнером.


///////////////////////////////////////////////////////////////////

	//Обобщенные алгоритмы (заголовочный файл <algorithm>). Предикаты.

	// алгоритм for_each() - вызов заданной функции для каждого элемента любой последовательности
	//(массив, vector, list...)
	//С помощью алгоритма for_each в любой последовательности с элементами любого типа
	//распечатайте значения элементов
	//Подсказка : неплохо вызываемую функцию определить как шаблон
	
	vector<int> nums = {10, 20, 30};
    list<string> words = {"hello", "world"};
    
    for_each(nums.begin(), nums.end(), print<int>);
    cout << endl;

    for_each(words.begin(), words.end(), print<string>);
    cout << endl;

	//С помощью алгоритма for_each в любой последовательности с элементами типа Point
	//измените "координаты" на указанное значение (такой предикат тоже стоит реализовать 
	//как шаблон) и выведите результат с помощью предыдущего предиката

	vector<Point> points = {{1, 1}, {2, 2}, {3, 2}, {1, 7}, {1, 7}};

	// PointTransformer класс функтор (повторяет поведение функций)
    for_each(points.begin(), points.end(), PointTransformer(5, 5));

    for_each(points.begin(), points.end(), print<Point>);
    cout << endl;


	//С помощью алгоритма find() найдите в любой последовательности элементов Point
	//все итераторы на элемент Point с указанным значением.

	Point target_point = {6, 12};

    auto it = points.begin();
    int count = 0;

	while (true) {
        it = find(it, points.end(), target_point);
        if (it != points.end()) { // find new
            cout << distance(points.begin(), it) << endl;
            ++it; 
            count++;
        } else { // reach the end
            break;
        }
    }
    cout << count << endl;
	
	
	//С помощью алгоритма sort() отсортируйте любую последовательность элементов Point. 
	/*
	template< class RandomIt >
	void sort( RandomIt first, RandomIt last );
	*/
	//По умолчанию алгоритм сортирует последовательность по возрастанию.
	//Что должно быть определено в классе Point?
	// Замечание: обобщенный алгоритм sort не работает со списком, так как
	//это было бы не эффективно => для списка сортировка реализована методом класса!!!
	
	printContainer(points);

	sort(points.begin(), points.end());

	printContainer(points);

	//Создайте глобальную функцию вида: bool Pred1_1(const Point& ), которая будет вызываться
	//алгоритмом find_if(), передавая в качестве параметра очередной элемент последовательности.
	//С помощью алгоритма find_if() найдите в любой последовательности элементов Point
	//итератор на элемент Point, удовлетворяющий условию: координаты x и y лежат в промежутке
	//[-n, +m].
	{
		auto it = find_if(points.begin(), points.end(), Pred1_1);

		if (it != points.end()) {
			cout << distance(points.begin(), it) << endl;
			cout << "{" << it->x << ", " << it->y << "}" << endl;
		}

		//С помощью алгоритма sort() отсортируйте любую последовательность элементов Rect,
		//располагая прямоугольники по удалению центра от начала координат.
		
		vector<Rect> rects = {
			Rect(0, 0, 10, 10),
			Rect(10, 0, 15, 5),
			Rect(100, 100, 101, 101)
		};


		sort(rects.begin(), rects.end(), [](const Rect& a, const Rect& b) {
			Vector center_a = a.getCenter();
			Vector center_b = b.getCenter();
			
			double dst_a = center_a.GetX() * center_a.GetX() + center_a.GetY() * center_a.GetY();
			double dst_b = center_b.GetX() * center_b.GetX() + center_b.GetY() * center_b.GetY();

			return dst_a < dst_b;
		});
	}

	{//transform
		/*
		template< class InputIt, class OutputIt, class UnaryOp >
		OutputIt transform( InputIt first1, InputIt last1, OutputIt d_first, UnaryOp unary_op );
		*/
		//Напишите функцию, которая с помощью алгоритма transform переводит 
		//содержимое объекта string в нижний регистр.
		//Подсказка: класс string - это "почти" контейнер, поэтому для него
		// определены методы begin() и end()

		string my_string = "Linear Algebra and Math Structures";
		to_lower_case(my_string);
		cout << my_string << endl;

		//Заполните list объектами string. С помощью алгоритма transform сформируте
		//значения "пустого" set, конвертируя строки в нижний регистр
		/*
		template< class Container >
		std::insert_iterator<Container>
		inserter( Container& c, typename Container::iterator i );
		*/
	
		list<string> words = {"ALGEBRA", "Math", "c++", "algebra", "C++"};
		set<string> lower;
		// inserter из-за пустого set'a
		transform(words.begin(), words.end(),
					inserter(lower, lower.begin()),
					[](const string& s) {
						string temp = s;
						to_lower_case(temp); 
						return temp;
					});
		
		for (const auto& word : lower) 
        	cout << word << " ";
		cout << endl;


	}
	{// map
		
		//Сформируйте любым способом вектор с элементами типа string.
		//Создайте (и распечатайте для проверки) map<string, int>, который будет
		//содержать упорядоченные по алфавиту строки и
		//количество повторений каждой строки в векторе
	
		vector<string> words = {"ALGEBRA", "Math", "c++", "ALGEBRA", "C++"};
		map<string, int> word_cnt;

		for (const auto& word : words) {
			word_cnt[word]++;
		}
		
		printMap(word_cnt);
	}

	return 0;
}

