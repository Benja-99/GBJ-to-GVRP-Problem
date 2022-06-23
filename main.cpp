#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <cmath>
#include <map>
#include <iomanip>
#include <sys/time.h>
#include <algorithm>
#include <ctime>
using namespace std;

#define _USE_MATH_DEFINES ;
unsigned t0, t1;
const int nodos = 100;
class node
{
public:
    int id = -1;
    int id_interno;
    string tipo;
    double longitude;
    double latitude;
    bool en_solucion = false;
};

const double pi = 3.14159265358979323846264338327950288;

/*
Nombre funcion: haversine
Descripcion: funcion que calcula la ditancia de haversine entre 2 nodos del grafo
Input:
- lat1: Input de tipo double el cual es la coordenada de latitud del nodo origen
- lon1: Input de tipo double el cual es la coordenada de longitud del nodo origen
- lat2: Input de tipo double el cual es la coordenada de latitud del nodo destino
- lon2: Input de tipo double el cual es la coordenada de longitud del nodo destino
Output: Retorna un double que contiene la distancia de haversine entre 2 nodos
*/
double haversine(double lat1, double lon1, double lat2, double lon2)
{

    double dLat = (lat2 - lat1) * pi / 180.0;
    double dLon = (lon2 - lon1) * pi / 180.0;

    // convert to radians
    lat1 = (lat1)*pi / 180.0;
    lat2 = (lat2)*pi / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}

/*
Nombre funcion: existeEnVector
Descripcion: funcion que revisa si un entero esta en el vector dado
Input:
- v: Input de tipo vector de enteros que es donde se busca el entero ingresado
- busqueda: Input de tipo int que es lo que se busca dentro del vector ingresado
Output: Retorna true si encuentra el entero dentro del vector, en caso contrario retorna false
*/
bool existeEnVector(vector<int> v, int busqueda)
{
    return find(v.begin(), v.end(), busqueda) != v.end();
}
/*
Nombre funcion: aun_queda_customers_sin_visitar
Descripcion: funcion que revisa si quedan nodos sin visitar
Input:
- route: Input de tipo vector de nodos que contiene los nodos de la ruta recien generada
- vector_nodos: Input de tipo vector de nodos que contiene todos los nodos de clientes, estaciones y el deposito
- num_customers: Input de tipo entero que contiene el numero total de nodos
- nodos_visitados: Input de tipo vector de enteros que contiene todos los nodos que han sido visitados
Output: Retorna true si aun quedan nodos sin visitar, en caso contrario retorna false
*/
bool aun_quedan_customers_sin_visitar(vector<node> &route, vector<node> &vector_nodos, int num_customers, vector<int> &nodos_visitados)
{
    bool flag = false;
    vector<node> aux;
    for (int i = 0; i < route.size(); i++)
    {
        if (!existeEnVector(nodos_visitados, route[i].id_interno))
        {
            nodos_visitados.push_back(route[i].id_interno);
        }
    }
    for (auto i_nodo : nodos_visitados)
    {
        vector_nodos[i_nodo].en_solucion = true;
    }
    if (nodos_visitados.size() < num_customers)
    {
        return true;
    }
    return false;
}
/*
Nombre funcion: mayor
Descripcion: funcion que entrega el numero mayor dentro del vector dado
Input:
- distancias: Input de tipo vector de double que contiene las distancias entre los nodos
Output: Retorna la posicion de la mayor distancia entre nodos encontrada
*/
int mayor(vector<double> distancias)
{
    int may = distancias[0];
    int pos = 0;
    int x;
    for (x = 1; x < 5; x++)
    {
        if (distancias[x] > may)
        {
            may = distancias[x];
            pos = x;
        }
    }
    return pos;
}
/*
Nombre funcion: PrintVec
Descripcion: funcion que retorna un string con el contenido de un vector de nodos
Input:
- vec: Input de tipo vector de nodos que contiene los nodos que se necesitan pasar a string
Output: Retorna un string con la informacion de los nodos dentro del vector
*/
string PrintVec(const vector<node> &vec)
{
    string to_return = "", aux;
    for (const auto &item : vec)
    {
        aux = to_string(item.id_interno);
        to_return += (" - " + aux + item.tipo);
    }
    return to_return;
}
/*
Nombre funcion: string_to_vector
Descripcion: funcion que separa un string por espacios y los ingresa a un vector
Input:
- linea: Input de tipo string que contiene la informacion que se quiere transformar a vector
Output: Retorna un vector de string, con el string ingresado separado por espacios
*/
vector<string> string_to_vector(string linea)
{
    vector<string> linea_aux;
    stringstream p;
    p << linea;
    while (p >> linea)
    {
        linea_aux.push_back(linea);
    }
    return linea_aux;
}
/*
Nombre funcion: cortar_nodos_inutiles
Descripcion: funcion que saca del vector ingresado los nodos no validos
Input:
- ruta: Input de tipo vector de nodos que contiene los nodos de la ruta que se quiere limpiar
Output: Retorna un vector de nodos sin los nodos innecesarios
*/
vector<node> cortar_nodos_inutiles(vector<node> ruta)
{
    vector<node> aux;

    for (size_t i = 0; i < ruta.size(); i++)
    {
        if (i == 0)
        {
            aux.push_back(ruta[0]);
        }
        else if (ruta[i].id_interno != 0)
        {
            aux.push_back(ruta[i]);
        }
    }
    return aux;
}
/*
Nombre funcion: create_nodes
Descripcion: funcion que crea los nodos obteniendo la informacion del archivo ingresado
Input:
- archivo: Input de tipo ifstream que contiene la informacion del archivo para leerlo
- num: Input de tipo entero que contiene la cantidad de los nodos que se leeran
- inicio_id_interno: Input de tipo entero que contiene desde donde comienza un id a ingresar a los nodos
Output: Retorna un vector de nodos que contiene nodos hasta la cantidad indicada en num
*/
vector<node> create_nodes(ifstream &archivo, int num, int inicio_id_interno)
{
    string linea;
    vector<string> linea_aux;
    vector<node> vector_aux;
    for (size_t i = 0; i < num; i++)
    {
        getline(archivo, linea);
        linea_aux = string_to_vector(linea);
        node nodo_aux;
        nodo_aux.id_interno = inicio_id_interno;
        inicio_id_interno++;
        nodo_aux.id = stof(linea_aux[0]);
        nodo_aux.tipo = linea_aux[1];
        nodo_aux.longitude = stof(linea_aux[2]);
        nodo_aux.latitude = stof(linea_aux[3]);
        vector_aux.push_back(nodo_aux);
    }
    return vector_aux;
}
/*
Nombre funcion: gbj
Descripcion: funcion que realiza backtracking recursivo para encontrar una ruta del problema
Input:
- vector_nodos: Es un vector de nodos que contiene todos los nodos disponibles
- solucion: Es un vector de nodos que es donde se ira creando la ruta solucion
- nivel_actual: Varible de tipo entera que contiene el nivel actual en el arbol de busqueda
- max_time: tiempo maximo permitido por vehiculo
- max_distance: la distancia maxima que es posible recorrer sin pasar a una estacion de reabastecimiento
- speed: varible de tipo double que representa la velocidad del vehiculo
- service_time: varible de tipo double que representa el tiempo que tarda en atender un cliente
- refuel_time: varible de tipo double que representa el tiempo que tarda rellenar un estanque
- nivel_mayor_distancia: entero que contiene el nivel donde se tiene la mayor distancia entre nodos
- distancia_nodo_actual_anterior: Vector de tipo double que contiene la informacion de las distancias entre nodos de todos los niveles
- tiempo_actuales: Vector de tipo entero que contiene el tiempo acumulado en cada nivel del arbol de busqueda
- fueles_actuales: Vector de tipo entero que contiene la cantidad de combustible en cada nivel del arbol de busqueda
Output:
La funcion no tiene retorno, se trabaja con las direcciones de memoria
 */
void gbj(vector<node> &vector_nodos, vector<node> &solucion, int nivel_actual, float max_time, float max_distance, float speed, float service_time, float refuel_time, int &nivel_mayor_distancia, vector<double> &distancias_nodo_actual_anterior, vector<int> &tiempos_actuales, vector<double> &fueles_actuales)
{

    int aux_time, aux_fuel, aux_dist, i_candidato;
    vector_nodos[0].en_solucion = true;

    for (i_candidato = vector_nodos.size(); i_candidato > 0; i_candidato--)
    {
        double distancia_entre_nodo_nuevo_y_anterior = haversine(vector_nodos[i_candidato].latitude, vector_nodos[i_candidato].longitude, solucion[nivel_actual - 1].latitude, solucion[nivel_actual - 1].longitude);
        if (vector_nodos[i_candidato].tipo == "c")
        {
            aux_time = tiempos_actuales[nivel_actual - 1] + service_time + (distancia_entre_nodo_nuevo_y_anterior / speed);
            aux_fuel = fueles_actuales[nivel_actual - 1] - distancia_entre_nodo_nuevo_y_anterior;
        }
        else
        {
            aux_time = tiempos_actuales[nivel_actual - 1] + refuel_time + (distancia_entre_nodo_nuevo_y_anterior / speed);
            aux_fuel = max_distance;
        }
        aux_dist = distancias_nodo_actual_anterior[nivel_actual - 1] + distancia_entre_nodo_nuevo_y_anterior;

        if (aux_fuel > 0 && aux_time <= 3 * max_time / 5 && !vector_nodos[i_candidato].en_solucion)
        {
            tiempos_actuales[nivel_actual] = aux_time;

            distancias_nodo_actual_anterior[nivel_actual] = distancia_entre_nodo_nuevo_y_anterior;
            fueles_actuales[nivel_actual] = aux_fuel;
            vector_nodos[i_candidato].en_solucion = true;
            solucion[nivel_actual] = vector_nodos[i_candidato];
            vector_nodos[vector_nodos[i_candidato].id_interno].en_solucion = true;
            gbj(vector_nodos, solucion, nivel_actual + 1, max_time, max_distance, speed, service_time, refuel_time, nivel_mayor_distancia, distancias_nodo_actual_anterior, tiempos_actuales, fueles_actuales);
            vector_nodos[i_candidato].en_solucion = false;

            vector_nodos[vector_nodos[i_candidato].id_interno].en_solucion = false;
            if (nivel_actual > nivel_mayor_distancia)
            {
                return;
            }
        }
    }
    nivel_mayor_distancia = mayor(distancias_nodo_actual_anterior);
    return;
}
/*
nombre: main
Descripcion: funcion principal, se leen los archivos y se almacena su contenido en varibles, luego se llama a la funcion
gbj en un while hasta que queden clientes sin visitar para encontrar una solucion al problema.
-Input por consola: El nombre del archivo a ejecutar
-Output: No tiene output, pero genera el archivo (nombre_problema).out el cual contiene la solucion al problema.
 */
int main()
{
    string archivo;
    cout << "Ingrese nombre del archivo (con .dat)\n";
    cin >> archivo;
    ifstream archivo_leido(archivo);
    string linea;
    string linea_inicial;
    string linea_depot;

    int id_nodos = 0;
    getline(archivo_leido, linea_inicial);
    getline(archivo_leido, linea_depot);
    vector<string> linea_ini = string_to_vector(linea_inicial);
    string name = linea_ini[0];
    float num_customers = stof(linea_ini[1]);
    float num_afs = stof(linea_ini[2]);
    float max_time = stof(linea_ini[3]);
    float max_distance = stof(linea_ini[4]);
    float speed = stof(linea_ini[5]);
    float service_time = stof(linea_ini[6]);
    float refuel_time = stof(linea_ini[7]);
    vector<string> linea_depi = string_to_vector(linea_depot);
    node node_depot;
    node_depot.id_interno = id_nodos;
    id_nodos++;
    node_depot.id = stof(linea_depi[0]);
    node_depot.tipo = linea_depi[1];
    node_depot.longitude = stof(linea_depi[2]);
    node_depot.latitude = stof(linea_depi[3]);
    getline(archivo_leido, linea);
    vector<node> nodes_afs = create_nodes(archivo_leido, num_afs - 1, id_nodos);

    vector<node> nodes_customer = create_nodes(archivo_leido, num_customers, id_nodos + 21);
    int total_nodos = num_afs - 1 + num_customers;
    vector<node> vector_nodos;
    vector_nodos.push_back(node_depot);

    for (auto node : nodes_afs)
    {
        vector_nodos.push_back(node);
    }
    for (auto node : nodes_customer)
    {
        vector_nodos.push_back(node);
    }

    vector<int> nodos_utilizados;
    bool flag_nodos_visitados = true;
    int id = 1;
    fstream file_out;
    file_out.open(name + ".out", std::ios_base::out);
    if (!file_out.is_open())
    {
        cout << "failed to open " << name + ".out" << '\n';
        return 0;
    }
    string ruta;

    while (flag_nodos_visitados)
    {
        vector<node> route(total_nodos);
        vector<double> distancias_nodo_actual_anterior(total_nodos);
        vector<int> tiempos_actuales(total_nodos);
        vector<double> fueles_actuales(total_nodos);
        route[0] = vector_nodos[0];
        route[0].en_solucion = true;
        distancias_nodo_actual_anterior[0] = 0;
        tiempos_actuales[0] = 0;
        fueles_actuales[0] = max_distance;
        vector_nodos[0].en_solucion = true;
        int nivel_mayor_distancia = -1;

        gbj(vector_nodos, route, 1, max_time, max_distance, speed, service_time, refuel_time, nivel_mayor_distancia, distancias_nodo_actual_anterior, tiempos_actuales, fueles_actuales);
        route = cortar_nodos_inutiles(route);
        route.push_back(vector_nodos[0]);
        ruta = PrintVec(route);
        file_out << ruta << endl;
        id++;
        if (!aun_quedan_customers_sin_visitar(route, vector_nodos, total_nodos - 1, nodos_utilizados))
        {
            flag_nodos_visitados = false;
        }
    }
}
