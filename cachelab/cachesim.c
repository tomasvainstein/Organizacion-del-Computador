#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cacheSimulador.h"

bool procesarTraza(FILE* traza, char** argv, bool modoVerboso) {
	char *linea = NULL;
	size_t tam = 0;

	int n = -1;
	int m = -1;

	int tam_cache = atoi(argv[2]);
	int asociatividad = atoi(argv[3]);
	int sets = atoi(argv[4]);

	if (modoVerboso) {
		n = atoi(argv[6]);
		m = atoi(argv[7]);
		modoVerboso = false;
	}

	cacheSimulador_t* cache = crearCache(tam_cache, sets, asociatividad);
	if (!cache) {
		return false;
	}

	int indiceOpracion = -1;

	while ((getline(&linea, &tam, traza) != EOF)) {
		indiceOpracion++;

		if (n <= indiceOpracion && indiceOpracion <= m) {
			modoVerboso = true;
		}

		procesarComando(cache, linea, modoVerboso, stdout);

		modoVerboso = false;

	}

	imprimirMetricas(cache, stdout);

	destruirCache(cache);

	free(linea);

	return true;

}

bool numeroInvalidoDeParametros (int argc){
	return (argc != 5 && argc != 8);
}

bool archivoExiste(const char* nombreArchivo) {
	FILE* archivo = fopen(nombreArchivo, "r");

	if (archivo) {
		fclose(archivo);
		return true;
	}

	return false;
}

bool esPotenciaDeDos(int x) {
	return x > 0 && (x & (x - 1)) == 0;
}

bool NMinvalidos(int n, int m) {
	return !((0 <= n) && (n <= m));
}

bool argumentosInvalidos (int argc, char** argv) {
	if (numeroInvalidoDeParametros(argc)) {
		fprintf(stderr, "Cantidad de parametros invalida :(\n");
		return true;
	}

	if (!archivoExiste(argv[1])) {
		fprintf(stderr, "El archivo no existe!!\n");
		return true;
	}

	if (!(esPotenciaDeDos(atoi(argv[2])) && (esPotenciaDeDos(atoi(argv[3]))) && (esPotenciaDeDos(atoi(argv[4]))))) {
		fprintf(stderr, "Los parametros C, E y/o S no son potencia de 2\n");
		return true;
	}

	if (argc == 8 && NMinvalidos(atoi(argv[6]), atoi(argv[7]))) {
		fprintf(stderr, "No se cumple la condicion 0 <= n <= m\n");
		return true;
	}

	return false;
}

int main(int argc, char** argv) {

	if (argumentosInvalidos(argc, argv)) return 1;

	bool modoVerboso = false;
	char* nombreArchivo = argv[1];
	FILE* traza = fopen(nombreArchivo, "r");

	if (argc == 8) {
		modoVerboso = true;
	}

	if (!procesarTraza(traza, argv, modoVerboso)) {
		return 1;
	};

	fclose(traza);

	return 0;
}