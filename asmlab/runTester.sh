#!/usr/bin/env bash
reset

echo " "
echo "**Compilando"

make tester
if [ $? -ne 0 ]; then
  echo "  **Error de compilacion"
  exit 1
fi

echo " "
echo "**Corriendo Valgrind"

command -v valgrind > /dev/null
if [ $? -ne 0 ]; then
  echo "ERROR: No se encuentra valgrind."
  exit 1
fi

valgrind --show-reachable=yes --leak-check=full --error-exitcode=1 ./tester
if [ $? -ne 0 ]; then
  echo "  **Error de memoria"
  exit 1
fi

echo " "
echo "**Corriendo diferencias con la catedra"

DIFFER="diff -d"
ERRORDIFF=0

$DIFFER salida.propios.caso1.txt salida.catedra.caso1.txt > /tmp/diff1
if [ $? -ne 0 ]; then
  echo "  **Discrepancia en el caso 1"
  cat /tmp/diff1
  ERRORDIFF=1
fi

$DIFFER salida.propios.caso2.txt salida.catedra.caso2.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancia en el caso 2"
  cat /tmp/diff2
  ERRORDIFF=1
fi

echo " "
if [ $ERRORDIFF -eq 0 ]; then
  echo "**Todos los tests pasan"
else
  exit 1
fi
echo " "

