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
void generarNombreArchivoComandasSemana(char[],char[],char[]);
void generarNombreArchivoComandasDia(char[],char[],char[],char[]);
void cargarComandasDiaArchivoSemanal(char[],FILE*,char[],char[],char[]);
int obtenerCantDiasMes(char[],char[]);
int obtenerNumeroDiaActual(char[]);
void avanzarDia(char[],Dia&);
void cargarArchivoSemanal(char[],Dia,int,char[],char[],char[]);
void ordenarArchivoComandasIDMozo(char[]);
int obtenerCantidadRegistrosArchivoComandas(FILE*);
void insertarOrdenadoIDMozoArrayComandas(Comanda[],int&,Comanda);
void insertarPosEspecificaArrayComandas(Comanda[],int&,Comanda,int);



int main(){
    cout << "\t\t.:: Programa de 'cierre' ::." << endl << endl;

    char nombreArchivoSemanal[30];
    inicializarCadenaCaracteres(nombreArchivoSemanal , 30);

    char numSemana[2];

    char fecha[11];
    bool fechaValida;

    char dia[3];
    char mes[3];
    char anio[5];
    Dia diaActual;
    int cantDiasMes;



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



    generarNombreArchivoComandasSemana(numSemana , mes , nombreArchivoSemanal);

    cargarArchivoSemanal(nombreArchivoSemanal , diaActual , cantDiasMes , dia , mes , anio);

    ordenarArchivoComandasIDMozo(nombreArchivoSemanal);



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

void generarNombreArchivoComandasSemana(char numSemana[], char mes[], char nombreArchivo[]){
    strcpy(nombreArchivo , "comandas_semana_s");

    strcat(nombreArchivo , numSemana);
    strcat(nombreArchivo , "-");
    strcat(nombreArchivo , mes);
    strcat(nombreArchivo , ".dat");
}

void generarNombreArchivoComandasDia(char dia[], char mes[], char anio[], char nombreArchivo[]){
    strcpy(nombreArchivo , "comandas_");

    strcat(nombreArchivo , dia);
    strcat(nombreArchivo , "-");
    strcat(nombreArchivo , mes);
    strcat(nombreArchivo , "-");
    strcat(nombreArchivo , anio);
    strcat(nombreArchivo , ".dat");
}

void cargarComandasDiaArchivoSemanal(char nombreArchivo[], FILE* archivoSemana, char dia[], char mes[], char anio[]){
    FILE* archivoComandasDia = fopen(nombreArchivo , "rb");

    if(archivoComandasDia != NULL){
        Comanda com;

        while(fread(&com , sizeof(Comanda) , 1 , archivoComandasDia) == 1){
            fwrite(&com , sizeof(Comanda) , 1 , archivoSemana);
        }

        fclose(archivoComandasDia);
    }
    else{
        cout << "\nNo hay archivo del dia " << dia << "-" << mes << "-" << anio << endl; 
    }
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

void avanzarDia(char dia[], Dia& diaActual){
    if(dia[0] == '0' && dia[1] == '9'){
        dia[0] = '1';
        dia[1] = '0';
    }
    else if(dia[0] == '1' && dia[1] == '9'){
        dia[0] = '2';
        dia[1] = '0';
    }
    else if(dia[0] == '2' && dia[1] == '9'){
        dia[0] = '3';
        dia[1] = '0';
    }
    else{
        dia[1]++;
    }

    if(strcmp(diaActual.nombre , "lunes") == 0){
        strcpy(diaActual.nombre , "martes");
    }
    else if(strcmp(diaActual.nombre , "martes") == 0){
        strcpy(diaActual.nombre , "miercoles");
    }
    else if(strcmp(diaActual.nombre , "miercoles") == 0){
        strcpy(diaActual.nombre , "jueves");
    }
    else if(strcmp(diaActual.nombre , "jueves") == 0){
        strcpy(diaActual.nombre , "viernes");
    }
    else if(strcmp(diaActual.nombre , "viernes") == 0){
        strcpy(diaActual.nombre , "sabado");
    }
    else if(strcmp(diaActual.nombre , "sabado") == 0){
        strcpy(diaActual.nombre , "domingo");
    }

    diaActual.numero++;
}

void cargarArchivoSemanal(char nombreArchivo[], Dia diaActual, int cantDiasMes, char dia[], char mes[], char anio[]){
    FILE* archivoSemanal = fopen(nombreArchivo , "wb");

    if(archivoSemanal != NULL){
        char nombreArchivoComandasDia[30];
        int i = 0;

        while(i < 6 && diaActual.numero <= cantDiasMes && strcmp(diaActual.nombre , "domingo") != 0){
            inicializarCadenaCaracteres(nombreArchivoComandasDia , 30);


            generarNombreArchivoComandasDia(dia , mes , anio , nombreArchivoComandasDia);

            cargarComandasDiaArchivoSemanal(nombreArchivoComandasDia , archivoSemanal , dia , mes , anio);


            avanzarDia(dia , diaActual);
            i++;
        }

        fclose(archivoSemanal);
    }
    else{
        cout << "Error: el 'archivo semanal' no se ha abierto correctamente." << endl;

        cout << "No se han podido cargar las comandas semanales en el archivo." << endl;
    }
}

void ordenarArchivoComandasIDMozo(char nombreArchivo[]){
    FILE* archivoSemanal = fopen(nombreArchivo , "rb+");

    if(archivoSemanal == NULL){
        cout << "Error: el 'archivo de comandas semanal' no se ha abierto correctamente." << endl;

        cout << "No se ha podido 'ordenar el archivo'." << endl;

        return;
    }

    int cantRegs = obtenerCantidadRegistrosArchivoComandas(archivoSemanal);

    Comanda comandas[cantRegs];
    int len = 0;

    Comanda com;

    while(fread(&com , sizeof(Comanda) , 1 , archivoSemanal) == 1){
        insertarOrdenadoIDMozoArrayComandas(comandas , len , com);
    }


    // Posicionando el puntero al inicio del archivo:
    fseek(archivoSemanal , 0 , SEEK_SET);

    for(int i = 0 ; i < len ; i++){
        fwrite(&comandas[i] , sizeof(Comanda) , 1 , archivoSemanal);
    }

    fclose(archivoSemanal);
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

void insertarOrdenadoIDMozoArrayComandas(Comanda comandas[], int& len, Comanda com){
    int i = 0;

    while(i < len && comandas[i].idMozo < com.idMozo){
        i++;
    }

    if(i == len){
        comandas[len] = com;
        len++;
    }
    else{
        insertarPosEspecificaArrayComandas(comandas , len , com , i);
    }
}

void insertarPosEspecificaArrayComandas(Comanda comandas[], int& len, Comanda com, int pos){
    for(int i = len ; i > pos ; i--){
        comandas[i] = comandas[i-1];
    }

    comandas[pos] = com;
    len++;
}