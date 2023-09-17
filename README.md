<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Repositorio de (Joaquin Vargas) - (104323) - (jvargas@fi.uba.ar)

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c ejemplo.c -o ejemplo
```

- Para ejecutar:

```bash
./ejemplo ejemplos/correcto.txt Pikachu Latigo
```

- Para ejecutar con valgrind:
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes ./ejemplo ejemplos/correcto.txt Pikachu Latigo
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

### Por ejemplo:

El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).

## Introducción 

El presente trabajo practico tiene como finalidad leer la máxima cantidad de pokemones posibles de un archivo y poder operar con los mismos. En este sentido se declararon previamente dos tipos de datos encapsulados llamados `pokemon_t` e `info_pokemon_t`.  

## Diseño

Para llevar a cabo este trabajo se tuvo en cuenta lo siguiente:

Considerando que se leerán pokemones, se definió a la estructura pokemon_t como una que posee:

- una cadena, de largo fijo, para el nombre.
- un enumerativo para representar el tipo.
- un vector ataque de 3 estructuras ataques. 

Ahora bien, el archivo en principio esta pensado como una tira de pokemones, es decir que se debe extraer la mayor cantidad posible, según lo pedido. Como el número de pokemones a leer es arbitraria y se pide que en algún momento sean ordenados, se decidió que la estructura info_pokemon_t, quien contendrá a dichos personajes, este conformada por:

- un pokemon_t **: Esto nos permitirá tratarlo como un vector de punteros a pokemon_t . La ventaja radica en que al momento de ordenarlos, las copias que se realizan en el proceso sean entre punteros a pokemon_t * y no entre las mismas estructuras. 
- un size_t para indicar la cantidad de pokemones contenidos. 

Resultando asi las estrucutras previamente declaradas:

```c
struct pokemon {
    char nombre[MAX_NOMBRE];
    enum TIPO tipo;
    struct ataque ataques[MAX_ATAQUES];
};

struct info_pokemon {
    pokemon_t **pokemones;
    size_t cantidad_pokemones;
};
```
Con la utilización de un puntero doble se impone el uso de memoria dinámica.