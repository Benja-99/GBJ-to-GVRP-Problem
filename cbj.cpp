#include <iostream>
#include <cstdio>
#include <cmath>
#include <list>
#include <iterator>
#include <iostream>
#include <ctime>

unsigned t0, t1;

using namespace std;

struct Nodo
{
    int nid;
    int Nnodo;
    char tipoNodo;
    double longitude;
    double latitude;
    bool incluido;
    double tiempoA;
};
const double radio = 4182.44949;
const double pi = 3.14159265358979323846264338327950288;
double Distancia_Haversine(double, double, double, double);
void busqueda(int[], double, int, Nodo *N, int, double, double, int, int, int, int, int, int, double, int[], int[], double, int, unsigned);

/*
Nombre funcion: Distancia_Haversine
Descripcion: funcion que calcula la ditancia de haversine entre 2 puntos o nodos del grafo
Input:
- longitude1: Input de tipo double el cual es la coordenada de longitud del nodo origen
- latitude1: Input de tipo double el cual es la coordenada de latitud del nodo origen
- longitude2: Input de tipo double el cual es la coordenada de longitud del nodo destino
- latitude2: Input de tipo double el cual es la coordenada de latitud del nodo destino
Output: Retorna un double que contiene la distancia de haversine entre 2 nodos
 */

double Distancia_Haversine(double longitude1, double latitude1, double longitude2, double latitude2)
{
    double c = pi / 180;
    double part1 = pow(sin(c * (latitude2 - latitude1) / 2), 2);
    double part2 = cos(c * latitude1) * cos(c * latitude2) * pow(sin(c * (longitude2 - longitude1) / 2), 2);
    double distancia = 2 * radio * asin(pow((part1 + part2), 0.5));
    return distancia;
}

/*
Nombre funcion: busqueda
Descripcion: funcion que realiza backtracking recursivo para encontrar una solucion al problema
Input:
- solucion[]: Es un arreglo de enteros que va construyendo la solucion
- calidad: Es una varible de tipo double que tiene la distancia acumulada hasta el ultimo elemento ingresado a la solucion
- nivel: Varible de tipo entera que tiene la posicion en el arreglo que se ingreso el ultimo elemento
- N: Arreglo de struct de Nodos, que almacenan la informacion de los nodos
- visitados: Entero que representa cuantos clientes han sido visitados
- tiempo: Varible de tipo doble que representa el tiempo acumulado hasta el ultimo elemento incorporado a la solucion
- combActual: Varible que contiene el combstible actual disponible por un automovil en especifico
- ncustomers: cantidad de clientes totales a visitar
- maxtime: tiempo maximo permitido por vehiculo
- maxdistance: la distancia maxima que es posible recorrer sin pasar a una estacion de reabastecimiento
- servicetime: varible de tipo double que representa el tiempo que tarda en atender un cliente
- refueltime: varible de tipo double que representa el tiempo que tarda rellenar un estanque
- speed: varible de tipo double que representa la velocidad del vehiculo
- msolucion[]: arreglo de enteros que tiene la mejor solucion alcanzada
- mnivel: varialbe entera que representa el nivel en el que se ingreso la mayor distancia
- mdistancia: varible de tipo double que representa la mayor distancia que ha sido incorporada a la ruta
- ciclo: varible entera que contiene el numero de vehiculos utilizados
Output:
La funcion no tiene retorno, se trabaja con las direcciones de memoria
 */

void busqueda(int solucion[], double calidad, int nivel, Nodo *N, int visitados, double tiempo, double combActual, int ncustomers, int nstations, int maxtime, int maxdistance, int servicetime, int refueltime, double speed, int msolucion[], int mnivel, double mdistancia, int ciclo, unsigned t0)
{
    double p, d;
    int candidato, x, n;
    double distancia = 0.0, distancia2 = 0.0, t;
    bool flag;

    t1 = clock();
    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    if (time > 300)
        return;

    if (visitados == ncustomers)
    {
        if (calidad < msolucion[0])
        {
            msolucion[0] = calidad;
            msolucion[nivel] = 0;
            for (int x = 0; x < (ncustomers + nstations) * 3 - 2; x++)
            {
                msolucion[x + 1] = solucion[x];
            }
        }
        return;
    }
    else
    {
        for (candidato = ncustomers + nstations; candidato >= 1; candidato--)
        { // explorando ramas
            distancia = Distancia_Haversine(N[solucion[nivel - 1]].longitude, N[solucion[nivel - 1]].latitude, N[candidato].longitude, N[candidato].latitude);
            distancia2 = Distancia_Haversine(N[candidato].longitude, N[candidato].latitude, N[0].longitude, N[0].latitude);
            printf("distancia %f\n", distancia2);
            if (N[candidato].tipoNodo == 'c')
                p = tiempo + distancia / speed + servicetime + distancia2 / speed;
            else
                p = tiempo + distancia / speed + refueltime + distancia2 / speed;
            d = combActual - distancia;
            if (p < maxtime && N[candidato].incluido == false && d >= 0)
            {
                solucion[nivel] = candidato;
                if (N[candidato].tipoNodo == 'c')
                    visitados++;
                else if (N[candidato].tipoNodo == 'f')
                    d = maxdistance;
                N[candidato].incluido = true;

                if (distancia > mdistancia)
                {
                    busqueda(solucion, calidad + distancia, nivel + 1, N, visitados, tiempo + distancia / speed, d, ncustomers, nstations, maxtime, maxdistance, servicetime, refueltime, speed, msolucion, nivel, distancia, ciclo, t0);
                }
                else
                    busqueda(solucion, calidad + distancia, nivel + 1, N, visitados, tiempo + distancia / speed, d, ncustomers, nstations, maxtime, maxdistance, servicetime, refueltime, speed, msolucion, mnivel, mdistancia, ciclo, t0);
                cout << "nivel : " << nivel << " mnivel : " << mnivel << "\n";
                for (int x = 0; x < (ncustomers + nstations) * 3; x++)
                {
                    cout << solucion[x] << " ";
                }
                cout << "\n";
                if (nivel != mnivel)
                {
                    N[solucion[nivel]].incluido = false;
                    solucion[nivel] = -1;
                    return;
                }
                if (N[candidato].tipoNodo == 'c')
                    visitados--;
                N[candidato].incluido = false;
            }
        }
        if (solucion[nivel - 1] != 0)
        {
            solucion[nivel] = 0;
            busqueda(solucion, calidad, nivel + 1, N, visitados, 0.0, maxdistance, ncustomers, nstations, maxtime, maxdistance, servicetime, refueltime, speed, msolucion, nivel + 1, 0.0, ciclo + 1, t0);
            solucion[nivel] = -1;
            return;
        }
        else
        {
            solucion[nivel] = -1;
            return;
        }
    }
}

/*
nombre: main
Descripcion: funcion principal, se leen los archivos y se almacena su contenido en varibles, luego se llama a la funcion
buscar para encontrar una solucion al problema, para finalmente construir el archivo que contiene la solucion al problema
-Input por consola: El nombre del archivo a ejecutar
-Output: No tiene output, pero genera el archivo solucion.txt el cual contiene la solucion al problema.
 */

int main()
{
    FILE *fp;
    char nombre[50];
    printf("Introduzca nombre del archivo (con .dat): ");
    scanf("%s", nombre);
    fp = fopen(nombre, "r");

    char name[10], nodetype;
    int ncustomers, nid, nstations, maxtime, maxdistance, servicetime, refueltime;
    double speed, longitude, latitude;

    int Did;
    double Dlongitude, Dlatitude;
    fscanf(fp, "%s %d %d %d %d %lf %d %d", name, &ncustomers, &nstations, &maxtime, &maxdistance, &speed, &servicetime, &refueltime);

    Nodo *nodos = new Nodo[ncustomers + nstations + 1];

    fscanf(fp, "%d %c %lf %lf", &nodos[0].nid, &nodos[0].tipoNodo, &nodos[0].longitude, &nodos[0].latitude);
    int i;
    int contador = 1;
    for (i = 0; i < nstations + ncustomers; i++)
    {
        fscanf(fp, "%d %c %lf %lf", &nodos[contador].nid, &nodos[contador].tipoNodo, &nodos[contador].longitude, &nodos[contador].latitude);
        nodos[i].incluido = false;
        contador++;
    }

    int sol[(ncustomers + nstations) * 3];
    int msol[(ncustomers + nstations) * 3];
    int mnivel;

    for (i = 0; i < (ncustomers + nstations) * 3; i++)
    {
        sol[i] = -1;
        msol[i] = -1;
        // mnivel[i] = -1;
    }
    msol[0] = 99999;

    int candidato = 0;
    int nivel = 1;
    int rama = 1;
    sol[0] = 0;

    t0 = clock();

    bool altura = true;
    busqueda(sol, 0.0, nivel, nodos, 0, 0.0, maxdistance, ncustomers, nstations, maxtime, maxdistance, servicetime, refueltime, speed, msol, 1, -1.0, 0, t0);
    t1 = clock();
    double time = (double(t1 - t0) / CLOCKS_PER_SEC);

    candidato = 1;
    while (msol[candidato] != -1)
    {
        candidato++;
    }
    candidato = 1;
    while (sol[candidato] != -1)
    {
        candidato++;
    }
    fclose(fp);

    fp = fopen("solution.dat", "w");
    bool f = true;
    double d = 0.0;
    double t = 0.0;
    int cantidad = 0, c = 0;
    while (msol[candidato] != -1)
    {
        cout << " " << msol[candidato] << " ";
        if (f == true && msol[candidato] == 0)
        {
            fprintf(fp, "%c%d-", nodos[msol[candidato]].tipoNodo, nodos[msol[candidato]].nid);
            f = false;
            cantidad++;
        }
        else if (f == false && msol[candidato] == 0)
        {
            fprintf(fp, "%c%d", nodos[msol[candidato]].tipoNodo, nodos[msol[candidato]].nid);

            fprintf(fp, " %lf %lf %lf \n", d, t, 0.0);
            d = 0.0;
            t = 0.0;
            fprintf(fp, "%c%d-", nodos[msol[candidato]].tipoNodo, nodos[msol[candidato]].nid);
            cantidad++;
        }
        else
        {
            fprintf(fp, "%c%d-", nodos[msol[candidato]].tipoNodo, nodos[msol[candidato]].nid);
            d = d + Distancia_Haversine(nodos[msol[candidato - 1]].longitude, nodos[msol[candidato - 1]].latitude, nodos[msol[candidato]].longitude, nodos[msol[candidato]].latitude);
            if (nodos[msol[candidato]].tipoNodo == 'c')
                t = t + Distancia_Haversine(nodos[msol[candidato - 1]].longitude, nodos[msol[candidato - 1]].latitude, nodos[msol[candidato]].longitude, nodos[msol[candidato]].latitude) / speed + servicetime;
            else if (nodos[msol[candidato]].tipoNodo == 'f')
                t = t + Distancia_Haversine(nodos[msol[candidato - 1]].longitude, nodos[msol[candidato - 1]].latitude, nodos[msol[candidato]].longitude, nodos[msol[candidato]].latitude) / speed + refueltime;
            else
                t = t + Distancia_Haversine(nodos[msol[candidato - 1]].longitude, nodos[msol[candidato - 1]].latitude, nodos[msol[candidato]].longitude, nodos[msol[candidato]].latitude) / speed;
            // printf("Distancia :  %lf Tiempo : %lf",d,t);
        }
        if (nodos[msol[candidato]].tipoNodo == 'c')
            c++;
        candidato++;
    }
    // cout << "\n";
    fprintf(fp, "%c%d", nodos[0].tipoNodo, nodos[0].nid);
    fprintf(fp, " %lf %lf %lf \n", d, t, 0.0);
    // rewind(fp);
    fprintf(fp, "%d %d %d %lf ", msol[0], c, cantidad, time);
    fclose(fp);
    return 0;
}