/* Archivo calculadora.x */
struct operandos {
    int a;
    int b;
};
program CAL_PROG{
 version CAL_VERS{
 int    SUMA(operandos)         = 1;
 int    RESTA(operandos)        = 2;
 int    MULTIPLICA(operandos)   = 3;
 double DIVIDE(operandos)       = 4;
} = 1;
} = 0x31111122;