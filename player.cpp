#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar
/** \brief OneOutFour es una función que para cada casilla vacía mira las de alrededor y
 *   añade un punto a cada jugador por cada ficha del jugador
 *
 * \param tablero El estado actual del tablero a evaluar
 * \param jugador Jugador activo
 * \param pjugador (Por referencia) puntuación del jugador activo
 * \param poponente (Por referencia) puntuación del jugador oponente
 * \return
 *
 */

void oneoutfour(const Environment &tablero, int jugador, double &pjugador, double &poponente)
{
    for(int i=0; i<7; i++)
    {
        for(int j=0; j<7; j++)
        {
            if(tablero.See_Casilla(i, j)==0)
            {
                if(i>0 && j>0)
                {
                    if(tablero.See_Casilla(i-1, j-1)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i-1, j-1)==0)
                        true;
                    else
                        poponente++;
                }
                if(i>0)
                {
                     if(tablero.See_Casilla(i-1, j)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i-1, j)==0)
                        true;
                    else
                        poponente++;
                }
                if(i>0 && j<6)
                {
                    if(tablero.See_Casilla(i-1, j+1)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i-1, j+1)==0)
                        true;
                    else
                        poponente++;
                }
                if(i<6 && j<6)
                {
                     if(tablero.See_Casilla(i+1, j+1)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i+1, j+1)==0)
                        true;
                    else
                        poponente++;
                }
                if(i<6)
                {
                     if(tablero.See_Casilla(i+1, j)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i+1, j)==0)
                        true;
                    else
                        poponente++;
                }
                if(i<6 && j>0)
                {
                     if(tablero.See_Casilla(i+1, j-1)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i+1, j-1)==0)
                        true;
                    else
                        poponente++;
                }
                if(j>0)
                {
                     if(tablero.See_Casilla(i, j-1)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i, j-1)==0)
                        true;
                    else
                        poponente++;
                }
                if(j<6)
                {
                     if(tablero.See_Casilla(i, j+1)==jugador)
                        pjugador++;
                    else if(tablero.See_Casilla(i, j+1)==0)
                        true;
                    else
                        poponente++;
                }
            }
        }
    }

}
/** \brief TwoOutFour es una función que para cada casilla vacía mira las lineas de 2 que hay alrededor y
 *   añade un valor wplayer o wopo a cada jugador por cada ficha del jugador
 *
 * \param tablero El estado actual del tablero a evaluar
 * \param jugador Jugador activo
 * \param pjugador (Por referencia) puntuación del jugador activo
 * \param poponente (Por referencia) puntuación del jugador oponente
 * \return
 *
 */
void twooutfour(const Environment &tablero, int jugador, double &pjugador, double &poponente)
{
    int wplayer = 10, wopo = 5;
    int oponente;
    if(jugador == 1)
        oponente = 2;
    else
        oponente = 1;

    for(int i=0; i<7; i++)
    {
        for(int j=0; j<7; j++)
        {
            if(tablero.See_Casilla(i, j)==0)
            {
                if(i>1 && j>1)
                {
                    if(tablero.See_Casilla(i-1, j-1)==jugador && tablero.See_Casilla(i-2, j-2)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j-1)==oponente && tablero.See_Casilla(i-2, j-2)==oponente)
                        poponente += wopo;

                }
                if(i>1 && j>1 && i<6 && j<6)
                {
                    if(tablero.See_Casilla(i-1, j-1)==jugador && tablero.See_Casilla(i+1, j+1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j-1)==oponente && tablero.See_Casilla(i+1, j+1)==oponente)
                        poponente += wopo;
                    if(tablero.See_Casilla(i-1, j+1)==jugador && tablero.See_Casilla(i+1, j-1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j+1)==oponente && tablero.See_Casilla(i+1, j-1)==oponente)
                        poponente += wopo;
                    if(tablero.See_Casilla(i+1, j-1)==jugador && tablero.See_Casilla(i-1, j+1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j-1)==oponente && tablero.See_Casilla(i-1, j+1)==oponente)
                        poponente += wopo;
                    if(tablero.See_Casilla(i+1, j+1)==jugador && tablero.See_Casilla(i-1, j-1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j+1)==oponente && tablero.See_Casilla(i-1, j-1)==oponente)
                        poponente += wopo;
                }
                if(i>1 && j<5)
                {
                    if(tablero.See_Casilla(i-1, j+1)==jugador && tablero.See_Casilla(i-2, j+2)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j+1)==oponente && tablero.See_Casilla(i-2, j+2)==oponente)
                        poponente += wopo;

                }
                if(i<5 && j>1)
                {
                    if(tablero.See_Casilla(i+1, j-1)==jugador && tablero.See_Casilla(i+2, j-2)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j-1)==oponente && tablero.See_Casilla(i+2, j-2)==oponente)
                        poponente += wopo;
                }
                if(i<5 && j<5)
                {
                    if(tablero.See_Casilla(i+1, j+1)==jugador && tablero.See_Casilla(i+2, j+2)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j+1)==oponente && tablero.See_Casilla(i+2, j+2)==oponente)
                        poponente += wopo;
                }
                if(j>1)
                {
                    if(tablero.See_Casilla(i, j-1)==jugador && tablero.See_Casilla(i, j-2)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j-1)==oponente && tablero.See_Casilla(i, j-2)==oponente)
                        poponente += wopo;
                }
                if(j>1 && j<6)
                {
                    if(tablero.See_Casilla(i, j-1)==jugador && tablero.See_Casilla(i, j+1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j-1)==oponente && tablero.See_Casilla(i, j+1)==oponente)
                        poponente += wopo;
                    if(tablero.See_Casilla(i, j+1)==jugador && tablero.See_Casilla(i, j-1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j+1)==oponente && tablero.See_Casilla(i, j-1)==oponente)
                        poponente += wopo;
                }
                if(i>1)
                {
                    if(tablero.See_Casilla(i-1, j)==jugador && tablero.See_Casilla(i-2, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j)==oponente && tablero.See_Casilla(i-2, j)==oponente)
                        poponente += wopo;

                }
                if(i>1 && i<6)
                {
                    if(tablero.See_Casilla(i-1, j)==jugador && tablero.See_Casilla(i+1, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j)==oponente && tablero.See_Casilla(i+1, j)==oponente)
                        poponente += wopo;
                    if(tablero.See_Casilla(i+1, j)==jugador && tablero.See_Casilla(i-1, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j)==oponente && tablero.See_Casilla(i-1, j)==oponente)
                        poponente += wopo;
                }
                if(j<5)
                {
                    if(tablero.See_Casilla(i, j+1)==jugador && tablero.See_Casilla(i, j+2)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j+1)==oponente && tablero.See_Casilla(i, j+2)==oponente)
                        poponente += wopo;
                }
                if(i<5)
                {
                    if(tablero.See_Casilla(i+1, j)==jugador && tablero.See_Casilla(i+2, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j)==oponente && tablero.See_Casilla(i+2, j)==oponente)
                        poponente += wopo;
                }
            }
        }
    }
}
/** \brief TwoOutFour es una función que para cada casilla vacía mira las lineas de 3 que hay alrededor y
 *   añade un valor wplayer o wopo a cada jugador por cada ficha del jugador
 *
 * \param tablero El estado actual del tablero a evaluar
 * \param jugador Jugador activo
 * \param pjugador (Por referencia) puntuación del jugador activo
 * \param poponente (Por referencia) puntuación del jugador oponente
 * \return
 *
 */
void threeoutfour(const Environment &tablero, int jugador, double &pjugador, double &poponente)
{
    int wplayer = 100, wopo = 50;
    int oponente;
    if(jugador == 1)
        oponente = 2;
    else
        oponente = 1;

    for(int i=0; i<7; i++)
    {
        for(int j=0; j<7; j++)
        {
            if(tablero.See_Casilla(i, j)==0)
            {
                if(i>2 && j>2)
                {
                    if(tablero.See_Casilla(i-1, j-1)==jugador && tablero.See_Casilla(i-2, j-2)==jugador && tablero.See_Casilla(i-3, j-3)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j-1)==oponente && tablero.See_Casilla(i-2, j-2)==oponente && tablero.See_Casilla(i-3, j-3)==oponente)
                        poponente += wopo;
                }
                if(i>1 && j>1 && i<6 && j<6)
                {
                    if(tablero.See_Casilla(i-1, j-1)==jugador && tablero.See_Casilla(i-2, j-2)==jugador && tablero.See_Casilla(i+1, j+1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j-1)==oponente && tablero.See_Casilla(i-2, j-2)==oponente && tablero.See_Casilla(i+1, j+1)==oponente)
                        poponente += wopo;
                }
                if(i>2 && j<4)
                {
                    if(tablero.See_Casilla(i-1, j+1)==jugador && tablero.See_Casilla(i-2, j+2)==jugador && tablero.See_Casilla(i-3, j+3)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j+1)==oponente && tablero.See_Casilla(i-2, j+2)==oponente && tablero.See_Casilla(i-3, j+3)==oponente)
                        poponente += wopo;
                }
                if(i>1 && j<5 && j>1 && i<6)
                {
                    if(tablero.See_Casilla(i-1, j+1)==jugador && tablero.See_Casilla(i-2, j+2)==jugador && tablero.See_Casilla(i+1, j-1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j+1)==oponente && tablero.See_Casilla(i-2, j+2)==oponente && tablero.See_Casilla(i+1, j-1)==oponente)
                        poponente += wopo;
                }
                if(i<4 && j>2)
                {
                    if(tablero.See_Casilla(i+1, j-1)==jugador && tablero.See_Casilla(i+2, j-2)==jugador && tablero.See_Casilla(i+3, j-3)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j-1)==oponente && tablero.See_Casilla(i+2, j-2)==oponente && tablero.See_Casilla(i+3, j-3)==oponente)
                        poponente += wopo;
                }
                if(i<5 && j>1 && j<6 && i>1)
                {
                    if(tablero.See_Casilla(i+1, j-1)==jugador && tablero.See_Casilla(i+2, j-2)==jugador && tablero.See_Casilla(i-1, j+1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j-1)==oponente && tablero.See_Casilla(i+2, j-2)==oponente && tablero.See_Casilla(i-1, j+1)==oponente)
                        poponente += wopo;
                }
                if(i<4 && j<4)
                {
                    if(tablero.See_Casilla(i+1, j+1)==jugador && tablero.See_Casilla(i+2, j+2)==jugador && tablero.See_Casilla(i+3, j+3)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j+1)==oponente && tablero.See_Casilla(i+2, j+2)==oponente && tablero.See_Casilla(i+3, j+3)==oponente)
                        poponente += wopo;
                }
                if(i<5 && j<5 && j>1 && i>1)
                {
                    if(tablero.See_Casilla(i+1, j+1)==jugador && tablero.See_Casilla(i+2, j+2)==jugador && tablero.See_Casilla(i-1, j-1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j+1)==oponente && tablero.See_Casilla(i+2, j+2)==oponente && tablero.See_Casilla(i-1, j-1)==oponente)
                        poponente += wopo;
                }
                if(j>2)
                {
                    if(tablero.See_Casilla(i, j-1)==jugador && tablero.See_Casilla(i, j-2)==jugador && tablero.See_Casilla(i, j-3)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j-1)==oponente && tablero.See_Casilla(i, j-2)==oponente && tablero.See_Casilla(i, j-3)==oponente)
                        poponente += wopo;
                }
                if(j>1 && j<6)
                {
                    if(tablero.See_Casilla(i, j-1)==jugador && tablero.See_Casilla(i, j-2)==jugador && tablero.See_Casilla(i, j+1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j-1)==oponente && tablero.See_Casilla(i, j-2)==oponente && tablero.See_Casilla(i, j+1)==oponente)
                        poponente += wopo;
                }
                if(i>2)
                {
                    if(tablero.See_Casilla(i-1, j)==jugador && tablero.See_Casilla(i-2, j)==jugador && tablero.See_Casilla(i-3, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j)==oponente && tablero.See_Casilla(i-2, j)==oponente && tablero.See_Casilla(i-3, j)==oponente)
                        poponente += wopo;
                }
                if(i>1 && i<6)
                {
                    if(tablero.See_Casilla(i-1, j)==jugador && tablero.See_Casilla(i-2, j)==jugador && tablero.See_Casilla(i+1, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i-1, j)==oponente && tablero.See_Casilla(i-2, j)==oponente && tablero.See_Casilla(i+1, j)==oponente)
                        poponente += wopo;
                }
                if(j<4)
                {
                    if(tablero.See_Casilla(i, j+1)==jugador && tablero.See_Casilla(i, j+2)==jugador && tablero.See_Casilla(i, j+3)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j+1)==oponente && tablero.See_Casilla(i, j+2)==oponente && tablero.See_Casilla(i, j+3)==oponente)
                        poponente += wopo;
                }
                if(i<4)
                {
                    if(tablero.See_Casilla(i+1, j)==jugador && tablero.See_Casilla(i+2, j)==jugador && tablero.See_Casilla(i+3, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j)==oponente && tablero.See_Casilla(i+2, j)==oponente && tablero.See_Casilla(i+3, j)==oponente)
                        poponente += wopo;
                }
                if(j<5 && j>1)
                {
                    if(tablero.See_Casilla(i, j+1)==jugador && tablero.See_Casilla(i, j+2)==jugador && tablero.See_Casilla(i, j-1)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i, j+1)==oponente && tablero.See_Casilla(i, j+2)==oponente && tablero.See_Casilla(i, j-1)==oponente)
                        poponente += wopo;
                }
                if(i<5 && i>1)
                {
                    if(tablero.See_Casilla(i+1, j)==jugador && tablero.See_Casilla(i+2, j)==jugador && tablero.See_Casilla(i-1, j)==jugador)
                        pjugador += wplayer;
                    if(tablero.See_Casilla(i+1, j)==oponente && tablero.See_Casilla(i+2, j)==oponente && tablero.See_Casilla(i-1, j)==oponente)
                        poponente += wopo;
                }
            }
        }
    }
}

double PuntuacionPropia(int jugador, const Environment &tablero){
    double puntuacion=0, pjugador=0, poponente=0;
    oneoutfour(tablero, jugador, pjugador, poponente);
    twooutfour(tablero, jugador, pjugador, poponente);
    threeoutfour(tablero, jugador, pjugador, poponente);
    puntuacion = pjugador - poponente;
    return puntuacion;
}



// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
/** \brief Función Valoración implementa la heurística que va a definir el valor de un tablero en función del jugador que juega
 *
 * \param estado Tablero a valorar
 * \param jugador entero con el jugador que está jugando
 * \return double con la valoración del tablero
 */

double Valoracion(const Environment &estado, int jugador)
{
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
        return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
        return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
        return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
        return PuntuacionPropia(jugador,estado);
}

/** \brief Función Poda_AlfaBeta implementa el algoritmo Minimax con la poda alfa-beta. Se ha implementado recursivamente
 * para que vaya recoriendo el arbol en profundidad.
 *
 * \param actual_ Nodo raiz desde donde vamos a desarrollar el algoritmo
 * \param jugador_ Jugador a Maximizar
 * \param profundidad Profundidad maxima del algoritmo
 * \param accion Acción que recomienda la poda alfa beta respecto a la estrategia
 * \param alpha Valor de la mejor opción hasta el momento a lo largo del camino para MAX
 * \param beta Valor de la mejor opción hasta el momento a lo largo del camino para MIN
 * \return Devuelve un doble con el valor que va generando necesario al ser una función recursiva
 *
 */
 static int contador=0;
double Poda_AlfaBeta(const Environment &actual_, int jugador_, int profundidad, int proflimite, Environment::ActionType &accion, double alpha, double beta)
{
    Environment nextEnv;
    double aux;
    int act = -1;
    int i, ganador = actual_.RevisarTablero();
    if(profundidad == proflimite ||  ganador!=0) //Si la profundidad es 0 se devuelve la ValoracionTest
    {
        contador++;
        return Valoracion(actual_, jugador_);
    }
    else
    {
        nextEnv = actual_.GenerateNextMove(act);
        while (!(nextEnv==actual_))
        {
            aux = Poda_AlfaBeta(nextEnv, jugador_, profundidad+1, proflimite, accion, alpha, beta);
            if(profundidad%2==0)
            {
                if(aux>alpha)
                {
                    alpha = aux;
                    if(profundidad==0)
                        accion = static_cast<Environment::ActionType>(act);
                }
                if(beta<=alpha)
                {
                    break;
                }
            }
            else
            {
                if(aux<beta)
                {
                    beta = aux;
                }
                if(beta<=alpha)
                {
                    break;
                }
            }
            nextEnv= actual_.GenerateNextMove(act);
            if(profundidad==0)
                cout << "Para el movimiento " << act-1 << " Valor MinMax = "<< aux <<" Nº iteraciones = " << contador << endl;
        }
        if (profundidad%2==0)
            return alpha;
        else
            return beta;
    }
}






// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<7; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[7]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<7; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    /*//--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //--------------------- COMENTAR Hasta aqui*/


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    //cout << "Estado con valor = " << PuntuacionPropia(jugador_, actual_) << endl;
    valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, menosinf, masinf);
    contador=0;
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}

