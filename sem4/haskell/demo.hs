
main = putStrLn "Hello, World!"


func :: Int -> Int
func x = (x + 3) * 2

x = func 5

plus :: Int -> Int -> Int
plus a b = a + b

plus' a = \x -> a + x

b = (plus 10) 20
c = (plus' 10) 20
