-------------------------------------
-- Практические задание 1. Часть 1 --
-------------------------------------

module Pr01_1 where


-- Напишите реализацию функций myFST, mySND, myTHRD для кортежа (a,b,c)

myFST :: (a,b,c) -> a
myFST (x,_,_) = x

mySND :: (a,b,c) -> b
mySND (_,y,_) = y

myTHRD :: (a,b,c) -> c
myTHRD (_,_,z) = z

-- Напишите реализацию стандартных функции для работы со списками:

-- myHead - определение (через сопоставление с образцом) функции отделения головы списка
myHead :: [a] -> a
myHead (x:_) = x

-- myTail - функция отделения хвоста списка
myTail :: [a] -> [a]
myTail (_:xs) = xs

-- myTake - взять первые n элементов списка
myTake :: Int -> [a] -> [a]
myTake n xs = if n <= 0 then []
              else case xs of
              []      -> []
              (x:xs') -> x : myTake (n-1) xs'

-- myTake n xs =
--   if n <= 0 then []
--   else if null xs then []
--   else let (x:xs') = xs in x : myTake (n-1) xs'

-- myDrop - отбросить первые n элементов списка
myDrop :: Int -> [a] -> [a]
myDrop n xs = if n <= 0 then xs
              else case xs of
              []      -> []
              (_:xs') -> myDrop (n-1) xs'


-- myProduct - перемножить все элементы списка
myProduct :: Num a => [a] -> a
myProduct [] = 1
myProduct (x:xs) = x * myProduct xs

-- myZip - попарное объединение двух списков в список пар, длина итогового списка по длине самого короткого из входных списков
myZip :: [a] -> [b] -> [(a,b)]
myZip [] _ = []
myZip _ [] = []
myZip (x:xs) (y:ys) = (x,y) : myZip xs ys

-- myZip3 объединение трех списков в список троек
-- myUnzip - разделение списка пар на пару списков

-- Напишите реализацию стандартных функции высшего порядка для работы со списками:
-- myFilter - применение предиката к каждому элементу списка (две реализации: с использованием охранных выражений и if-then-else)
-- myMap - применение функции одного аргумента к каждому элементу списка
myMap :: (a -> b) -> [a] -> [b]
myMap _ [] = []
myMap f (x:xs) = f x : myMap f xs

-- myZipWith - применение функции двух аргументов к двум спискам
myZipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
myZipWith _ [] _ = []
myZipWith _ _ [] = []
myZipWith f (x:xs) (y:ys) = f x y : myZipWith f xs ys

-- myZipWith3 - применение функции трех аргументов к трем спискам
-- myAll - проверяет удовлетворяют ли все элементы списка предикату
-- myAny - проверяет удовлетворяют ли хотя бы один элемент списка предикату
-- myComposition - композиция двух функций (.)

