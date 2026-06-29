#include <iostream>
using namespace std;

//Función 1 - concatenarCadenas
void concatenarCadenas(char destino[], char origen[]) {
    int lenDest = largoCadena(destino);

    for (int i = lenDest; i < lenDest + largoCadena(origen); i++) {
        destino[i] = origen[i - lenDest];
    }
}

void concatenarCadenasTruncado(char destino[], int destBuf, char origen[]) {
    int lenDest = largoCadena(destino);
    int realLen = lenDest;

    for (int i = lenDest; i < destBuf; i++) {
        destino[i] = origen[i - lenDest];
        realLen++;
    }

    destino[realLen] = '\0'; // Nos aseguramos de insertar el null character
}


//Función 2 - compararCadenas
int compararCadenas(char cadena1[], char cadena2[]) {
    int i = 0;
    while (cadena1[i] != '\0' && cadena2[i] != '\0') {
        if (cadena1[i] != cadena2[i]) {
            return cadena1[i] - cadena2[i]; // Devuelve la diferencia ASCII 
        }
        i++;
    }
    // si una termina antes, la diferencia de caracteres determina cuál es mayor o menor
    return cadena1[i] - cadena2[i];
}


//Función 3 - copiarCadenas
void copiarCadenas(char destino[], char origen[]) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0'; // Aseguramos el carácter centinela al final 
}


//Función 4 - largoCadena
int largoCadena(char cadena[]) {
    int c = 0;
    while (cadena[c] != '\0') {
        c++;
    }
    return c;
}


//Función 5 - contarLetrasEnCadena



//Función 6 - contarPalabrasEnCadena



//Función 7 - pasajeDiaACadena

void pasajeDiaACadena(int fecha, char destino[])
    {
    char meses[12][11] =
        {
            "enero",
            "febrero",
            "marzo",
            "abril",
            "mayo",
            "junio",
            "julio",
            "agosto",
            "septiembre",
            "octubre",
            "noviembre",
            "diciembre",
        };

    // Separamos la fecha AAAAMMDD.
    int anio = fecha / 10000;
    int mes = (fecha / 100) % 100;
    int dia = fecha % 100;
    int i= 0;

    // dia "de"
    if (dia >= 10)
        {
            destino[i++] = dia / 10 + '0';
        }
    destino[i++] = dia % 10 + '0';
    
    destino[i++] = ' ';
    destino[i++] = 'd';
    destino[i++] = 'e';
    destino[i++] = ' ';

    // mes "de" (se usa mes-1 porque el 4 en el arreglo en realidad es el mes 5, por ej.)
    int j = 0;

    while (meses[mes - 1][j] != '\0')
        {
            destino[i++] = meses[mes - 1][j++];
        }
    destino[i++] = ' ';
    destino[i++] = 'd';
    destino[i++] = 'e';
    destino[i++] = ' ';
//año
    destino[i++] = anio / 1000 + '0';
    destino[i++] = (anio / 100) % 10 + '0';
    destino[i++] = (anio / 10) % 10 + '0';
    destino[i++] = anio % 10 + '0';
    destino[i++] = '\0';
}

//Función 8 - pasajeSegundosACadena
void pasajeSegundosACadena(float segundos, char destino[]) 
    {
    //calculo de horas, es el numero dividido por 3600, Despues a la variable segundos se le restan las horas multiplicadas por 3600 para obtener los minutos y segundos.
    int horas = segundos / 3600;    
    segundos = segundos - horas * 3600;
    //calculo de minutos, es el numero dividido por 60, Despues a la variable segundos se le restan los min multiplicados por 60 para obtener los segundos y decimas de segundo.
    int minutos = segundos / 60;    
    segundos = segundos - minutos * 60;
    // el int "borra" los decimales tenés los segundos en segentero y decimasD.S. en decimas que al multiplicar x 10 te deja solo el prim digito
    int segEntero = segundos;
    int decimas = (segundos - segEntero) * 10;
    //todas las posiciones del vector se "llenan" con estas lineas de codigo.
    destino[0] = horas / 10 + '0';
    destino[1] = horas % 10 + '0';
    destino[2] = ':';
    destino[3] = minutos / 10 + '0';
    destino[4] = minutos % 10 + '0';
    destino[5] = ':';
    destino[6] = segEntero / 10 + '0';
    destino[7] = segEntero % 10 + '0';
    destino[8] = '.';
    destino[9] = decimas + '0';
    destino[10] = '\0';


int main() {
    cout << "--- PRUEBAS DEL TRABAJO PRÁCTICO ---" << endl << endl;

    //Prueba Función 1 - concatenarCadenas
    cout << "Prueba Función 1 - concatenarCadenas" << endl;
    const int size = 10;
    char cadena1[20] = "Hola ";
    char cadena1C[size] = "Hola ";
    char cadena2[] = "Mundo";

    concatenarCadenas(cadena1, cadena2);
    concatenarCadenasTruncado(cadena1C, size, cadena2); // Para poder saber cuando truncar, se requiere pasar el espacio alojado en la cadena de destino
    
    cout << cadena1 << endl;
    cout << cadena1C << endl;
    cout << "------------------------------------" << endl;

    //Prueba Función 2 - compararCadenas
    cout << "Prueba Función 2 - compararCadenas" << endl;
    char cadena1[20];
    char cadena2[20];
    cout << "Ingrese la primera cadena: ";
    cin.getline(cadena1, 20);
    cout << "Ingrese la segunda cadena: ";
    cin.getline(cadena2, 20);
    int resultado = compararCadenas(cadena1, cadena2);
    if (resultado == 0) {
        cout << "0" << endl; // Las cadenas son iguales
    } else if (resultado < 0) {
        cout << "Es menor" << endl; // La primera cadena es menor
    } else {
        cout << "Es mayor" << endl; // La primera cadena es mayor
    }
    cout << "------------------------------------" << endl;

    //Prueba Función 3 - copiarCadenas
    cout << "Prueba Función 3 - copiarCadenas" << endl;
    char cadenaDestino[30] = "Uruguay tiene 4 mundiales";
    char cadenaOrigen[30] = "Uruguay tiene 2 mundiales";
    //quiero que muestre la cadenaDestino y la cadenaOrigen antes de copiar, y luego que muestre la cadenaDestino después de copiar
    cout << "Cadena 1: " << cadenaDestino << endl;
    cout << "Cadena 2: " << cadenaOrigen << endl;
    copiarCadenas(cadenaDestino, cadenaOrigen); 
    cout << "Cadena 1 copiada: " << cadenaDestino << endl;
    cout << "------------------------------------" << endl;

    //Prueba Función 4 - largoCadena
    cout << "Prueba Función 4 - largoCadena" << endl;
    char cadena[20] = "Hola Mundo";
    int largo = largoCadena(cadena);
    cout << "Largo de la cadena: " << largo << endl;
    cout << "------------------------------------" << endl;
    
    //Prueba Función 5 - contarLetrasEnCadena
    cout << "Prueba Función 5 - contarLetrasEnCadena" << endl;
    
    cout << "------------------------------------" << endl;
    
    //Prueba Función 6 - contarPalabrasEnCadena
    cout << "Prueba Función 6 - contarPalabrasEnCadena" << endl;

    cout << "------------------------------------" << endl;

    //Prueba Función 7 - pasajeDiaACadena
    cout << "Prueba Función 7 - pasajeDiaACadena" << endl;
    char cadena[50];
    pasajeDiaACadena(20260406, cadena);
    cout << cadena << endl;
    cout << "------------------------------------" << endl;

    //Prueba Función 8 - pasajeSegundosACadena
    cout << "Prueba Función 8 - pasajeSegundosACadena" << endl;
    char cadena[11];
    pasajeSegundosACadena(45106.97, cadena);
    cout << cadena << endl;
    cout << "------------------------------------" << endl;

    return 0;
}
