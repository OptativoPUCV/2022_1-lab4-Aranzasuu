#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new -> key = key;
    new -> value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash * 32 + tolower(*ptr);
    }
    return (hash % capacity);
}

int is_equal(void* key1, void* key2){
    if(key1 == NULL || key2 == NULL) return 0;
    if(strcmp((char*) key1, (char*) key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    Pair *dato = createPair(key,value);
    map -> current = hash(key,map->capacity);
    Pair **aux = map->buckets;
    long posicion = map->current;
    while(aux[posicion] != NULL){
        posicion++;
        if(posicion == (map->capacity))
            posicion = 0;
    }
    map -> buckets[posicion] = dato;
    map -> current = posicion;
    map -> size++;
}
    

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap *Mapa = (HashMap *)malloc(sizeof(HashMap));
    Mapa -> buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
    Mapa -> size = 0;
    Mapa -> capacity = capacity;
    Mapa -> current = -1;
    for(int i = 0; i < capacity; i++){
        Mapa->buckets[i] = NULL;
    }
    return Mapa;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair *eliminar = searchMap(map,key);
    if(eliminar != NULL){
      eliminar->key = NULL;
      (map->size)--;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long posicion = hash(key,map->capacity);
    map->current = posicion;
    if (map->buckets[posicion] == NULL)
        return NULL;
    while(is_equal(key, map->buckets[posicion]->key) != 1){
        posicion++;
        if(posicion == (map->capacity)) 
          posicion = 0;
        if(map->buckets[posicion] == NULL) 
          return NULL;
    }
    map->current = posicion;
    return map->buckets[posicion];
}

Pair * firstMap(HashMap * map) {
    map->current = 0;
    long posicion = 0;
    if(map->size == 0) return NULL;
    while(posicion < map -> capacity){
      if(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
      {
          map->current = posicion;
          return map->buckets[posicion];
      }
      posicion++;
    }
    
    return NULL;
}

Pair * nextMap(HashMap * map) {
    long posicion;
    if(map->size == 0) return NULL;
    for(posicion = map->current + 1; posicion < map->capacity; posicion++){
      if(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
        map->current = posicion;
        return map->buckets[posicion];
    }
    return NULL;
}
