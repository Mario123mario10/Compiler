<style scoped>
table {
  font-family: Consolas;
}
</style>

# TKOM 24L

Mariusz Pakulski

Język programowania: C++

Opiekun projektu: mgr inż. Agnieszka Malanowska

## Temat

Język z wbudowanym typem słownika z określoną kolejnością elementów. <br/>
Kolejność elementów w słowniku jest określana w momencie jego tworzenia – argumentem konstruktora słownika jest funkcja określająca, w jakiej kolejności mają być wstawiane elementy.<br/>
Możliwe są podstawowe operacje na słowniku (dodawanie, usuwanie, wyszukiwanie elementów wg klucza, sprawdzanie, czy dany klucz znajduje się w słowniku itd.), iterowanie po elementach oraz wykonywanie na słowniku zapytań w stylu LINQ.

## Uruchomienie projektu
Aby uruchomić projekt należy z poziomu głównego katalogu projektu użyć komendy: ```./mario++ {ścieżka do pliku}```

## Założenia

- #### typowanie silne, dynamiczne (w dalszej części są przedstawione rezultaty takiego doboru typowania)
- #### mutowalność zmiennych (o tym jest później)
- #### implementacja w języku C++
- #### tworzenie i przypisywanie wartości zmiennych za pomocą znaku '='
```
x = 10
```
- #### wymóg deklaracji zmiennej wraz z inicjalizacją

Błędne jest wprowadzenie typu przed zmienną
```
Int x
x = 10
```

```
x           # To by spowodowało błąd - Nierozpoznany identyfikator x
x = 10
```
```
x = 10
x          # Gramatyka na to pozwala, wtedy nic się nie dzieje
```

- #### Obsługiwane typy:
  - liczbowe ```Int, Double```, operacje matematyczne zachowujące kolejność działań i nawiasowanie
  - znakowy ```String```, konkatenacja dwóch łańcuchów znakowych
  - logiczny ```Bool```, operacje logiczne
  - słownikowy ```Dict```, metody wywoływane po kropce
  - specjalny ```Null```, przechowuje wartości null

|Typ        |Zmienna     |   Zakres  |
|-----------|------------|-----------|
|Int        |x = 10      | od -2<sup>31</sup>+1 do 2<sup>31</sup>-1|
|Double     |y = 10.5    | od 1.7\*10<sup>-308</sup> do 1.7\*10<sup>308</sup>|
|String     |z = "string"| do 9\*10<sup>18</sup> znaków|
|Bool       |t = true    | true \| false |
|Null       |m = null    | null |
|Dict       |n = Dict()  | -|

Dalsze szczegóły związane z typami znajdują się w dalszej części dokumentacji.

- #### komentarze jedolinijkowe oznaczone znakiem ```#``` (rozpoczęcie w dowolnym miejscu, kończą się z końcem linii)
```
x = 5 # komentarz mówiący, że zmienna x przyjęła wartość 5
```
Poniższy komentarz zwróci błąd gdyż # wskazuje komentarz jednolinijkowy
```
x = 5 # komentarz mówiący,
       że zmienna x przyjęła wartość 5
```
- #### bloki kodu ograniczone przez nawiasy klamrowe

Znaki białe pomiędzy nawiasami klamrowymi nie mają wpływu na działanie nawiasu klamrowego
```
if (true){
    a = 1
}
```
```
if (true){a = 1}
```

- #### instrukcje:
    - warunkowa ```if``` z ```else```
    - pętli ```while```
    - sterujące ```return, break, continue```
- #### obsługa operatorów:
    - arytmetycznych ``` +,-,*,/,^ ```
    - porównujących ```<,>,<=,>=,==,!=```
    - logicznych ```||, &&, !```
    - dostępu ```.```
- #### leniwa tokenizacja
- #### moduły:
     - lekser (Rozpoznaje tokeny i przekazuje je do parsera)
     - parser (Buduje drzewa składniowe i przekazuje je do interpretera)
     - interpreter (Wykonuje kod na podstawie otrzymanych drzew składniowych, zarządza również błędami)
- #### gramatyka bezkontekstowa (po prawej stronie reguł może występować wiele symboli nieterminalnych)
- #### parser rekursywnie zstępujący (Drzewo wyprowadzenia jest budowane od korzenia do liści)
- #### wypisywanie zmiennych

W cudzysłowach znajduje się sformatowany tekst.
Tekst w klamrach oznacza wyrażenie, a poza klamrami znaki do dosłownego wypisania (pomijając kwestie escapowania).
Nawiasy klamrowe wyznaczają wartość wyrażenia, konwertują ją na string i wstawiają jego tekst w odpowiednie miejsce do sformatowanego tekstu instrukcji print.
Realizuję typowanie silne, więc na zmiennych musi być wykonana jawna konwersja na Stringa.  <br/>
Tą konwersje realizują nawiasy klamrowe.
W przypadku chęci wypisania znaku klamry otwierającej lub cudzysłowu należy przed nim wstawić znak "\".

### Poprawny zapis printa

```
print() #
```
```
a = 6.4
b = 6.6
c = -6.4
d = -6.6

print({e} + {f} + {g} + {h}) # 6 7 -6 -7
```
```
a = 1
b = 5
c = "a"
d = null
print({a} + {b} + {c} + {d}) # 1, 5, a, null
```

```
abc = "string"
print(abc) # string
```
```
abc = "string\""
print(abc) # string"
```

```
def print (a, b){
    print(a)
}
print(1, 2) # 1 (Nie ma błędu, ponieważ jest inna liczba argumentów niż przyjmuje funkcja wbudowana print)
```

### Niepoprawny zapis printa
```
6 print()
```
```
print
```
```
6 + print()
```

```
def print(a){ # Bład zdefiniowania drugi raz funkcji o tej samej nazwie i liczbie argumentów
    return a
}
print (1)
```

- #### możliwość definiowania własnych funkcji (wartość do funkcji przekazywana przez wartość)

Zakładam, że można wywołać return w dowolnym miejscu w kodzie. Wtedy dalsza część funkcji nie będzie się wykonywała. <br/>
Kiedy wywołamy return w głównej części programu to program się zakończy. <br/>
Funkcja będzie mogła zwracać null, zakładamy, że nie można zwracać wielu elementów po przecinku z funkcji. <br/>
Jeśli funkcja nic nie będzie zwracać, a będziemy chcieli coś z niej otrzymać to będziemy otrzymywać bład NoValueReturned

```
a = 5
def fun(a){
    a = a + 5
    return 10
}

b = fun(a)
print ({b}) # 10
print ({a}) # 5
```

```
a = 5
return a # zostanie zwrócona wartość a i zakończy się program
print ({a}) # nie zostanie wypisana wartość a
```
```
def fun(a){
    return a # zostanie zwrócona z funkcji wartość 1
    print ({a + 5}) # nie zostanie wypisana wartość 6
}
print(fun(1)) # 1
```
```
def fun(){
    return null
}

print (fun()) # null
```
```
x = 5
def fun(a){
    print ({a})
}

fun(x)     # 5
y = fun(x) # NoValueReturned: Function "fun" does not return a value in line 7, column 1
```

- #### rekurencja jest dozwolona

```
def func(x)
{
	if(x > 10){
        print(x) # 12 11
        func(x - 1)
	}
	return x
}

print({func(12)}) # 12
```

```
def func(x)
{
	if(x < 10){
		y = func(x + 1)
        print ({y}) # 10 9
	}
	return x
}

print ({func(8)}) # 8
```
```
def func1(x){
    if (x > 10){
        return true
    }
    return func2(x^2)
}

def func2(x){
    if (x <= 1){
        return false
    }
    return func1(x-3)
}

func1(1) # false
func1(2) # false
func1(3) # true
```
- #### dostępność funkcji w każdym miejscu programu (j.w.)

- #### funkcje można wywoływać z argumentami o dowolnych typach
```
def fun(x){
    print ({x}) # string
}               # 1
                # true
                # null
fun("string")
fun(1)
fun(true)
fun(null)
```

- #### przeciążanie funkcji jest dozwolone tylko wtedy jeśli funkcje mają różną ilość argumentów
```
fun(a, b, c)
fun(a, b, c, d)
```
- #### nie można stworzyć funkcji wewnątrz innej funkcji
```
def fun1(){
    def fun2()  # błąd składniowy
}
```

- #### priorytety operatorów:

| operator         | priorytet | łączność        |
|------------------|-----------|------------------|
| `.`              | 8         | lewostronna      |
| `()`             | 8         | lewostronna      |
| `!`              | 7         | prawostronna     |
| `-` (unarny)     | 7         | prawostronna     |
| `^`              | 6         | prawostronna     |
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
| `\|\|`           | 1         | lewostronna      |
| `=`              | 0         | -                |

- #### konwersja typów:

|        |   Int    | Double     | String     |  Bool    | Dict  | Null|
|--------|----------|------------|------------|----------|-------|--------|
|Int     | X        | to_double()| to_string()| to_bool()| X     | X      |
|Double  | to_int() | X          | to_string()| X        | X     | X      |
|String  | to_int() | to_double()| to_string()| to_bool()| X     | X      |
|Bool    | to_int() | X          | to_string()| X        | X     | X      |
|Dict    | X        | X          | to_string()| X        | X     | X      |
|Null    | X        | X          | to_string()| X        | X     | X      |

|Typ konwertowany   | Typ po konwertowaniu          |    Opis |
|----------------------|----------|-|
|Int    | Double  | Część całkowita powstałego doubla będzie zawierała wartość zmiennej typu Int, natomiast częścią ułamkową będzie 0 np. 3.0
|       | Bool    | Wartość 0 zamieni się na true, wartość 1 zamieni się na false, reszta wartości nie może zostac zamieniona na typ Bool
|Double | Int     | Zaokrąglenie modułu do najbliższej liczby całkowitej|
|String | Int     |         Można zamienić string na zmienną typu Int tylko kiedy ten string będzie zawierał jedynie cyfry |
|       | Double  |      Można zamienić string na zmienną typu Double tylko kiedy początek stringa będzie zawierał jedynie cyfry po których nastąpi znak kropki z przynajmniej jedną cyfrą następującą
|       | String  | Nic się nie zmieni|
|       | Bool    | Można zamienić string na zmienną typu Double kiedy wartością w stringu jest true lub false|
|Bool   | Int     | Wartość true zamieni się na 1, false na 0, nie można zamienić innych wartości na typ Int|
|Dict   | String  | Powstanie string złożony z kluczy oraz wartości słownika przedzielonych dwukropkiem np.<br/> {key1 : 1, key2 : 2} <br/>
|Null   | String  | Powstałą wartością będzie null|

### Przykłady konwersji

```
a = 6
b = a.to_double()
print ({b}) # 6.0
```
```
a = false
b = true
c = a.to_double()
d = b.to_double()
print ({c} + {d}) # 0.0, 1.0
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

print ({e} + {f} + {g} + {h}) # 6 7 -6 -7
```
```
a = "key1"
b = 1
c = "key2"
d = 2
dict1 = Dict(a : b, c : d)

print ({dict1}) # {key1 : 1, key2 : 2}
```



- #### akceptowane kombinacje typów dla operatorów wieloargumentowych

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

Dla kropki po lewej stronie może być Int, Double,  String, Bool, Dict, Null. Po prawej stronie będzie wywoływana metoda. <br/>
Dla = zmienna po lewej stronie ustali swój typ dynamicznie na podstawie wartości po prawej stronie. <br/>

Można nadpisać zmienną wartością innego typu:
```
a = 5
a = "string"
```


- #### Obsługa błędów

Przykładowe rodzaje błędów:

#### Lekser:
- LiteralOverflow 	- literał przekracza zakres liczbowy (Literal Overflow in line {line}, column {column})
```
a = 100000000000  # LiteralOverflow: Literal Overflow in line 1, column 1
```
- UnknownToken 		- token jest nierozpoznany (Unknown token {name} in line {line}, column {column})
```
§  # UnknownToken: Unknown token "§" in line 1, column 1
```
- StringTooLong 	- ciąg znaków jest za długi (String too long in line {line}, column {column})

```
    12345678901234
1   a = "aaaa..." # (a napisane 9*10^19 razy) StringTooLong: String too long in line 1, column 5
```

- InfiniteString 	- brak kończącego cudzysłowia dla typu string (String without end startin in line {line}, column {column})
```
a = "a
```

#### Parser
- UnexpectedToken 	- tokeny w niepoprawnej kolejności względem przyjętej gramatyki (Unexpected Token {string} in line {line}, column{column})
```
    1234567890123
1   if {a, b}{          # UnexpectedToken: Unexpected Token "{" in line 1, column 4
2       print ({a})
3   }
```
- DuplicateDefinition - ponowna definicja funkcji o tej samej nazwie (Duplicate function {name} in line {line}, column {column})
```
    123456789012345
1   def fun(a, b){}
2   def fun(a, b){} # DuplicateDefinition: Duplicate function fun in line 2, column 1
```

- ExpectingIdentifier - oczekiwano identyfikatora, a otrzymano inny symbol (Expecting identifier in line {line}, column {column})
```
    123456
1   a = 5
2   2b = a  # ExpectingIdentifier: Expecting identifier in line 2, column 1
```

- ExpectingExpression - oczekiwano wyrażenia, a otrzymano inny symbol (Expecting expression in line {line}, column {column})
```
    12345678901234
1   def fun(a){
2       return a
3   }
4   fun(2b)         # ExpectingExpression: Expecting expression in line 4, column 1
```
- MissingEndingBrace - brak nawiasu klamrowego zamykającego (Missing ending brace in line {line}, column {column})
```
     1234567890123456
1    def fun(){         # MissingEndingBrace: Missing ending brace in line 1, column 10
2        return true
3
4    print ({fun()})
```

#### Interpreter
- DivisionByZero - dzielenie przez zero (Division by zero try in line {line}, column {column})
```
    123456789
1   x = 5 / 0  # DivisionByZero: Division by zero try in line 1, column 5
```
- UndefinedVariable - użyto zmiennej, która nie została jeszcze zadeklarowana (No variable {name} in scope or not defined in line {line}, column {column})
```
    1234567890123456
1   y = 5
2   def func(x)
3   {
4 	    print ({y})   # UndefinedVariable: No variable "y" in scope or not defined in line 4, column 13
5       return x * x
6   }
7   func(y)
```

- WrongType - niezgodność typów (Operation between types {String} and {Int} is not allowed in line {line}, column {column})
```
     123456789012
1    x = "a"
2    y = 1
3    print ({x + y})   # WrongType: Operation between types String and Int is not allowed in line 3, column 9
```

- NotExactArguments - funkcja wywołana z nieprawidłową liczbą argumentów (Not exact number of arguments in line {line}, column {column})
```
     123456789012
1    def fun(a){
2        return a
3    }
4    fun() # NotExactArguments: Not exact number of arguments in line 4, column 1
```


- WrongFunction - funkcja przekazana jako argument przyjmuje niewłaściwą liczbę argumentów (Passed function does not take the required number of arguments in line (Not exact number of arguments in line {line}, column {column})

```
     123456789012345678901234567
1    def fun(key, index){
2        return key
3    }
4    dict1 = Dict({"key1" : 1})
5    dict2 = dict1.iterate(fun)     # WrongFunction: Not exact number of arguments in line 1, column 23
```

- FunctionNotFound - funkcja o takiej nazwie nie istnieje (Function not found {name} in line {line}, column {column})

```
     123456789
1    a = fun()  # FunctionNotFound: Function not found "fun" in line 1, column 5
```

- Overflow - wykonywanie operacji arytmetycznych powoduje przekroczenie dopuszczalnego zakresu dla danego typu (Overflow in line {line}, column {column})

```
     123456789012345
1    a = 2^27 * 2^27  # Overflow: Overflow in line 1, column 5
```
- NoValueReturned - nie można przypisać wyniku wywołania fukcji nic nie zwracającej do zmiennej <br/>
(NoValueReturned: Function {funkction_name} does not return a value in line {line}, column {column})
```
    1234567890123456789012345
1   x = 5
2   def fun(a){
3       print ({a})
4   }
5
6   fun(x)     # 5
7   y = fun(x) # NoValueReturned: Function "fun" does not return a value in line 7, column 1
```

- #### Testowanie
Projekt zamierzam testować zgodnie z popularną konwencją:
- Testy jednostkowe - do poszczególnych modułów
- Testy integracyjne - sprawdzające połączenia między modułami
- Testy akceptacyjne - działanie końcowego programu, będę one rozszerzeniem kodu zawartego w dokumentacji wstępnej, mają one na celu sprawdzenie wszystkich podstawowych założeń języka takich jak m.in.:
  - tworzenie zmiennych
  - instrukcja warunkowa i pętla
  - operatory arytmetyczne
  - rekursja
  - kolejność wykonywania działań
  - tworzenie słowników i działanie na nich
  - konkatenacja stringów
  - przekazywanie zmiennych przez wartość

Do testów akceptacyjnych użyję w szczególności fragmentów kodu zawartych w dokumentacji.


## Gramatyka

### Część składniowa (EBNF)
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|program                  |=| {function_definition \| statement};|
|function_definition      |=| "def ", id, "(", [parameter_list], ")", block;|
|parameter_list		      |=| id, {",", id};|
|block                    |=| "{", {statement}, "}"|
|statement                |=| variable_access|
|                         | |   \| if_statement|
|                         | |   \| while_statement|
|                         | |   \| assignment|
|                         | |   \| "break"|
|                         | |   \| "continue"|
|                         | |   \| return_statement|
|assignment               |=| id, "=", expression;|
|variable_access 		  |=| (function_call \| dict_statement \| linq_expression), {".", function_call};|
|function_call            |=| id, ["(", [argument_list], ")"];|
|argument_list			  |=| expression, {",", expression};|
|if_statement             |=| "if", "(", expression, ")", block, ["else", block];|
|while_statement          |=| "while", "(", expression, ")", block;|
|return_statement         |=| "return ", [expression];|
|| &nbsp;||
|interpolated_value       |=| "{", expression, "}";|
|| &nbsp;||
|dict_statement           |=| ["<", id, ">"], dict_argument_list;|
|dict_argument_list       |=| "{", [dict_pair, {",", dict_pair}], "}";|
|dict_pair                |=| expression, ":", expression;|
|| &nbsp;||
|linq_expression          |=| "select", "{", "from ", variable_access, ["where ", id], ["order by ", id], ["desc"], ["limit ", expression], "}";|


### Expression (EBNF)
|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|expression			      |=| or_term , {"\|\|", or_term};|
|or_term				  |=| and_term, {"&&", and_term};|
|and_term				  |=| comparison|
|                         | | \| "!", "(", comparison, ")" |
|                         | | \| "!", numeric_term |
|comparison			      |=| additive_expression, [("==" \| "!=" \| ">=" \| "<=" \| "<" \| ">"), additive_expression];|
|additive_expression      |=| multiplicative_expression, {("+" \| "-"), multiplicative_expression};|
|multiplicative_expression|=| factor, {("*" \| "/"), factor};|
|factor				      |=| exponent_factor, {"^", exponent_factor};|
|exponent_factor		  |=| [-], numeric_term;|
|numeric_term			  |=| num_const|
|                         | | \| bool_const|
|                         | | \| null_const|
|                         | | \| string_const|
|                         | | \| interpolated_value|
|                         | | \| "(", expression, ")"|
|                         | | \| variable_access;|

### Część leksykalna (wyrażenia regularne)

|Polecenie                | |                 Definicja|
|-------------------------|-|---------------------------------------------------------------------------------------------------------|
|int_const                |=| 0\|[1-9][0-9]\*                 |
|double_const		      |=| (0\|[1-9][0-9]\*)(\\.[0-9]+)?   |
|bool_const				  |=| true\|false                     |
|null_const               |=| null                            |
|string_const			  |=| "[[^"]\|\\\\"]*"                |
|id                       |=| [A-Za-z_][A-Za-z_0-9]*          |
|symbol                   |=| [-+\*/^\(\)\{\},:\.]\|==?\|!=?\|<=?\|>=?\|&&\|\\\|\\\| |
|comment                  |=| #[^\$]*\$                       |


## Podstawowe konstrukcje języka

### Komentarz kodu

```
x = 10 # tutaj jest komentarz
```

### Instrukcja warunkowa
#### Poprawne użycie instrukcji warunkowych
```
if(2 > 1){
    print ({10}) # 10
}
else{
    print ({5})
}
```
```
if(2 > 1){
    if (3 > 2){
        print ({10}) # 10
    }
}
```
```
if(2 > 1 && 4 > 3){
        print ({10}) # 10
}
```
```
if(2 > 1 || 4 > 3){
        print ({10}) # 10
}
```
```
if(!(2 > 1)){
    print ({10}) #
}
```
```
if(!(true)){
     print ({10}) #
}
```
```
if(!true){
     print ({10}) #
}
```
```
if(2 < 1){
     print ({10}) #
}
```
```
if(2 <= 1){
     print ({10}) #
}
```
```
if(2 > 1){
    print ({10}) # 10
}
```
```
if(2 >= 1){
    print ({10}) # 10
}
```
```
if(2 == 1){
    print ({10}) #
}
```
```
if(2 != 1){
    print ({10}) # 10
}
```


### Przykładowe błędy instrukcji warunkowych
#### Brak nawiasu klamrowego
```
if(2 > 1){
    if (3 > 2) print({10})
}
```
#### Else nie odnosi się do żadnego if
```
else{
    print ({5})
}
```
#### Napisanie warunku w instrukcji else
```
if (3>2){
    print ({10})
}
else(2>1){
    print ({5})
}
```
#### Zanegowanie wartości nie-boolowskiej
```
if (!(2)){
    print ({5})
}
```
```
if (!(0)){
    print ({5})
}
```
```
if (!(1)){
    print ({5})
}
```
#### Błąd porównania
```
x = 10;
y = 'ABC';
if(x > y)
{
    print ({1});
}
```

#### Niezgodny typ w instukcji warunkowej
```
if (5){
    print ({10})
}
```
```
if ("key"){
    print ({10})
}
```
```
if ("true"){
    print ({10})
}
```
```
if (Dict()){
    print ({10})
}
```

### Pętle
#### Poprawne użycie pętli
```
x = 7
while(x > 0){
    print ({x}) # 7
    x = x-3  # 4
}            # 1
```
```
x = 10
while(x > 0){
    y = 0
    while(y < x){
        print ({y})    # 0   # 0   # 0   # 0
        y = y + 8   # 8
    }
    print ({x})        # 10  # 7   # 4   # 1
    x = x-3
}
```
```
x = 10
while(true){
    print ({x})  # 10
}             # 10 ...
```
### Przykładowe błędy pętli
#### Brak warunku
```
x = 10
while(){
    print ({x})
    x = x-3
}
```
#### Zdefiniowanie wartości początkowej wewnątrz pętli
```
while((x = 5) < 8){
    print ({x})
}
```

### Tworzenie zmiennych

#### Przypisanie wartości do zmiennej
|Zmienna       |Typ    |
|--------------|-------|
|x = 10        |Int    |
|y = 10.5      |Double |
|z = "string"  |String |
|t = true      |Bool   |
|m = null      |Null   |

Typ Null określa brak wartości. Jedyną wartością o typie Null może być wartość null. <br/>
Przydaje się kiedy nie chcemy nic zwracać z funkcji oraz przy metodach na słownikach lower_bound oraz upper_bound o których później będzie mowa.

#### Wypisywanie wartości
```
x = 10
print ({x}) # 10
```
#### Błąd wykonywania operacji na zmiennej typu Null (null)
```
x = null
print ({x + 2})
```

#### Konkatenacja łańcuchów znaków
```
str1 = "poczatek "
str2 = "koniec"
str3 = str + str2
print ({str3}) # poczatek koniec
print ({str1 + str2}) # poczatek koniec
print ({str3 == str1 + str2}) # true
```

### Zakres zmiennych

Nie istnieją zmienne globalne.
Każde wywołanie funkcji posiada własny zbiór zmiennych (przekazane jako parametry oraz utworzone wewnątrz bloku funkcji).<br/>
Główna (poza-funkcyjna) część programu także posiada własny zbiór zmiennych (są to wszystkie zmienne utworzone na zewnątrz funkcji). <br/>
Zmienne utworzone wewnątrz bloku kodu (w klamrach) od momentu utworzenia będą istnieć także poza tym blokiem w ramach danej funkcji (lub w ramach głównej części programu jeśli zostały utworzone poza funkcjami). <br/>
Poza tym nie istnieje przykrywanie zmiennych. <br/>

```
x = 10
sum = x + 2
dif = x - 2
mul = x * 2

if(sum == 12)
{
    div = x / 2
    print ({div}) # 5
    print ({x}) # 10
    x = 7    # modyfikacja zmiennej sprzed ifa (nie tworzymy nowej, która by ją przykryła)
}

print ({div}) # 5
print ({x}) # 7
```

#### Definiowanie własnej funkcji ze zwracaniem wartości oraz wywołanie

```
def func(x)
{
	return x * x
}

y = 10
result = func(y)
print ({result}) # 100
print ({func(y)}) # 100

```
#### Błąd zmiennej globalnej wykorzystywanej w funkcji
```
y = 5
def func(x)
{
	print ({y})   # UndefinedVariable:  No variable "y" in scope or not defined in line 4, column 11
    return x * x
}
func(y)
```
#### Błąd wykorzytywania zmiennej z funkcji
```
def func(x)
{
	x = x + 5
    return x
}

y = 4
z = func(y)
print ({y}) # 4
print ({z}) # 9

print ({x}) # Błąd : # UndefinedVariable:  No variable "x" in scope or not defined in line 12, column 7
```

#### Mutowalność zmiennych

W przypadku chęci nadpisania zmiennych w tym samym bloku kodu można to zrobić poprzez nadanie nowej wartości tej samej nazwie zmiennej. <br/>
Nowa wartość może być innego typu, ponieważ tworzona jest zupełnie nowa zmienna, a poprzednia o tej samej nazwie jest zapominana.

```
x = 5
x = 5.8
x = "string"
x = false
x = null
x = Dict()
```

```
x = 5
def fun(x){
    print ({x}) # 5
    return x + 5
}

print ({x}) # 5
x = fun(x)
print ({x}) # 10
```

### Kolejność wykonywania działań

|Obliczenia| Wynik      |
|----------|------------|
|x = (5 / 2) + 1 * 2 |  4 |
|y = 4 * 4 / 3 + 3 * (4 + 2) | 23|
|z = (2 + 2) / (1 + 1) | 2|
|t = 4 ^ 3 ^ 2 | 262 144 (4^9)|


## Słowniki

### Zasady działania słownika

Słownik zawiera elementy złożone z klucza oraz wartości. Klucze muszą być unikalne.<br/>
Każdy element w słowniku musi mieć tę samą parę typów. Typ klucza może się różnić od typu wartości.<br/>
Możliwe typy kluczy oraz wartości to Int, String, Double, Bool, Dict.

### Tworzenie typu słownikowego

Słownik można tworzyć na 2 sposoby.<br/>
Pierwszy sposób polega na wywołaniu konstruktora klasy Dict, a drugi na użyciu operatorów.<br/>
W pierwszym, standardowym sposobie tworzenia słownika pierwszym opcjonalnym jego argumentem jest nazwa funkcji służącej do ustanawiania kolejności w słowniku.<br/>
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


Konstruktora można użyć na kilka sposobów:
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

Słownik można też utworzyć używając operatorów. Jest to zupełnie poprawne, ale niestandardowe podejście. <br/>
Zmieniona jest tylko zapis tworzenia słownika. <br/>
Nie ma żadnego słowa kluczowego oraz nazwa funkcji służącej do ustanawiania kolejności w słowniku znajduje się w nawiasach trójkątnych zamiast być argumentem funkcji.

1.  Tworzenie słownika pustego
```
dict1 = {}
```
2.	Tworzenie słownika wraz z podaniem początkowych elementów słownika
```
key1 = „key1”
value1 = 1
key2 = „key2”
value2 = 2
dict1 = {key1 : value1, key2 : value2}
```
3.	Tworzenie słownika bez podania początkowych elementów słownika ale z podaniem funkcji służącej do ustanawiania kolejności w słowniku
```
def compare(key1, value1, key2, value2){
    return key1 < key2
}

dict1 = <compare>{}
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
dict1 = <compare>{key1 : value1, key2 : value2}
```

### Operacje na słowniku

1. print - wypisanie elementów ze słownika
```
dict1 = Dict({“key1” : 1, “key2” : 2})
dict1.print() # key1 : 1
              # key2 : 2
```
```
dict1 = Dict({{"key11" : 1, "key12" : 2} : 1, {"key21" : 1, "key22" : 2} : 2})
dict1.print() # {"key11" : 1, "key12" : 2} : 1
              # {"key21" : 1, "key22" : 2} : 2
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
print ({dict1.get_value(„key1”)}) # 2
```

5. exists - sprawdzanie, czy dany klucz znajduje się w słowniku

Funkcja jako argument przyjmuje klucz na takich samych zasadach jak remove.
```
dict1 = Dict({“key1” : 1, “key2” : 2})
print ({dict1.exists(„key1”)}) # true
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
print ({dict1.size()}) # 2
```

8.  lower_bound - znajdowanie pierwszego niemniejszego elementu niż podany

Argumenty tak samo jak przy dodawaniu. <br/>
Gdy znajdziemy pierwszy niemniejszy element niż podany w słowniku według funkcji ustalania kolejności to zwracamy klucz tego elementu.

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})
print ({dict1.lower_bound(“key2”, 1)}) # key2
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})
print ({dict1.lower_bound(“key2”, 1)}) # key2
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})
print ({dict1.lower_bound(“key4”, 1)}) # null
```

```
def compare(key1, value1, key2, value2){
return value1 < value2
}

dict1 = Dict(compare, {“key1” : 1, “key2” : 2, "key3" : 3})
print ({dict1.lower_bound(“key3”, 1)}) # key1

```
```
dict1 = Dict(compare, {“key1” : 1, “key2” : 2, "key3" : 3})
a = dict1.lower_bound(“key3”, 1))
print ({a}) # key1
```


9.  upper_bound - znajdowanie pierwszego większego elementu niż podany

Analogicznie jak funkcja lower_bound tylko zwracany jest klucz pierwszego większego elementu.
```
dict1 = Dict({“key1” : 1, “key2” : 2, “key3” : 3})
print ({dict1.upper_bound(“key2”, 1)}) # key3
```
```
dict1 = Dict({“key1” : 1, “key3” : 3, “key4” : 4})
print ({dict1.upper_bound(“key2”, 1)}) # key3
```
```
dict1 = Dict({“key1” : 1, “key2” : 2, “key3” : 3})
print ({dict1.upper_bound(“key4”, 1)}) # null
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

11. iterate - iterowanie po elementach słownika i wykonywanie na każdym wskazaną funkcję przekazując do niej jako argumenty aktualne klucz, wartość oraz indeks <br/>
(numer elementu słownika licząc od 1)

Funkcja zwraca słownik którego klucze odpowiadają kluczom elementów ze słownika bazowego, dla których funkcja zwraca pewien wynik. <br/>
Wartości zwróconego słownika odpowiadają wynikom funkcji iterującej dla tych elementów, dla których zostały zwrócone jakieś wyniki.
```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def fun(key, value, indeks){
    if (value > 1){
        return value
    }
}

print ({dict1.iterate(fun)}) # key2 : 2
                             # key3 : 3
```

```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def fun(key, value, indeks){
    if (indeks < 3 && value > 1){
        return value + 10
    }
}

dict1.iterate(fun).print() # key2 : 12
```
```
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def fun(key, value, indeks){
    if (indeks < 3 && value > 1){
        return key + " : " + value
    }
}

dict1.iterate(fun).print() # key2 : key2 : 2
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
print ({results_dict.accumulate(0, sum)}) # 5
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
dict1 = Dict({“key1” : 1, “key2” : 2, "key3" : 3})

def fun(key, value, indeks){
    if (value > 1){
        return value + 10
    }
}

dict2 = select{
from dict1.iterate(fun)
}

dict2.print() # key2 : 12
              # key3 : 13

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
        return value
    }
}

def from_fun (dict){
    return dict.iterate(iterate_fun)
}

dict2 = select{
from from_fun(dict1)
}

dict2.print() # key2 : 2
              # key3 : 3
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
limit dict1.accumulate(0, sum)
}

dict2.print()   # 1 : 1
                # 3 : -1
```

