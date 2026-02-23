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

list :: [Int]
list = [1,2,3,4,5]

list' :: [Int]
list' = -2:0:list

emptyList = []

-- myHead - определение (через сопоставление с образцом) функции отделения головы списка
myHead :: [a] -> a
myHead [] = error "Empty list"
myHead (x:_) = x

resHead = myHead list
resHeadEmpty = myHead emptyList

-- myTail - функция отделения хвоста списка
myTail :: [a] -> [a]
myTail [] = error "Empty list"
myTail (_:xs) = xs

resTail = myTail list
resTailEmpty = myTail emptyList

-- myTake - взять первые n элементов списка
myTake :: Int -> [a] -> [a]
myTake n xs = if n <= 0 then []
              else case xs of
              []      -> []
              (x:xs') -> x : myTake (n-1) xs'

resTake = myTake 3 list
resTakeEmpty = myTake 3 emptyList

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

resDrop = myDrop 3 list
resDropEmpty = myDrop 3 emptyList


-- myProduct - перемножить все элементы списка
myProduct :: Num a => [a] -> a
myProduct [] = 1
myProduct (x:xs) = x * myProduct xs

resProduct = myProduct list
resProductEmpty = myProduct emptyList

-- myZip - попарное объединение двух списков в список пар, длина итогового списка по длине самого короткого из входных списков
myZip :: [a] -> [b] -> [(a,b)]
myZip [] _ = []
myZip _ [] = []
myZip (x:xs) (y:ys) = (x,y) : myZip xs ys

resZip = myZip list list
resZip' = myZip list list'
resZipEmpty = myZip list emptyList

-- myZip3 объединение трех списков в список троек
myZip3 :: [a] -> [b] -> [c] -> [(a,b,c)]
myZip3 [] _ _ = []
myZip3 _ [] _ = []
myZip3 _ _ [] = []
myZip3 (x:xs) (y:ys) (z:zs) = (x,y,z) : myZip3 xs ys zs

resZip3 = myZip3 list list' list
resZip3Empty = myZip3 list list' emptyList

-- myUnzip - разделение списка пар на пару списков
myUnzip :: [(a,b)] -> ([a], [b])
myUnzip [] = ([], [])
myUnzip ((x,y):xs) = let (xs', ys') = myUnzip xs in (x:xs', y:ys')

resUnzip = myUnzip (resZip)
resUnzip' = myUnzip (myZip list list')

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

