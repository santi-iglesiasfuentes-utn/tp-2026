#include<iostream>
#include<cstdio>
#include<cstring>

using namespace std;

// Estructuras:

struct Comanda_historica {
    char fecha[11];
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stock;
};



// Prototipos de funciones:

// Prototipos de funciones de "archivo de inventario:"
void actualizarArchivoInventario(Comanda);
long busquedaBinariaArchivoInventario(Comanda);
void modificarRegistroEspecifico(long,Comanda);

// Prototipos de funciones de 'mozos':
int busquedaSecArchivoMozos(char[]);
Mozo crearMozo(Comanda_historica,int);
void inicializarPasswordMozo(char[],int);
void crearContraseniaMozo(char[],int,char[]);
void cargarMozoArchivoMozos(Mozo);
void modificarComisionesMozoArchivoMozos(int,Comanda_historica);

// Prototipos de funciones 'archivo de comandas del dia':
Comanda cargarInfoComanda(Comanda_historica);
void obtenerMozoPosEspecificaArchivoMozos(int,Mozo&);
void agregarComandaArchivoComandasDia(char[],Comanda);
void reiniciarNombreArchivoComandasDia(char[]);
void ordenarArchivoComandasDia(char[]);
int obtenerCantidadRegistrosArchivoComandas(FILE*);
void insertarOrdenadoArrayComandasDia(Comanda[],int&,Comanda);
void insertarPosEspecificaArrayComandasDia(Comanda[],int&,Comanda,int);




int main(){
    FILE* archivoComandas = fopen("comandas_historicas.dat" , "rb");

    if(archivoComandas != NULL){
        int numComanda = 1;

        Comanda_historica com;

        // Variables para el 'archivo de mozos':
        char nombreMozo[50];
        int posMozo;
        int idMozo = 1;
        Mozo mozo;


        // Variables para el 'archivo de comandas':
        char nombreArchivoComandasDia[30];
        Comanda comanda;



        while( fread(&com , sizeof(Comanda_historica) , 1 , archivoComandas) == 1 ){
            // Reinciando variables:
            reiniciarNombreArchivoComandasDia(nombreArchivoComandasDia);

            // cout << "Numero de comanda: " << numComanda << endl;



            /* Mozos: -------------------------------------------------------- */

            if(numComanda == 1){
                mozo = crearMozo(com , idMozo);
                cargarMozoArchivoMozos(mozo);

                idMozo++;
            }
            else{
                strcpy(nombreMozo , com.nombreMozo);

                posMozo = busquedaSecArchivoMozos(nombreMozo);

                if(posMozo == -1){
                    mozo = crearMozo(com , idMozo);
                    cargarMozoArchivoMozos(mozo);

                    idMozo++;
                }
                else if(posMozo != -2){
                    modificarComisionesMozoArchivoMozos(posMozo , com);
                }
            }



            /* Comanda: ---------------------------------------------------------------*/

            // Creando el nombre del archivo de dias:
            strcat(nombreArchivoComandasDia , "comandas_");
            strcat(nombreArchivoComandasDia , com.fecha);
            strcat(nombreArchivoComandasDia , ".dat");

            comanda = cargarInfoComanda(com);

            agregarComandaArchivoComandasDia(nombreArchivoComandasDia , comanda);

            ordenarArchivoComandasDia(nombreArchivoComandasDia);




            /* Inventario: ---------------------------------------------------------------*/

            actualizarArchivoInventario(comanda);




            numComanda++;
        }

        fclose(archivoComandas);
    }
    else{
        cout << "Error: el archivo de comandas historicas no se ha abierto correctamente." << endl;
    }

    return 0;
}






// Funciones:

// Funciones de "inventario":

void actualizarArchivoInventario(Comanda com){
    long posProducto = busquedaBinariaArchivoInventario(com);

    // cout << "Posicion: " << posProducto << endl;

    if(posProducto != -1){
        modificarRegistroEspecifico(posProducto , com);
    }
}

long busquedaBinariaArchivoInventario(Comanda com){
    FILE* archivo = fopen("inventario.dat" , "rb");

    if(archivo == NULL){
        cout << "Error: el archivo de 'inventario' no se ha abierto correctamente." << endl;

        cout << "No se ha podido obtener la posicion del producto en el archivo." << endl;

        return -1;
    }

    // Posicionando el puntero al final del archivo:
    fseek(archivo , 0 , SEEK_END);

    int cantRegs = ftell(archivo) / sizeof(Producto);

    long izquierda = 0;
    long derecha = cantRegs - 1;
    long mitad;
    long pos = -1;
    Producto prod;


    while(izquierda <= derecha && pos == -1){
        mitad = (izquierda + derecha) / 2;

        fseek(archivo , mitad * sizeof(Producto) , SEEK_SET);

        fread(&prod , sizeof(Producto) , 1 , archivo);

        if(prod.codigo == com.codigoProducto){
            pos = mitad;
        }
        else if(prod.codigo < com.codigoProducto){
            izquierda = mitad + 1;
        }
        else{
            derecha = mitad - 1;
        }
    }

    fclose(archivo);

    return pos;
}

void modificarRegistroEspecifico(long posProd, Comanda com){
    FILE* archivo = fopen("inventario.dat" , "rb+");

    if(archivo == NULL){
        cout << "Error: el archivo de 'inventario' no se ha abierto correctamente." << endl;
        cout << "No se ha podido modificar el stock restante." << endl;

        return;
    }

    Producto prod;


    fseek(archivo , posProd * sizeof(Producto) , SEEK_SET);

    fread(&prod , sizeof(Producto) , 1 , archivo);


    prod.stock -= com.cantidad;


    fseek(archivo , -1 * (long)sizeof(Producto) , SEEK_CUR);

    fwrite(&prod , sizeof(Producto) , 1 , archivo);


    fclose(archivo);
}



// Funciones de "mozos":

int busquedaSecArchivoMozos(char nombreMozo[]){
    FILE* archivoMozos = fopen("mozos.dat" , "rb");

    if(archivoMozos == NULL){
        cout << "Error: el 'archivo de mozos' no se ha abierto correctamente." << endl;

        cout << "No se ha podido buscar la posicion del mozo dentro del archivo." << endl;

        return -2;
    }

    int posMozo = -1;
    Mozo mozo;

    while(fread(&mozo , sizeof(Mozo) , 1 , archivoMozos) == 1 && posMozo == -1){
        if(strcmp(mozo.nombre , nombreMozo) == 0){
            posMozo = mozo.idMozo - 1;
        }
    }

    return posMozo;
}

Mozo crearMozo(Comanda_historica com, int idMozo){
    Mozo m;

    m.idMozo = idMozo;

    strcpy(m.nombre , com.nombreMozo);


    inicializarPasswordMozo(m.password , 20);
    crearContraseniaMozo(m.nombre , 50 , m.password);


    m.totalComision += com.comision;

    return m;
}

void inicializarPasswordMozo(char password[], int max){
    for(int i = 0 ; i < max ; i++){
        password[i] = 0;
    }
}

void crearContraseniaMozo(char nombreMozo[], int maxNombre, char password[]){
    int i = 0;

    while(i < maxNombre-1 && nombreMozo[i] != '\0' && nombreMozo[i] != ' '){
        password[i] = nombreMozo[i];

        i++;
    }

    // Realizando sumas en cada posicion de la 'password':
    for(int i = 0 ; password[i] != '\0' ; i++){
        password[i] += 5;
    }
}

void cargarMozoArchivoMozos(Mozo mozo){
    FILE* archivoMozos = fopen("mozos.dat" , "ab");

    if(archivoMozos == NULL){
        cout << "Error: el 'archivo de mozos' no se ha abierto correctamente." << endl;

        cout << "No se ha podido cargar el mozo en el archivo." << endl;

        return;
    }

    fwrite(&mozo , sizeof(Mozo) , 1 , archivoMozos);

    fclose(archivoMozos);
}

void modificarComisionesMozoArchivoMozos(int posMozo, Comanda_historica com){
    FILE* archivoMozos = fopen("mozos.dat" , "rb+");

    if(archivoMozos == NULL){
        cout << "Error: el 'archivo de mozos' no se ha abierto correctamente." << endl;

        cout << "No se han podido modificar las comisiones del mozo." << endl;

        return;
    }

    Mozo mozo;

    // Modificar el registro especifico:
    fseek(archivoMozos , posMozo * sizeof(Mozo) , SEEK_SET);

    fread(&mozo , sizeof(Mozo) , 1 , archivoMozos);

    mozo.totalComision += com.comision;

    fseek(archivoMozos , -1 * (long)sizeof(Mozo) , SEEK_CUR);

    fwrite(&mozo , sizeof(Mozo) , 1 , archivoMozos);

    fclose(archivoMozos);
}



// Funciones de "comandas":

Comanda cargarInfoComanda(Comanda_historica com){
    Comanda comanda;

    char nombreMozo[50];
    strcpy(nombreMozo , com.nombreMozo);

    int posMozo = busquedaSecArchivoMozos(nombreMozo);

    Mozo mozo;
    obtenerMozoPosEspecificaArchivoMozos(posMozo , mozo);


    comanda.idMozo = mozo.idMozo;
    comanda.codigoProducto = com.codigoProducto;
    comanda.cantidad = com.cantidad;
    comanda.comision = com.comision;

    return comanda;
}

void obtenerMozoPosEspecificaArchivoMozos(int pos, Mozo& infoMozo){
    FILE* archivoMozos = fopen("mozos.dat" , "rb");

    if(archivoMozos == NULL){
        cout << "Error: el 'archivo de mozos' no se ha abierto correctamente." << endl;

        cout << "No se ha podido obtener la informacion del mozo." << endl;

        return;
    }

    fseek(archivoMozos , pos * sizeof(Mozo) , SEEK_SET);

    fread(&infoMozo , sizeof(Mozo) , 1 , archivoMozos);

    fclose(archivoMozos);
}

void agregarComandaArchivoComandasDia(char nombreArchivo[], Comanda com){
    FILE* archivo = fopen(nombreArchivo , "ab+");

    if(archivo != NULL){
        fwrite(&com , sizeof(Comanda) , 1 , archivo);

        fclose(archivo);
    }
    else{
        cout << "Error: el archivo no se ha abierto correctamente." << endl;

        cout << "No se ha podido cargar la comanda en el archivo correspondiente." << endl;
    }
}

void reiniciarNombreArchivoComandasDia(char nombreArchivoComandasDia[]){
    for(int i = 0 ; nombreArchivoComandasDia[i] != '\0' ; i++){
        nombreArchivoComandasDia[i] = 0;
    }
}

void ordenarArchivoComandasDia(char nombreArchivoComandas[]){
    FILE* archivo = fopen(nombreArchivoComandas , "rb+");

    if(archivo == NULL){
        cout << "Error: el archivo no se ha abierto correctamente." << endl;

        cout << "No se ha podido ordenar el archivo." << endl;

        return;
    }

    int cantRegs = obtenerCantidadRegistrosArchivoComandas(archivo);

    Comanda comandasDia[cantRegs];
    int len = 0;

    Comanda aux;

    while(fread(&aux , sizeof(Comanda) , 1 , archivo) == 1){
        insertarOrdenadoArrayComandasDia(comandasDia , len , aux);
    }


    // Posicionando el puntero el principio del archivo:
    fseek(archivo , 0 , SEEK_SET);

    for(int i = 0 ; i < len ; i++){
        aux = comandasDia[i];

        fwrite(&aux , sizeof(Comanda) , 1 , archivo);
    }

    fclose(archivo);
}

int obtenerCantidadRegistrosArchivoComandas(FILE* archivo){
    int cantRegs;
    long posInicial = ftell(archivo);

    // Posicionando el puntero al final del archivo:
    fseek(archivo , 0 , SEEK_END);

    cantRegs = ftell(archivo) / sizeof(Comanda);

    // Devolviendo al puntero a la posicion inicial:
    fseek(archivo , posInicial , SEEK_SET);

    return cantRegs;
}

void insertarOrdenadoArrayComandasDia(Comanda comandasDia[], int& len, Comanda com){
    int i = 0;

    while(i < len && comandasDia[i].idMozo < com.idMozo){
        i++;
    }

    if(i == len){
        comandasDia[len] = com;
        len++;
    }
    else{
        insertarPosEspecificaArrayComandasDia(comandasDia , len , com , i);
    }
}

void insertarPosEspecificaArrayComandasDia(Comanda comandasDia[], int& len, Comanda com, int pos){
    for(int i = len ; i > pos ; i--){
        comandasDia[i] = comandasDia[i-1];
    }

    comandasDia[pos] = com;
    len++;
}