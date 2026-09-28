#include <iostream>
#include <cstring>

using namespace std;


#define CANTIDAD_CORREDORES_TOTAL 2000

#define LARGO_CAMPO_CATEGORIA 50
#define LARGO_CAMPO_POSICION 16
#define LARGO_CAMPO_ID 6
#define LARGO_CAMPO_NOMBRE 50
#define LARGO_CAMPO_GENERO 10
#define LARGO_CAMPO_TIEMPOS 20
#define LARGO_CAMPO_LOCALIDAD 40
#define LARGO_CAMPO_LLEGADA 11


#define NOMBRE_ARCHIVO_CORREDORES_DEFAULT "Archivo corredores 4Refugios.bin"
#define NOMBRE_ARCHIVO_PODIOS_DEFAULT "Informe Podios 4Refugios.bin"


struct RegCorredores {
    int numero;
    char nombreApellido[LARGO_CAMPO_NOMBRE];
    char categoria[LARGO_CAMPO_CATEGORIA];
    char genero;
    char localidad[LARGO_CAMPO_LOCALIDAD];
    char llegada[LARGO_CAMPO_LLEGADA];
};

struct RegPodio {
    char categoria[LARGO_CAMPO_CATEGORIA];
    char posicion[LARGO_CAMPO_POSICION];
    char numero[LARGO_CAMPO_ID];
    char nombreApellido[LARGO_CAMPO_NOMBRE];
    char genero[LARGO_CAMPO_GENERO];
    char localidad[LARGO_CAMPO_LOCALIDAD];
    char tiempo[LARGO_CAMPO_LLEGADA];
};

struct HeadersPodio {
    char categoria[LARGO_CAMPO_CATEGORIA + 1] = "";
    char posicion[LARGO_CAMPO_POSICION + 1] = "";
    char numero[LARGO_CAMPO_ID + 1] = "";

    char nombreApellido[LARGO_CAMPO_NOMBRE] = "";
    char genero[LARGO_CAMPO_GENERO + 1] = "";
    char localidad[LARGO_CAMPO_LOCALIDAD] = "";
    char tiempo[LARGO_CAMPO_TIEMPOS] = "";
};

//estas estaban en el cpp1
int tiempoADecimas(const char[]);
void pasajeDecimasACadena(int, char[]);
void ordenar(RegCorredores[], int);
void sobreescribirLlegada(FILE*, RegCorredores&);
void leerCorredores(RegCorredores[], FILE*);
void establecerLargoCampo(char[], int, const char[]);
void establecerLargoCampo(char[], int, const char);
void establecerLargoCampo(char[], int, int);
void establecerLargoCampoCentrado(char[], int, const char[]);
void establecerLargoCampoCentrado(char[], int, const char);
void establecerLargoCampoCentrado(char[], int, int);
void establecerLargoHeaders(HeadersPodio&);

// Devuelve cuántas categorías diferentes existen.
int obtenerCantidadCategorias(RegCorredores[], int);

// Carga en una matriz de char todas las categorías diferentes.
void obtenerCategorias(RegCorredores[], int, char[][LARGO_CAMPO_CATEGORIA], int);

// Obtiene el podio de una categoría
void obtenerPodioCategoria(RegCorredores[], int, char[], RegCorredores[]);

// Ordena alfabéticamente las categorías.
void ordenarCategorias(char[][LARGO_CAMPO_CATEGORIA], int);

// Genera el archivo final con el podio de todas las categorías.
void generarPodios(FILE*, RegCorredores[], int, char[][LARGO_CAMPO_CATEGORIA], int);

int main() {
    RegCorredores corredores[CANTIDAD_CORREDORES_TOTAL] = {};

    char carpetaRuta[] = "./";
    char nombreDelArchivo[] = NOMBRE_ARCHIVO_CORREDORES_DEFAULT;
    char ruta[300];
    strcpy(ruta, carpetaRuta);
    strcat(ruta, nombreDelArchivo);
    
    FILE* fCorredores = fopen(ruta, "rb+");
    if (!fCorredores) {
        cout << "Hubo un error al intentar abrir el archivo. Revise que la ruta sea correcta.\n";
        cout << "Ruta detectada: " << ruta << endl;
        return 1;
    }
    
    int cantidadCorredores = 0;
    RegCorredores reg;
    while (fread(&reg, sizeof(RegCorredores), 1, fCorredores)) {
        sobreescribirLlegada(fCorredores, reg);
        corredores[cantidadCorredores] = reg;
        cantidadCorredores++;
    }
    fclose(fCorredores);
    
    //ordenamos los corredores 
    ordenar(corredores, cantidadCorredores);

    //cuantas categorias existen
    int cantidadCategorias = obtenerCantidadCategorias(corredores, cantidadCorredores);

    //matriz para guardar todas las categorias diferentes
    char categorias[CANTIDAD_CORREDORES_TOTAL][LARGO_CAMPO_CATEGORIA];

    obtenerCategorias(corredores, cantidadCorredores, categorias, cantidadCategorias);

    //ordenamos las categorias
    ordenarCategorias(categorias, cantidadCategorias);

    char nombreArchivoPodios[] = NOMBRE_ARCHIVO_PODIOS_DEFAULT;
    char rutaPodios[100];
    strcpy(rutaPodios, carpetaRuta);
    strcat(rutaPodios, nombreArchivoPodios);


    FILE* fPodios = fopen(rutaPodios, "wb");
    if (!fPodios) {
        cout << "Hubo un error al intentar crear el archivo. Revise que la ruta sea correcta.\n";
        cout << "Ruta detectada: " << rutaPodios << endl;
        return 1;
    }

    //Genera los podios 
    generarPodios(fPodios, corredores , cantidadCorredores , categorias , cantidadCategorias);
    cout << "Informe generado." << endl;
    fclose(fPodios);
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

/// Ordena por tiempo, los -1 van al final
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

void establecerLargoHeaders(HeadersPodio& headers) {
    establecerLargoCampoCentrado(headers.categoria, LARGO_CAMPO_CATEGORIA + 1, "Categoría");
    establecerLargoCampoCentrado(headers.posicion, LARGO_CAMPO_POSICION + 1, "Posición");
    establecerLargoCampoCentrado(headers.numero, LARGO_CAMPO_ID + 1, "N°"); // "°" ocupa 2 bytes
    establecerLargoCampoCentrado(headers.nombreApellido, LARGO_CAMPO_NOMBRE, "Nombre");
    establecerLargoCampoCentrado(headers.genero, LARGO_CAMPO_GENERO + 1, "Género");
    establecerLargoCampoCentrado(headers.localidad, LARGO_CAMPO_LOCALIDAD, "Localidad");
    establecerLargoCampoCentrado(headers.tiempo, LARGO_CAMPO_TIEMPOS, "Tiempo");
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
        strncpy(dest, src, availableSpace);
        dest[availableSpace] = '\0';
        return;
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
        dest[availableSpace] = '\0';
        return;
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

//crea la matriz con todas las categorias diferentes y devuelve el numero de categorias diferentes 
int obtenerCantidadCategorias(RegCorredores corredores[], int cantidadCorredores) {
    int cantidadCategorias = 0;
    char categoriasVistas[CANTIDAD_CORREDORES_TOTAL][LARGO_CAMPO_CATEGORIA];

    for (int i = 0; i < cantidadCorredores; i++) {
        bool categoriaYaExiste = false;
        for (int j = 0; j < cantidadCategorias; j++) {
            if (strcmp(corredores[i].categoria,categoriasVistas[j]) == 0) { categoriaYaExiste = true; break; }
        }
        
        if (categoriaYaExiste == false) {
            strcpy(categoriasVistas[cantidadCategorias],corredores[i].categoria);
            cantidadCategorias++;
        }
    }
    return cantidadCategorias;
}

void obtenerCategorias(RegCorredores corredores[], int cantidadCorredores, char categorias[][LARGO_CAMPO_CATEGORIA],int cantidadCategorias) {
    int cantidadGuardadas = 0;
    for (int i = 0; i < cantidadCorredores && cantidadGuardadas < cantidadCategorias; i++) {
        bool categoriaYaExiste = false;
        for (int j = 0; j < cantidadGuardadas; j++) {
            if (strcmp(corredores[i].categoria, categorias[j]) == 0) { categoriaYaExiste = true; break;}
        }

        if (!categoriaYaExiste && !strstr(corredores[i].categoria, "No Termino")) {
            strcpy(categorias[cantidadGuardadas], corredores[i].categoria);
            cantidadGuardadas++;
        }
    }
}

// Ordena las categorías alfabeticamente con un bubble sort
void ordenarCategorias(char categorias[][LARGO_CAMPO_CATEGORIA], int cantidadCategorias) {
    for (int i = 0; i < cantidadCategorias - 1;i++) {
        for (int j = 0; j < cantidadCategorias - 1 - i; j++) {
            if (strcmp(categorias[j], categorias[j + 1]) > 0) {
                char aux[LARGO_CAMPO_CATEGORIA];
                strcpy(aux,categorias[j]);
                strcpy(categorias[j], categorias[j + 1]);
                strcpy(categorias[j + 1],aux);
            }
        }
    }
}

// obtiene los 3 primeros por categoria, ya estando ordenados los corredores por tiempo
void obtenerPodioCategoria(RegCorredores corredores[], int cantidadCorredores, char categoria[], RegCorredores podio[]) {
    int cantidadPodio = 0;
    for (int i = 0; i < 3; i++) {podio[i].numero = -1;} //-1 si no se llenó el podio
    for (int i = 0; i < cantidadCorredores && cantidadPodio < 3; i++) {
        if (strcmp(corredores[i].categoria,categoria) == 0) {
            if (tiempoADecimas(corredores[i].llegada) != -1) {
                podio[cantidadPodio] = corredores[i];
                cantidadPodio++;
            }
        }
    }
}

RegPodio generarRegistroPodio(
    RegCorredores corredor, 
    int posicion
) {
    RegPodio reg;

    establecerLargoCampo(reg.categoria, LARGO_CAMPO_CATEGORIA, corredor.categoria);
    establecerLargoCampoCentrado(reg.posicion, LARGO_CAMPO_POSICION, posicion);
    establecerLargoCampo(reg.numero, LARGO_CAMPO_ID, corredor.numero);
    establecerLargoCampo(reg.nombreApellido, LARGO_CAMPO_NOMBRE, corredor.nombreApellido);
    establecerLargoCampoCentrado(reg.genero, LARGO_CAMPO_GENERO, corredor.genero);
    establecerLargoCampo(reg.localidad, LARGO_CAMPO_LOCALIDAD, corredor.localidad);
    establecerLargoCampoCentrado(reg.tiempo, LARGO_CAMPO_TIEMPOS, corredor.llegada);

    return reg;
}

void generarPodios(FILE* file, RegCorredores corredores[], int cantidadCorredores, char categorias[][LARGO_CAMPO_CATEGORIA], int cantidadCategorias) {
    HeadersPodio headers;
    establecerLargoHeaders(headers);

    cout << headers.categoria
         << headers.posicion
         << headers.numero
         << headers.nombreApellido
         << headers.genero
         << headers.localidad
         << headers.tiempo
         << endl;

    fwrite(&headers, sizeof(HeadersPodio), 1, file);
    
    for (int i = 0; i < cantidadCategorias; i++) {
        RegCorredores podio[3];
        obtenerPodioCategoria(corredores, cantidadCorredores, categorias[i], podio);

        for (int j = 0; j < 3; j++) {
            if (podio[j].numero == -1) continue;
            
            RegPodio reg = generarRegistroPodio(
                podio[j],
                j + 1
            );

            cout << reg.categoria
                 << reg.posicion
                 << reg.numero
                 << reg.nombreApellido
                 << reg.genero
                 << reg.localidad
                 << reg.tiempo
                 << endl;

            fwrite(&reg, sizeof(RegPodio), 1, file);
        }

        cout << "\n\n";
    }
}