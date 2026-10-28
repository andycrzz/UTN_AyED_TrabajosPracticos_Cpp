/*
Funciones próximas a implementar:
2. Diseño y desarrollo de una consola interactiva para poder obtener información sobre una materia específica
*/

#include <iostream>
#include <cstring>

using namespace std;

struct RegistroArchivo{
    int codigo;
    char nombre[50];
    char correlatividad[50];
    char cursadaRequerida[3];
    char finalRequerido[3];
};

struct NodoCorrelativa {
    int codMateria;
    bool cursadaAprobada;
    bool finalAprobado;
    NodoCorrelativa* sig;
};

struct Materia {
    int codigo;
    char nombre[50];
    NodoCorrelativa* correlativas;
    Materia* sig;
};

// --- FUNCIONES AUXILIARES DE LISTAS ---

// Busca una materia en la lista principal. Si no existe, la crea y la inserta al final.
Materia* buscarOInsertarMateria(Materia* &lista, int codigo, const char* nombre = "") {
    Materia* aux = lista;
    Materia* ant = NULL;
    while (aux != NULL && aux->codigo != codigo) {
        ant = aux;
        aux = aux->sig;
    }
    if (aux != NULL) {
        return aux; 
    }
    
    Materia* nuevo = new Materia();
    nuevo->codigo = codigo;
    strcpy(nuevo->nombre, nombre);
    nuevo->correlativas = NULL;
    nuevo->sig = NULL;
    
    if (ant == NULL) {
        lista = nuevo;
    } else {
        ant->sig = nuevo;
    }
    return nuevo;
}

// Busca el código de una materia dado su nombre (necesario porque el archivo guarda el nombre de la correlativa)
int buscarCodigoPorNombre(Materia* lista, const char* nombre) {
    Materia* aux = lista;
    while (aux != NULL) {
        if (strcmp(aux->nombre, nombre) == 0) {
            return aux->codigo;
        }
        aux = aux->sig;
    }
    return -1;
}

// Inserta un nuevo nodo en la sublista de correlativas
void insertarCorrelativa(NodoCorrelativa* &listaCorrelativas, int codCorrelativa, bool reqCursada, bool reqFinal) {
    NodoCorrelativa* nuevo = new NodoCorrelativa();
    nuevo->codMateria = codCorrelativa;
    nuevo->cursadaAprobada = reqCursada;
    nuevo->finalAprobado = reqFinal;
    nuevo->sig = NULL;
    
    // Inserción al final de la sublista
    if (listaCorrelativas == NULL) {
        listaCorrelativas = nuevo;
    } else {
        NodoCorrelativa* aux = listaCorrelativas;
        while (aux->sig != NULL) {
            aux = aux->sig;
        }
        aux->sig = nuevo;
    }
}

Materia* importarMaterias(const char* nombreArchivo){
    Materia* listaMaterias = NULL;
    FILE* f = fopen(nombreArchivo, "rb");
    if (!f){
        cout << "Error al abrir el archivo." << nombreArchivo << endl;
        return NULL;
    }

    RegistroArchivo reg;

    fread(&reg, sizeof(RegistroArchivo), 1, f);
    while (!feof(f)){
        Materia* matActual = buscarOInsertarMateria(listaMaterias, reg.codigo, reg.nombre);
        if (strcmp(reg.correlatividad, "Ninguna") != 0){
            int codCorrelativa = buscarCodigoPorNombre(listaMaterias, reg.correlatividad);
            bool reqCursada = (strcmp(reg.cursadaRequerida, "Si") == 0);
            bool reqFinal = (strcmp(reg.finalRequerido, "Si") == 0);
            insertarCorrelativa(matActual->correlativas, codCorrelativa, reqCursada, reqFinal);
        }
        fread(&reg, sizeof(RegistroArchivo), 1, f);
    }
    fclose(f);
    return listaMaterias;
}

// Función auxiliar para verificar visualmente que las listas se enlazaron bien
void mostrarMaterias(Materia* lista) {
    Materia* aux = lista;

    while (aux != NULL) {
        cout << "[" << aux->codigo << "] " << aux->nombre << endl;
        
        NodoCorrelativa* corr = aux->correlativas;
        if (corr == NULL) {
            cout << "  -> Sin correlativas requeridas." << endl;
        } else {
            // Recorremos la sublista (Correlativas de esta materia)
            while (corr != NULL) {
                cout << "  -> Correlativa requerida (Codigo " << corr->codMateria << "): "
                     << "Cursada=" << (corr->cursadaAprobada ? "SI" : "NO") 
                     << " | Final=" << (corr->finalAprobado ? "SI" : "NO") << endl;
                corr = corr->sig;
            }
        }
        cout << "------------------------------------------------" << endl;
        aux = aux->sig;
    }
}

Materia* mostrarMenu(Materia* lista) {
    int id = 1;
    int opt = 0;
    
    Materia* nodoActual = lista;
    cout << "\n== Menú ==\n";
    while (nodoActual) {
        cout << "[" << nodoActual->codigo << "] " << nodoActual->nombre << " - Código " << nodoActual->codigo << endl;
        nodoActual = nodoActual->sig;

        id++;
    }

    cout << "Seleccione una materia [1-" << id - 1 << "]: ";
    cin >> opt;
    
    while (opt <= 0 || opt > id) {
        cout << "Opción inválida." << endl;
        cout << endl << "Seleccione una materia [1-" << id - 1 << "]: ";
        cin >> opt;
    }

    return buscarOInsertarMateria(lista, opt);
}

void mostrarCorrelativasPorMateria(Materia* materia, Materia* listaMaterias) {
    NodoCorrelativa* correlativas = materia->correlativas;
    
    
    NodoCorrelativa* nodoActual = correlativas;
    cout << "\n\n";
    if (!nodoActual) {
        cout << "La materia " << materia->nombre << " no tiene correlativas." << endl;
        return;
    }
    
    cout << "Correlativas para la materia " << materia->nombre << ":\n";

    while (nodoActual) {
        Materia* materiaRelacionada = buscarOInsertarMateria(listaMaterias, nodoActual->codMateria);
        
        cout << "- " << materiaRelacionada->nombre << " (Código " << materiaRelacionada->codigo << "): ";
        cout << "Cursada requerida: " << (nodoActual->cursadaAprobada ? "Si" : "No");
        cout << " | Final requerido: " << (nodoActual->finalAprobado ? "Si" : "No") << endl;
        nodoActual = nodoActual->sig;
    }
}

int main() {
    char exit[5] = "";

    Materia* planDeEstudios = importarMaterias("materia_correlativas.bin");
    if (planDeEstudios == NULL) {
        cout << "Error: No se pudo cargar el plan de estudios. Revisa el archivo .bin" << endl;
        return 1; 
    }

    cout << "=== PLAN DE ESTUDIOS CARGADO EXITOSAMENTE ==="<< endl;
    mostrarMaterias(planDeEstudios);
    do {
        Materia* materiaSeleccionada = mostrarMenu(planDeEstudios);
        mostrarCorrelativasPorMateria(materiaSeleccionada, planDeEstudios);
        
        cout << "\n¿Quiere revisar otra materia (S/n)? ";
        cin >> exit;
    } while (exit[0] == 's' || exit[0] == 'S');

    // NOTA: Aca hay que llamar a la función de limpieza de memoria (los delete) antes de cerrar el programa.
    
    cout << "\nPresione cualquier tecla para salir...";
    cin.ignore(); 
    cin.get();    
    return 0;
}
