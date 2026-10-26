#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;



// Estructuras:

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Dia {
    int numero;
    char nombre[10];
};



// Prototipos de funciones "generales":
void inicializarCadenaCaracteres(char[],int);


// Prototipos de funciones:
bool validarFecha(char[]);
void separarFecha(char[],char[],char[],char[]);
int obtenerCantidadDiasRestantesSemana(Dia);
void generarNombreArchivoComandasSemana(char[],char[],char[]);
void generarNombreArchivoComandasDia(char[],char[]);
int obtenerCantDiasMes(char[],char[]);
int obtenerNumeroDiaActual(char[]);
void avanzarDia(char[]);
int obtenerCantidadRegistrosArchivoComandas(FILE*);
void apareoArchivos(char[][11],int,char[]);



int main(){
    cout << "\t\t.:: Programa de 'cierre' ::." << endl << endl;

    char nombreArchivoSemanal[30];
    inicializarCadenaCaracteres(nombreArchivoSemanal , 30);

    char nombreArchivoComandasDia[30];
    inicializarCadenaCaracteres(nombreArchivoComandasDia , 30);

    char numSemana[2];

    char fecha[11];
    bool fechaValida;

    char dia[3];
    char mes[3];
    char anio[5];
    Dia diaActual;
    int cantDiasMes;
    int cantDiasRestantesSemana;
    int res;


    char fechas[6][11];
    int lenFechas = 0;

    for(int i = 0 ; i < 6 ; i++){
        inicializarCadenaCaracteres(fechas[i] , 11);
    }




    cout << "Ingrese la fecha del primer dia de la semana en formato 'DD-MM-AAAA': ";
    cin >> fecha;

    fechaValida = validarFecha(fecha);

    while(fechaValida == false){
        cout << "Error: la fecha ingresada es invalida." << endl << endl;

        cout << "Ingrese la fecha del primer dia de la semana en formato 'DD-MM-AAAA': ";
        cin >> fecha;

        fechaValida = validarFecha(fecha);
    }

    cout << "Ingrese el nombre del primer dia de la semana: ";
    cin >> diaActual.nombre;

    cout << "Ingrese el numero de semana del mes actual: ";
    cin >> numSemana;



    separarFecha(dia , mes , anio , fecha);

    cantDiasMes = obtenerCantDiasMes(mes , anio);
    diaActual.numero = obtenerNumeroDiaActual(dia);
    cantDiasRestantesSemana = obtenerCantidadDiasRestantesSemana(diaActual);



    generarNombreArchivoComandasSemana(numSemana , mes , nombreArchivoSemanal);




    // Cargando la matriz de "fechas": -----------------------------------------

    int i = 0;

    while(i < cantDiasRestantesSemana && diaActual.numero <= cantDiasMes){
        strcpy(fechas[i] , fecha);
        lenFechas++;

        avanzarDia(fecha);
        diaActual.numero++;
        i++;
    }




    apareoArchivos(fechas , lenFechas , nombreArchivoSemanal);




    cout << "\n\nSe ha generado el archivo de:" << endl;
    cout << "Mes: " << mes << endl;
    cout << "Semana: " << numSemana << endl;

    return 0;
}




// Funciones "generales":

void inicializarCadenaCaracteres(char cadena[], int max){
    for(int i = 0 ; i < max ; i++){
        cadena[i] = 0;
    }
}




// Funciones:

bool validarFecha(char fecha[]){
    bool fechaValida = false;

    int dia = (fecha[0] - 48) * 10 + (fecha[1] - 48);
    int mes = (fecha[3] - 48) * 10 + (fecha[4] - 48);
    int anio = (fecha[6] - 48) * 1000 + (fecha[7] - 48) * 100 + (fecha[8] - 48) * 10 + (fecha[9] - 48);

    if(mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12){
        if(dia >= 1 && dia <= 31){
            fechaValida = true;
        }
    }
    else if(mes == 4 || mes == 6 || mes == 9 || mes == 11){
        if(dia >= 1 && dia <= 30){
            fechaValida = true;
        }
    }
    else{
        if(mes == 2){
            if(anio % 4 == 0){
                if(dia >= 1 && dia <=29){
                    fechaValida = true;
                }
            }
            else if(dia >= 1 && dia <= 28){
                fechaValida = true;
            }
        }
    }

    return fechaValida;
}

void separarFecha(char dia[], char mes[], char anio[], char fecha[]){
    dia[0] = fecha[0];
    dia[1] = fecha[1];
    dia[2] = '\0';

    mes[0] = fecha[3];
    mes[1] = fecha[4];
    mes[2] = '\0';

    anio[0] = fecha[6];
    anio[1] = fecha[7];
    anio[2] = fecha[8];
    anio[3] = fecha[9];
    anio[4] = '\0';
}

int obtenerCantidadDiasRestantesSemana(Dia diaActual){
    int diasRestantes;

    if(strcmp(diaActual.nombre , "lunes") == 0){
        diasRestantes = 6;
    }
    else if(strcmp(diaActual.nombre , "martes") == 0){
        diasRestantes = 5;
    }
    else if(strcmp(diaActual.nombre , "miercoles") == 0){
        diasRestantes = 4;
    }
    else if(strcmp(diaActual.nombre , "jueves") == 0){
        diasRestantes = 3;
    }
    else if(strcmp(diaActual.nombre , "viernes") == 0){
        diasRestantes = 2;
    }
    else{
        diasRestantes = 1;
    }

    return diasRestantes;
}

void generarNombreArchivoComandasSemana(char numSemana[], char mes[], char nombreArchivo[]){
    strcpy(nombreArchivo , "comandas_semana_s");

    strcat(nombreArchivo , numSemana);
    strcat(nombreArchivo , "-");
    strcat(nombreArchivo , mes);
    strcat(nombreArchivo , ".dat");
}

void generarNombreArchivoComandasDia(char fecha[], char nombreArchivo[]){
    strcpy(nombreArchivo , "comandas_");

    strcat(nombreArchivo , fecha);
    strcat(nombreArchivo , ".dat");
}

int obtenerCantDiasMes(char mes[], char anio[]){
    int cantDiasMes;

    int mesInt = (mes[0] - 48) * 10 + (mes[1] - 48);
    int anioInt = (anio[0] - 48) * 1000 + (anio[1] - 48) * 100 + (anio[2] - 48) * 10 + (anio[3] - 48);

    if(mesInt == 1 || mesInt == 3 || mesInt == 5 || mesInt == 7 || mesInt == 8 || mesInt == 10 || mesInt == 12){
        cantDiasMes = 31;
    }
    else if(mesInt == 4 || mesInt == 6 || mesInt == 9 || mesInt == 11){
        cantDiasMes = 30;
    }
    else{
        if(mesInt == 2 && anioInt % 4 == 0){
            cantDiasMes = 29;
        }
        else{
            cantDiasMes = 28;
        }
    }

    return cantDiasMes;
}

int obtenerNumeroDiaActual(char dia[]){
    int diaInt = (dia[0] - 48) * 10 + (dia[1] - 48);

    return diaInt;
}

void avanzarDia(char fecha[]){
    // 02-06-2025

    if(fecha[0] == '0' && fecha[1] == '9'){
        fecha[0] = '1';
        fecha[1] = '0';
    }
    else if(fecha[0] == '1' && fecha[1] == '9'){
        fecha[0] = '2';
        fecha[1] = '0';
    }
    else if(fecha[0] == '2' && fecha[1] == '9'){
        fecha[0] = '3';
        fecha[1] = '0';
    }
    else{
        fecha[1]++;
    }
}

int obtenerCantidadRegistrosArchivoComandas(FILE* archivo){
    int cantRegs;
    long posInicial = ftell(archivo);


    // Posicionando el puntero al final del archivo:
    fseek(archivo , 0 , SEEK_END);

    cantRegs = ftell(archivo) / sizeof(Comanda);

    // Devolviendo el puntero a la posicion inicial:
    fseek(archivo , posInicial , SEEK_SET);


    return cantRegs;
}

void apareoArchivos(char fechas[][11], int lenFechas, char nombreArchivoSemanal[]){
    FILE* archivoSemanal = fopen(nombreArchivoSemanal , "wb");

    if(archivoSemanal == NULL){
        cout << "Error: el 'archivo semanal' no se ha abierto correctamente." << endl;

        cout << "No se han podido generar el archivo semanal." << endl;

        return;
    }




    char nombresArchivosComandasDia[6][30];

    for(int i = 0 ; i < lenFechas ; i++){
        generarNombreArchivoComandasDia(fechas[i] , nombresArchivosComandasDia[i]);
    }



    bool archivosActivos[6];
    Comanda comandasPlanillas[6];
    FILE* archivos[6];

    // Inicializando los arrays:
    for(int i = 0 ; i < 6 ; i++){
        archivosActivos[i] = false;

        comandasPlanillas[i].idMozo = -1;

        archivos[i] = NULL;
    }




    for(int i = 0 ; i < lenFechas ; i++){
        archivos[i] = fopen(nombresArchivosComandasDia[i] , "rb");

        // Si la planilla del dia no existe:
        if(archivos[i] == NULL){
            archivosActivos[i] = false;
        }
        else{
            // Si la planilla existe:

            int cantRegs = obtenerCantidadRegistrosArchivoComandas(archivos[i]);

            if(cantRegs == 0){
                fclose(archivos[i]);

                archivosActivos[i] = false;

                archivos[i] = NULL;
            }
            else{
                archivosActivos[i] = true;

                // Leo la primer comanda del archivo abierto:
                fread(&comandasPlanillas[i] , sizeof(Comanda) , 1 , archivos[i]);
            }
        }
    }



    while(archivosActivos[0] == true || archivosActivos[1] == true || archivosActivos[2] == true || archivosActivos[3] == true || archivosActivos[4] == true || archivosActivos[5] == true){
        int posMinimo = -1;
        int menorIdMozo = 999999;

        for(int i = 0 ; i < lenFechas ; i++){
            if(comandasPlanillas[i].idMozo != -1 && comandasPlanillas[i].idMozo < menorIdMozo){
                menorIdMozo = comandasPlanillas[i].idMozo;

                posMinimo = i;
            }
        }


        if(posMinimo == -1){
            break;
        }


        fwrite(&comandasPlanillas[posMinimo] , sizeof(Comanda) , 1 , archivoSemanal);


        // Leyendo el siguiente registro del archivo correspondiente:
        int leido = fread(&comandasPlanillas[posMinimo] , sizeof(Comanda) , 1 , archivos[posMinimo]);

        if(leido != 1){
            // Si el archivo se queda sin registros para leer:

            fclose(archivos[posMinimo]);

            archivosActivos[posMinimo] = false;

            comandasPlanillas[posMinimo].idMozo = -1;
        }
    }

    fclose(archivoSemanal);
}