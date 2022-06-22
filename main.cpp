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
bool aun_quedan_customers_sin_visitar(vector<node> &route, vector<node> &vector_nodos, int num_customers, vector<int> &nodos_visitados)
{
    bool flag = false;
    vector<node> aux;
    for (int i = 0; i < route.size(); i++)
    {
        if (!existeEnVector(nodos_visitados, route[i].id_interno))
        {
            if (route[i].tipo == "c")
            {
                nodos_visitados.push_back(route[i].id_interno);
            }
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

string PrintVec(const vector<node> &vec)
{
    string to_return = "", aux;
    for (const auto &item : vec)
    {

        // cout << item.id_interno << " " << item.id << " " << item.tipo << "|";
        aux = to_string(item.id_interno);
        to_return += (" - " + aux + item.tipo);
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

void gbj(vector<node> &vector_nodos, vector<node> &solucion, int nivel_actual, float max_time, float max_distance, float speed, float service_time, float refuel_time, unsigned t0, int &nivel_mayor_distancia, vector<double> &distancias_nodo_actual_anterior, vector<int> &tiempos_actuales, vector<double> &fueles_actuales)
{
    // PrintVec(vector_nodos);
    // printf("NIVEL ACTUAL %d - FUEL ACTUAL %f - TIEMPO ACTUAL %d\n", nivel_actual, fueles_actuales[nivel_actual], tiempos_actuales[nivel_actual]);

    int aux_time, aux_fuel, aux_dist, i_candidato;
    vector_nodos[0].en_solucion = true;

    // for (i_candidato = vector_nodos.size(); i_candidato > 0; i_candidato--)
    // {
    //     double distancia_entre_nodo_nuevo_y_anterior = haversine(vector_nodos[i_candidato].latitude, vector_nodos[i_candidato].longitude, solucion[nivel_actual - 1].latitude, solucion[nivel_actual - 1].longitude);
    //     // double distancia_entre_nodo_nuevo_y_depot = haversine(vector_nodos[i_candidato].latitude, vector_nodos[i_candidato].longitude, solucion[0].latitude, solucion[0].longitude);
    //     if (vector_nodos[i_candidato].tipo == "c")
    //     {
    //         aux_time = tiempos_actuales[nivel_actual - 1] + service_time + (distancia_entre_nodo_nuevo_y_anterior / speed);
    //         aux_fuel = fueles_actuales[nivel_actual - 1] - distancia_entre_nodo_nuevo_y_anterior;
    //     }
    //     else
    //     {
    //         aux_time = tiempos_actuales[nivel_actual - 1] + refuel_time + (distancia_entre_nodo_nuevo_y_anterior / speed);
    //         aux_fuel = max_distance;
    //     }
    //     aux_dist = distancias_nodo_actual_anterior[nivel_actual - 1] + distancia_entre_nodo_nuevo_y_anterior;

    //     if (aux_fuel > 0 && aux_time <= 4 * max_time / 5 && !vector_nodos[i_candidato].en_solucion)
    //     {
    //         tiempos_actuales[nivel_actual] = aux_time;

    //         distancias_nodo_actual_anterior[nivel_actual] = distancia_entre_nodo_nuevo_y_anterior;
    //         fueles_actuales[nivel_actual] = aux_fuel;
    //         vector_nodos[i_candidato].en_solucion = true;
    //         solucion[nivel_actual] = vector_nodos[i_candidato];
    //         vector_nodos[vector_nodos[i_candidato].id_interno].en_solucion = true;
    //         // printf("NIVEL %d - AGREGADO ID: %d - DISTANCIA ENTRE NODOS[ANTERIOR-%d][ACTUAL-%d]: %f - FUEL ACTUAL: %f - FUEL INICIAL: %f\n", nivel_actual, vector_nodos[i_candidato].id_interno, solucion[nivel_actual - 1].id_interno, solucion[nivel_actual].id_interno, distancias_nodo_actual_anterior[nivel_actual], fueles_actuales[nivel_actual], fueles_actuales[nivel_actual - 1]);
    //         // PrintVec(solucion);
    //         gbj(vector_nodos, solucion, nivel_actual + 1, max_time, max_distance, speed, service_time, refuel_time, t0, nivel_mayor_distancia, distancias_nodo_actual_anterior, tiempos_actuales, fueles_actuales);
    //         vector_nodos[i_candidato].en_solucion = false;

    //         vector_nodos[vector_nodos[i_candidato].id_interno].en_solucion = false;
    //         if (nivel_actual > nivel_mayor_distancia)
    //         {
    //             return;
    //         }
    //     }
    // }
    // nivel_mayor_distancia = mayor(distancias_nodo_actual_anterior);
    // return;
    for (auto &candidato : vector_nodos)
    {

        double distancia_entre_nodo_nuevo_y_anterior = haversine(candidato.latitude, candidato.longitude, solucion[nivel_actual - 1].latitude, solucion[nivel_actual - 1].longitude);
        // double distancia_entre_nodo_nuevo_y_depot = haversine(candidato.latitude, candidato.longitude, solucion[0].latitude, solucion[0].longitude);
        if (candidato.tipo == "c")
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

        if (aux_fuel > 0 && aux_time <= 4 * max_time / 5 && !candidato.en_solucion)
        {
            tiempos_actuales[nivel_actual] = aux_time;

            distancias_nodo_actual_anterior[nivel_actual] = distancia_entre_nodo_nuevo_y_anterior;
            fueles_actuales[nivel_actual] = aux_fuel;
            candidato.en_solucion = true;
            solucion[nivel_actual] = candidato;
            vector_nodos[candidato.id_interno].en_solucion = true;
            // printf("NIVEL %d - AGREGADO ID: %d - DISTANCIA ENTRE NODOS[ANTERIOR-%d][ACTUAL-%d]: %f - FUEL ACTUAL: %f - FUEL INICIAL: %f\n", nivel_actual, candidato.id_interno, solucion[nivel_actual - 1].id_interno, solucion[nivel_actual].id_interno, distancias_nodo_actual_anterior[nivel_actual], fueles_actuales[nivel_actual], fueles_actuales[nivel_actual - 1]);
            // PrintVec(solucion);
            gbj(vector_nodos, solucion, nivel_actual + 1, max_time, max_distance, speed, service_time, refuel_time, t0, nivel_mayor_distancia, distancias_nodo_actual_anterior, tiempos_actuales, fueles_actuales);
            if (nivel_actual > nivel_mayor_distancia)
            {
                candidato.en_solucion = false;
                vector_nodos[candidato.id_interno].en_solucion = false;
                return;
            }
            candidato.en_solucion = false;

            vector_nodos[candidato.id_interno].en_solucion = false;
        }
    }
    nivel_mayor_distancia = mayor(distancias_nodo_actual_anterior);
    return;
}

int main()
{

    // cout << "Ingrese su nombre y apellido\n";
    // cin >> archivo;
    string archivo = "AB101.dat";
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
    // PrintVec(nodes_afs);
    // cout << endl;
    // PrintVec(nodes_customer);
    // cout << endl;
    int total_nodos = num_afs - 1 + num_customers;
    cout << num_customers << endl;
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
    // for (size_t i = 0; i < total_nodos; i++)
    // {
    //     cout << "ID_INTERNO: " << vector_nodos[i].id_interno << " I: " << i << endl;
    // }

    cout << endl;
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
        t0 = clock();
        int nivel_mayor_distancia = -1;

        gbj(vector_nodos, route, 1, max_time, max_distance, speed, service_time, refuel_time, t0, nivel_mayor_distancia, distancias_nodo_actual_anterior, tiempos_actuales, fueles_actuales);
        // PrintVec(vector_nodos);
        route = cortar_nodos_inutiles(route);
        route.push_back(vector_nodos[0]);
        ruta = PrintVec(route);
        file_out << ruta << endl;
        t1 = clock();
        double time = (double(t1 - t0) / CLOCKS_PER_SEC);
        id++;
        // if (!aun_quedan_customers_sin_visitar(route, vector_nodos, num_customers, nodos_utilizados))
        if (!aun_quedan_customers_sin_visitar(route, vector_nodos, num_customers, nodos_utilizados))
        {
            flag_nodos_visitados = false;
        }

        // PrintVec(vector_nodos);
        // for (auto i : nodos_utilizados)
        // {
        //     cout << i << " - ";
        // }
        // cout << endl;
    }
}
