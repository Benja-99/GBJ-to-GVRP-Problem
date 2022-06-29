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
#include <time.h>
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
bool existeEnVector(vector<int> v, int busqueda)
{
    return find(v.begin(), v.end(), busqueda) != v.end();
}
bool aun_quedan_customers_sin_visitar(vector<int> &route, vector<node> &vector_nodos, int num_customers, vector<int> &nodos_visitados)
{
    bool flag = false;
    vector<node> aux;
    for (int i = 0; i < route.size(); i++)
    {
        if (!existeEnVector(nodos_visitados, route[i]))
        {
            // if (route[i].tipo == "c")
            // {
            nodos_visitados.push_back(route[i]);
            // }
        }
    }
    for (auto i_nodo : nodos_visitados)
    {

        vector_nodos[i_nodo].en_solucion = true;
        // cout << i_nodo << " - " << vector_nodos[i_nodo].id_interno << " | ";
    }
    // cout << endl;
    cout << "CANTIDAD CUSTOMERS VISITADOS: " << nodos_visitados.size() << "CANTIDAD DE CUSTOMERS: " << num_customers << endl;
    if (nodos_visitados.size() < num_customers)
    {
        return true;
    }
    return false;
}

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

string PrintVec(const vector<int> &vec, vector<node> nodes)
{
    string to_return = "", aux;
    for (const auto &item : vec)
    {

        // cout << item.id_interno << " " << item.id << " " << item.tipo << "|";
        aux = to_string(item);
        to_return += (" - " + aux + nodes[item].tipo);
    }
    // for (size_t i = 0; i < vec.size(); i++)
    // {
    //     cout << vec[i].id_interno << " " << vec[i].id << " " << i
    //          << " " << vec[i].en_solucion << "|";
    // }

    // cout << endl;
    return to_return;
}

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

void reiniciar_nodes_afs(vector<node> &nodes_afs)
{
    for (auto &nodo : nodes_afs)
    {
        if (nodo.tipo == "f")
        {
            nodo.en_solucion = false;
        }
    }
}

// vector<node> create_nodes(ifstream &archivo, int num, int inicio_id_interno)
// {
//     string linea;
//     vector<string> linea_aux;
//     vector<node> vector_aux;
//     for (size_t i = 0; i < num; i++)
//     {
//         getline(archivo, linea);
//         linea_aux = string_to_vector(linea);
//         node nodo_aux;
//         nodo_aux.id_interno = inicio_id_interno;
//         inicio_id_interno++;
//         nodo_aux.id = stof(linea_aux[0]);
//         nodo_aux.tipo = linea_aux[1];
//         nodo_aux.longitude = stof(linea_aux[2]);
//         nodo_aux.latitude = stof(linea_aux[3]);
//         vector_aux.push_back(nodo_aux);
//     }
//     return vector_aux;
// }
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

void gbj(vector<node> &vector_nodos, vector<int> &solucion, int nivel_actual, float max_time, float max_distance, float speed, float service_time, float refuel_time, unsigned t0, int &nivel_mayor_distancia, vector<double> &distancias_nodo_actual_anterior, vector<int> &tiempos_actuales, vector<double> &fueles_actuales)
{
    // PrintVec(vector_nodos);
    printf("NIVEL ACTUAL %d - FUEL ACTUAL %f - TIEMPO ACTUAL %d\n", nivel_actual, fueles_actuales[nivel_actual - 1], tiempos_actuales[nivel_actual - 1]);

    int aux_time, aux_fuel, aux_dist, i_candidato;
    vector_nodos[0].en_solucion = true;

    for (i_candidato = vector_nodos.size(); i_candidato > 0; i_candidato--)
    {
        double distancia_entre_nodo_nuevo_y_anterior = haversine(vector_nodos[i_candidato].latitude, vector_nodos[i_candidato].longitude, vector_nodos[solucion[nivel_actual - 1]].latitude, vector_nodos[solucion[nivel_actual - 1]].longitude);
        // double distancia_entre_nodo_nuevo_y_depot = haversine(vector_nodos[i_candidato].latitude, vector_nodos[i_candidato].longitude, solucion[0].latitude, solucion[0].longitude);
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
            solucion[nivel_actual] = i_candidato;
            vector_nodos[i_candidato].en_solucion = true;
            printf("NIVEL %d - AGREGADO ID: %d - DISTANCIA ENTRE NODOS[ANTERIOR-%d][ACTUAL-%d]: %f - FUEL ACTUAL: %f - FUEL INICIAL: %f - CANTIDAD ELEMENTOS SOLUCION %ld\n", nivel_actual, vector_nodos[i_candidato].id_interno, vector_nodos[solucion[nivel_actual - 1]].id_interno, vector_nodos[solucion[nivel_actual]].id_interno, distancias_nodo_actual_anterior[nivel_actual], fueles_actuales[nivel_actual], fueles_actuales[nivel_actual - 1], solucion.size());
            gbj(vector_nodos, solucion, nivel_actual + 1, max_time, max_distance, speed, service_time, refuel_time, t0, nivel_mayor_distancia, distancias_nodo_actual_anterior, tiempos_actuales, fueles_actuales);
            // PrintVec(solucion);
            printf("NIVEL %d - ELIMINADO ID: %d - DISTANCIA ENTRE NODOS[ANTERIOR-%d][ACTUAL-%d]: %f - FUEL ACTUAL: %f - FUEL INICIAL: %f\n", nivel_actual, vector_nodos[i_candidato].id_interno, vector_nodos[solucion[nivel_actual - 1]].id_interno, vector_nodos[solucion[nivel_actual]].id_interno, distancias_nodo_actual_anterior[nivel_actual], fueles_actuales[nivel_actual], fueles_actuales[nivel_actual - 1]);
            vector_nodos[i_candidato].en_solucion = false;
            solucion[nivel_actual] = -1;
            vector_nodos[vector_nodos[i_candidato].id_interno].en_solucion = false;
            if (nivel_actual > nivel_mayor_distancia)
            {
                return;
            }
        }
    }
    nivel_mayor_distancia = mayor(distancias_nodo_actual_anterior);
    printf("NIVEL %d SIN NODOS - NIVEL MAYOR DISTANCIA %d\n", nivel_actual, nivel_mayor_distancia);
    return;
}

int main()
{
    // string archivo;
    // cout << "Ingrese nombre del archivo (con .dat)\n";
    // cin >> archivo;
    string archivo = "AB201.dat";
    ifstream archivo_leido(archivo);
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

    // vector<int> nodos_utilizados;
    // bool flag_nodos_visitados = true;
    // int id = 1;
    // fstream file_out;
    // file_out.open(name + ".out", std::ios_base::out);
    // if (!file_out.is_open())
    // {
    //     cout << "failed to open " << name + ".out" << '\n';
    //     return 0;
    // }
    // string ruta;

    // while (flag_nodos_visitados)
    // {
    // vector<int> route(total_nodos);
    // vector<double> distancias_nodo_actual_anterior(total_nodos);
    // vector<int> tiempos_actuales(total_nodos);
    // vector<double> fueles_actuales(total_nodos);
    // route[0] = 0;
    // distancias_nodo_actual_anterior[0] = 0;
    // tiempos_actuales[0] = 0;
    // fueles_actuales[0] = max_distance;
    // int nivel_mayor_distancia = 10000000;

    // gbj(nodes, route, 1, max_time, max_distance, speed, service_time, refuel_time, t0, nivel_mayor_distancia, distancias_nodo_actual_anterior, tiempos_actuales, fueles_actuales);
    // cout << "------------------------------------------------------------------------------------------\n";
    // // PrintVec(vector_nodos);
    // // route = cortar_nodos_inutiles(route);
    // route.push_back(0);
    // ruta = PrintVec(route, vector_nodos);
    // file_out << ruta << endl;
    // t1 = clock();
    // double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    // id++;
    // if (!aun_quedan_customers_sin_visitar(route, vector_nodos, total_nodos - 1, nodos_utilizados))
    // // if (!aun_quedan_customers_sin_visitar(route, vector_nodos, total_nodos, nodos_utilizados))
    // {
    //     flag_nodos_visitados = false;
    // }

    // PrintVec(vector_nodos);
    // for (auto i : nodos_utilizados)
    // {
    //     cout << i << " - ";
    // }
    // cout << endl;
    // }
}
