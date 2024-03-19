# TKOM 24L

Mariusz Pakulski

Język programowania: C++

Opiekun projektu: mgr inż. Agnieszka Malanowska

## Temat

Język z wbudowanym typem słownika z określoną kolejnością elementów. <br/>
Kolejność elementów w słowniku jest określana w momencie jego tworzenia – argumentem konstruktora słownika jest funkcja określająca, w jakiej kolejności mają być wstawiane elementy.<br/>
Możliwe są podstawowe operacje na słowniku (dodawanie, usuwanie, wyszukiwanie elementów wg klucza, sprawdzanie, czy dany klucz znajduje się w słowniku itd.), iterowanie po elementach oraz wykonywanie na słowniku zapytań w stylu LINQ.

## Uruchomienie projektu
Aby uruchomić projekt należy z poziomu głównego katalogu projektu użyć komendy: ```./m++ {ścieżka do pliku}```

## Założenia podstawowe
- typowanie silne, dynamiczne
- mutowalność zmiennych
- tworzenie i przypisywanie wartości zmiennych za pomocą znaku '='
- wymóg deklaracji zmiennej wraz z inicjalizacją
- Obsługiwane typy:
  - liczbowe ```Int, Double```, operacje matematyczne zachowujące kolejność działań i nawiasowanie
  - znakowy ```String```, konkatenacja dwóch łańcuchów znakowych
  - logiczny ```Bool```, operacje logiczne
  - słownikowy ```Dict```, metody wywoływane po kropce
  - specjalny ```Special```, przechowuje wartości Null
- komentarze jedolinijkowe oznaczone znakiem ```#``` (rozpoczęcie w dowolnym miejscu, kończą się z końcem linii)
- możliwość definiowania własnych funkcji (wartość do funkcji przekazywana przez wartość)
- bloki kodu ograniczone przez nawias klamrowy
- obsługa rekurencji (funkcja może wywoływać siebie samą)
- dostępność funkcji zdefiniowanych przed i po bloku
- instrukcje:
    - warunkowa ```if``` z ```else```
    - pętli ```while```
    - sterujące ```return, break, continue```
- funkcja ```print()```do wyświetlenia danych
- obsługa błędów
- obsługa operatorów:
    - arytmetycznych ``` +,-,*,/,^ ```
    - porównujących ```<,>,<=,>=,==,!=```
    - logicznych ```||, &&, !```
    - dostępu ```.```

## Założenia niefunkcjonalne

- leniwa tokenizacja
- implementacja w języku C++
- moduły:
     - lekser (Rozpoznaje tokeny i przekazuje je do parsera)
     - parser (Buduje drzewa składniowe i przekazuje je do interpretera)
     - interpreter (Wykonuje kod na podstawie otrzymanych drzew składniowych, zarządza również błędami)
- gramatyka bezkontekstowa (po prawej stronie reguł może występować wiele symboli nieterminalnych)
- parser rekursywnie zstępujący (Drzewo wyprowadzenia jest budowane od korzenia do liści)
- priorytety operatorów:

| operator         | priorytet | łączność        |
|------------------|-----------|------------------|
| `.`              | 8         | lewostronna      |
| `()`             | 8         | lewostronna      |
| `!`              | 7         | prawostronna     |
| `-` (unarny)     | 7         | prawostronna     |
| `^`              | 6         | lewostronna      |
| `*`              | 5         | lewostronna      |
| `/`              | 5         | lewostronna      |
| `+`              | 4         | lewostronna      |
| `-` (binarny)    | 4         | lewostronna      |
| `>`              | 3         | lewostronna      |
| `>=`             | 3         | lewostronna      |
| `<`              | 3         | lewostronna      |
| `<=`             | 3         | lewostronna      |
| `==`             | 3         | lewostronna      |
| `!=`             | 3         | lewostronna      |
| `&&`             | 2         | lewostronna      |
| `\|\|`             | 1       | lewostronna      |
| `=`              | 0         | -                |

- konwersja typów:

|        |   Int    | Double     | String     |  Bool    | Dict  | Special|
|--------|----------|------------|------------|----------|-------|--------|
|Int     | X        | to_double()| to_string()| to_bool()| X     | X      |
|Double  | to_int() | X          | to_string()| X        | X     | X      |
|String  | to_int() | to_double()| X          | to_bool()| X     | X      |
|Bool    | to_int() | to_double()| to_string()| X        | X     | X      |
|Dict    | X        | X          | to_string()| X        | X     | X      |
|Special | X        | X          | to_string()| X        | X     | X      |

|Konwersje             |    Opis |
|----------------------|----------|
|Int, Bool -> Double   | Typ double w przypadku nie posiadania części ułamkowej przy wypisywaniu będzie miał jedno miejsce po przecinku np. 3.0|
|Double -> Int         | Zaokrąglenie modułu do najbliższej liczby całkowitej|
|String -> Int, Double |  W przypadku gdy będzie można zamienić stringa na inta to będzie można dokonać zamiany, w przeciwnym wypadku pojawi się błąd|
|String -> Bool        | Kiedy string zawierałby wartość true albo false, albo liczbę całkowitą to da się zamienić.|
|Bool -> Int           | Wartość true zamieni się na 1, false na 0|


### Przykłady konwersji

```
a = 6
b = a.to_double()
print(b.to_string()) # 6.0
```
```
a = false
b = true
c = a.to_double()
d = b.to_double()
print(c.to_string() + ", " + d.to_string()) # 0.0, 1.0
```
```
a = 6.4
b = 6.6
c = -6.4
d = -6.6

e = a.to_int()
f = b.to_int()
g = c.to_int()
h = d.to_int()

print(e.to_string(), f.to_string(), g.to_string(), h.to_string()) # 6 7 -6 -7
```
- dwie możliwości wypisywania zmiennych; poprzez zwykłą funkcję print oraz @print

Realizuję typowanie silne, więc na zmiennych musi być wykonana jawna konwersja na Stringa.  <br/>
Dlatego w funcji print należy przekonwertować jawnie zmienną jeśli nie jest ona typem String.

W funkcji @print klamry zawierają w sobie wyrażenie, które może być zmienną (w tym słownikiem), wyznaczaną wartością lub wywołaniem funkcji itd.<br/>
Natomiast poza klamrami są znaki, które mają zostać wypisane. Klamry dodatkowo zamieniają zmienną na stringa. <br/>
Pozwala to niepisanie .to_string() oraz cudzysłowów.<br/>
W przypadku chęci wypisania znaku klamry należy wstawić podwójną klamrę ( {{ lub }} ).

### Poprawny zapis @printa
```
a = 6.4
b = 6.6
c = -6.4
d = -6.6

@print({e} {f} {g} {h}) # 6 7 -6 -7
```

```
a = 1
b = 5
c = "a"
d = Null
@print({a}, {b}, {c}, {d}) # 1, 5, a, Null
```
```
a = 5
@print({{a}} {a} {{{a}}}) # {a} 5 {5}
```
```
@print() # Null
```
```
def compare(key1, value1, key2, value2){
    return value1 < value2
}
@print({Dict(compare, {"key1" : 2, "key2" : 1})}) # key2 1
                                                  # key1 2
```
```
dict1 = Dict({"key1" : 5, "key2" : 6})

def fun_iterate(key, value, index){
    return index
}

@print({dict1.iterate()}) # key1 1
                          # key2 2
```

#### Błąd użycia @printa

```
6 @print()
```
```
@print
```
```
6 + @print()
```

#### Poprawne użycie printa
```
a = 5
print(a) # 5
```
```
a = 5
b = 10
print(a.to_string(), " ", b.to_string()) # 5 10
```

```
a = 5
print(a) # 5
```
```
a = 5
b = 10
print(a.to_string() + " " + b.to_string()) # 5 10
```
```
a = "a"
b = 10
c = Null
print(a + " " + b.to_string() + " " + c.to_string()) # a 10 Null
```
#### Niepoprawne użycie printa
```
a = "a"
b = 10
c = Null
print(a + " " + b + " " + c)
```

- akceptowane kombinacje typów dla operatorów wieloargumentowych

| operator         | Int do Inta | Double do Double | String do Stringa| Bool do Boola|
|------------------|---|---|---|---|
| `^`              | X | X |   |   |
| `*`              | X | X |   |   |
| `/`              | X | X |   |   |
| `+`              | X | X | X |   |
| `-` (binarny)    | X | X |   |   |
| `>`              | X | X | X | X |
| `>=`             | X | X | X | X |
| `<`              | X | X | X | X |
| `<=`             | X | X | X | X |
| `==`             | X | X | X | X |
| `!=`             | X | X | X | X |
| `&&`             |   |   |   | X |
| `\|\|`           |   |   |   | X |

- Dla kropki po lewej stronie może być Int, Double,  String, Bool, Dict, Special. Po prawej stronie będzie wywoływana metoda.
- Dla = zmienna po lewej stronie ustali swój typ dynamicznie na podstawie wartości po prawej stronie.

Można nadpisać zmienną wartością innego typu:
```
a = 5
a = "string"
```



## Gramatyka EBNF

### Założenia:

Zakładam, że można wywołać return w dowolnym miejscu w kodzie.
Będzie mogła zwracać Null, zakładamy, że nie można zwracać wielu elementów po przecinku z funkcji

### EBNF Część składniowa
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|program                  |=| {function_definition \| statement};|
|function_definition      |=| "def ", id, "(", [parameter_list], ")", block;|
|block                    |=| "{", {statement}, "}"|
|statement                |=| variable_access
|                         | |   \| if_statement
|                         | |   \| while_statement
|                         | |   \| assignment
|                         | |   \| "break"
|                         | |   \| "continue"
|                         | |   \| return_statement;|
|assignment               |=| id, "=", dexpression;|
|variable_access 		  |=| (function_call \| dict_expression), {".", function_call};|
|function_call            |=| normal_function_call|
|                         |=| \| print_call;|
|normal_function_call     |=| id, ["(", [argument_list], ")"];|
|argument_list			  |=| dexpression, {",", dexpression};|
|if_statement             |=| "if", "(", expression, ")", block, ["else", block];|
|while_statement          |=| "while", "(", expression, ")", block;|
|return_statement         |=| "return ", [dexpression];|

### EBNF Expression
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|expression			      |=| or_term , {"\|\|", or_term};|
|or_term				  |=| and_term, {"&&", and_term};|
|and_term				  |=| comparison|
|                         | | \| "!", "(", comparison, ")";|
|comparison			      |=| additive_expression, [("==" \| "!=" \| ">=" \| "<=" \| "<" \| ">"), additive_expression];|
|additive_expression      |=| multiplicative_expression, {("+" \| "-"), multiplicative_expression};|
|multiplicative_expression|=| factor, {("*" \| "/"), factor};|
|factor				      |=| exponent_factor, {"^", exponent_factor};|
|exponent_factor		  |=| [-], numeric_term;|
|numeric_term			  |=| num_const|
|                         | | \| "(", expression, ")"|
|                         | | \| variable_access;|


### EBNF dla słownika
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|dexpression              |=| expression|
|                         | | \| dict_expression;|
|dict_expression          |=| dict_const, "(", [ dict_argument_list \| id, [",", dict_argument_list]], ")";|
|dict_argument_list       |=| "{}"|
|                         | | \| "{", dict_pair, {",", dict_pair}, "}";|
|dict_pair                |=| expression, ":", expression;|

### EBNF dla zapytań LINQ
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|linq_expression          |=| "select", "{", "from ", (variable_access \| dict_expression), ["where ", id], ["order by ", id], ["desc"], ["limit ", (int_const \| variable_access)], "}"|

### EBNF Id
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|parameter_list		      |=| id, {",", id};|
|id                       |=| letter, {letter \| digit \| "_"};|

### EBNF Funkcja @print
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|print_call               |=| "@print", "(", [print_content], ")";|
|print_content            |=| {text | interpolated_variable | double_brace};|
|text                     |=| not_brace_char, {not_brace_char};|
|interpolated_variable    |=| "{", [dexpression], "}";|
|double_brace             |=| "{{" || "}}";|


## EBNF Część leksykalna
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|num_const				  |=| int_const, [".", {digit}];|
|int_const                |=| zero_digit|
|                         | | \| non_zero_digit, {digit};|
|bool_const				  |=| "true" \| "false";|
|null_const               |=| "Null";|
|dict_const               |=| "Dict";|
|string_const			  |=| '"', {char}, '"';|
|char					  |=| digit \| not_digit;|
|not_brace_char           |=| letter \| digit \| not_brace_special_char;|
|digit                    |=| zero_digit \| not_zero_digit;|
|zero_digit				  |=| "0";|
|not_zero_digit			  |=| "1" \| "2" \| "3" \| ...;|
|not_digit				  |=| letter \| special_char;|
|special_char             |=| not_brace_special \| "{" \| "}";
|not_brace_special_char   |=| "$" \| "@" \| ...;|
|letter 				  |=| "a" \| ... \| "z" \| "A" \| ... \| "Z";|


## Podstawowe instrukcje języka

### Komentarz kodu

```
x = 10 # tutaj jest komentarz
```

##### Błąd przypisania zmiennej
```
x = '10;
print(x);
```

### Instrukcja warunkowa
#### Poprawne użycie instrukcji warunkowych
```
if(2 > 1){
    print(10) # 10
}
else{
    print(5)
}
```
```
if(2 > 1){
    if (3 > 2){
        print(10) # 10
    }
}
```
```
if(2 > 1 && 4 > 3){
        print(10) # 10
}
```
```
if(2 > 1 || 4 > 3){
        print(10) # 10
}
```
```
if(!(2 > 1)){
    print(10) #
}
```
```
if(!(true)){
    print(10) #
}
```
```
if(2 < 1){
    print(10) #
}
```
```
if(2 <= 1){
    print(10) #
}
```
```
if(2 > 1){
    print(10) # 10
}
```
```
if(2 >= 1){
    print(10) # 10
}
```
```
if(2 == 1){
    print(10) #
}
```
```
if(2 != 1){
    print(10) # 10
}
```


#### Przykładowe błędy instrukcji warunkowych
##### Brak nawiasu klamrowego
```
if(2 > 1){
    if (3 > 2) print(10)
}
```
##### Else nie odnosi się do żadnego if
```
else{
    print(5)
}
```
##### Napisanie warunku w instrukcji else
```
else(2>1){
    print(5)
}
```
##### Zanegowanie wartości nie-boolowskiej
```
if (!(2)){
    print(5)
}
```
```
if (!(0)){
    print(5)
}
```
```
if (!(1)){
    print(5)
}
```
##### Błąd porównania
```
x = 10;
y = 'ABC';
if(x > y)
{
    print(1);
}
```

##### Nizgodny typ w instukcji warunkowej
```
if (5){
    print(10)
}
```
```
if ("key"){
    print(10)
}
```
```
if ("true"){
    print(10)
}
```
```
if (Dict()){
    print(10)
}
```

### Pętle
#### Poprawne użycie pętli
```
x = 10
while(x > 0){
    print(x)
    x = x-3
}
```
```
x = 10
while(x > 0){
    y = 0
    while(y < x){
        print(y)
        y = y + 2
    }
    print(x)
    x = x-3
}
```
```
x = 10
while(true){
    print(x)
}
```
#### Przykładowe błędy pętli
##### Brak warunku
```
x = 10
while(){
    print(x)
    x = x-3
}
```
##### Zdefiniowanie wartości początkowej wewnątrz pętli
```
while((x = 5) < 8){
    print(x)
}
```

## Zakres zmiennych

Zmienne mogą być deklarowane w dowolnym miejscu programu, ale zostają one usunięte po opuszczeniu bloku kodu, w którym zostały utworzone (ich nazwy zostają zwolnione w pozostałej części kodu). <br/>
Nie istnieją zmienne globalne.

### Przypisanie wartości do zmiennej
|Zmienna       |Typ    |
|--------------|-------|
|x = 10        |Int    |
|y = 10.5      |Double |
|z = "string"  |String |
|t = true      |Bool   |
|m = Null      |Special|

### Wypisywanie wartości
```
x = 10
print(x) # 10
```
### Błąd wykonywania operacji na zmiennej typu Special (Null)
```
x = Null
print(x + 2)
```

### Konkatenacja łańcuchów znaków
```
str1 = "poczatek "
str2 = "koniec"
str3 = str + str2
print(str3) # poczatek koniec
print(str1 + str2) # poczatek koniec
print(str3 == str1 + str2) # true
```

### Dostępność zmiennych

```
x = 10
sum = x + 2
dif = x - 2
mul = x * 2

if(sum == 12)
{
    div = x / 2
    print(div) # 5
    print(x) # 10
}
print(div) # zmienna już nie istnieje
```
### Definiowanie własnej funkcji ze zwracaniem wartości oraz wywołanie

```
def func(x)
{
	return x * x
}

y = 10
result = func(y)
print(result) # 100
print(func(y)) # 100

```
### Błąd zmiennej globalnej wykorzystywanej w funkcji
```
y = 5
def func(x)
{
	print(y)
    return x * x
}
func(y)
```

### Rekurencja

```
def func(x)
{
	if(x > 10){
		func(x - 1)
	}
	return 5
}

print(func(8)) # 5

```

### Przekazywanie przez wartość
```
def func(x)
{
	x = x + 5
    return 5
}

y = 4
z = func(y)
print(y) # 4
print(z) # 9
```


### Kolejność wykonywania działań

|Obliczenia| Wynik      |
|----------|------------|
|x = (5 / 2) + 1 * 2 |  4 |
|y = 4 * 4 / 3 + 3 * (4 + 2) | 23|
|z = (2 + 2) / (1 + 1) | 2|

### Escaping stałych znakowych
```
print("Przykładowy tekst\""); # Przykładowy tekst" #
```

## Słowniki

### Zasady działania słownika

Słownik zawiera elementy złożone z klucza oraz wartości. Klucze muszą być unikalne.<br/>
Każdy element w słowniku musi mieć tę samą parę typów. Typ klucza może się różnić od typu wartości.<br/>
Możliwe typy kluczy oraz wartości to Int, String, Double, Bool.

### Tworzenie typu słownikowego

Pierwszym opcjonalnym argumentem słownika jest nazwa funkcji służącej do ustanawiania kolejności w słowniku.<br/>
Funkcja porównuje 2 elementy słownika i w ten sposób określa relację liniowo-porządkującą na nim. <br/>
Określa ona czy pierwszy z dwóch elementów ma się znaleźć przed drugim w porządku słownika.<br/>
Drugim opcjonalnym argumentem słownika jest zawartość słownika. <br/>
W przypadku remisu w funkcji porządkującej, zachowana zostaje kolejność dodawania elementów. <br/>

```
def compare(key1, value1, key2, value2){
    return key1 + value1 < key2 + value2
}

dict1 = Dict(compare, {1 : 1, 2: 2, 3 : -1}) # kolejność w słowniku -  1:1, 3:-1, 2:2
```

W przypadku gdy takiej funkcji się nie poda to kolejność w słowniku jest ustanawiana poprzez funkcję, która porównuje elementy w sposób domyślny:
```
def compare(key1, value1, key2, value2){
return key1 < key2
}
```
Kolejność dla typu String jest domyślnie leksykograficzna, dla typu Int zgodnie z kolejnością liczb całkowitych, dla typu Double zgodnie z kolejnością liczb rzeczywistych, a dla typu Bool zgodnie z kolejnością wartości logicznych (fałsz < prawda).


Słownik można utworzyć w takim razie na kilka sposobów.
1.  Tworzenie słownika pustego
```
dict1 = Dict()
```
2.	Tworzenie słownika wraz z podaniem początkowych elementów słownika
```
key1 = „key1”
value1 = 1
key2 = „key2”
value2 = 2
dict1 = Dict({key1 : value1, key2 : value2})
```
3.	Tworzenie słownika bez  podania początkowych elementów słownika ale z podaniem funkcji służącej do ustanawiania kolejności w słowniku
```
def compare(key1, value1, key2, value2){
    return key1 < key2
}

dict1 = Dict(compare)
```

4.	Tworzenie słownika wraz z podaniem początkowych elementów słownika oraz z podaniem funkcji służącej do ustanawiania kolejności w słowniku
```
def compare(key1, value1, key2, value2){
return key1 < key2
}

key1 = „key1”
value1 = 1
key2 = „key2”
value2 = 2
dict1 = Dict(compare, {key1 : value1, key2 : value2})
```



### Operacje na słowniku

1. print - wypisanie elementów ze słownika
```
dict1 = Dict({“key1” : 1, “key2” : 2})
dict1.print() # key1 : 1
              # key2 : 2
```
2.	add - dodawanie do słownika

Funkcja przyjmuje 2 argumenty. Pierwszym argumentem jest klucz, drugim wartość elementu, który chcemy dodać do słownika. <br/>
Możliwe typy argumentów zgodne z zasadami słownika.
```
dict1 = Dict()
dict1.add(„key1”, 1)
dict1.print() # key1 : 1
```

3. remove - usuwanie ze słownika

Funkcja przyjmuje klucz elementu do usunięcia. Klucz musi występować w słowniku. Inaczej wystąpi błąd. <br/>
Możliwe typy argumentu zgodne z zasadami słownika.
```
dict1 = Dict({“key1” : 1, “key2” : 2})
dict1.remove(„key1”)
dict1.print() # key2 : 2
```
```
dict1 = Dict()
dict1.add("key1", 1)
dict1.remove("key1")
dict1.add(2, "value2")
```

Kiedy utworzony słownik nie ma podanych początkowych jego elementów to następny dodany do niego element może mieć dowolny typ klucza i wartości. <br/>
Natomiast wtedy kolejny dodany element musi mieć ten sam typ klucza jak i wartości jak pierwszy dodany element.<br/>
Kiedy w trakcie wykonywania operacji na słowniku stanie się on w pewnym momencie pusty to wtedy będzie można dodać do niego element o dowolnym typie klucza i wartości.


4. get_value - wyszukiwanie elementów wg klucza

Funkcja jako argument przyjmuje klucz na takich samych zasadach jak remove.
```
dict1 = Dict({“key1” : 1, “key2” : 2})
print(dict1.get_value(„key1”)) # 2
```

5. exists - sprawdzanie, czy dany klucz znajduje się w słowniku

Funkcja jako argument przyjmuje klucz na takich samych zasadach jak remove.
```
dict1 = Dict({“key1” : 1, “key2” : 2})
print(dict1.exists(„key1”)) # true
```

6.	set - zmiana wartości o podanym kluczu

Argumenty tak samo jak przy dodawaniu przy czym klucz musi występować w słowniku, bo inaczej wystąpi błąd.
```
dict1 = Dict({“key1” : 1, “key2” : 2})
dict1.set("key1", 3)
dict1.print() # key1 : 3
              # key2 : 2
```

7.	size - sprawdzenie liczby elementów w słowniku
```
dict1 = Dict({“key1” : 1, “key2” : 2})
print(dict1.size()) # 2
```

8.  lower_bound - znajdowanie pierwszego niemniejszego elementu niż podany

Argumenty tak samo jak przy dodawaniu. <br/>
Gdy znajdziemy pierwszy niemniejszy element niż podany w słowniku według funkcji ustalania kolejności to zwracamy klucz tego elementu.

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})
print(dict1.lower_bound(“key2”, 1)) # key2
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})
print(dict1.lower_bound(“key2”, 1)) # key2
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})
print(dict1.lower_bound(“key4”, 1)) # Null
```

```
def compare(key1, value1, key2, value2){
return value1 < value2
}

dict1 = Dict(compare, {“key1” : 1, “key2” : 2, "key3" : 3})
print(dict1.lower_bound(“key3”, 1)) # key1

```

9.  upper_bound - znajdowanie pierwszego większego elementu niż podany

Analogicznie jak funkcja lower_bound tylko zwracany jest klucz pierwszego większego elementu.
```
dict1 = Dict({“key1” : 1, “key2” : 2, “key3” : 3})
print(dict1.upper_bound(“key2”, 1)) # key3
```
```
dict1 = Dict({“key1” : 1, “key3” : 3, “key4” : 4})
print(dict1.upper_bound(“key2”, 1)) # key3
```
```
dict1 = Dict({“key1” : 1, “key2” : 2, “key3” : 3})
print(dict1.upper_bound(“key4”, 1)) # Null
```



10. accumulate - akumulacja elementów zgodnie z podanym wzorem

Funkcja posiada 2 argumenty. <br/>
Pierwszy argument to wartość początkowa akumulacji (typ taki jak wartości w słowniku), a drugi to funkcja akumulująca.<br/>
Funkcja akumulująca musi mieć 3 argumenty:
- poprzednią wartość funkcji akumulującej
- klucz bieżącego elementu słownika
- jego wartość.
<br/><br/>

```
dict1 = Dict({“key1” : 1, “key2” : 2, “key3” : 3})

def sum(start, key, value){
    return start + value
}

dict1.accumulate(0, sum) # 6
```
```
dict1 = Dict({“key1” : 1, “key2” : 2, “key3” : 3})

def sum(start, key, value){
    if (start != ""){
        return start + " " + key
    }
    else{
        return key
    }
}

dict1.accumulate("", sum) # "key1 key2 key3"
```

11. iterate - iterowanie po elementach słownika i wykonywanie na każdym wskazaną funkcję przekazując do niej jako argumenty aktualne klucz, wartość oraz indeks (numer elementu słownika licząc od 1)
```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def fun(key, value, indeks){
    if (value > 1){
        return key + ":" + value
    }
}

print(dict1.iterate(fun)) # key2 : 2
                          # key3 : 3
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def fun(key, value, indeks){
    if (indeks < 3 && value > 1){
        return key + ":" + value
    }
}

dict1.iterate(fun).print() # key2 : 2
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3, "key4" : 4})

def fun(key, value, indeks){
    if (indeks < 4 && value > 1){
        return value
    }
    return 0
}

def sum(start, key, value){
    return start + value
}

results_dict = dict1.iterate(fun)
print(results_dict.accumulate(0, sum)) # 5
```

Tak to będzie zaimplementowane mniej więcej w C++
```
std::map<int, int> iterate(func wypisz){
    dict = {}
    int indeks = 0
    for ((key, value) : string1){
        dict[key] = wypisz(key, value, indeks)
        indeks = indeks + 1
    }
    return dict
}
```

// gdy funkcja powinna coś zwracać (przypisujemy gdzieś tą wartość) to jeśli nie zwróci to powinien być błąd, jeśli nie musi nic zwracać tylko printować to nie ma błędu<br/>
// Zapytać się co z białymi znakami zrobić<br/>


## Zapytania o słownik:

Zapytanie będzie zrealizowane poprzez funkcję select z
- obowiązkowym from, który przyjmuje jedynie typ słownikowy
- opcjonalnym where, który przyjmuje identyfikator funkcji filtrującej.
- opcjonalnym order by, który przyjmuje funkcję sortującą jako argument. Domyślnie sortowanie przebiega rosnąco po kluczach.
- opcjonalnym desc (malejąco),
- opcjonalnym przycięciem pierwszych wyników (instrukcja limit, po której może występować jedynie wartość typu Int).


```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3, "key4" : 4})

select
from dict1 # key1 : 1
           # key2 : 2
           # key3 : 3
           # key4 : 4
```
```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3, "key4" : 4})

def where_fun(key, value){
    return key > "key1" && value < 4
}

dict2 = select{
from dict1
where where_fun
desc
limit 1
}

dict2.print() # key3 : 3
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3, "key4" : 4})

def order_fun(key1, value1, key2, value2){
    return value2 > value1
}

def where_fun(key, value){
    return key > "key1" && value < 4
}

dict2 = select{
from dict1
where where_fun
order by order_fun
desc
}

dict2.print() # key3 : 3
              # key2 : 2

```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3, "key4" : 4})

def order_fun(key1, value1, key2, value2){
    return value2 > value1
}

def where_fun(key, value){
    return key > "key1" && value < 4
}

dict2 = select{
from dict1
where where_fun
order by order_fun
desc
limit 1
}

dict2.print() # key3 : 3
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3, "key4" : 4})

def order_fun(key1, value1, key2, value2){
    return value2 > value1
}

def where_fun(key, value){
    return key > "key1" && value < 4
}

x = 3
y = 2
def limit_fun(x, y){
    return x - y
}

dict2 = select{
from dict1
where where_fun
order by order_fun
desc
limit limit_fun(x, y)
}

dict2.print() # key3 : 3
```

```
dict1 = select{
from Dict({“key1” : 1, “key2” : 2, "key3" : 3, "key4" : 4})
}

dict2.print() # key3 : 3
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def fun(key, value, indeks){
    if (value > 1){
        return key + ":" + value
    }
}

dict2 = select{
from dict1.iterate(fun)
}

dict2.print() # key1 : key1 : 1
              # key2 : key2 : 2
              # key3 : key3 : 3
```
```
def fun (key, value){
    return Dict({key : value})
}

dict2 = select{
from fun("key1", 1)
}

dict2.print() # key1 : 1
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def iterate_fun(key, value, index){
    if (value > 1){
        return key + ":" + value.to_string()
    }
}

def from_fun (dict){
    return dict.iterate(iterate_fun)
}

dict2 = select{
from from_fun(dict1)
}

dict2.print() # key2 : key2 : 2
              # key3 : key3 : 3
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, “key3” : -1})

def sum(start, key, value){
    return start + value
}

dict2 = select{
from dict1
limit dict1.accumulate(0, sum)
}

dict2.print() # key1 : 1
              # key2 : 2
```

```
def compare(key1, value1, key2, value2){
    return key1 + value1 < key2 + value2
}

dict1 = Dict(compare, {1 : 1, 2: 2, 3 : -1}) # kolejność w słowniku -  1:1, 3:-1, 2:2

def sum(start, key, value){
    return start + value
}

dict2 = select{
from dict1
limit dict1.accumulate(0, sum) # Funkcja wyjaśniona później
}

dict2.print() # 1 : 1
              # 3 : -1
```

Pytanie:
Jesli porządek elementów w słowniku ma byc okreslony nie tylko na podstawie samych kluczy ale może być okreslony także na pdstawie dowolnych wyrażeń dla kluczy i wartości to w jaki sposób mamy zagwarantować efektywne operacje na słowniku skoro binarne przeszukiwanie opiera się na porównaniach które w tym przypadku potrzebowałyby znać wartość dla szukanego klucza. Przykładowo aby efektywnie dostać wartość klucza potrzebowalibyśmy najpierw znać tę wartość klucza, bo inaczej nie moglibyśmy dokonywac porównać z elementami które przeszukujemy.


