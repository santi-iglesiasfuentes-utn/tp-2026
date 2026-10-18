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



// Prototipos de funciones:
void generarNombreArchivoSemanal(char[],char[],char[]);
void generarCadenaMes(char[],int);
void mostrarInformacionArchivo(char[],char[],char[]);





int main(){
    cout << "\t\t.:: Resumen ::." << endl << endl;

    char nombreArchivo[30];
    char numSemana[2];
    char mes[3];

    int mesInt;


    cout << "Ingrese el numero de mes actual: ";
    cin >> mesInt;

    cout << "Ingrese el numero de semana actual: ";
    cin >> numSemana;


    generarCadenaMes(mes , mesInt);

    generarNombreArchivoSemanal(nombreArchivo , numSemana , mes);



    cout << endl << endl << endl;



    mostrarInformacionArchivo(nombreArchivo , mes , numSemana);


    return 0;
}





// Funciones:

void generarNombreArchivoSemanal(char nombreArchivo[], char numSemana[], char mes[]){
    strcpy(nombreArchivo , "comandas_semana_s");

    strcat(nombreArchivo , numSemana);
    strcat(nombreArchivo , "-");
    strcat(nombreArchivo , mes);
    strcat(nombreArchivo , ".dat");
}

void generarCadenaMes(char mes[], int mesInt){
    if(mesInt < 10){
        mes[0] = '0';
        mes[1] = (48 + mesInt);
    }
    else{
        mes[0] = '1';

        if(mesInt == 10){
            mes[1] = '0';
        }
        else if(mesInt == 11){
            mes[1] = '1';
        }
        else{
            mes[1] = '2';
        }
    }

    mes[2] = '\0';
}

void mostrarInformacionArchivo(char nombreArchivo[], char mes[], char numSemana[]){
    cout << "\t\t.:: Archivo Semanal ::." << endl << endl;

    cout << "Mes: " << mes << endl;
    cout << "Numero de semana: " << numSemana << endl << endl << endl;



    FILE* archivo = fopen(nombreArchivo , "rb");
    
    if(archivo != NULL){
        Comanda com;

        int cantProdsVendidosMozo = 0, cantProdsVendidosBuffet = 0;
        float totalComision = 0;
        int idMozo;

        int leido = fread(&com , sizeof(Comanda) , 1 , archivo);

        while(leido == 1){
            idMozo = com.idMozo;

            while(leido == 1 && idMozo == com.idMozo){
                cantProdsVendidosMozo += com.cantidad;
                totalComision += com.comision;

                cantProdsVendidosBuffet += com.cantidad;

                leido = fread(&com , sizeof(Comanda) , 1 , archivo);
            }

            cout << "Informacion mozo " << idMozo << ":" << endl;
            cout << "Cantidad de productos vendidos: " << cantProdsVendidosMozo << endl;
            cout << "Total de comision a pagar: $" << totalComision << endl << endl;


            // Reiniciando variables:
            cantProdsVendidosMozo = 0;
            totalComision = 0;
        }

        cout << "Cantidad de productos vendidos por el buffet: " << cantProdsVendidosBuffet << endl << endl;

        fclose(archivo);
    }
    else{
        cout << "Error: el 'archivo de comandas semanal' no se ha abierto correctamente." << endl;

        cout << "No se pudo mostrar la informacion del archivo semanal." << endl;
    }
}