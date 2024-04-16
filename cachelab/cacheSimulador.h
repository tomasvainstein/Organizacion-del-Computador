#include "contador.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct cacheSimulador cacheSimulador_t;

cacheSimulador_t* crearCache(int tamañoCache, unsigned int numeroDeSets, int asociatividad);

void procesarComando(cacheSimulador_t* cache, char* comando, bool modoVerboso, FILE* archivoSalida);

void imprimirMetricas(cacheSimulador_t* cache, FILE* archivoSalida);

void destruirCache(cacheSimulador_t* cache);