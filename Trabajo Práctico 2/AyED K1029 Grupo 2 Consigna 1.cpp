#include <iostream>
#include <cstring>

#define CANTIDAD_CORREDORES_TOTAL 2000
#define CANTIDAD_CORREDORES_CARRERA 1000

#define LARGO_CAMPO_POSICIONES 16
#define LARGO_CAMPO_ID 6
#define LARGO_CAMPO_GENERO 10
#define LARGO_CAMPO_NOMBRE 40
#define LARGO_CAMPO_CATEGORIA 50
#define LARGO_CAMPO_LOCALIDAD 20
#define LARGO_CAMPO_TIEMPOS 20


#define NOMBRE_ARCHIVO_CORREDORES_DEFAULT "Archivo corredores 4Refugios.bin"
#define NOMBRE_INFORME_CLASICA_DEFAULT "Informe Carrera Clásica.bin"
#define NOMBRE_INFORME_NONSTOP_DEFAULT "Informe Carrera NonStop.bin"

using namespace std;

struct RegCorredores {
    int numero;

    char nombreApellido[50];
    char categoria[50]; // 4 Refugios <clásica/nonstop> - <DAMAS/CABALLEROS> (<rango de edad>)
    char genero; // M / F
    char localidad[40];
    char llegada[11]; // DNF, DNF (NL), DSP (FE) o el tiempo de llegada (formato HH:MM:SS.D)
};

struct HeadersInforme {
    char posGral[LARGO_CAMPO_POSICIONES] = "";
    char posGenero[LARGO_CAMPO_POSICIONES + 1] = "";
    char posCat[LARGO_CAMPO_POSICIONES] = "";
    char corredorId[LARGO_CAMPO_ID + 1] = "";

    char nombreApellido[LARGO_CAMPO_NOMBRE] = "";
    char categoria[LARGO_CAMPO_CATEGORIA + 1] = "";
    char genero[LARGO_CAMPO_GENERO + 1] = "";
    char localidad[LARGO_CAMPO_LOCALIDAD] = "";
    char total[LARGO_CAMPO_TIEMPOS] = "";
    char difPrimero[LARGO_CAMPO_TIEMPOS] = "";
    char difAnterior[LARGO_CAMPO_TIEMPOS] = "";
};

struct RegInforme {
    char posGral[LARGO_CAMPO_POSICIONES] = ""; // debe figurar como "Pos. Gral."
    char posGenero[LARGO_CAMPO_POSICIONES] = ""; // debe figurar como "Pos. Género"
    char posCat[LARGO_CAMPO_POSICIONES] = ""; // debe figurar como "Pos. Cat."
    
    char corredorId[LARGO_CAMPO_ID] = ""; // debe figurar como "N°"

    char nombreApellido[LARGO_CAMPO_NOMBRE] = "";
    char categoria[LARGO_CAMPO_CATEGORIA] = "";
    char genero[LARGO_CAMPO_GENERO] = "";
    char localidad[LARGO_CAMPO_LOCALIDAD] = "";

    char total[LARGO_CAMPO_TIEMPOS] = ""; // debe figurar como "Total"
    char difPrimero[LARGO_CAMPO_TIEMPOS] = ""; // debe figurar como "Diferencia primero"
    char difAnterior[LARGO_CAMPO_TIEMPOS] = ""; // debe figurar como "Diferencia anterior"
};

// Declaraciones
int calcularPosGeneral(RegCorredores[], int);
int calcularPosGenero(RegCorredores[], int);
int calcularPosCategoria(RegCorredores[], int);
int calcularDifPrimero(RegCorredores[], int);
int calcularDifAnterior(RegCorredores[], int);
void establecerLargoCampo(char[], int, const char[]);
void establecerLargoCampo(char[], int, const char);
void establecerLargoCampo(char[], int, int);
void establecerLargoCampoCentrado(char[], int, const char[]);
void establecerLargoCampoCentrado(char[], int, const char);
void establecerLargoCampoCentrado(char[], int, int);
void establecerLargoHeaders(HeadersInforme&);
void generarInforme(FILE*, RegCorredores[], int);
RegInforme generarRegistroInforme(RegCorredores, const char[], const char[], const char[], const char[], const char[], const char[]);
void leerCorredores(RegCorredores[], FILE*);
void loadData(char[], int, char[], int, char[], int);
void ordenar(RegCorredores[], int);
void pasajeDecimasACadena(int, char[]);
void setIfEmpty(char[], const char[]);
void sobreescribirLlegada(FILE*, RegCorredores&);
int tiempoADecimas(const char[]);


int main() {
    RegCorredores corredores[CANTIDAD_CORREDORES_TOTAL] = {};
    RegCorredores clasica[CANTIDAD_CORREDORES_CARRERA], nonstop[CANTIDAD_CORREDORES_CARRERA];

    char carpetaRuta[200] = "./";
    char nombreDelArchivo[100] = "";
    char ruta[300] = "";

    // 1. Archivo corredores (lectura + escritura por el reemplazo)
    strcpy(nombreDelArchivo, NOMBRE_ARCHIVO_CORREDORES_DEFAULT);
    strcpy(ruta, carpetaRuta);
    strcat(ruta, nombreDelArchivo);

    FILE* fCorredores = fopen(ruta, "rb+");
    if (!fCorredores) {
        cout << "Hubo un error al intentar abrir el archivo. Revise que la ruta sea correcta.\n";
        cout << "Ruta detectada: " << ruta << endl;
        return 1;
    }

    leerCorredores(corredores, fCorredores);
    fclose(fCorredores);

    int nC = 0, nN = 0;
    for (int i = 0; i < CANTIDAD_CORREDORES_TOTAL; i++) {
        if (corredores[i].numero == 0) break;
        
        if (strstr(corredores[i].categoria, "Clasica") != NULL)
        clasica[nC++] = corredores[i];
        else
        nonstop[nN++] = corredores[i];
    }
    
    ordenar(clasica, nC);
    ordenar(nonstop, nN);
    
    // 2. Informe Clásica
    strcpy(nombreDelArchivo, NOMBRE_INFORME_CLASICA_DEFAULT);
    strcpy(ruta, carpetaRuta);
    strcat(ruta, nombreDelArchivo);
    
    FILE* fListadoClasica = fopen(ruta, "wb");
    if (!fListadoClasica) {
        cout << "Hubo un error al intentar crear el archivo. Revise que la ruta sea correcta.\n";
        cout << "Ruta detectada: " << ruta << endl;
        return 1;
    }

    cout << "--- Informe carrera Clásica ---" << endl;
    generarInforme(fListadoClasica, clasica, nC);
    cout << "Informe generado." << endl;
    fclose(fListadoClasica);
    
    // 3. Informe NonStop
    strcpy(nombreDelArchivo, NOMBRE_INFORME_NONSTOP_DEFAULT);
    strcpy(ruta, carpetaRuta);
    strcat(ruta, nombreDelArchivo);
    
    FILE* fListadoNonStop = fopen(ruta, "wb");
    if (!fListadoNonStop) {
        cout << "Hubo un error al intentar crear el informe. Revise que la ruta sea correcta.\n";
        cout << "Ruta detectada: " << ruta << endl;
        return 1;
    }
    
    cout << "--- Informe carrera NonStop ---" << endl;
    generarInforme(fListadoNonStop, nonstop, nN);
    cout << "Informe generado." << endl;
    fclose(fListadoNonStop);

    return 0;
}

void sobreescribirLlegada(FILE* f, RegCorredores& reg) {
    if (reg.llegada[0] == 'D') {
        strcpy(reg.llegada, "No Termino");
        fseek(f, -sizeof(RegCorredores), SEEK_CUR); // retrocedemos al registro anterior
        fwrite(&reg, sizeof(RegCorredores), 1, f);
        fflush(f); // fuerza la escritura antes de seguir leyendo
    }
}

void leerCorredores(RegCorredores corredores[], FILE* file) {
    RegCorredores reg;
    int i = 0;

    while (fread(&reg, sizeof(RegCorredores), 1, file)) {
        sobreescribirLlegada(file, reg);
        corredores[i++] = reg;
    };
}

// Ordena por tiempo, los -1 van al final
void ordenar(RegCorredores v[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            int t1 = tiempoADecimas(v[j].llegada);
            int t2 = tiempoADecimas(v[j+1].llegada);
            
            if (t1 == -1) t1 = 99999999;
            if (t2 == -1) t2 = 99999999;
            
            if (t1 > t2) {
                RegCorredores aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
    }
}

void generarInforme(FILE* file, RegCorredores v[], int n) {
    HeadersInforme headers;
    
    establecerLargoHeaders(headers);
    
    fwrite(&headers, sizeof(HeadersInforme), 1, file);
    
    cout << headers.posGral 
         << headers.posGenero 
         << headers.posCat
         << headers.corredorId
         << headers.nombreApellido 
         << headers.categoria 
         << headers.genero
         << headers.localidad 
         << headers.total 
         << headers.difPrimero 
         << headers.difAnterior
         << endl;
    
    for (int i = 0; i < n; i++) {
        //todas las variables y las cadenas
        int posGral = calcularPosGeneral(v, i);
        int posGenero = calcularPosGenero(v, i);
        int posCat = calcularPosCategoria(v, i);
        
        char posGralChar[LARGO_CAMPO_POSICIONES] = "";
        char posGeneroChar[LARGO_CAMPO_POSICIONES] = "";
        char posCatChar[LARGO_CAMPO_POSICIONES] = "";
        
        // cast de posiciones a char[]
        snprintf(posGralChar, LARGO_CAMPO_POSICIONES, "%d", posGral);
        snprintf(posGeneroChar, LARGO_CAMPO_POSICIONES, "%d", posGenero);
        snprintf(posCatChar, LARGO_CAMPO_POSICIONES, "%d", posCat);
        
        if (posGral == -1) strcpy(posGralChar, "-");
        if (posGenero == -1) strcpy(posGeneroChar, "-");
        if (posCat == -1) strcpy(posCatChar, "-");

        //estos 3 son calculados como int porque las funciones para calcular la posicion devuelven int pero en realidad en el struct son char y para poder mostrarlos como "-", conviene que sea char.
        int difPrimero = calcularDifPrimero(v, i);
        int difAnterior = calcularDifAnterior(v, i);
        int tiempoTotal = tiempoADecimas(v[i].llegada);
        
        char tiempoTotalChar[20] = "";
        pasajeDecimasACadena(tiempoTotal, tiempoTotalChar);
        char diferenciaPrimeroChar[20] = "";
        char diferenciaAnteriorChar[20] = "";
        
        //llenar las cadenas de las Diferencias 
        if (difPrimero == -1) {
            strcpy(diferenciaPrimeroChar, "-"); 
        }
        else { pasajeDecimasACadena(difPrimero, diferenciaPrimeroChar); 
        } 
        if (difAnterior == -1) {
            strcpy(diferenciaAnteriorChar, "-");
        } 
        else { pasajeDecimasACadena(difAnterior, diferenciaAnteriorChar); 
            } 
            
        RegInforme reg = generarRegistroInforme(
            v[i],
            posGralChar,
            posGeneroChar,
            posCatChar,
            tiempoTotalChar,
            diferenciaPrimeroChar,
            diferenciaAnteriorChar
        );
        
        
        cout << reg.posGral 
             << reg.posGenero 
             << reg.posCat
             << reg.corredorId
             << reg.nombreApellido 
             << reg.categoria 
             << reg.genero 
             << reg.localidad 
             << reg.total 
             << reg.difPrimero 
             << reg.difAnterior
             << endl;
        
        fwrite(&reg, sizeof(RegInforme), 1, file);    
    }
}

RegInforme generarRegistroInforme(
    RegCorredores corredor, 
    const char posGral[],
    const char posGenero[],
    const char posCat[],
    const char total[], 
    const char difPrimero[], 
    const char difAnterior[]
) {
    RegInforme reg;

    establecerLargoCampoCentrado(reg.posGral, LARGO_CAMPO_POSICIONES, posGral);
    establecerLargoCampoCentrado(reg.posGenero, LARGO_CAMPO_POSICIONES, posGenero);
    establecerLargoCampoCentrado(reg.posCat, LARGO_CAMPO_POSICIONES, posCat);

    establecerLargoCampo(reg.corredorId, LARGO_CAMPO_ID, corredor.numero);
    
    establecerLargoCampo(reg.nombreApellido, LARGO_CAMPO_NOMBRE, corredor.nombreApellido);
    establecerLargoCampo(reg.categoria, LARGO_CAMPO_CATEGORIA, corredor.categoria);
    establecerLargoCampoCentrado(reg.genero, LARGO_CAMPO_GENERO, corredor.genero);
    establecerLargoCampo(reg.localidad, LARGO_CAMPO_LOCALIDAD, corredor.localidad);

    establecerLargoCampoCentrado(reg.total, LARGO_CAMPO_TIEMPOS, total);
    establecerLargoCampoCentrado(reg.difPrimero, LARGO_CAMPO_TIEMPOS, difPrimero);
    establecerLargoCampoCentrado(reg.difAnterior, LARGO_CAMPO_TIEMPOS, difAnterior);

    return reg;
}

void establecerLargoHeaders(HeadersInforme& headers) {
    establecerLargoCampoCentrado(headers.posGral, LARGO_CAMPO_POSICIONES, "Pos. Gral.");
    establecerLargoCampoCentrado(headers.posGenero, LARGO_CAMPO_POSICIONES + 1, "Pos. Género."); // Tildes ocupan 2 bytes también
    establecerLargoCampoCentrado(headers.posCat, LARGO_CAMPO_POSICIONES, "Pos. Cat.");
    establecerLargoCampoCentrado(headers.corredorId, LARGO_CAMPO_ID + 1, "N°"); // "°" ocupa 2 bytes
    establecerLargoCampoCentrado(headers.nombreApellido, LARGO_CAMPO_NOMBRE, "Nombre");
    establecerLargoCampoCentrado(headers.categoria, LARGO_CAMPO_CATEGORIA + 1, "Categoría");
    establecerLargoCampoCentrado(headers.genero, LARGO_CAMPO_GENERO + 1, "Género");
    establecerLargoCampoCentrado(headers.localidad, LARGO_CAMPO_LOCALIDAD, "Localidad");
    establecerLargoCampoCentrado(headers.total, LARGO_CAMPO_TIEMPOS, "Total");
    establecerLargoCampoCentrado(headers.difPrimero, LARGO_CAMPO_TIEMPOS, "Diferencia primero");
    establecerLargoCampoCentrado(headers.difAnterior, LARGO_CAMPO_TIEMPOS, "Diferencia anterior");
}

void establecerLargoCampo(char dest[], int destBuf, const char src[]) {
    strcpy(dest, src);
    
    for (int i = strlen(src); i < destBuf - 1; i++) {
        dest[i] = ' ';
    }
    dest[destBuf - 1] = '\0';
}

void establecerLargoCampo(char dest[], int destBuf, const char src) {
    dest[0] = src;

    for (int i = 1; i < destBuf - 1; i++) {
        dest[i] = ' ';
    }
    dest[destBuf - 1] = '\0';
}

void establecerLargoCampo(char dest[], int destBuf, int src) {
    snprintf(dest, destBuf, "%d", src); // easy int to char[] conversion
    
    for (int i = strlen(dest); i < destBuf - 1; i++) {
        dest[i] = ' ';
    }
    dest[destBuf - 1] = '\0';
}

void establecerLargoCampoCentrado(char dest[], int destBuf, const char src[]) {
    int srcLen = strlen(src);
    int availableSpace = destBuf - 1;

    if (srcLen >= availableSpace) {
        strcpy(dest, src);
    }
    
    int spacesToInsert = availableSpace - srcLen;
    int paddingLeft = spacesToInsert / 2;
    int paddingRight = spacesToInsert - paddingLeft; // No siempre seran ambos lados simétricos

    int idx = 0;
    for (int i = 0; i < paddingLeft; i++) {
        dest[idx++] = ' ';
    }

    
    for (int i = 0; i < srcLen; i++) {
        dest[idx++] = src[i];
    }

    for (int i = 0; i < paddingRight; i++) {
        dest[idx++] = ' ';
    }

    dest[availableSpace] = '\0';
}

void establecerLargoCampoCentrado(char dest[], int destBuf, const char src) {
    int availableSpace = destBuf - 1;
    
    int spacesToInsert = availableSpace - 1;
    int paddingLeft = spacesToInsert / 2;
    int paddingRight = spacesToInsert - paddingLeft; // No siempre seran ambos lados simétricos

    int idx = 0;
    for (int i = 0; i < paddingLeft; i++) {
        dest[idx++] = ' ';
    }

    dest[idx++] = src;

    for (int i = 0; i < paddingRight; i++) {
        dest[idx++] = ' ';
    }

    dest[availableSpace] = '\0';
}

void establecerLargoCampoCentrado(char dest[], int destBuf, int src) {
    char aux[6] = "";
    snprintf(aux, destBuf, "%d", src);
    int srcLen = strlen(aux);

    int availableSpace = destBuf - 1;

    if (srcLen >= availableSpace) {
        strcpy(dest, aux);
    }
    
    int spacesToInsert = availableSpace - srcLen;
    int paddingLeft = spacesToInsert / 2;
    int paddingRight = spacesToInsert - paddingLeft; // No siempre seran ambos lados simétricos
    
    int idx = 0;
    for (int i = 0; i < paddingLeft; i++) {
        dest[idx++] = ' ';
    }
    
    for (int i = 0; i < srcLen; i++) {
        dest[idx++] = aux[i];
    }

    for (int i = 0; i < paddingRight; i++) {
        dest[idx++] = ' ';
    }

    dest[availableSpace] = '\0';
}

// Convierte "HH:MM:SS.D" a decimas. Si es "No Termino" devuelve -1.
int tiempoADecimas(const char llegada[]) {
    if (llegada[0] == 'N') return -1;
    int h = (llegada[0]-'0')*10 + (llegada[1]-'0');
    int m = (llegada[3]-'0')*10 + (llegada[4]-'0');
    int s = (llegada[6]-'0')*10 + (llegada[7]-'0');
    int d = (llegada[9]-'0');
    return ((h*3600 + m*60 + s)*10 + d);
}

void pasajeDecimasACadena(int decimasTotal, char destino[]) {
    if (decimasTotal == -1) {
        strcpy(destino, "No Termino");
        return;
    }

    int horas = decimasTotal / 36000;
    decimasTotal = decimasTotal - horas * 36000;

    int minutos = decimasTotal / 600;
    decimasTotal = decimasTotal - minutos * 600;

    int segundos = decimasTotal / 10;
    int decimas = decimasTotal % 10;

    destino[0] = horas / 10 + '0';
    destino[1] = horas % 10 + '0';
    destino[2] = ':';

    destino[3] = minutos / 10 + '0';
    destino[4] = minutos % 10 + '0';
    destino[5] = ':';

    destino[6] = segundos / 10 + '0';
    destino[7] = segundos % 10 + '0';
    destino[8] = '.';

    destino[9] = decimas + '0';
    destino[10] = '\0';
}

// Calcula la posición general
int calcularPosGeneral(RegCorredores v[], int i) {
    if (tiempoADecimas(v[i].llegada) == -1) {
        return -1;
    }
    //la posicion que se muestra tiene que ser i+1 pq sino se muestra la posicion 0, además ya está ordenado el reg por tiempo de menor a mayor asi que no tiene que hacer nada 
    return i+1;
}

// Calcula la posición por género.
int calcularPosGenero(RegCorredores v[], int i) {
    if (tiempoADecimas(v[i].llegada) == -1) {
        return -1;
    }

    int pos = 1;

    for (int j = 0; j < i; j++) {
        //chequea cuantos del mismo genero hay con el contador pos
        if (v[j].genero == v[i].genero) {
            pos++;
        }
    }

    return pos;
}

// Calcula la posición por categoría.
int calcularPosCategoria(RegCorredores v[], int i) {
    if (tiempoADecimas(v[i].llegada) == -1) {
        return -1;
    }
    int pos = 1;
    for (int j = 0; j < i; j++) {
        //como la categoria esta en un array char usamos strcmp y si son iguales sumamos al contador
        if (strcmp(v[j].categoria, v[i].categoria) == 0) {
            pos++;
        }
    }
    return pos;
}

// Calcula la diferencia entre él y el primero.
int calcularDifPrimero(RegCorredores v[], int i) {
    if (tiempoADecimas(v[i].llegada) == -1) {
        return -1;
    }
  //esto está hecho así porque después a la hora de mostrar conviene mostrar todos los campos vacios como un - y conviene usar el -1 que ya usamos, hicimos lo mismo en la funcion para calcular al anterior
    if (i == 0) {
        return -1;
    }

    int tiempoCorredor = tiempoADecimas(v[i].llegada);
    int tiempoPrimero = tiempoADecimas(v[0].llegada);

    return tiempoCorredor - tiempoPrimero;
}

// Calcula la diferencia entre él y el anterior.
int calcularDifAnterior(RegCorredores v[], int indice) {

    // el primero no tiene anterior 
    if (indice == 0) {
        return -1;
    }
    if (tiempoADecimas(v[indice].llegada) == -1) {
        return -1;
    }

    int tiempoCorredor = tiempoADecimas(v[indice].llegada);
    int tiempoAnterior = tiempoADecimas(v[indice - 1].llegada);

    return tiempoCorredor - tiempoAnterior;
}
