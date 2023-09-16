#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>

#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#define MAX_NOMBRE 50
#define MAX_ATAQUES 3

#define STR_AUX 50

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo;
	struct ataque ataques[MAX_ATAQUES];
};

struct info_pokemon {
	pokemon_t **pokemones;
	size_t cantidad_pokemones;
};

//--------------------pokemon-------------------------------------------------------

pokemon_t *pokemon_crear()
{
	pokemon_t *pokemon = malloc(sizeof(pokemon_t));
	if (pokemon == NULL)
		return NULL;

	return pokemon;
}

void pokemon_destruir(pokemon_t *pokemon)
{
	free(pokemon);
}

bool pokemon_comparar_nombre(pokemon_t *pokemon, const char *nombre)
{
	if (pokemon == NULL)
		return false;

	if (strcmp(pokemon->nombre, nombre) != 0)
		return false;

	return true;
}

//--------------------info_pokemon--------------------------------------------------

informacion_pokemon_t *info_pokemon_crear()
{
	informacion_pokemon_t *ip = malloc(sizeof(informacion_pokemon_t));
	if (ip == NULL)
		return NULL;

	ip->pokemones = NULL;
	ip->cantidad_pokemones = 0;

	return ip;
}

bool info_pokemon_es_vacio(informacion_pokemon_t *ip)
{
	if (ip == NULL)
		return false;

	if (ip->cantidad_pokemones != 0)
		return false;

	return true;
}

bool info_pokemon_agregar_final(informacion_pokemon_t *ip, pokemon_t *pokemon)
{
	if (ip == NULL || pokemon == NULL)
		return false;

	pokemon_t **pokemones_extendido =
		realloc(ip->pokemones,
			sizeof(pokemon_t *) * (ip->cantidad_pokemones + 1));
	if (pokemones_extendido == NULL)
		return false;

	ip->pokemones = pokemones_extendido;
	ip->cantidad_pokemones++;

	ip->pokemones[ip->cantidad_pokemones - 1] = pokemon;

	return true;
}

bool info_pokemon_ordenar(informacion_pokemon_t *ip)
{
	if (ip == NULL)
		return false;

	size_t cant_pokemones = (size_t)pokemon_cantidad(ip);

	for (size_t i = 0; i < cant_pokemones; i++) {
		for (size_t j = 0; j < cant_pokemones - i - 1; j++) {
			if (strcmp(pokemon_nombre(ip->pokemones[j]),
				   pokemon_nombre(ip->pokemones[j + 1])) > 0) {
				pokemon_t *aux = ip->pokemones[j];
				ip->pokemones[j] = ip->pokemones[j + 1];
				ip->pokemones[j + 1] = aux;
			}
		}
	}

	return true;
}

//-----------------------------Parseo pokemon---------------------------------------

bool char_conversor_tipo(char c, enum TIPO *tipo)
{
	switch (c) { //seteo el tipo de ataque
	case 'N':
		*tipo = NORMAL;
		return true;

	case 'F':
		*tipo = FUEGO;
		return true;

	case 'A':
		*tipo = AGUA;
		return true;

	case 'P':
		*tipo = PLANTA;
		return true;

	case 'E':
		*tipo = ELECTRICO;
		return true;

	case 'R':
		*tipo = ROCA;
		return true;
	}

	return false;
}

bool parsear_nombre_tipo(const char *str, pokemon_t *pokemon)
{
	if (str[0] == '\0')
		return false;
	//si viene una linea vacia. nunca pasara porque por lo menos lee un \n
	char tipo;

	int cant_nombre_tipo =
		sscanf(str, "%[^;];%c\n", pokemon->nombre, &tipo);
	if (cant_nombre_tipo != 2)
		return false;

	if (!isalpha(tipo))
		return false;

	if (char_conversor_tipo(tipo, &pokemon->tipo) == false)
		return false;

	return true;
}

bool parsear_ataque(const char *str, struct ataque *ataque)
{
	//viene algo del estilo -> ataque1;tipo;poder = Lanzallamas;F;4
	char tipo;
	unsigned int poder = 0;
	int cantidad =
		sscanf(str, "%[^;];%[^;];%u\n", ataque->nombre, &tipo, &poder);

	if (cantidad != 3)
		return false;

	ataque->poder = poder;

	if (char_conversor_tipo(tipo, &ataque->tipo) == false)
		return false;

	return true;
}

//----------------------------------------------------------------------------------

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	return NULL;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	return 0;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	return NULL;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	return FUEGO;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	return 0;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	return 0;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
}
