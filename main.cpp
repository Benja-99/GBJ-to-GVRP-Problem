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
#include <chrono>
using namespace std;

#define _USE_MATH_DEFINES ;
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

const double pi = 3.1415926535;

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
bool aun_quedan_customers_sin_visitar(vector<node> &route, vector<node> &vector_nodos, int total_nodos, vector<int> &nodos_visitados)
{
    bool flag = false;
    vector<node> aux;
    for (auto nodo : route)
    {
        if (!existeEnVector(nodos_visitados, nodo.id_interno))
        {
            nodos_visitados.push_back(nodo.id_interno);
        }
    }
    for (auto i_nodo : nodos_visitados)
    {
        vector_nodos[i_nodo].en_solucion = true;
    }
    if (nodos_visitados.size() < total_nodos)
    {
        return true;
    }
    return false;
}
/*
Nombre funcion: nodo_en_solucion
Descripcion: funcion que revisa si el nodo pasado como parametro se encuentra en el vector solucion ingresado.
Input:
- solucion: Vector de nodos que contiene los nodos ingresados hasta el momento que conforman una posible solucion
- node_candidato: Nodo candidato que se busca revisar si es que ya esta ingresado en la solucion
Output: Retorna true si el nodo ya esta en la solucion y false si no esta.
*/
bool nodo_en_solucion(vector<node> solucion, node node_candidato)
{
    vector<int> solucion_int;
    for (auto node : solucion)
    {
        solucion_int.push_back(node.id_interno);
    }
    return existeEnVector(solucion_int, node_candidato.id_interno);
}

/*
Nombre funcion: sumar_customers
Descripcion: funcion suma la cantidad de customers en una ruta de nodos ingresada
Input:
- suma: Input de tipo entero que contiene la suma de las rutas anteriores ya ingresadas
- route: Input de vector de nodos que contiene una ruta de la posible solucion
Output: No retorna nada porque se trabaja con las direcciones de memoria
*/
void sumar_customers(int &suma, vector<node> route)
{
    for (auto nodo : route)
    {
        if (nodo.tipo == "c")
        {
            suma++;
        }
    }
}

/*
Nombre funcion: mayor
Descripcion: funcion que entrega el numero mayor dentro del vector dado
Input:
- distancias: Input de tipo vector de double que contiene las distancias entre los nodos
Output: Retorna la posicion de la mayor distancia entre nodos encontrada
*/
int mayor(vector<node> ruta)
{
    vector<double> distancias(ruta.size());
    for (size_t i = 0; i < ruta.size() - 1; i++)
    {
        distancias[i] = haversine(ruta[i].latitude, ruta[i].longitude, ruta[i + 1].longitude, ruta[i + 1].latitude);
    }

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
    string to_return = "", aux, distancia_string, aux_2;
    double distancia;
    // for (size_t i = 0; i < vec.size() - 1; i++)
    for (size_t i = 0; i < vec.size(); i++)
    {
        aux = to_string(vec[i].id_interno);
        aux_2 = to_string(vec[i + 1].id_interno);
        distancia = haversine(vec[i].latitude, vec[i].longitude, vec[i + 1].latitude, vec[i + 1].longitude);
        distancia_string = to_string(distancia);
        to_return += (aux + vec[i].tipo + " - ");
        // to_return += (aux + "|" + aux_2 + " " + distancia_string + " - ");
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
void create_nodes(ifstream &archivo, int num, int inicio_id_interno, vector<node> &nodes)
{
    string linea;
    vector<string> linea_aux;
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
        nodes.push_back(nodo_aux);
    }
}

/*
Nombre funcion: distancia_total_ruta
Descripcion: funcion que calcula la distancia total que recorre una ruta
Input:
- ruta: Input de tipo vector de nodos que contiene los nodos de la ruta recien generada
Output: Retorna un valor de tipo double que contiene la distancia total recorrida por la ruta ingresada
*/
double distancia_total_ruta(vector<node> ruta)
{
    double distancia = 0;
    for (size_t i = 0; i < ruta.size() - 1; i++)
    {
        distancia = distancia + haversine(ruta[i].latitude, ruta[i].longitude, ruta[i + 1].latitude, ruta[i + 1].longitude);
    }
    return distancia;
}

/*
Nombre funcion: useless_route
Descripcion: Funcion que revisa si una ruta ingresada contiene clientes visitados o no
Input:
- route: Input de tipo vector de nodos que contiene los nodos de la ruta recien generada
Output: Retorna false si se encuentra un cliente visitado en route y true si es que no se encuentra
*/
bool useless_route(vector<node> route)
{
    for (auto nodo : route)
    {
        if (nodo.tipo == "c")
        {
            return false;
        }
    }
    return true;
}

/*
Nombre funcion: agregar_node_depot
Descripcion: funcion que intenta agregar el nodo depot al final de una ruta generada por backtracking
Input:
- route: Input de tipo vector de nodos que contiene los nodos de una ruta
- nodes: Es un vector de nodos que contiene todos los nodos disponibles
- speed: varible de tipo double que representa la velocidad del vehiculo
- max_distance: la distancia maxima que es posible recorrer sin pasar a una estacion de reabastecimiento
- service_time: varible de tipo double que representa el tiempo que tarda en atender un cliente
- refuel_time: varible de tipo double que representa el tiempo que tarda rellenar un estanque
- max_time: tiempo maximo permitido por vehiculo
Output:
La funcion no tiene retorno, se trabaja con las direcciones de memoria
 */
void agregar_node_depot(vector<node> &route, vector<node> nodes, float speed, float max_distance, float service_time, float refuel_time, float max_time)
{
    vector<double> distancias, tiempos, fueles;
    bool flag_help = true, flag_aux = false;
    double distancia, distancia_node_depot, last_distance, distancia_last_node_nuevo_node, last_time, last_fuel, aux_time, aux_fuel, aux_dist;
    node last_node, node_depot = nodes[0];
    vector<node> return_nodes;
    distancias.push_back(0);
    tiempos.push_back(0);
    fueles.push_back(max_distance);

    for (size_t i = 1; i < route.size(); i++)
    {
        distancia = haversine(route[i - 1].latitude, route[i - 1].longitude, route[i].latitude, route[i].longitude);
        distancias.push_back(distancia + distancias[i - 1]);
        if (route[i].tipo == "c")
        {
            tiempos.push_back(tiempos[i - 1] + service_time + (distancia / speed));
            fueles.push_back(fueles[i - 1] - distancia);
        }
        else
        {
            tiempos.push_back(tiempos[i - 1] + refuel_time + (distancia / speed));
            fueles.push_back(max_distance);
        }
    }
    last_distance = distancias.back();
    last_fuel = fueles.back();
    last_time = tiempos.back();
    return_nodes.push_back(route.back());
    while (flag_help)
    {
        for (auto &nodo : nodes)
        {
            nodo.en_solucion = false;
        }
        last_node = return_nodes.back();
        if (last_node.id_interno == 0)
        {
            return;
        }
        distancia_node_depot = haversine(last_node.latitude, last_node.longitude, node_depot.latitude, node_depot.longitude);
        if ((last_fuel - distancia_node_depot) >= 0)
        {
            route.push_back(node_depot);
            flag_help = false;
            break;
        }
        for (auto nodo : nodes)
        {
            if (nodo.id_interno != 0 && nodo.tipo == "f")
            {
                distancia_last_node_nuevo_node = haversine(last_node.latitude, last_node.longitude, nodo.latitude, nodo.longitude);
                aux_time = last_time + refuel_time + (distancia_last_node_nuevo_node / speed);
                aux_dist = last_distance + distancia_last_node_nuevo_node;
                aux_fuel = last_fuel - distancia_last_node_nuevo_node;
                if (aux_fuel >= 0 && aux_time <= max_time && !nodes[nodo.id_interno].en_solucion)
                {
                    aux_fuel = max_distance;
                    route.push_back(nodo);
                    return_nodes.push_back(nodo);
                    nodes[nodo.id_interno].en_solucion = true;

                    last_distance = aux_dist;
                    last_fuel = aux_fuel;
                    last_time = aux_time;
                    flag_aux = false;
                    break;
                }
                else
                {
                    flag_aux = true;
                }
            }
        }
        if (flag_aux)
        {
            break;
        }
    }
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
- distancia_actual: Vector de tipo double que contiene la informacion de las distancias entre nodos de todos los niveles
- tiempo_actuales: Vector de tipo entero que contiene el tiempo acumulado en cada nivel del arbol de busqueda
- fueles_actuales: Vector de tipo entero que contiene la cantidad de combustible en cada nivel del arbol de busqueda
Output:
La funcion no tiene retorno, se trabaja con las direcciones de memoria
 */
void gbj(vector<node> &vector_nodos, vector<node> &solucion, int nivel_actual, float max_time, float max_distance, float speed, float service_time, float refuel_time, int &nivel_mayor_distancia, double &distancia_actual, int tiempo_actual, double fuel_actual, std::chrono::_V2::system_clock::time_point &t0)
{
    double fuel_inicial = fuel_actual;
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> float_ms = t1 - t0;
    double duracion_algoritmo = (float_ms.count()) / 1000;
    if (duracion_algoritmo == 300)
    {
        return;
    }

    int aux_time, aux_fuel, aux_dist, i_candidato;

    node node_candidato;

    for (i_candidato = vector_nodos.size(); i_candidato > 0; i_candidato--)
    {
        node_candidato = vector_nodos[i_candidato];
        if (nodo_en_solucion(solucion, node_candidato))
        {
            node_candidato.en_solucion = true;
        }
        double distancia_entre_nodo_nuevo_y_anterior = haversine(node_candidato.latitude, node_candidato.longitude, solucion[nivel_actual - 1].latitude, solucion[nivel_actual - 1].longitude);
        if (node_candidato.tipo == "c")
        {
            aux_time = tiempo_actual + service_time + (distancia_entre_nodo_nuevo_y_anterior / speed);
            aux_fuel = fuel_actual - distancia_entre_nodo_nuevo_y_anterior;
        }
        else
        {
            aux_time = tiempo_actual + refuel_time + (distancia_entre_nodo_nuevo_y_anterior / speed);
            aux_fuel = fuel_actual - distancia_entre_nodo_nuevo_y_anterior;
        }
        aux_dist = distancia_actual + distancia_entre_nodo_nuevo_y_anterior;

        if (aux_fuel > 0 && aux_time <= 4 * max_time / 6 && !node_candidato.en_solucion)
        {
            if (node_candidato.tipo == "f")
            {
                aux_fuel = max_distance;
            }
            tiempo_actual = aux_time;

            distancia_actual = distancia_entre_nodo_nuevo_y_anterior;
            fuel_actual = aux_fuel;
            node_candidato.en_solucion = true;
            solucion[nivel_actual] = node_candidato;
            vector_nodos[node_candidato.id_interno].en_solucion = true;
            gbj(vector_nodos, solucion, nivel_actual + 1, max_time, max_distance, speed, service_time, refuel_time, nivel_mayor_distancia, distancia_actual, tiempo_actual, fuel_actual, t0);
            t1 = std::chrono::high_resolution_clock::now();
            float_ms = t1 - t0;
            duracion_algoritmo = (float_ms.count()) / 1000;
            if (duracion_algoritmo == 300)
            {
                return;
            }
            vector_nodos[node_candidato.id_interno].en_solucion = false;
            if (nivel_actual > nivel_mayor_distancia)
            {
                return;
            }
        }
    }

    nivel_mayor_distancia = mayor(solucion);
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
    ifstream archivo_leido("instancias/" + archivo);
    string linea;

    int id_nodos = 0;
    getline(archivo_leido, linea);
    vector<string> linea_vec = string_to_vector(linea);
    string name = linea_vec[0];
    float num_customers = stof(linea_vec[1]);
    float num_afs = stof(linea_vec[2]);
    float max_time = stof(linea_vec[3]);
    float max_distance = stof(linea_vec[4]);
    float speed = stof(linea_vec[5]);
    float service_time = stof(linea_vec[6]);
    float refuel_time = stof(linea_vec[7]);
    getline(archivo_leido, linea);
    linea_vec = string_to_vector(linea);
    node node_depot;
    node_depot.id_interno = id_nodos;
    node_depot.id = stof(linea_vec[0]);
    node_depot.tipo = linea_vec[1];
    node_depot.longitude = stof(linea_vec[2]);
    node_depot.latitude = stof(linea_vec[3]);
    id_nodos++;
    vector<node> nodes;
    nodes.push_back(node_depot);
    int total_nodos = num_afs + num_customers;
    create_nodes(archivo_leido, total_nodos, id_nodos, nodes);

    vector<int> nodos_utilizados;
    bool flag_nodos_visitados = true;
    fstream file_out;
    file_out.open(name + ".out", std::ios_base::out);
    if (!file_out.is_open())
    {
        cout << "failed to open " << name + ".out" << '\n';
        return 0;
    }
    string ruta;
    double total_distancia_recorrida = 0;
    int suma_customers = 0, num_vehiculos = 0;
    vector<vector<node>> all_routes;
    auto start = std::chrono::high_resolution_clock::now();
    while (flag_nodos_visitados)
    {
        vector<node> route(total_nodos);
        double distancia_actual = 0;
        int tiempo_actual = 0;
        double fuel_actual = max_distance;
        bool busqueda_terminada = false;
        route[0] = nodes[0];

        int nivel_mayor_distancia = -1;
        auto t0 = std::chrono::high_resolution_clock::now();
        gbj(nodes, route, 1, max_time, max_distance, speed, service_time, refuel_time, nivel_mayor_distancia, distancia_actual, tiempo_actual, fuel_actual, t0);
        auto t1 = std::chrono::high_resolution_clock::now();
        route = cortar_nodos_inutiles(route);
        agregar_node_depot(route, nodes, speed, max_distance, service_time, refuel_time, max_time);
        if (route.back().id_interno != 0)
            route.push_back(nodes[0]);
        if (route.size() == 1)
        {
            break;
        }
        all_routes.push_back(route);
        if (!aun_quedan_customers_sin_visitar(route, nodes, total_nodos, nodos_utilizados))
        {
            flag_nodos_visitados = false;
        }
    }
    for (auto route : all_routes)
    {
        if (!useless_route(route))
        {
            ruta = PrintVec(route);
            file_out << ruta + "Distancia recorrida: " << distancia_total_ruta(route) << endl;
            total_distancia_recorrida = total_distancia_recorrida + distancia_total_ruta(route);
            sumar_customers(suma_customers, route);
            num_vehiculos++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> float_ms = end - start;
    double duracion_algoritmo = (float_ms.count()) / 1000;
    file_out << "Distancia total recorrida: " << total_distancia_recorrida << endl;
    file_out << "Total customers visitados: " << suma_customers << endl;
    file_out << "Total vehiculos utilizados: " << num_vehiculos << endl;
    file_out << "Tiempo de ejecucion: " << duracion_algoritmo << endl;
}
