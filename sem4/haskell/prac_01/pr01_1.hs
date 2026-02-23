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

myFilter :: (a -> Bool) -> [a] -> [a]
myFilter _ [] = []
myFilter p (x:xs) | p x       = x : myFilter p xs
                  | otherwise = myFilter p xs

myFilterIf :: (a -> Bool) -> [a] -> [a]
myFilterIf _ [] = []
myFilterIf p (x:xs) =
    if p x then x : myFilterIf p xs
    else myFilterIf p xs

isEven :: Int -> Bool
isEven x = x `mod` 2 == 0

resFilter = myFilter isEven list
resFilterIf = myFilterIf isEven list

-- myMap - применение функции одного аргумента к каждому элементу списка
myMap :: (a -> b) -> [a] -> [b]
myMap _ [] = []
myMap f (x:xs) = f x : myMap f xs

foo :: Int -> Int
foo = \x -> (x + 3) * 2

resMap = myMap foo list
resMap' = myMap foo list'
resMapEmpty = myMap foo emptyList

-- myZipWith - применение функции двух аргументов к двум спискам
myZipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
myZipWith _ [] _ = []
myZipWith _ _ [] = []
myZipWith f (x:xs) (y:ys) = f x y : myZipWith f xs ys

resZipWith = myZipWith (+) list list'
resZipWith' = myZipWith (\x y -> x * y) list list'

-- myZipWith3 - применение функции трех аргументов к трем спискам
myZipWith3 :: (a -> b -> c -> d) -> [a] -> [b] -> [c] -> [d]
myZipWith3 _ [] _ _ = []
myZipWith3 _ _ [] _ = []
myZipWith3 _ _ _ [] = []
myZipWith3 f (x:xs) (y:ys) (z:zs) = f x y z : myZipWith3 f xs ys zs

resZipWith3 = myZipWith3 (\x y z -> x + y * z) list list' (myMap foo list)

-- myAll - проверяет удовлетворяют ли все элементы списка предикату
myAll :: (a -> Bool) -> [a] -> Bool
myAll _ [] = True
myAll p (x:xs) = p x && myAll p xs

resMyAll = myAll isEven list
resMyAll' = myAll isEven [2,4,6,8]

-- myAny - проверяет удовлетворяют ли хотя бы один элемент списка предикату
myAny :: (a -> Bool) -> [a] -> Bool
myAny _ [] = False
myAny p (x:xs) = p x || myAny p xs

resMyAny = myAny isEven list
resMyAny' = myAny isEven [1,3,5,7]

-- myComposition - композиция двух функций (.)
myComposition :: (b -> c) -> (a -> b) -> (a -> c)
myComposition f g = \x -> f (g x)

myDoubleToInt :: Double -> Int
myDoubleToInt = \x -> floor x

myIntToString :: Int -> String
myIntToString = \x -> show x

resComposition = myComposition myIntToString myDoubleToInt 3.14
resComposition' = myComposition (\x -> show x) (\x -> x `mod` 2 == 0)  4
