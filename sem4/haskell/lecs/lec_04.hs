------------------------------------------------------------------------
-- Лекция 04 Введение в Haskell. Свертки и алгебраические типы данных --
------------------------------------------------------------------------

module Lec04 where

{-
При написании многих (но далеко не всех) рекурсивных функций можно увидеть некоторые схожие части

Например у меня есть следующие стандартные функции для обработки списков
mySum     - суммирует     все элементы в списке
myProduct - перемножает   все элементы в списке
myConcat  - конкатенирует все элементы в списке

Вот их реализация:
-}
mySum :: [Integer] -> Integer
mySum [] = 0
mySum (x:xs) = x + mySum xs

myProduct :: [Integer] -> Integer
myProduct [] = 1
myProduct (x:xs) = x * myProduct xs

myConcat :: [[a]] -> [a]
myConcat [] = []
myConcat (x:xs) = x ++ myConcat xs

{-
Рассмотрим их детально:
- Похожи ли у них типы?
    Типы стрелочные и результат выражения более плоский чем входные аргументы
- Похожи ли базы рекурсии?
    Не совсем, но часть левее равенства одинаковая
- Похожи ли тела рекурсивных выражений?
    Разделение на голову и хвост при сравнении с образцом, функции двух аргументов, использование элемента из списка и рекурсивный вызов
- В чем различия?
    Разные функции вычисляют результат и разные базовые значения в рекурсии

Можно переписать функции mySum, myProduct, myConcat в префиксном стиле
! Hint:
f a b = a + b
f 1 2
1 `f` 2
(*) 1 2
1 * 2

Реализуем функцию, которая обобщить три функции реализованные выше:
-}

myFold :: (a -> b -> b) -> b -> [a] -> b
myFold f z [] = z
myFold f z (x:xs) = f x (myFold f z xs)



{-
Свертки

Сворачивать список можно влево и вправо:

foldr
e1 : (e2 : (e3 : [])) --> e1 `f` (e2 `f` (e3 `f` z))

     :                           f
    / \                         / \
  e1   :        foldr f z    e1    f
      / \     ------------->      / \
    e2   :                      e2   f
        / \                         / \
      e3   []                     e3   z


-- foldr (+) 0 [1,2,3]

foldr (+) 0 [1,2,3] => 1 + foldr (+) 0 [2,3]
                    => 1 + (2 + foldr (+) [3])
                    => 1 + (2 + (3 + foldr (+) 0 []))
                    => 1 + (2 + (3 + 0))
                    => 1 + (2 + 3) => 1 + 5 => 6

-- foldr (\n b -> odd n && b) True [1,2,3]

Реализация правой свертки:
-}

myFoldr :: (a -> b -> b) -> b -> [a] -> b
myFoldr f z [] = z
myFoldr f z (x:xs) = f x (myFold f z xs)


{-

foldl
e1 : (e2 : (e3 : [])) --> ((z `f` e1) `f` e2) `f` e3

     :                             f
    / \                           / \
  e1   :        foldl f z        f   e3
      / \     ------------->    / \
    e2   :                     f   e2
        / \                   / \
      e3   []                z  e1

-- foldl (+) 0 [1,2,3]

foldl (+) 0 [1,2,3] => foldl (+) (0 + 1) [2,3]
                    => foldl (+) ((0 + 1) + 2) [3]
                    => foldl (+) (((0 + 1) + 2) + 3) []
                    => ((0 + 1) + 2) + 3 => (1 + 2) + 3 => 3 + 3 => 6

Реализация левой свертки:
-}

myFoldl :: (b -> a -> b) -> b -> [a] -> b
myFoldl f z [] = z
myFoldl f z (x:xs) = myFoldl f (f z x) xs

{-
В свертках появляются отложенные вычисления (thunk) - могут перегружать память
Эта проблема решается форсированием вычислений и строгими версиями сверток

Наибольшая эффективность вычислений у левых сверток, НО сложности при работе с бесконечными структурами

Правая свертка может работать с бесконечными списками

То есть если есть необходимость использования бесконечных структур - берем правую свертку
        если - нет, то - левую



Как и почему работают бесконечности?
a = [1..]
take 5 a

Берем стандартные реализации функции и оператора:
- проверяет все ли значения удовлетворяют предикату:
all :: (a -> Bool) -> [a] -> Bool
all p = foldr (\x b -> p x && b) True
- стандартный И в функциональной реализации:
(&&) :: Bool -> Bool -> Bool
True && x = x
False && _ = False

шаги выполнения вычисления:
all (<2) [1..] →
foldr (\x b -> x<2 && b) True [1..] →
foldr (\x b -> x<2 && b) True (1 : [2..]) →
(\x b -> x<2 && b) 1 (foldr (\x b -> x<2 && b) True [2..]) →
(1<2) && (foldr (\x b -> x<2 && b) True [2..]) →
True && (foldr (\x b -> x<2 && b) True [2..]) →
True && (foldr (\x b -> x<2 && b) True (2 : [3..])) →
True && ((\x b -> x<2 && b) 2 (foldr (\x b -> x<2 && b) True [4..])) →
True && ((2<2) && (foldr (\x b -> x<2 && b) True [3..])) →
True && (False && (foldr (\x b -> x<2 && b) True [3..])) →
                🠕 здесь игнорируется правый аргумент (согласно реализации оператора &&)
True && False →
False

-- partition (> 0) [1,2,-3,4,-5,-6]
-- takeWhile (/= "stop") ["hello", "send", "stop", "receive"]
-- span (/= "stop") ["hello", "send", "stop", "receive"]

Реализовать myPartition с использованием сверток
-}

myPartition :: (a -> Bool) -> [a] -> ([a], [a])
myPartition p xs = foldr localF ([], []) xs
    -- localF :: a -> ([a], [a]) -> ([a], [a])
    where localF z (xs, ys) | p z       = (z:xs, ys)
                            | otherwise = (xs, z:ys)




{-
Сканы

Левый скан (накапливает результаты слева направо):
scanl (#) init [a, b, ...] ≡ [init, init # a, (init # a) # b, ...]

scanl :: (b -> a -> b) -> b -> [a] -> [b]
scanl _ z [] = [z]
scanl (#) z (x:xs) = z : scanl (#) (z # x) xs

scanl (++) "Result: " ["A","B","C"]
scanl (*) 1 [1..] !! 5

В отличие от левой свертки может работать с бесконечными структурами


Правый скан (накапливает результаты справа налево):

scanr :: (a -> b -> b) -> b -> [a] -> [b]
scanr _ z [] = [z]
scanr f z (x:xs) = f x q : qs where qs@(q:_) = scanr f z xs

scanr (+) 0 [1,2,3]
scanr (++) " obtained" ["A","B","C"]


Тождества связывающие сканы и свертки:
head (scanr f z xs) ≡ foldr f z xs
last (scanl f z xs) ≡ foldl f z xs

-}

-- Реализация функции flip
-- :t (flip (:))
myFlip :: (a -> b -> c) -> b -> a -> c
myFlip f y x = f x y


--------------------------------------------------------

{-
Типы помогают предсказывать поведение программы и избежать нежелательное поведение

Алгебраические типы данных (АТД) — это составные типы:
    тип-сумма (объединение)
    тип-произведение (декартово произведение)
    тип-экспонента (функция)

АТД строятся из конструкторов
    каждый конструктор это функция
    конструктор может иметь аргументы, которые задают поля структуры

Для работы с конструкторами используется сопоставление с образцом (pattern matching)



Как написать свой тип?
    - Тип-сумма:
data MyTypeName = MyDataConstructor1 | MyDataConstructor2 | ... | MyDataConstructorN
data - ключевое слово для определения типа
     MyTypeName - имя типа
                  MyDataConstructor1 - конструктор данных
                                     | - разделитель конструктора данных для типа-суммы
                                       MyDataConstructor2 - конструктор данных
Например:
data Bool = True | False
-}
typeSumVar :: Bool
typeSumVar = True

{-
    - Тип-произведение:
data MyTypeName а = MyDataConstructor a a
data - ключевое слово для определения типа
     MyTypeName a - имя типа-конструктора с параметром a
                    MyDataConstructor - конструктор данных
                                      a - типовая переменная (type variable)
Например:
-}
data Point a = PointBox a a
typeProdVar :: Point Int
typeProdVar = PointBox 2 3

{-
    - Тип-экспонента:
data MyTypeName а = MyDataConstructor (a -> a)
data - ключевое слово для определения типа
     MyTypeName а - имя типа-экспоненты с параметром a
                    MyDataConstructor - конструктор данных
                                      (a -> a) - тип-экспонента (функциональный тип)
Например:
-}
data Endom a = EndomBox (a -> a) -- эндоморфизм (упаковка стрелки)
typeExpVar :: Endom Float
typeExpVar = EndomBox (*2)

appEndom :: Endom a -> a -> a -- распаковка стрелки
appEndom (EndomBox f) = f
-- typeExpVar 5
-- appEndom typeExpVar 5



{-
Типы используются для контроля поведения программы
Например: некоторые значения и функции могут отсутствовать
    есть вероятность, что в ходе работы программы будут повреждены данные или их не удастся распарсить
    тогда можно использовать тип с возможно отсутствующим значением
    то есть значение может быть ли не быть (это похоже на тип-сумму в которой есть тип произведение)

data Maybe a = Nothing | Just a
     Maybe — алгебраический тип с двумя конструкторами:
               Nothing — отсутствие значения
                         Just a — содержит значение типа a
Используя тип Maybe напишем реализацию map в которой может отсутствовать функция и/или список для обработки:
myMaybeMap :: ...
myMaybeMap ...
-}

{-
Результаты работы чистых функций могут отсутствовать
Например поиск значений в списке может завершится неудачей:
    find
Реализуем функцию myFind:
myFind :: ...
myFind ...
-}

{-
При отсутствии данных можно использовать значения по умолчанию:
maybe
-}

{-
Тип Maybe показывает, что в процессе работы что-то пошло не так, но можно сделать более сложное поведение
Тип Either имеет более сложные конструкторы данных:

data Either a b = Left a | Right b
     Either  — алгебраический тип с двумя конструкторами:
                  Left a - ошибочное поведение
                           Right b - правильное поведение

either :: (a -> c) -> (b -> c) -> Either a b -> c
-}

{-
В типах можно использовать рекурсию
Пример реализации типа списка:
data MyList a = MyEmpty | MyCons a (MyList a)
     MyList a — алгебраический тип с конструктором MyEmpty (пустой список) и MyCons (элемент и хвост списка)
mylist = MyCons 3 (MyCons 5 MyEmpty)
-}

{-
Упаковка и распаковка значений в контейнер:
-}
data Box a = Box a deriving Show

unBox :: Box a -> a
unBox (Box x) = x



{-
Так как большая часть типов в функциях из стандартной библиотеки полиморфны, то можно использовать свои типы сразу
-}

data InfNumber a = MinusInfinity
                 | Number a
                 | PlusInfinity
                 deriving Show

infMax MinusInfinity x = x
infMax x MinusInfinity = x
infMax PlusInfinity _ = PlusInfinity
infMax _ PlusInfinity = PlusInfinity
infMax (Number a) (Number b) = Number (max a b)
-- foldr infMax MinusInfinity $ map Number [1,2,3]
-- foldr infMax MinusInfinity $ ((map Number [1,2,3]) ++ [PlusInfinity])
-- foldr (\x y -> infMax (Number x) y) MinusInfinity [1,2,3]



{-
Параметрический полиморфизм
Одна реализация может принимать произвольные типы
f :: a -> b -> (a, a)
f    x    y =  (x, x)



Специальный (ad hoc) полиморфизм
Есть контекст (ограничения) накладываемые на тип значений
mySum :: Num a => a -> a -> a
mySum             x    y =  x + y
         Num -- накладывает ограничения на тип a

mySum (1 :: Int) (1 :: Int)
mySum (1 :: Double) (1 :: Double)
mySum (1 :: Double) (1 :: Int)
mySum 'a' 'b'
-}

{-
Синонимы типов:
type SynonymTypeName = TypeName
type - ключевое слово для использования синонимов

newtype - объявление обертки над существующим типом с одним конструктором и одним типом

-}





{-
Реализуем пример приготовления торта из материалов прошлой лекции:

> (Масло-шоколадная смесь) состоит из растопленных на слабом огне масла и шоколада
> [Тесто для торта] состоит из 8 взбитых яиц, муки, сахара и разрыхлителя
> {Тесто для шоколадного торта} – это [тесто для торта], перемешанное с (масло-шоколадной смесью)
> Шоколадный торт – это {тесто для шоколадного торта}, выпеченное в духовке при 200C в течение 25 минут.
-}

-- Типы описания составляющих:
data Ingredients = Oil | Chocolate | Egg | Flour | Shugar | BakingPowder deriving Show
data FillingMix = OilChocolateMix deriving Show
data Dough = CakeDough deriving Show
data CakeDough = ChocolateCakeDough deriving Show
data Cake = ChocolateCake deriving Show
data Action = Bake deriving Show

-- Функции, которые описывают процесс приготовления частей торта
makeCakeMix :: Ingredients -> Ingredients -> FillingMix
makeCakeMix Oil Chocolate = OilChocolateMix
makeCakeMix Chocolate Oil = OilChocolateMix
-- ...

cakeDough :: Ingredients -> Ingredients -> Ingredients -> Ingredients -> Dough
cakeDough Egg Flour Shugar BakingPowder = CakeDough
-- ...

chocolateCakeDough :: Dough -> FillingMix -> CakeDough
chocolateCakeDough CakeDough OilChocolateMix = ChocolateCakeDough
-- ...

chocolateCake :: CakeDough -> Action -> Cake
chocolateCake ChocolateCakeDough Bake = ChocolateCake
-- ...

-- Промежуточные стадии приготовления торта:
myDough = cakeDough Egg Flour Shugar BakingPowder
notMyDough = cakeDough Egg Egg Egg Egg -- ! не работает
myMix = makeCakeMix Oil Chocolate
myCakeDough = chocolateCakeDough myDough myMix
-- Финальный торт:
myCake = chocolateCake myCakeDough Bake

{-
Типы можно расширить и параметризовать для отслеживания объема и количества ингредиентов
-}




--------------------------------------------------------
{-
Пример использования типов для создания медицинских карточек:
-}

patientInfo1 :: String -> String -> Int -> Int -> String
patientInfo1 fname lname age height = name ++ " " ++ ageHeight
    where name = lname ++ ", " ++ fname
          ageHeight = "(Age: " ++ show age ++ "; height: " ++ show height ++ " sm)"
-- patientInfo1 "John" "Doe" 20 200

type FirstName = String
type LastName = String
type Age = Int
type Height = Int

patientInfo2 :: FirstName -> LastName -> Age -> Height -> String
patientInfo2 fname lname age height = name ++ " " ++ ageHeight
    where name = lname ++ ", " ++ fname
          ageHeight = "(Age: " ++ show age ++ "; height: " ++ show height ++ " sm)"
-- patientInfo2 "John" "Doe" 20 200

type PatientName = (FirstName, LastName)

firtsName :: PatientName -> String
firtsName patient = fst patient

lastName :: PatientName -> String
lastName patient = snd patient

patientInfo3 :: PatientName -> Age -> Height -> String
patientInfo3 patient age height = name ++ " " ++ ageHeight
    where name = lname ++ ", " ++ fname
          ageHeight = "(Age: " ++ show age ++ "; height: " ++ show height ++ " sm)"
          fname = fst patient
          lname = snd patient
-- patientInfo3 ("John", "Doe") 20 200


-- Создание пользовательских типов - ключевое слово data
-- конструктор данных. тип перечисления (тип сумма)
data Sex = Male | Female -- Bool = True | False -- ИЛИ
s = Male
-- s

-- сопоставление с типов образцом
sexInitial :: Sex -> Char
sexInitial Male = 'M'
sexInitial Female = 'F'

{-
Некоторые встроенные типы можно представлять как перечисления:
data Char = '\NUL' | ... | 'a' | 'b' | 'c' | 'd' | ... | '\1114111'
data Int = -9223372036854775808 | ... | -2 | -1 | 0 | 1 | 2 | ... | 9223372036854775807
data Integer = ... | -2 | -1 | 0 | 1 | 2 | ...

Для использования как литералы в образцах

isAnswer :: Integer -> Bool
isAnswer 42 = True
isAnswer _ = False
-}

data RhType = Pos | Neg
data ABOType = A | B | AB | O

-- тип произведение (декартово произведение)
data BloodType = BloodType ABOType RhType -- И
-- :t BloodType -- тип похож на функцию, но без исполняемого кода (просто конструктор располагает данные в памяти)

patient1BT :: BloodType
patient1BT = BloodType A Pos
patient2BT :: BloodType
patient2BT = BloodType O Neg
patient3BT :: BloodType
patient3BT = BloodType AB Pos

showRh :: RhType -> String
showRh Pos = "+"
showRh Neg = "-"

showABO :: ABOType -> String
showABO A = "A"
showABO B = "B"
showABO AB = "AB"
showABO O = "O"

showBloodType :: BloodType -> String
showBloodType (BloodType abo rh) = showABO abo ++ showRh rh

canDonateTo :: BloodType -> BloodType -> Bool
canDonateTo (BloodType O _) _ = True
canDonateTo _ (BloodType AB _) = True
canDonateTo (BloodType A _) (BloodType A _) = True
canDonateTo (BloodType B _) (BloodType B _) = True
canDonateTo _ _ = False

type MiddleName = String
data Name = Name FirstName LastName | NameWithMiddle FirstName MiddleName LastName

showName :: Name -> String
showName (Name f l) = f ++ " " ++ l
showName (NameWithMiddle f m l) = f ++ " " ++ m ++ " " ++ l

data Patient = Patient Name Sex Int Int Int BloodType

johnDoe :: Patient
johnDoe = Patient (Name "John" "Doe") Male 43 188 92 (BloodType AB Pos)

getName :: Patient -> Name
getName (Patient n _ _ _ _ _) = n
getAge :: Patient -> Int
getAge (Patient _ _ a _ _ _) = a
getBloodType :: Patient -> BloodType
getBloodType (Patient _ _ _ _ _ bt) = bt

-- синтаксис записей (использование меток полей) -- ! метки полей видимы глобально, т.е. можно использовать один раз в модуле !
data PatientNew = PatientNew {name :: Name
                            , sex :: Sex
                            , age :: Int
                            , height :: Int
                            , weight :: Int
                            , bloodType :: BloodType}

jackieSmith :: PatientNew
jackieSmith = PatientNew {name = Name "Jackie" "Smith"
                        , age = 43
                        , sex = Female
                        , height = 157
                        , weight = 52
                        , bloodType = BloodType O Neg }
-- height jackieSmith
-- showBloodType (bloodType jackieSmith)
-- showName (name jackieSmith)
-- jackieSmithUpdated = jackieSmith { age = 44 }

