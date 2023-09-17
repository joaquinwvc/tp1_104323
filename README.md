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
En resumen, `pokemon_t` representa a un pokemon que posee nombre, tipo y 3 ataques. A su vez, `info_pokemon_t` se considera como un vector dinámico de punteros a `pokemon_t` con un índice que indica la cantidad de pokemones que contiene. El contrato para el primero es que de existir `pokemon_t` tiene que ser uno con todos sus campos completos y válidos. Para el segundo, `info_pokemon_t`, `cantidad_pokemones` indica el número de elementos que posee `pokemon_t **` y en caso de estar vacio se setean en 0 y null, respectivamente. 

Con esta implementación de los tipos de datos encapsulados las siguientes primitivas quedan inmediatamente definidas:
-	`pokemon_cantidad`
-	`pokemon_nombre`
-	`pokemon_tipo`
-	`pokemon buscar_ataque`
-	`con_cada_pokemon`
-	`con_cada_ataque`
-	`pokemon_destruir_todo`

Esto nos lleva a implementar desde cero la lectura y carga de los pokemones. Considerando las validaciones pertinentes y lo que se debe devolver en caso de alguna falla se pensó lo siguiente. Se detallan algunas consideraciones especiales para entender el comportamiento de la función.

En el TP0 se implementaron las funciones de parseo desde un cadena de chars. Ahora esa misma cadena será proporcionada por `fgets`. La lectura será conseguida mediante un ciclo while hasta la primera falla de la función mencionada . Además, se sabe que los pokemones vienen ordenados como, nombre y tipo, y luego, tres ataques seguidos con sus correspondientes campos. Lo que se hace en cada lectura completa de un pokemon (nombre, tipo y ataques), ya sea con los datos correctos o incorrectos, es setearlos en un `pokemon_t` declarado en el stack. 

```c
char buffer[MAX_BUFFER];
while (fgets(buffer, MAX_BUFFER, archivo) != NULL) {
		pokemon_t pokemon_aux;

	if (parsear_nombre_tipo(buffer, &pokemon_aux) == false)
			break;

    int cant_ataques_completados = 0;

	for (size_t i = 0; i < MAX_ATAQUES; i++) {
		if (fgets(buffer, MAX_BUFFER, archivo) == NULL)
			break;

		if (parsear_ataque(buffer, pokemon_aux.ataques + i) == false)
				break;

        cant_ataques_completados++;
    }
}
```

Las funciones de parseo están realizadas con el fin de devolver false únicamente en caso de no conseguir algún dato. Pero si la falla está en alguno de los tres ataques, se utiliza la variable `cant_ataques_completados` para corroborar que se hayan leído correctamente la cantidad esperada.

```c
if (cant_ataques_completados != MAX_ATAQUES)
    break;
```

A continuación, se explica el motivo de completar un pokemon en el stack. 
Solamente si la lectura del pokemon fue exitosa, se procede a reservar memoria para un `pokemon_t`. De esta forma, se aprovecha la fantástica característica de las copias de las estructuras. Si no existió algún inconveniente al completar el pokemon del Stack, se copia todo en el reservado del Heap. 

```c
pokemon_t *pokemon = pokemon_crear();
    if (pokemon == NULL)
		break;
*pokemon = pokemon_aux
```

Se puede lograr así que el agrandamiento del vector de `pokemon_t *` se realice únicamente si el pokemon leído es válido. La función que lo lleva a cabo es `info_pokemon_agregar_final` que como su nombre lo indica agrega el pokemon reservado en el Heap anteriormente al vector de `ip`.

```c
if (info_pokemon_agregar_final(ip, pokemon) == false) {
	pokemon_destruir(pokemon);
	break;
}
```

La representacion en memoria es la siguiente:

<div align="center">
<img width="100%" src="img/distribucion en memoria.svg">
</div>

Si bien no se impone algún ordenamiento al definir el tda `info_pokemon_t`, en la función `pokemon_cargar_archivo` se decide realizarlo mediante el método burbujeo.

## Funcionamiento

El flujo general de la funcion `pokemon_cargar_archivo` es expone a continuacion. El requerimiento que posee es que devuelva `NULL` unicamente en caso de falla en la apertura de archivo, fallo en la reserva de memoria del contenedor de pokemones o si la misma se encuentra vacia. En cualquier otro caso, se devuelve la mayor cantidad de pokemones validos leidos.

<div align="center">
<img width="100%" src="img/diagrama_flujo.svg">
</div>
