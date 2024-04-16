#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cacheSimulador.h"
#include "contador.h"

typedef struct linea {
	int validBit;
	int dirtyBit;
	unsigned int tagIndex;
	int lastUsed;
} linea_t;

void inicializarLineaVacia(linea_t* linea) {
	linea->validBit = 0;
	linea->dirtyBit = 0;
	linea->tagIndex = -1;
	linea->lastUsed = -1;
}

void inicializarLineasVaciasArray(linea_t* lineas, int asociatividad) {
	for (int i = 0; i < asociatividad; i++) {
		inicializarLineaVacia(lineas + i);
	}
}

typedef struct set {
	linea_t* lineas;
	int asociatividad;
} set_t;

bool inicializarSetVacio(set_t* set, int asociatividad) {
	set->asociatividad = asociatividad;

	linea_t* lineas = calloc(asociatividad, sizeof(linea_t));
	if (!lineas) {
		return false;
	}

	inicializarLineasVaciasArray(lineas, asociatividad);

	set->lineas = lineas;

	return true;
}

void destruirSet(set_t* set) {
	free(set->lineas);
}

void destruirArrayDeSets(set_t* sets, unsigned int numeroDeSets) {
	for (unsigned int i = 0; i < numeroDeSets; i++) {
		destruirSet(sets + i);
	}
	free(sets);
}

bool inicializarArrayDeSets(set_t* sets, unsigned int numeroDeSets, int asociatividad) {
	for (unsigned int i = 0; i < numeroDeSets; i++) {
		if (!inicializarSetVacio(sets + i, asociatividad)) {
			destruirArrayDeSets(sets, numeroDeSets);
			return false;
		}
	}

	return true;
}

struct cacheSimulador {
	unsigned int numeroDeSets;
	set_t* sets;
	int tamanioBloque;
	int indiceOperacionActual;
	contador_t* contador;
};

typedef enum tipoDeAcceso {
	WRITTING,
	READING,
} tipoDeAcceso_t;

typedef enum caso {
	CACHE_HIT,
	CLEAN_CACHE_MISS,
	DIRTY_CACHE_MISS,
} caso_t;

unsigned calcularOffset(unsigned n) {
	unsigned offset = 0;
	while (n != 1){
		offset += 1;
		n >>= 1;
	}
	return offset;
}

unsigned int calcularTag(size_t desplazamientoConjuntos, size_t desplazamientoBloque, unsigned int direccionAcceso) {
	unsigned int tag = direccionAcceso >> (desplazamientoConjuntos + desplazamientoBloque);
	return tag;
}

unsigned int calcularSetIndex(size_t desplazamientoConjuntos, size_t desplazamientoBloque, unsigned int direccionAcceso) {
	unsigned int indexMask = (1 << (desplazamientoBloque + desplazamientoConjuntos)) - 1;
	unsigned int setIndex = (direccionAcceso & indexMask) >> desplazamientoBloque;
	return setIndex;
}

int lineaParaDesalojar(set_t set) {
	for (int i = 0; i < set.asociatividad; i++) {
		if (!set.lineas[i].validBit) {
			return i;
		}
	}

	int resultIndex = 0;

	for (int i = 0; i < set.asociatividad; i++) {
		if (set.lineas[i].lastUsed < set.lineas[resultIndex].lastUsed) {
			resultIndex = i;
		}
	}

	return resultIndex;
}

void asignarTipoAccesoYdireccionAcceso(char* comando, tipoDeAcceso_t* tipoAcceso, unsigned int* direccionAcceso) {
	char* arrayComando[5];
	char* fragmento = strtok(comando, " ");
	int i = 0;
	while (fragmento != NULL){
		arrayComando[i++] = fragmento;
		fragmento = strtok(NULL, " ");
	}

    if(strcmp(arrayComando[1], "R")==0){
        *tipoAcceso = READING;
    } else{
        *tipoAcceso = WRITTING;
    }

	*direccionAcceso = strtoul(arrayComando[2], NULL, 0);
}

int obtenerTagCoincidente(set_t set, unsigned int tagAComparar) {
	int tagCoincidente = set.asociatividad;

	for (int i = 0; i < set.asociatividad; i++) {
		if ((set.lineas[i].tagIndex == tagAComparar) && set.lineas[i].validBit) {
			tagCoincidente = i;
			break;
			}
	}

	return tagCoincidente;
}


cacheSimulador_t* crearCache(int tamañoCache, unsigned int numeroDeSets, int asociatividad) {
	cacheSimulador_t* cache = malloc(sizeof(cacheSimulador_t));
	if (cache == NULL) {
		return NULL;
	}

	contador_t* contador = crearContador();
	if (contador == NULL) {
		free(cache);
		return NULL;
	}

	set_t* sets = calloc(numeroDeSets, sizeof(set_t));
	if (sets == NULL) {
		free(cache);
		free(contador);
		return NULL;
	}

	if (!inicializarArrayDeSets(sets, numeroDeSets, asociatividad)) {
		free(cache);
		free(contador);
		free(sets);
		return NULL;
	}

	cache->sets = sets;
	cache->contador = contador;
	cache->indiceOperacionActual = 0;
	cache->numeroDeSets = numeroDeSets;
	cache->tamanioBloque = tamañoCache / (numeroDeSets * asociatividad);

	return cache;
}

void procesarComando(cacheSimulador_t* cache, char* comando, bool modoVerboso, FILE* archivoSalida) {
	int tiempoDeAcceso = 1;

	caso_t caso = CACHE_HIT;

	int tagIndexPrevio = -1;

	int validBitPrint = 0;
	int dirtyBitPrint = 0;
    int ultimoUsoPrint = -1;

	bool isDirtyMiss = false;

	tipoDeAcceso_t tipoAcceso;
	unsigned int direccionAcceso;
	asignarTipoAccesoYdireccionAcceso(comando, &tipoAcceso, &direccionAcceso);

	unsigned desplazamientoBloque = calcularOffset(cache->tamanioBloque);
	unsigned desplazamientoConjuntos = calcularOffset(cache->numeroDeSets);

	unsigned int setIndex = calcularSetIndex(desplazamientoConjuntos, desplazamientoBloque, direccionAcceso);
	unsigned int tagIndex = calcularTag(desplazamientoConjuntos, desplazamientoBloque, direccionAcceso);

	int indiceDeLineaCoincidente = obtenerTagCoincidente(cache->sets[setIndex], tagIndex);
	int indiceDeLineaPrint = indiceDeLineaCoincidente;

	if (indiceDeLineaCoincidente < cache->sets->asociatividad) { // cache hit
		validBitPrint = cache->sets[setIndex].lineas[indiceDeLineaCoincidente].validBit;
		dirtyBitPrint = cache->sets[setIndex].lineas[indiceDeLineaCoincidente].dirtyBit;
		ultimoUsoPrint = cache->sets[setIndex].lineas[indiceDeLineaCoincidente].lastUsed;
		tagIndexPrevio = tagIndex;

		cache->sets[setIndex].lineas[indiceDeLineaCoincidente].lastUsed = cache->indiceOperacionActual;

		if (tipoAcceso == WRITTING) {
			cache->sets[setIndex].lineas[indiceDeLineaCoincidente].dirtyBit = 1;
			incAccessTimeW(cache->contador, tiempoDeAcceso);
		} else {
			incAccessTimeR(cache->contador, tiempoDeAcceso);
		}

	} else { // miss
		int indiceLineaADesalojar = lineaParaDesalojar(cache->sets[setIndex]);
		int lineaADesalojarValidbit = cache->sets[setIndex].lineas[indiceLineaADesalojar].validBit;
		int lineaADesalojarDirtybit = cache->sets[setIndex].lineas[indiceLineaADesalojar].dirtyBit;

		ultimoUsoPrint = cache->sets[setIndex].lineas[indiceLineaADesalojar].lastUsed;
		validBitPrint = lineaADesalojarValidbit;
		indiceDeLineaPrint = indiceLineaADesalojar;
		dirtyBitPrint = lineaADesalojarDirtybit;
		tagIndexPrevio = cache->sets[setIndex].lineas[indiceLineaADesalojar].tagIndex;

		cache->sets[setIndex].lineas[indiceLineaADesalojar].tagIndex = tagIndex;
		cache->sets[setIndex].lineas[indiceLineaADesalojar].validBit = 1; 

		if (lineaADesalojarDirtybit) { // dirty cache miss
			caso = DIRTY_CACHE_MISS;
			tiempoDeAcceso += 2 * 100;
			isDirtyMiss = true;
			incBytesWritten(cache->contador, cache->tamanioBloque);
		} else { // clean cache miss
			caso = CLEAN_CACHE_MISS;
			tiempoDeAcceso += 100;
		}


		if (tipoAcceso == READING) {
			incMissR(cache->contador);
			if (isDirtyMiss) {
				incDirtyMissR(cache->contador);
			}
			incAccessTimeR(cache->contador, tiempoDeAcceso);
			cache->sets[setIndex].lineas[indiceLineaADesalojar].dirtyBit = 0;
		} else {
			incMissW(cache->contador);
			if (isDirtyMiss) {
				incDirtyMissW(cache->contador);
			}
			incAccessTimeW(cache->contador, tiempoDeAcceso);
			cache->sets[setIndex].lineas[indiceLineaADesalojar].dirtyBit = 1;
		}

		incBytesRead(cache->contador, cache->tamanioBloque);
		cache->sets[setIndex].lineas[indiceLineaADesalojar].lastUsed = cache->indiceOperacionActual;

	}

	if (modoVerboso) {
		fprintf(archivoSalida, "%d ", cache->indiceOperacionActual);
		if (caso == CACHE_HIT) {
		fprintf(archivoSalida, "1");
        }
        else if (caso == CLEAN_CACHE_MISS) {
            fprintf(archivoSalida, "2a");
        }
        else if (caso == DIRTY_CACHE_MISS) {
            fprintf(archivoSalida, "2b");
        }

		fprintf(archivoSalida, " %01x %01x", setIndex, tagIndex);
		fprintf(archivoSalida, " %d", indiceDeLineaPrint);
        if (tagIndexPrevio == -1) {
				fprintf(archivoSalida, " -1");
			} else {
				fprintf(archivoSalida, " %01x", tagIndexPrevio);
			}
		fprintf(archivoSalida, " %d", validBitPrint);
		fprintf(archivoSalida, " %d", dirtyBitPrint);
		if (cache->sets->asociatividad > 1) {
			if (ultimoUsoPrint == -1) {
				fprintf(archivoSalida, " 0");
			} else {
			fprintf(archivoSalida, " %d", ultimoUsoPrint);
			}
		}
		fprintf(archivoSalida, "\n");
	}

	if (tipoAcceso == READING) {
		incLoads(cache->contador);
	} else {
		incStores(cache->contador);
	}

	cache->indiceOperacionActual++;
}


void imprimirMetricas(cacheSimulador_t* cache, FILE* archivoSalida) {
	contador_t* contador = cache->contador;

	char* otherWay = "-way";
	char* directMapping = "direct-mapped";
	int tamañoCache = (cache->tamanioBloque * cache->numeroDeSets * cache->sets->asociatividad) / 1000;

	if (cache->sets->asociatividad == 1) {
		fprintf(archivoSalida, "%s, ", directMapping);
	} else {
		fprintf(archivoSalida, "%d%s, ", cache->sets->asociatividad, otherWay);
	}

	fprintf(archivoSalida, "%d sets, size = %dKB\n", cache->numeroDeSets, tamañoCache);

	fprintf(archivoSalida, "loads %d stores %d total %d\n", getLoads(contador), getStores(contador), getTotalAccesses(contador));

	fprintf(archivoSalida, "rmiss %d wmiss %d total %d\n", getMissR(contador), getMissW(contador), getTotalMisses(contador));

	fprintf(archivoSalida, "dirty rmiss %d dirty wmiss %d\n", getDirtyMissR(contador), getDirtyMissW(contador));

	fprintf(archivoSalida, "bytes read %d bytes written %d\n", getBytesRead(contador), getBytesWritten(contador));

	fprintf(archivoSalida, "read time %d write time %d\n", getAccessTimeR(contador), getAccessTimeW(contador));

	fprintf(archivoSalida, "miss rate %f\n", getTotalMissRate(contador));
}

void destruirCache(cacheSimulador_t* cache) {
	destruirArrayDeSets(cache->sets, cache->numeroDeSets);
	destruirContador(cache->contador);
	free(cache);
}