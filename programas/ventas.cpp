#include<iostream>
#include<cstdio>
#include<cstring>

using namespace std;


// Constantes:

const float TASA_COMISION = 0.10f;



// Estructuras:

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



// Prototipos de funciones "generales":
int obtenerLongitudCadenaCaracteres(char[]);
void inicializarCadenaCaracteres(char[],int);


// Prototipos de funciones:
bool validarFecha(char[]);
void generarNombreArchivoComandas(char[],char[]);
void cargarComandasEnArchivo(char[],char[]);
int validarInfoMozo(int,char[]);
int obtenerCantidadRegistrosArchivoMozos();
int busquedaDirectaArchivoMozos(Mozo&,int);
int busquedaBinariaArchivoInventario(int,Producto&);
bool verificarStockProducto(int,Producto);
void ordenarArchivoComandasIDMozo(char[]);
int obtenerCantidadRegistrosArchivoComandas(FILE*);
void insertarOrdenadoIDMozoArrayComandas(Comanda[],int&,Comanda);
void insertarPosEspecificaArrayComandas(Comanda[],int&,Comanda,int);




int main(){
    char nombreArchivoComandas[30];
    inicializarCadenaCaracteres(nombreArchivoComandas , 30);

    char fecha[11];
    bool fechaValida;


    cout << "Ingrese la fecha en formato 'DD-MM-AAAA': ";
    cin >> fecha;

    fechaValida = validarFecha(fecha);

    while(fechaValida == false){
        cout << "\nError: la fecha ingresada es invalida." << endl;

        cout << "\n\nIngrese la fecha en formato 'DD-MM-AAAA': ";
        cin >> fecha;

        fechaValida = validarFecha(fecha);
    }


    cout << endl << endl << endl;




    generarNombreArchivoComandas(nombreArchivoComandas , fecha);

    cargarComandasEnArchivo(fecha , nombreArchivoComandas);

    ordenarArchivoComandasIDMozo(nombreArchivoComandas);



    return 0;
}




// Funciones "generales":

int obtenerLongitudCadenaCaracteres(char cadena[]){
    int longitud = 0;
    
    for(int i = 0 ; cadena[i] != '\0' ; i++){
        longitud++;
    }

    return longitud;
}

void inicializarCadenaCaracteres(char cadena[], int max){
    for(int i = 0 ; i < max ; i++){
        cadena[i] = 0;
    }
}



// Funciones:

bool validarFecha(char fecha[]){
    // 04-08-2026

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

void generarNombreArchivoComandas(char nombreArchivo[], char fecha[]){
    strcpy(nombreArchivo , "comandas_");

    strcat(nombreArchivo , fecha);
    strcat(nombreArchivo , ".dat");
}

void cargarComandasEnArchivo(char fecha[], char nombreArchivo[]){
    FILE* archivoComandas = fopen(nombreArchivo , "wb");

    if(archivoComandas == NULL){
        cout << "Error: el archivo no se ha abierto correctamente." << endl;

        cout << "No se han podido cargar comandas en el archivo." << endl;

        return;
    }

    Comanda com;

    int idMozo;
    char passwordMozo[20];
    int infoValida = -1;
    int cantMozos = obtenerCantidadRegistrosArchivoMozos();

    char opc = 's';



    while(opc == 's'){
        cout << "\t\t.:: Comandas " << fecha << " ::." << endl << endl;



        // Verificar la informacion del mozo: ------------------------------------------------------------------------------

        inicializarCadenaCaracteres(passwordMozo , 20);

        cout << "Digite el ID de mozo: ";
        cin >> idMozo;

        while(idMozo < 1 || idMozo > cantMozos){
            cout << "Error: el ID de mozo ingresado no es valido." << endl << endl;

            cout << "Digite el ID de mozo: ";
            cin >> idMozo;
        }

        cout << "Ingrese la contrasenia: ";
        cin >> passwordMozo;

        infoValida = validarInfoMozo(idMozo , passwordMozo);



        while(infoValida == -1){
            cout << "Error: la informacion del mozo es incorrecta." << endl << endl;


            inicializarCadenaCaracteres(passwordMozo , 20);

            cout << "Digite el ID de mozo: ";
            cin >> idMozo;

            while(idMozo < 1 || idMozo > cantMozos){
                cout << "Error: el ID de mozo ingresado no es valido." << endl << endl;

                cout << "Digite el ID de mozo: ";
                cin >> idMozo;
            }

            cout << "Ingrese la contrasenia: ";
            cin >> passwordMozo;

            infoValida = validarInfoMozo(idMozo , passwordMozo);
        }





        // Cargar la informacion de la comanda:   -----------------------------------------------------------------

        int reiniciarComanda = 1;

        int codigoProd;
        int posProd;
        Producto prod;

        int cantUnidades;
        bool hayStock;




        com.idMozo = idMozo;

        while(reiniciarComanda == 1){
            cout << "Ingrese el codigo de producto: ";
            cin >> codigoProd;

            posProd = busquedaBinariaArchivoInventario(codigoProd , prod);

            while(posProd == -1){
                cout << "Error: el codigo de producto ingresado no existe." << endl << endl;

                cout << "Ingrese el codigo de producto: ";
                cin >> codigoProd;

                posProd = busquedaBinariaArchivoInventario(codigoProd , prod);
            }

            com.codigoProducto = codigoProd;




            cout << "Digite la cantidad de unidades: ";
            cin >> cantUnidades;

            while(cantUnidades <= 0){
                cout << "Error: el numero ingresado es invalido." << endl << endl;

                cout << "Digite la cantidad de unidades: ";
                cin >> cantUnidades;
            }

            hayStock = verificarStockProducto(cantUnidades , prod);

            if(hayStock == true){
                reiniciarComanda = 0;

                com.cantidad = cantUnidades;

                // Calculando comision:
                com.comision = (prod.precio * com.cantidad) * TASA_COMISION;
            }
            else{
                cout << "Error: no hay tanta cantidad del producto '" << codigoProd << "' disponible." << endl << endl;

                cout << "Informacion del producto:" << endl;
                cout << "Codigo de producto: " << prod.codigo << endl;
                cout << "Descripcion: " << prod.descripcion << endl;
                cout << "Cantidad disponible: " << prod.stock << endl << endl;

                cout << "Reiniciando la comanda..." << endl << endl << endl << endl << endl;

                reiniciarComanda = 1;
            }
        }


        // Escribir la comanda en el archivo:
        fwrite(&com , sizeof(Comanda) , 1 , archivoComandas);


        cout << "\n\nCargar una nueva comanda (s/n): ";
        cin >> opc;

        cout << endl << endl << endl << endl << endl;
    }

    fclose(archivoComandas);
}

int validarInfoMozo(int idMozo, char passwordMozo[]){
    Mozo mozo;
    int infoValida = -1;

    int leido = busquedaDirectaArchivoMozos(mozo , idMozo);

    if(leido == 1){
        int longPasswordIngresada = obtenerLongitudCadenaCaracteres(passwordMozo);
        int longPasswordMozo = obtenerLongitudCadenaCaracteres(mozo.password);

        if(longPasswordIngresada != longPasswordMozo){
            infoValida = -1;
        }
        else{
            int i = 0;

            while(passwordMozo[i] != '\0' && mozo.password[i] != '\0'){
                if(passwordMozo[i] != mozo.password[i]){
                    infoValida = -1;

                    break;
                }
                else{
                    infoValida = 1;
                }

                i++;
            }
        }
    }

    return infoValida;
}

int obtenerCantidadRegistrosArchivoMozos(){
    FILE* archivo = fopen("mozos.dat" , "rb");

    if(archivo == NULL){
        cout << "Error: el 'archivo de mozos' no se ha abierto correctamente." << endl;

        cout << "No se ha podido obtener la 'cantidad de registros del archivo de mozos'." << endl;
        
        return -1;
    }


    fseek(archivo , 0 , SEEK_END);

    int cantRegs = ftell(archivo) / sizeof(Mozo);

    fclose(archivo);


    return cantRegs;
}

int busquedaDirectaArchivoMozos(Mozo& mozo, int idMozo){
    FILE* archivoMozos = fopen("mozos.dat" , "rb");

    if(archivoMozos == NULL){
        cout << "Error: el 'archivo de mozos' no se ha abierto correctamente." << endl;

        cout << "No se ha podido obtener la informacion del mozo de forma directa." << endl;
        
        return -1;
    }


    int claveInicial = 1;
    long pos = idMozo - claveInicial;


    fseek(archivoMozos , pos * sizeof(Mozo) , SEEK_SET);

    int leido = fread(&mozo , sizeof(Mozo) , 1 , archivoMozos);

    fclose(archivoMozos);


    return leido;
}

int busquedaBinariaArchivoInventario(int codProducto, Producto& prod){
    FILE* archivo = fopen("inventario.dat" , "rb");

    if(archivo == NULL){
        cout << "Error: el 'archivo de inventario' no se ha abierto correctamente." << endl;
        
        cout << "No se ha podido realizar la 'busqueda binaria' en el archivo." << endl;

        return -1;
    }
    
    
    fseek(archivo , 0 , SEEK_END);
    int cantRegs = ftell(archivo) / sizeof(Producto);

    long izquierda = 0;
    long derecha = cantRegs - 1;
    long medio;
    int posProd = -1;

    while(izquierda <= derecha && posProd == -1){
        medio = (izquierda + derecha) / 2;

        fseek(archivo , medio * sizeof(Producto) , SEEK_SET);
        fread(&prod , sizeof(Producto) , 1 , archivo);

        if(prod.codigo == codProducto){
            posProd = medio;
        }
        else if(prod.codigo < codProducto){
            izquierda = medio + 1;
        }
        else{
            derecha = medio - 1;
        }
    }

    fclose(archivo);

    return posProd;
}

bool verificarStockProducto(int cantidad, Producto prod){
    bool hayStock;

    if(cantidad <= prod.stock){
        hayStock = true;
    }
    else{
        hayStock = false;
    }

    return hayStock;
}

void ordenarArchivoComandasIDMozo(char nombreArchivo[]){
    FILE* archivoComandas = fopen(nombreArchivo , "rb+");

    if(archivoComandas == NULL){
        cout << "Error: el 'archivo de comandas' no se ha abierto correctamente." << endl;

        cout << "No se ha podido 'ordenar el archivo'." << endl;

        return;
    }

    int cantRegs = obtenerCantidadRegistrosArchivoComandas(archivoComandas);

    Comanda comandas[cantRegs];
    int len = 0;

    Comanda com;

    while(fread(&com , sizeof(Comanda) , 1 , archivoComandas) == 1){
        insertarOrdenadoIDMozoArrayComandas(comandas , len , com);
    }



    // Posicionando el puntero al principio del archivo:
    fseek(archivoComandas , 0 , SEEK_SET);


    for(int i = 0 ; i < len ; i++){
        fwrite(&comandas[i] , sizeof(Comanda) , 1 , archivoComandas);
    }

    fclose(archivoComandas);
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

    while((i < len) && (comandas[i].idMozo < com.idMozo)){
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