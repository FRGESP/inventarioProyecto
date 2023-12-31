// inventario.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "iostream"
#include "string"
#include "vector"
#include <chrono>
#include <ctime>
using namespace std;
#define MAX 50

int cantidadObjetos = 5;
int cantidadCategorias = 3;
int cantidadPedidos = 0;
int ayudaGlobal;
string stringGlobal;
vector<string> categorias;

int visitado[11], camino[20], df, cont = 0, sal = 0, fst = 0,enc=0;
const char* ciudades[] = { "Moroleon", "Yuriria", "Valle", "Tarimbaro", "Salvatierra", "Salamanca", "Cortazar", "Morelia", "Acambaro", "Celaya", "Irapuato" };
int vecinas[11][11] = {
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0} };



//Listas simples
struct provedores
{
    string nombreCliente;
    string direccion;
    provedores* siguiente;
    provedores* atras;
}*primero, *ultimo;

//Listas doblemente enlazadas
struct historialMovimientos
{
    string fechaMov;
    string producto;
    string antiguo;
    string nuevo;
    string tipo;
    historialMovimientos* siguiente;
    historialMovimientos* atras;
}*primeroh, *ultimoh;

//Hashing
struct productosPorCategorias
{
    string ID;
    string categoria;
    struct productosPorCategorias* next;
};

typedef productosPorCategorias* hashnode;


//Necesarias para la cola de prioridad
struct nodo {
    string id;
    int caducidad;
    struct nodo* sgte;
};

//Estructura de la cola
struct caducidad {
    nodo* delante;
    nodo* atras;
};

//crear un nuevo nodo
struct nodo* crearNodo(string x, int pr) {
    struct nodo* nuevoNodo = new(struct nodo);
    nuevoNodo->id = x;
    nuevoNodo->caducidad = pr;
    return nuevoNodo;
};

//Pilas
struct necesitanRestock
{
    string ID;
    necesitanRestock* a;

}*cima, * adelante;

//Para busqueda binaria y ordenamiento por quickshort
union IDints {
    int idEntero;
};

union IDints codigosToInts[MAX];
int indiceUnion = 0;

//Para ordenamiento por insercion y seleccion
struct IdAndInt
{
    int entero;
    string ID;
};

struct IdAndInt IDyEntero[MAX];

//Estructura
struct historialPedidos
{
    string nombreProvedor;
    int folio;
    int monto;
    string fecha;
};

//Arboles BInarios de Busqueda
struct nodoColaSimple {
    int dato;
    nodoColaSimple* der;
    nodoColaSimple* izq;
    nodoColaSimple* padre;
};
nodoColaSimple* arbol = NULL;


struct historialPedidos pedidos[MAX];

//Cola simple
struct pedidosPendientes
{
    string nombreProvedor;
    int folio;
    int monto;
    string fecha;
    struct pedidosPendientes* sgte;
};

typedef struct pedidosPendientes* punteroColaSimple;

struct Cola
{
    punteroColaSimple post; // punteroColaSimple del primer elemento
    punteroColaSimple pre;   // punteroColaSimple ultimo elemento
};

//Datos de los productos
struct productos
{
    string nombre;
    string categoria;
    int cantidad;
    int precio;
    string id;
}typedef node;

struct productos stock[MAX];


void cargaDatos(struct productos[],struct caducidad &caduc);
void agregarProductos(struct productos[], struct caducidad& caduc);
void encolar(struct caducidad& q, string valor, int prioridad); 
void ordenarPrioridad(struct caducidad& q);
void ordenarPrioridad(struct caducidad& q);
void agregarCaducidad(struct caducidad& q, string c, int pr);
void mostrarCaducidad(struct caducidad q, int cantidad, struct productos p[]);
string convertirFecha(int fecha);
void idTranslate(string id, int cantidad, struct productos producto[]);
bool buscarCodigo(string ident, int cantidad, struct productos producto[]);
void mostrarCategorias();
string ingresarFecha();
void agregarProvedor();
void mostrarProvedores();
void eliminarProvedor();
void eliminarProducto(string idEliminar, struct productos a[], int cantidad);
void mostrarProductos(struct productos a[], int cantidad);
void agregarMoviemiento(struct productos producto[], struct caducidad& caduc);
void mostrarListaPU();
void mostrarListaUP();
void mostrarProvedores();
string obtenerHora();
int stringtokey(string name);
void agregarCategoriaHASH(string nombre, string categoria);
int buscarnodo(string nName);
void ImprimirLista(hashnode n, struct productos producto[]);
void IniciarTabla();
void ingresarProductoRestock(string ID);
void sacarProductosRestock();
void imprimirProductosRestock();
int restock(struct productos producto[]);
int IDtoIndex(string ident, int cantidad, struct productos producto[]);
void administracionProvedores();
int buscarIndexPorID(int cantidad, union IDints a[], string ID);
void codigosAenteros(union IDints arreglo[], struct productos producto[]);
void quicksort(union IDints arreglo[], int primero, int ultimo);
int consultaIdExiste(int cantidad, union IDints a[], string ID);
void agregarPreciosUnion(struct IdAndInt numeros[]);
void mostrarMenorAMayor(struct IdAndInt numeros[], string orden);
void seleccion(struct IdAndInt numeros[]);
void insercion(struct IdAndInt numeros[]);
nodoColaSimple* crearNodo(int n, nodoColaSimple* padre);
void insertarNodo(nodoColaSimple*& arbol, int n, nodoColaSimple* padre);
bool busquedaArbol(nodoColaSimple* arbol, int n);
void inicializarCola(Cola& cola);
bool colaVacia(const Cola& cola);
void encolar(Cola& cola);
int desencolar(Cola& cola);
void mostrarCola(const Cola& cola);
void vaciarCola(Cola& cola);
void mostrarpedido(struct historialPedidos pedido[], int cantidad, int folio);
void preOrden(nodoColaSimple* arbol);
void menucola();
void buscarProvedor(string provedorBuscar);
int verificarExistenciaProvedor(string provedorBuscar);
void administracionCreditos(Cola& cola);
void adminitrarMovimientos(struct caducidad caduc);
void administracionVisualuzacionDatos(struct caducidad caduc);
void dfs(int of);
int busqueda(const char en[15]);
void corregidor(char* ent);
void grafos();

hashnode HashTable[1000];
int tablesize = 20;


void menu()
{
    cout << endl << "---------------------------------------------------" << endl;
    cout << "               INVENTARIO          " << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " Seleccione una opcion:" << endl;
    cout << " 1. Movimiento" << endl;
    cout << " 2. Visualizacion de productos" << endl;
    cout << " 3. Provedores" << endl;
    cout << " 4. Creditos" << endl;
    cout << " 5. Ruta" << endl;
    cout << " 6. Salir" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Ingrese el numero de la opcion deseada: ";
}

void menuMovimiento()
{
    cout << endl << "---------------------------------------------------" << endl;
    cout << "               Movimiento          " << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " Seleccione una opcion:" << endl;
    cout << " 1. Hacer movimiento en el inventario" << endl;
    cout << " 2. Movimientos del mas antiguo al mas actual" << endl;
    cout << " 3. Movimientos del mas actual al mas antiguo" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Ingrese el numero de la opcion deseada: ";
}

void menuVisualizacionProductos()
{
    cout << endl << "---------------------------------------------------" << endl;
    cout << "               Visualizacion de productos          " << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " Seleccione una opcion:" << endl;
    cout << " 1. Buscar por ID" << endl;
    cout << " 2. Vista general de productos" << endl;
    cout << " 3. Mostrar productos del mas barato al mas caro" << endl;
    cout << " 4. Mostrar productos del mas caro al mas barato" << endl;
    cout << " 5. Mostrar productos por fecha de caducidad" << endl;
    cout << " 6. Mostrar productos por categoria" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Ingrese el numero de la opcion deseada: ";
}

int main()
{
    struct caducidad caduc;
    caduc.atras = NULL;
    caduc.delante = NULL;
    int op,aux,key;
    string categoria;
    cargaDatos(stock, caduc);
    Cola cola;
    inicializarCola(cola);
    
    do
    {
        menu();
        cin >> op;
        cin.ignore();
        switch (op)
        {
        case 1:
            adminitrarMovimientos(caduc);
            break;
        case 2:
            administracionVisualuzacionDatos(caduc);
            break;
        case 3:
            system("cls");
            administracionProvedores();
            break;
        case 4:
            system("cls");
            administracionCreditos(cola);
            break;
        case 5:
            system("cls");
            grafos();
            system("pause");
            system("cls");
            break;
        case 6:
            break;
        default:
            cout << "Opcion no valida";
            system("pause");
            system("cls");
            break;
        }
        system("cls");
    } while (op != 6);
    return 0;
}

void adminitrarMovimientos(struct caducidad caduc)
{
    system("cls");
    int op,indice;
    string ID;
    bool hecho = false;
    do
    {
        menuMovimiento();
        cin >> op;
        cin.ignore();
        switch (op)
        {
        case 1:
            agregarMoviemiento(stock, caduc);
            hecho = true;
            break;
        case 2:
            system("cls");
            mostrarListaPU();
            system("pause");
            hecho = true;
            break;
        case 3:
            system("cls");
            mostrarListaUP();
            system("pause");
            hecho = true;
            break;
        default:
            cout << "Opcion no valida"<<endl;
            system("pause");
            system("cls");
            break;
        }
    } while (hecho==false);
    system("cls");
}

void administracionVisualuzacionDatos(struct caducidad caduc)
{
    system("cls");
    int op,aux,key;
    string ID;
    bool hecho = false;
    do
    {
        menuVisualizacionProductos();
        cin >> op;
        cin.ignore();
        switch (op)
        {
        case 1:
            system("cls");
            cout << "Ingrese el ID: ";
            getline(cin, ID);
            cout << endl;
            idTranslate(ID,cantidadObjetos,stock);
            system("pause");
            hecho = true;
            break;
        case 2:
            system("cls");
            mostrarProductos(stock, cantidadObjetos);
            cout << endl;
            system("pause");
            hecho = true;
            break;
        case 3:
            system("cls");
            mostrarMenorAMayor(IDyEntero, "menor");
            system("pause");
            hecho = true;
            break;
        case 4:
            system("cls");
            mostrarMenorAMayor(IDyEntero, "mayor");
            system("pause");
            hecho = true;
            break;
        case 5:
            system("cls");
            mostrarCaducidad(caduc, cantidadObjetos, stock);
            system("pause");
            hecho = true;
            break;
        case 6:
            system("cls");
            mostrarCategorias();
            cout << "\n\nSeleccione la categoria deseada: ";
            cin >> aux;
            key = buscarnodo(categorias[aux - 1]);
            if (key == -1)
            {
                cout << "La categoria no se encuentra" << endl;
            }
            else
            {
                system("cls");
                cout << "Se muestran los productos de la categoria " << categorias[aux - 1] << endl << endl;
                ImprimirLista(HashTable[key], stock);
                cout << endl << endl;
            }
            system("pause");
            system("cls");
            hecho = true;
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            system("cls");
            break;
        }
    } while (hecho == false);
    system("cls");
}

void cargaDatos(struct productos objeto[], struct caducidad& caduc)
{
    string ID, categoria;

    objeto[0].nombre = "Coca cola";
    objeto[0].categoria = "Bebidas";
    objeto[0].cantidad = 150;
    agregarCaducidad(caduc, "001", 20231212);
    objeto[0].id = "001";
    objeto[0].precio = 15;
    ID = objeto[0].id;
    categoria = objeto[0].categoria;
    agregarCategoriaHASH(ID, categoria);

    objeto[1].nombre = "Desodorante";
    objeto[1].categoria = "Cuidado Personal";
    objeto[1].cantidad = 214;
    agregarCaducidad(caduc,"002", 999999999);
    objeto[1].id = "002";
    objeto[1].precio = 22;
    ID = objeto[1].id;
    categoria = objeto[1].categoria;
    agregarCategoriaHASH(ID, categoria);


    objeto[2].nombre = "Shampoo";
    objeto[2].categoria = "Cuidado Personal";
    objeto[2].cantidad = 145;
    agregarCaducidad(caduc, "003", 999999999);
    objeto[2].id = "003";
    objeto[2].precio = 35;
    ID = objeto[2].id;
    categoria = objeto[2].categoria;
    agregarCategoriaHASH(ID, categoria);

    objeto[3].nombre = "Pan de caja";
    objeto[3].categoria = "Alimentos";
    objeto[3].cantidad = 34;
    agregarCaducidad(caduc, "004", 20231412);
    objeto[3].id = "004";
    objeto[3].precio = 24;
    ID = objeto[3].id;
    categoria = objeto[3].categoria;
    agregarCategoriaHASH(ID, categoria);

    objeto[4].nombre = "Galletas";
    objeto[4].categoria = "Alimentos";
    objeto[4].cantidad = 41;
    agregarCaducidad(caduc, "005", 20241001);
    objeto[4].id = "005";
    objeto[4].precio = 11;
    ID = objeto[4].id;
    categoria = objeto[4].categoria;
    agregarCategoriaHASH(ID, categoria);

    categorias.push_back("Bebidas");
    categorias.push_back("Cuidado Personal");
    categorias.push_back("Alimentos");

}

void agregarProductos(struct productos objeto[], struct caducidad &caduc)
{
    int op = 0, aux,indice,clave;
    string cadenaAux;
    string codigo;
    string fecha;
    bool ban=false;
    cout << "Ingrese el nombre del producto: ";
    getline(cin, cadenaAux);
    system("cls");
    objeto[cantidadObjetos].nombre=cadenaAux;
    cout << endl << "Nombre guardado"<<endl << endl;
    do
    {
        cout << "Estas son las categoriass existentes:\n";
        mostrarCategorias();
        cout << endl<<"\nDesea agregar el producto a una categoria existente o crear una nueva categoria?"<<endl<<endl;
        cout << "1. Agregar a una categoria existente" << endl;
        cout << "2. Crear una categoria nueva" << endl;
        cout << "Ingrese la opcion deseada: ";
        cin >> op;
        cin.ignore();
        system("cls");
        if (op == 1)
        {
            mostrarCategorias();
            cout << "\nSeleccione la categoria deseada: ";
            cin >> aux;
            cin.ignore();
            objeto[cantidadObjetos].categoria = categorias[aux - 1];
            break;
        }
        else if (op == 2)
        {
            cout << "\nEscriba el nombre de la nueva categoria: ";
            getline(cin, cadenaAux);
            categorias.push_back(cadenaAux);
            objeto[cantidadObjetos].categoria = cadenaAux;
            cantidadCategorias++;
            break;
        }
        else
        {
            cout << "Opcion no valida, por favor intente de nuevo" << endl;
            system("pause");
            system("cls");
        }  
    } while (op != 2 && op != 1);
    system("cls");
    cout << endl << "Categoria asignada" << endl << endl;
    cout << "Ingrese la cantidad del producto: ";
    cin >> objeto[cantidadObjetos].cantidad;
    system("cls");
    cout << endl << "Cantidad guardada" << endl << endl;
    do
    {
        cout << "Asigne un nuevo ID al producto, tendra que ser uno que no se ha usado: ";
        cin >> codigo;
        cin.ignore();
        indice = buscarIndexPorID(cantidadObjetos, codigosToInts,codigo);
        if (indice == -1)
        {
            
            ban = true;
        }
        else
        {
            cout << "Ese codigo esta ocupado, por favor intente de nuevo" << endl << endl;
        }
    } while (ban==false);
    objeto[cantidadObjetos].id = codigo;
    system("cls");
    cout << endl << "Codigo guardado" << endl << endl;
    cout << endl << "Es momento de ingresar la fecha de caducidad: "<<endl<<endl;
    cout << "1. Producto sin fecha de caducidad" << endl;
    cout << "2. Producto con fecha de caducidad" << endl;
    cout << "Ingrese la opcion deseada: ";
    cin >> op;
    cin.ignore();
    if (op == 1)
        agregarCaducidad(caduc, codigo, 999999999);
    else
    {
       
        fecha = ingresarFecha();
        aux = stoi(fecha);
        agregarCaducidad(caduc, codigo, aux);
    }
    system("cls");
    cout << endl << "Caducidad guardada" << endl << endl;
    cout << "Ingrese el precio del producto: ";
    cin >> objeto[cantidadObjetos].precio;
    system("cls");
    agregarCategoriaHASH(objeto[cantidadObjetos].id, objeto[cantidadObjetos].categoria);
    cantidadObjetos++;
    system("cls");
    cout <<"Producto Guardado con extito"<<endl;
}

//Colas de prioridad para la caducidad

void encolar(struct caducidad& q, string valor, int prioridad) {
    struct nodo* aux = crearNodo(valor, prioridad);
    aux->sgte = NULL;

    if (q.delante == NULL) {
        q.delante = aux;//encolar el primer elemento
    }
    else {
        (q.atras)->sgte = aux;//en caso de que exista un elemento, apunta al �ltimo elemento
    }
    q.atras = aux;
}

//Mostrar Cola

void mostrarCaducidad(struct caducidad q, int cantidad, struct productos p[]) {
    struct nodo* aux;
    aux = q.delante;
    string ap;
    enc = 0;
    cout << "Mostrando productos por caducidad" << endl;
    while (aux != NULL) {

        ap = aux->id;
        idTranslate(ap, cantidad, p);
        if (enc == 0)
        {
            cout << "Caducidad: " << convertirFecha(aux->caducidad) << endl << endl;
        }
        aux = aux->sgte;
        enc = 0;
    }

}

//Ordenar por prioridad (criterio de ordenamiento ->burbuja)

void ordenarPrioridad(struct caducidad& q) {
    struct nodo* aux1, * aux2;
    int p_aux;
    string c_aux;

    aux1 = q.delante;

    while (aux1->sgte != NULL) {
        aux2 = aux1->sgte;
        while (aux2 != NULL) {
            if (aux1->caducidad > aux2->caducidad) {
                p_aux = aux1->caducidad;
                c_aux = aux1->id;

                aux1->caducidad = aux2->caducidad;
                aux1->id = aux2->id;

                aux2->caducidad = p_aux;
                aux2->id = c_aux;
            }
            aux2 = aux2->sgte;
        }
        aux1 = aux1->sgte;
    }
}
//encolar (insertar) un elemento al nodo
void agregarCaducidad(struct caducidad& q, string c, int pr) {
    //encolar caracteres
    encolar(q, c, pr);
    //Ordenar por prioridad
    ordenarPrioridad(q);
}

//LIstas simples 

void agregarProvedor()
{
    provedores* nuevo = new provedores();
    cout << "Inserte el nombre del provedor: ";
    getline(cin, nuevo->nombreCliente);
    cout << "El numero del telefono del provedor: ";
    getline(cin, nuevo->direccion);

    if (primero == NULL)
    {
        primero = nuevo;
        primero->siguiente = NULL;
        ultimo = nuevo;
    }
    else
    {
        ultimo->siguiente = nuevo;
        nuevo->siguiente = NULL;
        ultimo = nuevo;
    }
    cout << "Provedor agregado" << endl;
}

void mostrarProvedores()
{
    provedores* actual = new provedores();
    actual = primero;
    if (primero != NULL)
    {
        while (actual != NULL)
        {
            cout << endl << "Nombre: " << actual->nombreCliente << endl;
            cout << "Telefono: " << actual->direccion << endl ;
            actual = actual->siguiente;
        }
    }
    else
    {
        cout << "No existen provedores registrados aun";
    }
}


void eliminarProvedor()
{
    if (primero != NULL)
    {
        provedores* actual = primero;
        provedores* anterior = NULL;
        string provedorBuscar;

        cout << "Ingrese el nombre del provedor a eliminar: ";
        getline(cin, provedorBuscar);

        bool encontrado = false;

        while (actual != NULL && !encontrado)
        {
            if (actual->nombreCliente == provedorBuscar)
            {
                if (actual == primero) // Eliminar el primer nodo
                {
                    primero = actual->siguiente;
                }
                else if (actual == ultimo) // Eliminar el �ltimo nodo
                {
                    anterior->siguiente = NULL;
                    ultimo = anterior;
                }
                else // Eliminar cualquier otro nodo
                {
                    anterior->siguiente = actual->siguiente;
                }

                delete actual;
                cout << "Provedor " << provedorBuscar << " eliminado" << endl;
                encontrado = true;
            }
            else
            {
                anterior = actual;
                actual = actual->siguiente;
            }
        }

        if (!encontrado)
            cout << endl << "Provedor no encontrado";
    }
    else
    {
        cout << "No hay provedores registrados aun" << endl;
    }
}

int verificarExistenciaProvedor(string provedorBuscar)
{
    if (primero != NULL)
    {
        provedores* actual = primero;
        provedores* anterior = NULL;

        bool encontrado = false;

        while (actual != NULL && !encontrado)
        {
            if (actual->nombreCliente == provedorBuscar)
            {
                return 1;
                encontrado = true;
            }
            else
            {
                anterior = actual;
                actual = actual->siguiente;
            }
        }

        if (!encontrado)
        {
            cout << endl << "Provedor no encontrado";
            return -1;
        }
            
    }
    else
    {
        cout << "No hay provedores registrados aun" << endl;
        return 0;
    }
}

void buscarProvedor(string provedorBuscar)
{
    if (primero != NULL)
    {
        provedores* actual = primero;
        provedores* anterior = NULL;

        bool encontrado = false;

        while (actual != NULL && !encontrado)
        {
            if (actual->nombreCliente == provedorBuscar)
            {
                cout << "Nombre: " << actual->nombreCliente << endl;
                cout << "Telefono: " << actual->direccion << endl;
                actual = actual->siguiente;

                encontrado = true;
            }
            else
            {
                anterior = actual;
                actual = actual->siguiente;
            }
        }

        if (!encontrado)
        {
            cout << endl << "Provedor no encontrado";
        }

    }
    else
    {
        cout << "No hay provedores registrados aun" << endl;
    }
}

void administracionProvedores()
{
    bool hecho = false;
    int op;
    do
    {
        cout << "Ingrese el tipo de movimiento que desea realizar: ";
        cout << endl << "---------------------------------------------------" << endl;
        cout << "               Administracion provedores           " << endl;
        cout << "---------------------------------------------------" << endl;
        cout << " Seleccione una opci�n:" << endl;
        cout << " 1. Agregar provedor" << endl;
        cout << " 2. Mostrar provedores" << endl;
        cout << " 3. Eliminar provedor" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "Ingrese el numero de la opcion deseada: ";
        cin >> op;
        cin.ignore();
        switch (op)
        {
        case 1:
            system("cls");
            agregarProvedor();
            system("pause");
            system("cls");
            hecho = true;
            break;
        case 2:
            system("cls");
            mostrarProvedores();
            system("pause");
            system("cls");
            hecho = true;
            break;
        case 3:
            system("cls");
            eliminarProvedor();
            system("pause");
            system("cls");
            hecho = true;
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
        }
    } while (hecho == false);
} 
    

//Listas dobles
void agregarMoviemiento(struct productos producto[], struct caducidad& caduc)
{
    historialMovimientos* nuevo = new historialMovimientos();
    bool ban = false, hecho = false;
    string cadena,auxiliar,strAntiguo,claveID;
    string tiempo;
    char time[25];
    int fecha,op,aux,aux2,numAnterior,operacion,indice;
    tiempo = obtenerHora();
    nuevo->fechaMov=tiempo;
    do {
        system("cls");
        
        cout << "Ingrese el tipo de movimiento que desea realizar: ";
        cout << endl << "---------------------------------------------------" << endl;
        cout << "               Realizar Movimiento           " << endl;
        cout << "---------------------------------------------------" << endl;
        cout << " Seleccione una opcion:" << endl;
        cout << " 1. Agregar producto" << endl;
        cout << " 2. Cambiar Nombre a producto existente" << endl;
        cout << " 3. Cambiar Precio a producto existente" << endl;
        cout << " 4. Dar Entrada" << endl;
        cout << " 5. Dar salida" << endl;
        cout << " 6. Eliminar Producto" << endl;
        cout << " 7. Reabastecimiento" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "Ingrese el numero de la opcion deseada: ";
        cin >> op;
        cin.ignore();

        switch (op)
        {
        case 1:
            system("cls");
            agregarProductos(producto, caduc);
            nuevo->antiguo = "Producto sin existencia pasada";
            nuevo->tipo = "Agregar producto";
            nuevo->producto = producto[cantidadObjetos - 1].nombre;
            nuevo->nuevo = producto[cantidadObjetos - 1].nombre;
            hecho = true;
            break;
        case 2:
            system("cls");
            indice= consultaIdExiste(cantidadObjetos, codigosToInts, claveID);
            cout << endl << "Ingrese el nuevo nombre: ";
            getline(cin, auxiliar);
            nuevo->antiguo = producto[indice].nombre;
            nuevo->nuevo = auxiliar;
            producto[indice].nombre = auxiliar;
            nuevo->tipo = "Cambio de Nombre";
            nuevo->producto = producto[indice].nombre;
            hecho = true;
            break;

        case 3:
            system("cls");
            indice = consultaIdExiste(cantidadObjetos, codigosToInts, claveID);
            cout << endl << "Ingrese el nuevo precio: ";
            cin >> aux;
            nuevo->antiguo = to_string(producto[indice].precio);  
            producto[indice].precio = aux;
            nuevo->nuevo = to_string(aux);
            nuevo->tipo = "Cambio de Precio";
            nuevo->producto = producto[indice].nombre;
            hecho = true;
            break;
        case 4:
            system("cls");
            indice = consultaIdExiste(cantidadObjetos, codigosToInts, claveID);
            cout << endl << "La cantidad a agregar: ";
            cin >> aux;
            operacion = producto[indice].cantidad + aux;
            nuevo->antiguo = to_string(producto[indice].cantidad);
            producto[indice].cantidad = operacion;
            nuevo->nuevo = to_string(operacion);
            nuevo->tipo = "Entrada";
            nuevo->producto = producto[indice].nombre;
            hecho = true;
            break;
        case 5:
            system("cls");
            do
            {
                indice = consultaIdExiste(cantidadObjetos, codigosToInts, claveID);
                cout << endl << "La cantidad a quitar: ";
                cin >> aux2;
                operacion = producto[indice].cantidad - aux2;
                if (operacion >= 0)
                {
                    nuevo->antiguo = to_string(producto[indice].cantidad);
                    producto[indice].cantidad = operacion;
                    nuevo->nuevo = to_string(operacion);
                    nuevo->tipo = "Salida";
                    nuevo->producto = producto[indice].nombre;
                    ban = true;
                }
                else
                {
                    cout << "La cantidad que desea quitar no es valida, existen menos productos que la cantidad indicada, intente de nuevo" << endl;
                    system("pause");
                    system("cls");
                }
            } while (ban == false);
            nuevo->tipo = "Salida";
            hecho = true;
            break;
        case 6:
            system("cls");
            indice = consultaIdExiste(cantidadObjetos, codigosToInts, claveID);
            nuevo->antiguo = producto[indice].nombre;
            nuevo->nuevo = "Eliminado";
            nuevo->producto = producto[indice].nombre;
            nuevo->tipo = "Eliminar Objeto";
            eliminarProducto(producto[indice].id, producto, cantidadObjetos);
            hecho = true;
            system("pause");
            break;
        case 7:
            system("cls");
            aux=restock(producto);
            if (aux == -1)
            {
                auxiliar = "Se hizo consulta por productos con menos de ";
                auxiliar.append(to_string(ayudaGlobal));
                auxiliar.append(" unidades");
                nuevo->antiguo = "Sin elementos por reabastecer";
                nuevo->nuevo = "Sin elementos por reabastecer";
                nuevo->producto = auxiliar;
                nuevo->tipo = "Reabastecimiento";
                hecho = true;
            }
            else if (aux == -2)
            {
                auxiliar = "Se hizo consulta por productos con menos de ";
                auxiliar.append(to_string(ayudaGlobal));
                auxiliar.append(" unidades");
                nuevo->antiguo = "Solo consulta";
                nuevo->nuevo = "Solo consulta";
                nuevo->producto = auxiliar;
                nuevo->tipo = "Reabastecimiento";
                hecho = true;
            }
            else
            {
                auxiliar="Se reabastecio un total de ";
                auxiliar.append(to_string(aux));
                auxiliar.append(" unidades");
                nuevo->antiguo = stringGlobal;
                nuevo->nuevo = auxiliar;
                nuevo->producto = "Varios";
                nuevo->tipo = "Reabastecimiento";
                hecho = true;
            }
            system("pause");
            system("cls");
        default:
            cout << "Opcion no valida" << endl;
            break;
        }
    } while (hecho==false);
    
    if (primeroh == NULL)
    {
        primeroh = nuevo;
        primeroh->siguiente = NULL;
        primeroh->atras = NULL;
        ultimoh = nuevo;
    }
    else
    {
        ultimoh->siguiente = nuevo;
        nuevo->siguiente = NULL;
        nuevo->atras = ultimoh;
        ultimoh = nuevo;
    }
}

void mostrarListaPU()
{
    historialMovimientos* actual = new historialMovimientos();
    actual = primeroh;
    cout << endl << "Mostrando movimientos del mas antiguo al mas actual" << endl;
    if (primeroh != NULL)
    {
        while (actual != NULL)
        {
            cout << endl << "Fecha: " << actual->fechaMov << endl;
            cout << "Producto: " << actual->producto << endl;
            cout << "Tipo: " << actual->tipo << endl;
            cout << "Antiguo: " << actual->antiguo << endl;
            cout << "Nuevo: " << actual->nuevo << endl;
            actual = actual->siguiente;
        }
    }
    else
    {
        cout << "No se han hecho movimientos aun" << endl;
    }
}

void mostrarListaUP()
{
    historialMovimientos* actual = ultimoh; 
    cout << endl << "Mostrando movimientos del mas actual al mas antiguo" << endl;
   
    if (primeroh != NULL)
    {
        while (actual != NULL)
        {
            cout << endl << "Fecha: " << actual->fechaMov << endl;
            cout << "Producto: " << actual->producto << endl;
            cout << "Tipo: " << actual->tipo << endl;
            cout << "Antiguo: " << actual->antiguo << endl;
            cout << "Nuevo: " << actual->nuevo << endl;
            actual = actual->atras;
        }
    }
    else
    {
        cout << "No se han hecho movimientos aun" << endl;
    }
}

string convertirFecha(int fecha)
{
    string s = to_string(fecha);
    string aux;
    int cont = 0;
    if (s.length() > 8)
        return "No aplica";
    else
    {
        int i = s.length() - 4;

        for (i; i <= s.length(); i++)
        {
            if (cont == 2 || cont == 4)
                aux.append("/");
            aux += s[i];
            cont++;
        }

        for (int i = 0; i < s.length() - 4; i++)
        {
            aux += s[i];
        }
        return aux;
    }
}


//Aplicando Busqueda Secuencial
void idTranslate(string ident,int cantidad, struct productos producto[])
{
    int i;
    bool band = false;
    for (i = 0; i < cantidad; i++)
    {
        if (producto[i].id == ident)
        {
            band = true;
            break;
        }
    }
    if (band == false)
    {
        enc = -1;
    }
    else if (band == true)
    {
        cout << "Nombre: " << producto[i].nombre << endl;
        cout << "Categoria: " << producto[i].categoria << endl;
        cout << "Cantidad: " << producto[i].cantidad << endl;
        cout << "Precio: " << producto[i].precio << endl;
    }
}


int IDtoIndex(string ident, int cantidad, struct productos producto[])
{
    int i,cont;
    for (i = 0; i < cantidad; i++)
    {
        if (producto[i].id == ident)
        {
            return i;
        }
    }
    return -1;
}
bool buscarCodigo(string ident, int cantidad, struct productos producto[])
{
    int i;
    bool band = false;
    for (i = 0; i < cantidad; i++)
    {
        if (producto[i].id == ident)
        {
            band = true;
            break;
        }
    }
    if (band == false)
    {
        cout << "Codigo disponible, se ha asignado al producto" << endl;
        return false;

    }
    else if (band == true)
    {
        cout << "El codigo ya esta ocupado, ingrese otro: " << endl;
        return true;
    }
}

void mostrarCategorias()
{
    int i = 0;
    for (const string& cadena : categorias) {
        cout << i + 1 << "- " << cadena << "|| ";
        i++;
    }
}

string ingresarFecha()
{
    int year, mes, dia;

    cout << "Ingrese el anio (YYYY): ";
    cin >> year;
    cin.ignore();


    while (year < 1000 || year > 9999) {
        cout << "A�o no valido. Ingrese un anio valido (YYYY): ";
        cin >> year;
        cin.ignore();

    }

    cout << "Ingrese el mes (1-12): ";
    cin >> mes;
    cin.ignore();


    while (mes < 1 || mes > 12) {
        cout << "Mes no valido. Ingrese un mes valido (1-12): ";
        cin >> mes;
        cin.ignore();

    }

    cout << "Ingrese el dia (1-31): ";
    cin >> dia;
    cin.ignore();


    while ((mes == 2 && (dia < 1 || dia > 28)) ||
        ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && (dia < 1 || dia > 30)) ||
        ((mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) && (dia < 1 || dia > 31))) {
        cout << "Dia no valido. Ingrese un dia valido segun el mes: ";
        cin >> dia;
        cin.ignore();

    }

    string fecha = to_string(year);

    

    if (dia < 10) {
        fecha += "0" + to_string(dia);
    }
    else {
        fecha += to_string(dia);
    }

    if (mes < 10) {
        fecha += "0" + to_string(mes);
    }
    else {
        fecha += to_string(mes);
    }

    return fecha;
}

void eliminarProducto(string idEliminar, struct productos a[], int cantidad) 
{
    int posEliminar = -1;
    for (int i = 0; i < cantidad; i++) {
        if (a[i].id == idEliminar) {
            posEliminar = i;
            break;
        }
    }

    if (posEliminar != -1) {
        for (int i = posEliminar; i < cantidad - 1; i++) {
            a[i] = a[i + 1];
        }

        cantidadObjetos--;
        cout << "Producto con ID " << idEliminar << " eliminado exitosamente." << endl;
    }
    else {
        cout << "No se encontr� un producto con el ID " << idEliminar << "." << endl;
    }
}
void mostrarProductos(struct productos a[], int cantidad) 
{

    cout << endl<< "---------------------------------------------------------------------------------------------";
    for (int i = 0; i < cantidad; i++)
    {
        cout << endl << i+1<<"- | ID: " << a[i].id << "| Nombre: " << a[i].nombre << " | Precio: " << a[i].precio << " | Categoria: " << a[i].categoria << " | Cantidad: " << a[i].cantidad << endl;
        cout << "---------------------------------------------------------------------------------------------";
    }
}



string obtenerHora() 
{
    auto currentTime = chrono::system_clock::now();
    auto currentTimeInSeconds = chrono::system_clock::to_time_t(currentTime);
    tm localTime;
    localtime_s(&localTime, &currentTimeInSeconds);
    char timeString[100];
    strftime(timeString, sizeof(timeString), "%d/%m/%Y %H:%M:%S", &localTime);
    string formattedTime(timeString);
    return formattedTime;
}

//Hashing
int stringtokey(string name)
{
    int s = 0;
    int j = 1;

    for (char c : name)
    {
        s = s + (c * j);
        j++;
    }
    return s;
}

void agregarCategoriaHASH(string nombre, string categoria)
{
    int ascii, key;
    hashnode n1 = new productosPorCategorias;
    hashnode n2;

    n1->ID = nombre;
    n1->categoria = categoria;
    n1->next = NULL;
    ascii = stringtokey(categoria);
    key = ascii % tablesize;

    if (HashTable[key] == NULL)
    {
        HashTable[key] = n1;
    }
    else
    {
        n2 = HashTable[key];
        while (n2->next != nullptr)
        {
            n2 = n2->next;
        }
        n2->next = n1;
    }
}

int buscarnodo(string nName)
{
    int ascii = stringtokey(nName);
    int key = ascii % tablesize;
    hashnode n = HashTable[key];

    while (n != NULL)
    {
        if (n->categoria == nName)
        {
            return key;
        }
        n = n->next;
    }

    return -1;
}

void ImprimirLista(hashnode n, struct productos producto[])
{
    for (hashnode n1 = n; n1 != NULL; n1 = n1->next)
    {
        idTranslate(n1->ID,cantidadObjetos,producto);
        cout << endl;
    }
}

void IniciarTabla()
{
    for (int i = 0; i < tablesize; i++)
    {
        HashTable[i] = NULL;
    }
}

//Pilas

void ingresarProductoRestock(string ID)
{
    if (!cima)
    {
        cima = new necesitanRestock;
        cima->ID = ID;
        cima->a = NULL;
        return;
    }
    adelante = new necesitanRestock;
    adelante->ID = ID;
    adelante->a = cima;
    cima = adelante;
}

void sacarProductosRestock()
{
    if (!cima)
    {
        cout << "\n\nSin objetos por reabastecer";
        return;
    }
    adelante = new(necesitanRestock);
    adelante = cima;
    cima = adelante->a;
    delete(adelante);
}
void imprimirProductosRestock()
{
    int i, ca = 0;
    adelante = cima;
    while (adelante)
    {
        cout << endl;
        idTranslate(adelante->ID,cantidadObjetos,stock);
        adelante = adelante->a;
    }
}
int restock(struct productos producto[])
{
    int valor, i,suma=0,index,aux,cont=0,op;
    string cadena;


    cout << "Ingrese la cantidad de valor minimo de unidades: ";
    cin >> valor;
    stringGlobal = "Se hizo reabastecimiento a productos con menos de ";
    stringGlobal.append(to_string(valor));
    stringGlobal.append(" unidades");
    for (i = 0; i < cantidadObjetos; i++)
    {
        if (producto[i].cantidad <= valor)
        {
            ingresarProductoRestock(producto[i].id);
            cont++;
        }
    }
    if (cont == 0)
    {
        cout << "No existen productos que se necesiten reabastecer" << endl;
        ayudaGlobal = valor;
        return -1;
    }
    else
    {
        system("cls");
        cout << "Estos son los productos con menos de " << valor << " unidades" << endl << endl;
        imprimirProductosRestock();

        cout << endl<<"Desea reabastecer los productos?" << endl;
        cout << "1. Si" << endl;
        cout << "2. En otro momento" << endl;
        cout << "Elija la opcion deseada: ";
        cin >> op;
        if (op == 1)
        {
            system("cls");
            while (cont > 0)
            {
                index = IDtoIndex(cima->ID, cantidadObjetos, producto);
                ayudaGlobal = index;
                cout << endl << endl << "Se hara reabastecimiento del producto " << producto[index].nombre << " que tiene solo " << producto[index].cantidad << " unidades" << endl << endl;
                cout << "Ingrese la cantidad a agregar del producto: ";
                cin >> aux;
                cin.ignore();
                producto[index].cantidad += aux;
                cout << endl << "Ahora el producto cuenta con: " << producto[index].cantidad << " unidades " << endl;
                sacarProductosRestock();
                cont--;
                suma += aux;
            }
            return suma;
        }
        else
        {
            cout << endl<<"Ok..."<<endl;
            ayudaGlobal = valor;
            return -2;
        }
    }
    
}
void codigosAenteros(union IDints arreglo[], struct productos producto[])
{
    int i, valor;
    for (i = 0; i < cantidadObjetos; i++)
    {
        valor = stringtokey(producto[i].id);
        arreglo[i].idEntero = valor;
    }

}

//Quickshort
void quicksort(union IDints arreglo[], int primero, int ultimo)
{
    int central, i, j, pivote, temporal;
    //Posici�n del elemento central
    central = (primero + ultimo) / 2;
    //Obetener el valor del elemento central;
    pivote = arreglo[central].idEntero;
    //Separar los dos segmentos
    i = primero;
    j = ultimo;
    do {
        //Elementos menor al pivote
        while (arreglo[i].idEntero < pivote)i++;
        //Elementos mayor al pivote
        while (arreglo[j].idEntero > pivote)j--;
        if (i <= j)
        {
            //Intercambio de valores
            temporal = arreglo[i].idEntero;
            arreglo[i].idEntero = arreglo[j].idEntero;
            arreglo[j].idEntero = temporal;
            i++;
            j--;
        }

    } while (i <= j);
    if (primero < j)
    {
        quicksort(arreglo, primero, j);
    }
    if (i < ultimo)
    {
        quicksort(arreglo, i, ultimo);
    }
}
//Busqueda binaria
int buscarIndexPorID(int cantidad, union IDints a[],string ID)
{
    int numeroBuscar = stringtokey(ID);
    codigosAenteros(codigosToInts, stock);
    quicksort(codigosToInts, 0, cantidadObjetos - 1);
    int mitad, izq, der;
    izq = 0;
    der = cantidad - 1;

    while (izq <= der)
    {
        mitad = (izq + der) / 2;
        if (numeroBuscar > a[mitad].idEntero)
        {
            izq = mitad + 1;
        }
        else if (numeroBuscar < a[mitad].idEntero)
        {
            der = mitad - 1;
        }
        else
        {
            return mitad;
        }
    }
    return -1;
}

int consultaIdExiste(int cantidad, union IDints a[], string ID)
{
    int indice;
    bool ban=false;
    string codigo;
    do
    {
        cout << "Ingrese el ID del codigo producto deseado: ";
        cin >> codigo;
        cin.ignore();
        indice = buscarIndexPorID(cantidadObjetos, codigosToInts, codigo);
        if (indice == -1)
        {
            cout << "Este codigo no existe en el registro, porfavor intente de nuevo" << endl << endl;
        }
        else
        {
            ban = true;
        }
    } while (ban == false);
    return indice;
}


void agregarPreciosUnion(struct IdAndInt numeros[])
{
    for (int i = 0; i < cantidadObjetos; i++)
    {
        numeros[i].entero = stock[i].precio;
        numeros[i].ID = stock[i].id;
    }
}

void mostrarMenorAMayor(struct IdAndInt numeros[], string orden)
{
    agregarPreciosUnion(numeros);

   if (orden == "mayor")
    {
       cout << "Mostrar elementos del mas caro al mas barato:" << endl << endl;
        seleccion(numeros);
   }
   else if (orden == "menor")
   {
       cout << "Mostrar elementos del mas barato al mas caro:" << endl << endl;
       insercion(numeros);
    }
   

    

    for (int i = 0; i < cantidadObjetos; i++)
    {
        idTranslate(numeros[i].ID, cantidadObjetos, stock);
        cout << endl << endl;
    }
}

//Mayor a menor
void insercion(struct IdAndInt numeros[])
{
    int i, pos;
    IdAndInt temp;  // Variable temporal para almacenar el nodo actual

    for (i = 1; i < cantidadObjetos; i++)
    {
        temp = numeros[i];
        pos = i;

        while ((pos > 0) && (numeros[pos - 1].entero > temp.entero))
        {
            // Realiza el intercambio tanto para el entero como para la cadena de texto
            numeros[pos] = numeros[pos - 1];
            pos--;
        }

        // Coloca el nodo temporal en su posici�n correcta
        numeros[pos] = temp;
    }
}

//Menor a mayor
void seleccion(struct IdAndInt numeros[])
{
   
    int i, j, max, aux;
    string auxstring;
    for (i = 0; i < cantidadObjetos; i++)
    {
        max = i;
        for (j = i + 1; j < cantidadObjetos; j++)
        {
            if (numeros[j].entero > numeros[max].entero)
            {
                max = j;
            }
        }
        aux = numeros[i].entero;
        auxstring = numeros[i].ID;
        numeros[i].entero = numeros[max].entero;
        numeros[i].ID = numeros[max].ID;
        numeros[max].entero = aux;
        numeros[max].ID = auxstring;
    }
}

nodoColaSimple* crearNodo(int n, nodoColaSimple* padre) {
    nodoColaSimple* nuevo_nodo = new nodoColaSimple();
    nuevo_nodo->dato = n;
    nuevo_nodo->der = NULL;
    nuevo_nodo->izq = NULL;
    nuevo_nodo->padre = padre;

    return nuevo_nodo;
}

//Funci�n para insrtar el elemento al �rbol

void insertarNodo(nodoColaSimple*& arbol, int n, nodoColaSimple* padre)
{
    if (arbol == NULL) { //Si el arbol esta vacio

        nodoColaSimple* nuevo_nodo = crearNodo(n, padre);
        arbol = nuevo_nodo;
    }
    else { //si el arbol tiene un nodo o mas
        int valorRaiz = arbol->dato;//obtenemos el valor de la raiz
        if (n < valorRaiz) {//si el elemento es menor a la raiz, insertamos en izq
            insertarNodo(arbol->izq, n, arbol);
        }
        else {//si el elemento es menor a la raiz, insertamos en der
            insertarNodo(arbol->der, n, arbol);
        }
    }
}

bool busquedaArbol(nodoColaSimple* arbol, int n) 
{
    if (arbol == NULL) {
        return false;
    }
    else if (arbol->dato == n) {
        return true;
    }
    else if (n < arbol->dato) {
        return busquedaArbol(arbol->izq, n);
    }
    else {
        return busquedaArbol(arbol->der, n);
    }
}

void inicializarCola(Cola& cola)
{
    // Inicializando punteros de la cola
    cola.post = NULL;
    cola.pre = NULL;
}

bool colaVacia(const Cola& cola)
{
    return cola.post == NULL;
}

void encolar(Cola& cola)
{
    string x, concatenacion, hora, provedorBuscar;
    int monto, folio,aux,op2,valido;
    punteroColaSimple p_aux = new(struct pedidosPendientes);
    bool verdad=false;
    do
    {
        cout << "Ingrese el nombre del provedor: ";
        getline(cin, x);
        aux = verificarExistenciaProvedor(x);
        if (aux == 1)
        {
            p_aux->nombreProvedor = x;
            pedidos[cantidadPedidos].nombreProvedor = x;
            cout << "Ingrese el monto del credito: ";
            cin >> monto;
            p_aux->monto = monto;
            pedidos[cantidadPedidos].monto = monto;
            hora = obtenerHora();
            p_aux->fecha = hora;
            pedidos[cantidadPedidos].fecha = hora;
            p_aux->sgte = NULL;
            concatenacion = x.append(to_string(monto));
            folio = stringtokey(concatenacion);
            p_aux->folio = folio;
            pedidos[cantidadPedidos].folio = folio;
            insertarNodo(arbol, folio, NULL);
            cantidadPedidos++;
            cout << stringtokey(concatenacion);
            if (cola.post == NULL)
            {
                cola.post = p_aux;
            }
            else
            {
                cola.pre->sgte = p_aux;
            }
            cola.pre = p_aux;
        }
        else if (aux == -1)
        {
            system("cls");
            cout << "El provedor no se encuentra en el registro" << endl;
            cout << endl<<"Estos son los provedores registrados" << endl;
            mostrarProvedores();
            cout << endl<<"Desea volverlo a intentar o salir al menu" << endl;
            cout << "1. Volver a intentar" << endl;
            cout << "2. Salir al menu" << endl;
            cout << "Ingrese la opcion deseada: ";
            cin >> op2;
            cin.ignore();
            do
            {
                if (op2 == 1)
                {
                    cout << "Ok!" << endl;
                    verdad = true;
                    break;
                }
                else if (op2 == 2)
                {
                    return;
                }
                else
                {
                    cout << "Opcion no valida" << endl;
                }
            } while (verdad == false);
            system("pause");
            system("cls");
        }
    } while (aux != 1 && aux != 0);
    
}

int desencolar(Cola& cola)
{
    int n;
    punteroColaSimple p_aux = cola.post;
    n = p_aux->folio;
    cola.post = (cola.post)->sgte;
    delete (p_aux);
    return n;
}

void mostrarCola(const Cola& cola)
{
    punteroColaSimple p_aux = cola.post;
    while (p_aux != NULL)
    {
        cout << "Folio: " << p_aux->folio << endl;
        cout << "Fecha de registro: " << p_aux->fecha << endl;
        /*cout << "Provedor: " << p_aux->nombreProvedor << endl;*/
        buscarProvedor(p_aux->nombreProvedor);
        cout << "Monto a credito: " << p_aux->monto << " pesos" << endl << endl;
        p_aux = p_aux->sgte;
    }
}

void vaciarCola(Cola& cola)
{
    punteroColaSimple p_aux, r_aux;
    p_aux = cola.post;
    while (p_aux != NULL)
    {
        r_aux = p_aux;
        p_aux = p_aux->sgte;
        delete (r_aux);
    }
    cola.post = NULL;
    cola.pre = NULL;
}

void mostrarpedido(struct historialPedidos pedido[], int cantidad, int folio)
{
    bool encontrado = false;
    for (int i = 0; i < cantidad; i++)
    {
        if (pedido[i].folio == folio)
        {
            encontrado = true;
            cout << "Folio: " << pedido[i].folio << endl;
            cout << "Fecha de registro: " << pedido[i].fecha << endl;
            cout << "Proveedor: " << pedido[i].nombreProvedor << endl;
            cout << "Monto a credito: " << pedido[i].monto << " pesos" << endl << endl;
        }
    }

    if (!encontrado)
    {
        cout << "El elemento con folio " << folio << " no existe en el historial de pedidos." << endl;
    }
}
void preOrden(nodoColaSimple* arbol)
{
    if (arbol == NULL) {
        return;
    }
    else {
        cout << arbol->dato << "  ";
        preOrden(arbol->izq);
        preOrden(arbol->der);
    }
}

void menucola()
{
    cout << endl << "---------------------------------------------------" << endl;
    cout << "               Creditos          " << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " Seleccione una opcion:" << endl;
    cout << " 1. Registrar nuevo credito" << endl;
    cout << " 2. Mostrar creditos pendientes" << endl;
    cout << " 3. Consultar creditos" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Ingrese el numero de la opcion deseada: ";
}

void administracionCreditos(Cola& cola)
{
    system("cls");
    int dato, op, pagar,op2=99;
    string x;
    bool hecho=false;
    bool valido = false;
    do
    {
        menucola();
        cin >> op;
        cin.ignore();
        switch (op)
        {
        case 1:
            if (primero != NULL)
            {
                system("cls");
                encolar(cola);
                cout << endl << "Credito agregado" << endl;
                system("pause");
                return;
            }
            else
            {
                cout << "De de alta a los provedores primero" << endl;
                cout << endl << "Regresando al menu";
                system("pause");
                return;
            }
            hecho = true;
            break;
        case 2:
            system("cls");
            cout << "\nMostrando creditos pendientes\n";
            if (!colaVacia(cola))
            {
                mostrarCola(cola);
                cout << endl << "Desea marcar como pagado el credito mas antiguo?" << endl;
                cout << "1. Deseo pagar mi credito" << endl;
                cout << "2. En otro momento" << endl;
                cout << "Ingrese la opcion deseada: ";
                cin >> pagar;
                if (pagar == 1)
                {
                    x= desencolar(cola);
                    cout << "\nCredito pagado" << endl;
                    system("pause");
                    return;
                }
                else
                {
                    cout << "Ok!" << endl;
                    system("pause");
                    return;
                }
            }
            else
            {
                cout << "\nNo hay creditos pendientes" << endl;
                system("pause");
                return;
            }
            hecho = true;
            break;
        case 3:
            system("cls");
            cout << "Mostrando folios registrados" << endl << endl;
            preOrden(arbol);
            cout << "\nInserta el folio a buscar: ";
            cin >> dato;
            if (busquedaArbol(arbol, dato) == true) 
            {
                mostrarpedido(pedidos, cantidadPedidos, dato);
                system("pause");
                return;
            }
            else {
                cout << "\nEl folio: " << dato << " No EXISTE en el registro";
                system("pause");
                return;
            }
            cout << "\n\n";
            hecho = true;
            break;
        case 4:
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
        }
    } while (op != 4);    
}

void grafos()
{
    char origen[15], destino[15], seg[3];
    seg[0] = 'S';
    int o, d, i;

    cout << "Este programa crea una ruta de una ciudad a otra\n"
        << "Las ciudades en nuestro sistema son:\n"
        << "-Moroleon "
        << "-Yuriria "
        << "-Valle "
        << "-Tarimbaro "
        << "-Salvatierra\n"
        << "-Salamanca "
        << "-Cortazar "
        << "-Morelia "
        << "-Acambaro "
        << "-Celaya "
        << "-Irapuato\n";


    memset(visitado, 0, sizeof(visitado));
    cout << "\nIntruzca la ciudad de origen: ";
    cin >> origen;
    corregidor(origen);
    o = busqueda(origen);

    while (o == 99)
    {
        cout << "Entrada no valida intente de nuevo: ";
        cin >> origen;
        corregidor(origen);
        o = busqueda(origen);
    }

    cout << "Introduzca la ciuad de destino: ";
    cin >> destino;
    corregidor(destino);
    d = busqueda(destino);

    while (d == 99)
    {
        cout << "Entrada no valida intente de nuevo: ";
        cin >> destino;
        corregidor(destino);
        d = busqueda(destino);
    }

    df = d;

    if (df == o)
    {
        cout << "Usted esta en " << ciudades[o] << endl;
    }
    else
    {
        dfs(o);
        camino[0] = o;
        cout << "\nSe muestra ruta encontrada de " << ciudades[o] << " a " << ciudades[df] << ":\n";
        for (i = 0; i <= cont; i++)
        {
            if (i == cont)
            {
                cout << ciudades[camino[i]] << endl;
            }
            else
            {
                cout << ciudades[camino[i]] << "-->";
            }
        }
    }

    cont = 0;
    sal = 0;
    fst = 0;
}

void dfs(int of)
{
    int i;
    visitado[of] = 1;
    cont++;

    for (i = 0; i < 11; i++)
    {
        if ((vecinas[of][i] == 1) && (visitado[i] != 1))
        {
            if (i == df)
            {
                camino[cont] = i;
                fst = 1;
                sal = 1;
                break;
            }
        }
    }

    if (fst == 0)
    {
        for (i = 0; i < 11; i++)
        {
            if (sal == 1)
            {
                break;
            }
            else
            {
                if ((vecinas[of][i] == 1) && (visitado[i] != 1))
                {
                    camino[cont] = i;
                    dfs(i);
                }
            }
            if (i == 10)
            {
                cont--;
            }
        }
    }
}

int busqueda(const char en[15])
{
    int i, lon, j, cont = 99;

    lon = strlen(en);

    for (i = 0; i < 11; i++)
    {
        j = -1;

        while (ciudades[i][j + 1] == en[j + 1])
        {
            j++;
        }

        if (j == lon)
        {
            cont = i;
            break;
        }
    }

    if (cont == 99)
    {
        return 99;
    }
    else
    {
        return cont;
    }
}

void corregidor(char* ent)
{
    int lon, i;
    lon = strlen(ent);
    ent[0] = toupper(ent[0]);

    for (i = 1; i < lon; i++)
    {
        ent[i] = tolower(ent[i]);
    }
}