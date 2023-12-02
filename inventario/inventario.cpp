// inventario.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "string"
#include "vector"
#include <chrono>
#include <ctime>
using namespace std;
#define MAX 50

int cantidadObjetos = 5;
int cantidadCategorias = 3;
int ayudaGlobal;
string stringGlobal;
vector<string> categorias;



//Listas simples
struct clientes
{
    string nombreCliente;
    string direccion;
    clientes* siguiente;
    clientes* atras;
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

//Para busqueda binaria y ordenamiento por insercion
union IDints {
    int idEntero;
};

union IDints codigosToInts[MAX];
int indiceUnion = 0;

struct IdAndInt
{
    int entero;
    string ID;
};

struct IdAndInt IDyEntero[MAX];

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
void agregarCliente();
void mostrarClientes();
void eliminarCliente();
void eliminarProducto(string idEliminar, struct productos a[], int& cantidad);
void mostrarProductos(struct productos a[], int cantidad);
void agregarMoviemiento(struct productos producto[], struct caducidad& caduc);
void mostrarListaPU();
void mostrarListaUP();
void mostrarClientes();
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
void administracionCliente();
int buscarIndexPorID(int cantidad, union IDints a[], string ID);
void codigosAenteros(union IDints arreglo[], struct productos producto[]);
void quicksort(union IDints arreglo[], int primero, int ultimo);
int consultaIdExiste(int cantidad, union IDints a[], string ID);
void agregarPreciosUnion(struct IdAndInt numeros[]);
void mostrarMenorAMayor(struct IdAndInt numeros[], string orden);
void seleccion(struct IdAndInt numeros[]);
void insercion(struct IdAndInt numeros[]);




hashnode HashTable[1000];
int tablesize = 20;

void menu()
{
    cout << endl << "---------------------------------------------------" << endl;
    cout << "               LISTAS ORDENADAS C++           " << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " Seleccione una opcion:" << endl;
    cout << " 1. Hacer Movimiento" << endl;
    cout << " 2. Clientes" << endl;
    cout << " 3. Mostrar caducidades" << endl;
    cout << " 4. Movimientos del mas antiguo al mas actual" << endl;
    cout << " 5. Movimientos del mas actual al mas antiguo" << endl;
    cout << " 6. Mostrar Categorias" << endl;
    cout << " 7. Mostrar Elementos del mas caro al mas barato" << endl;
    cout << " 8. Mostrar Elementos del mas barato al mas caro" << endl;
    cout << " 7. Salir" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Ingrese el número de la opción deseada: ";
}

int main()
{
    struct caducidad caduc;
    caduc.atras = NULL;
    caduc.delante = NULL;
    int op,aux,key;
    string categoria;
    cargaDatos(stock, caduc);


    do
    {
        menu();
        cin >> op;
        cin.ignore();
        switch (op)
        {
        case 1:
            system("cls");
            agregarMoviemiento(stock,caduc);
            break;
        case 2:
            system("cls");
            administracionCliente();
            break;
        case 3:
            system("cls");
            mostrarCaducidad(caduc, cantidadObjetos, stock);
            system("pause");
            break;
        case 4:
            system("cls");
            mostrarListaPU();
            system("pause");
            break;
        case 5:
            system("cls");
            mostrarListaUP();
            system("pause");
            break;
        case 6:
            mostrarCategorias();
            cout << "\nSeleccione la categoria deseada: ";
            cin >> aux;
            cin.ignore();
            key = buscarnodo(categorias[aux-1]);
            if (key == -1)
            {
                cout << "La categoria no se encuentra" << endl;
            }
            else
            {
                system("cls");
                cout << "Se muestran los productos de la categoria "<< categorias[aux - 1] << endl<<endl;
                ImprimirLista(HashTable[key],stock);
                cout << endl << endl;
            }
            system("pause");
            break;
        case 7:
            system("cls");
            mostrarMenorAMayor(IDyEntero, "mayor");
            system("pause");
            break;
        case 8:
            system("cls");
            mostrarMenorAMayor(IDyEntero, "menor");
            system("pause");
            break;
        case 9:
            break;
        default:
            cout << endl << "Opcion no valida";

        }
        system("cls");
    } while (op != 9);

    return 0;
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
        cout << endl<<"\n¿Desea agregar el producto a una categoria existente o crear una nueva categoria?"<<endl;
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
        (q.atras)->sgte = aux;//en caso de que exista un elemento, apunta al último elemento
    }
    q.atras = aux;
}

//Mostrar Cola

void mostrarCaducidad(struct caducidad q, int cantidad, struct productos p[]) {
    struct nodo* aux;
    aux = q.delante;
    string ap;
    
    while (aux != NULL) {

        ap = aux->id;
        idTranslate(ap, cantidad, p);
        cout <<"Caducidad: "<< convertirFecha(aux->caducidad) << endl<<endl;
        aux = aux->sgte;
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

void agregarCliente()
{
    clientes* nuevo = new clientes();
    cout << "Inserte el nombre del cliente: ";
    getline(cin, nuevo->nombreCliente);
    cout << "Inserte la direccion del cliente: ";
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
    cout << "Cliente agregado" << endl;
}

void mostrarClientes()
{
    clientes* actual = new clientes();
    actual = primero;
    if (primero != NULL)
    {
        while (actual != NULL)
        {
            cout << endl << "Nombre: " << actual->nombreCliente << endl;
            cout << "Direccion: " << actual->direccion << endl ;
            actual = actual->siguiente;
        }
    }
    else
    {
        cout << "No existen clientes registrados";
    }
}


void eliminarCliente()
{
    if (primero != NULL)
    {
        clientes* actual = primero;
        clientes* anterior = NULL;
        string clienteBuscar;

        cout << "Ingrese el nombre del cliente a eliminar: ";
        getline(cin, clienteBuscar);

        bool encontrado = false;

        while (actual != NULL && !encontrado)
        {
            if (actual->nombreCliente == clienteBuscar)
            {
                cout << "\nEl cliente " << clienteBuscar << " existe en la lista" << endl;

                if (actual == primero) // Eliminar el primer nodo
                {
                    primero = actual->siguiente;
                }
                else if (actual == ultimo) // Eliminar el último nodo
                {
                    anterior->siguiente = NULL;
                    ultimo = anterior;
                }
                else // Eliminar cualquier otro nodo
                {
                    anterior->siguiente = actual->siguiente;
                }

                delete actual;
                cout << "Cliente " << clienteBuscar << " eliminado" << endl;
                encontrado = true;
            }
            else
            {
                anterior = actual;
                actual = actual->siguiente;
            }
        }

        if (!encontrado)
            cout << endl << "Elemento no encontrado";
    }
    else
    {
        cout << "No hay clientes registrados" << endl;
    }
}

void administracionCliente()
{
    bool hecho = false;
    int op;
    do
    {
        cout << "Ingrese el tipo de movimiento que desea realizar: ";
        cout << endl << "---------------------------------------------------" << endl;
        cout << "               Administracion clientes           " << endl;
        cout << "---------------------------------------------------" << endl;
        cout << " Seleccione una opción:" << endl;
        cout << " 1. Agregar cliente" << endl;
        cout << " 2. Mostrar CLientes" << endl;
        cout << " 3. Eliminar Clientes" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "Ingrese el número de la opción deseada: ";
        cin >> op;
        cin.ignore();
        switch (op)
        {
        case 1:
            system("cls");
            agregarCliente();
            system("pause");
            system("cls");
            hecho = true;
            break;
        case 2:
            system("cls");
            mostrarClientes();
            system("pause");
            system("cls");
            hecho = true;
            break;
        case 3:
            system("cls");
            eliminarCliente();
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
        
        cout << "Ingrese el tipo de movimiento que desea realizar: ";
        cout << endl << "---------------------------------------------------" << endl;
        cout << "               Realizar Movimiento           " << endl;
        cout << "---------------------------------------------------" << endl;
        cout << " Seleccione una opción:" << endl;
        cout << " 1. Agregar producto" << endl;
        cout << " 2. Cambiar Nombre a producto existente" << endl;
        cout << " 3. Cambiar Precio a producto existente" << endl;
        cout << " 4. Dar Entrada" << endl;
        cout << " 5. Dar salida" << endl;
        cout << " 6. Eliminar Producto" << endl;
        cout << " 7. Reabastecimiento" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "Ingrese el número de la opción deseada: ";
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
            cout << endl << "Estos son los productos actuales:" << endl << endl;
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
            cout << endl << "Estos son los productos actuales:" << endl << endl;
            indice = consultaIdExiste(cantidadObjetos, codigosToInts, claveID);
            cout << endl << "Ingrese el nuevo precio: ";
            cin >> aux;
            nuevo->antiguo = to_string(producto[indice].precio);  // Cambiar aquí a to_string
            producto[indice].precio = aux;
            nuevo->nuevo = to_string(aux);
            nuevo->tipo = "Cambio de Precio";
            nuevo->producto = producto[indice].nombre;
            hecho = true;
            break;
        case 4:
            cout << endl << "Estos son los productos actuales:" << endl << endl;
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
            do
            {
                cout << endl << "Estos son los productos actuales:" << endl << endl;
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
                    cout << "La cantidad que desea quitar no es valida, intente de nuevo" << endl;
                }
            } while (ban == false);
            nuevo->tipo = "Salida";
            hecho = true;
            break;
        case 6:
            cout << endl << "Estos son los productos actuales:" << endl << endl;
            indice = consultaIdExiste(cantidadObjetos, codigosToInts, claveID);
            cin.ignore();
            nuevo->antiguo = producto[indice].nombre;
            nuevo->nuevo = "Eliminado";
            nuevo->producto = producto[indice].nombre;
            nuevo->tipo = "Eliminar Objeto";
            eliminarProducto(producto[indice].id, producto, cantidadObjetos);
            hecho = true;
            break;
        case 7:
            
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
}

void mostrarListaUP()
{
    historialMovimientos* actual = ultimoh; 
    cout << endl << "Mostrando movimientos del mas actual al mas antiguo" << endl;
    while (actual != NULL)
    {
        cout << endl << "Fecha: " <<actual->fechaMov << endl;
        cout << "Producto: " << actual->producto << endl;
        cout << "Tipo: " << actual->tipo << endl;
        cout << "Antiguo: " << actual->antiguo << endl;
        cout << "Nuevo: " << actual->nuevo << endl;
        actual = actual->atras; 
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
        cout << "El producto no se encuentra en el inventario" << endl;
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
        cout << "Año no valido. Ingrese un anio valido (YYYY): ";
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

void eliminarProducto(string idEliminar, struct productos a[], int& cantidad) 
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

        cantidad--;
        cout << "Producto con ID " << idEliminar << " eliminado exitosamente." << endl;
    }
    else {
        cout << "No se encontró un producto con el ID " << idEliminar << "." << endl;
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
    int valor, i,suma=0,index,aux,cont=0;
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
    //Posición del elemento central
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
        cout << numeros[i].entero << endl;
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

        // Coloca el nodo temporal en su posición correcta
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