/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */

#endif

// 1

/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */

int bitAnd(int x, int y)
{
  // Se invierten los bits de x e y, 
  // aplicandoles un or quedan con 1 los bits que no tienen en comun x e y
  // y luego se vuelven a invertir los bits

  int notX = ~x;
  int notY = ~y;
  int notResult = notX | notY;
  return ~notResult;
}

/*
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only ~ and &
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   0111 - 1110 --> int mask = 0110
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */

int bitMatch(int x, int y)
{
  // Buscamos los bits 0 en comun por un lado y por el otro los 1

  int notMatch = ~(~x & ~y) & ~(x & y);
  int match = ~notMatch;
  return match;
}

/*
 * bitNor - ~(x|y) using only ~ and &
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */

int bitNor(int x, int y)
{
  // Negamos x e y y aplicamos un and para obtener los 1s en comun de los negados

  int notX = ~x;
  int notY = ~y;
  int result = notX & notY;
  return result;
}

/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 * 0100 - 0101 --> 0001
 * 1011 0101 --> & --> 0001
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */

int bitXor(int x, int y)
{
  // Misma lógica que  bitMatch pero los que no tienen en comun

  int result = ~(~x & ~y) & ~(x & y);
  return result;
}

/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */

int allOddBits(int x)
{
  // Haciendo un and con la mascara nos quedamos con los bits que tienen 1 de X en las
  // mismas posiciones que mask (impares) y luego con el xor comparamos los bits que tienen en comun
  // Si tienen todos en comun result es 1, sino 0

  int mask = 0xAA | 0xAA <<8 | 0xAA<<16 | 0xAA<<24;
  int and = x & mask;
  int result = !(and^mask);
  return result;
}

/*
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */

int anyEvenBit(int x)
{
  // Uso una mascara y le hago un and al numero, si ese numero que obtengo es != a 0 
  // entonces la funcion devuelve 1, sino devuelve 0

  int mask = 0x55 | 0x55 <<8 | 0x55<<16 | 0x55<<24 ;
  int and = x & mask;
  int result = !!(and);
  return result;
}

/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 * 00010010 00110100 01010110 01111000 - 01010110001101000001001001111000
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */

int byteSwap(int x, int n, int m)
{
  // Obtenemos los bytes que queremos intercambiar y creamos una mask2 con 0s en esas posiciones.
  // se corre los bytes a sus posiciones de destinos y se hace un |.

  int mask = 0xFF;
  int shift_n = n<<3;
  int shift_m = m<<3;
  int byte_n = (x >> (shift_n)) & mask;
  int byte_m = (x >> (shift_m)) & mask;
  int mask2 = ~((mask << shift_n) | (mask << shift_m));
  return ((x & mask2) | (byte_m << shift_n) | (byte_n << shift_m));
}

/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */

int fitsBits(int x, int n) {
  // Calculo el número de bits a desplazar y extendemos el signo de x a n bits.
  // Devolvemos 1 si x extendido es igual a x, 0 en caso contrario.

  int shift = 32 + (~n + 1);
  int shifted = (x << shift) >> shift;
  return !(shifted ^ x);
}

/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */

int negate(int x)
{
  // En complemento a 2 se obtiene el negado invirtiendo los bits y sumando 1.

  return ~x + 1;
}

/*
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */

int sign(int x)
{
  // Si x es positivo (no 0) num es 1 y devuelve 1; y num es 0 y devuelve 0 si x es 0
  // si x es negativo num vale 0 y devuelve -1

  int msb = (x >> 31) & 1;
  int num = msb ^ (!!x);
  return num + (~msb + 1);
}

// 3

/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */

int addOK(int x, int y) {
  // El overflow sucede cuando la suma de dos numeros da otro con signo diferente (pega la vuelta)

  int suma = x + y;
  int signo_x = x >> 31;
  int signo_y = y >> 31;
  int signo_sum = suma >> 31;
  int overflow = ((signo_x ^ signo_sum) & (signo_y ^ signo_sum));
  return !overflow;
}

/*
 * bitMask - Generate a mask consisting of all 1's
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */

int bitMask(int highbit, int lowbit)
{
  // Se generan 2 mascaras una con 0s desde highbit y otra desde lowbit. Se hace un xor para ver los bits que no tienen en común y un AND
  // mask2 para el caso en que highbit<lowbit y que devuelva 0

  int mask1 = ~0;
  int mask2 = (~0 << lowbit);
  mask1 = mask1 << (highbit) << 1;
  return (mask1 ^ mask2) & mask2;
}

/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */

int conditional(int x, int y, int z)
{
  // Si x es 0 queda 0x00000000 sino 0xFFFFFFFF
  // Si es 0 devuelve z, sino y

  int mask = ((!!x)<<31)>>31; 
  int result = (y & mask) | (z & ~mask); 
  return result;
}

/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */

int isAsciiDigit(int x) {

  // Se calcula la diferencia de x con el minimo y el maximo. Dependiendo si la diferencia es positiva o negativa y si la diferencia
  // con el maximo es o no 0, se retorna el valor correspondiente.
  
  int min = 0x30;  
  int max = 0x39; 
  int difMin = x + (~min + 1); 
  int difMax = x + (~max + 1);
  int signDifMin = difMin >> 31;
  int signDifMax = difMax >> 31; 
  int result= !((signDifMin | (!signDifMax)) & (!!difMax));
  return result;
}

/*
 * isGreater - if x > y  then return 1, else return 0
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */

int isGreater(int x, int y)
{
  // Se devuelve 1 si x es positivo y y es negativo, o si el signo de la diferencia es negativo y no ha habido overflow
  // Si hay overflow devuelve 1...11. Puede haber overflow cuando y neg y x positivo o cuando y positivo y x negativo
  // Si x es pos y Y es neg devuelve 1. Si el signo es diferente y no hay overflow devuelve 1, sino devuelve 0

  int signX = x >> 31;
  int signY = y >> 31;
  int dif = y + (~x) + 1;
  int signDif = dif >> 31;
  int overflow = (~signX & signY & ~signDif) | (signX & ~signY & signDif);
  return ((!signX) & signY) | !!(signDif & !overflow);
}

/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */

int replaceByte(int x, int n, int c)
{
  // Mask2 tiene ceros en la posicion del byte a reemplazar
  // Se shiftea c a la posicion del byte n y se hace un |

  int mask = 0xFF;
  int shift = n << 3;
  int mask2 = ~(mask << (shift));
  return (x & mask2) | (c << (shift));
}

// 4

/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */

int absVal(int x)
{
  // Si x es neg mask = 0xFFFFFFFF sino 0
  // y luego se hace como un condicional, si es negativo se calcula su negado, sino se devuelve x
  
  int mask = x >> 31; 
  int result = ((~x+1) & mask) | (x & ~mask); 
  return result;
}

/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */

int bang(int x) {
  // Si x | neg da un numero positivo en complemento a 2 significa
  // que x es cero, entonces eso + 1 es 0 si es negativo, 1 si es cero

  int neg = ~x + 1;
  int is_zero = ((x | neg) >> 31) + 1; 
  return is_zero;
}

/*
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */

int isNonZero(int x)
{
  // Utilizamos la misma lógica que en bang, pero que devuelve 1 si el numero NO es cero

  int neg = ~x + 1;
  int is_zero = ((x | neg) >> 31) + 1; // 0 si es negativo, 1 si es cero
  return is_zero ^ 1;
}

/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */

int logicalNeg(int x)
{
  // Igual a bang

  int neg = ~x + 1;
  int is_zero = ((x | neg) >> 31) + 1; 
  return is_zero;
}

// float

/*
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */

unsigned floatAbsVal(unsigned uf)
{
  // Se utiliza una máscara para eliminar el bit de signo. Luego una máscara con 1 en exp y 0 en el resto.
  // Si uf es NaN, se devuelve uf sino abs_f

  unsigned abs_mask = 0x7FFFFFFF;  
  unsigned nan_mask = 0x7F800000; 
  unsigned abs_f = uf & abs_mask;   
  if (abs_f > nan_mask)        
    return uf;
  else 
    return abs_f;
}

/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */

int floatIsEqual(unsigned uf, unsigned ug) {
  // Se verifiva si es nan, luego si es +0 y -0. Finalmente se comparan los valores.

  unsigned int nan = 0x7F800000;
  unsigned int abs_f = uf & 0x7FFFFFFF;
  unsigned int abs_g = ug & 0x7FFFFFFF;
  
  if ((abs_f > nan) || (abs_g > nan))
    return 0;

  if ((abs_f==0) && (abs_g==0))
    return 1;

  return uf == ug;
}

/*
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */

unsigned floatNegate(unsigned uf)
{
  // Se utiliza una mascara que saca el bit de signo. Luego otra con 1 en exp y 0 en el resto.
  // Luego se verifica si uf es Nan y en caso contrario le cambia el signo.

  unsigned abs_uf = uf & 0x7fffffff;
  unsigned nan = 0x7f800000; 
  if (abs_uf > nan)
    return uf;
  return uf ^ 0x80000000;
}

/*
 * floatIsLess - Compute f < g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */

int floatIsLess(unsigned uf, unsigned ug)
{
  // Se consideran los casos en que uf o ug son NaN o el caso +0 -0
  // luego se comparan los signos y dependiendo el caso en que sean diferentes o iguales se evalua
  // si son singos diferentes entonces el de signo = 1 es menor; si ambos son negativos el de modulo mas grande es el menor y si ambos 
  // son positivos el de modulo mas chico es el menor

  unsigned int nan = 0x7F800001;
  unsigned int abs_f = uf & 0x7FFFFFFF;
  unsigned int abs_g = ug & 0x7FFFFFFF;
  int sign_f = uf >> 31;
  int sign_g = ug >> 31;

  if ((abs_f > nan) || (abs_g > nan))
    return 0;

  if (!(abs_f) && (!abs_g))
    return 0;

  if (sign_f == 1 && sign_g == 0)
    return 1;
  else if (sign_f == 0 && sign_g == 1)
    return 0;
  else if (sign_f == 1 && sign_g == 1) {
    if (abs_f > abs_g)
      return 1;
    else 
      return 0;
  } else if (sign_f == 0 && sign_g == 0) {
      if (abs_g > abs_f)
        return 1;
      else 
        return 0;
  }
  return 0;
}

/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */

int floatFloat2Int(unsigned uf) {
  // Si uf es NaN o infinito, retorna 0x80000000. SI es menor que 1, retorna 0.
  // Si el exponente es mayor que 23, la fracción debe ser desplazada a la izquierda
  // Retornar el resultado con el signo correspondiente

  int sign = uf >> 31;
  int exp = ((uf >> 23) & 0xFF) - 127;
  int frac = uf & 0x7FFFFF;
  int result;

  if (exp > 31)
    return 0x80000000u;

  if (exp < 0)
    return 0;

  frac = frac | 0x800000;

  if (exp > 23)
    result = frac << (exp - 23);
  else
    result = frac >> (23 - exp);

  if (sign)
    return -result;
  else 
    return result;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */

unsigned floatPower2(int x) {
    // Se establecen los exponentes maximo y minimo. Si x es mas chico que el minimo exp se devuelve 0, si es mayor +infinito
    // Luego sumamos el bias y lo corremos a la posicion correcta

    int min_exp = -126;
    int max_exp = 127;
    int exp;
    unsigned int float_val;
    
    if (x < min_exp)
        return 0;
    else if (x > max_exp)
        return 0x7f800000;

    exp = x + max_exp;
    float_val = exp << 23;
    return float_val;
}