//Simulador de carrera de autos (Multihebra)
#include <iostream>                 //Libreria para entrada y salida
#include <thread>                   //Libreria para hilos
#include <vector>                   //Libreria para vector
#include <random>                   //Libreria para random_device
#include <chrono>                   //Libreria para chrono
#include <cstdlib>                  //Libreria para atoi
#include <mutex>                    //Libreria para mutex
#include <condition_variable>       //Libreria para variables de condicion
#include <algorithm>                //Libreria para la funcion shuffle

std::mutex mtx;                                 //Mutex para proteger las variables compartidas
std::condition_variable cv;                     //Variable de condicion para sincronizar los hilos
std::vector<bool> autosTerminados;              //Vector que indica si un auto ya termino la carrera
std::vector<int> velocidades;                   //Vector que almacena la velocidad de cada auto
std::vector<int> ordenTurnos;                   //Vector que almacena el orden de los turnos
std::vector<std::pair<int, int>> ordenLlegada;  //Vector que almacena el orden de llegada de los autos
int autosMovidos = 0;                           //Variable que indica el numero de autos que ya se movieron
int lugarActual = 1;                            //Variable que indica el lugar actual de los autos

//Funcion que simula el movimiento de un auto
void moverAuto(int idAuto, int distanciaTotal, int numeroAutos) {
    //Inicializar la semilla para los numeros aleatorios
    std::srand(std::time(nullptr) + idAuto);

    //Distancia recorrida por el auto
    int distanciaRecorrida = 0;

    // Mientras el auto no haya llegado a la meta
    while(distanciaRecorrida < distanciaTotal) { 
        {
            //Bloquear el acceso a la variable autosMovidos
            std::unique_lock<std::mutex> lock(mtx);
            // Esperar a que sea el turno del auto
            cv.wait(lock, [idAuto] { return ordenTurnos[autosMovidos] == idAuto || autosTerminados[ordenTurnos[autosMovidos]]; });

            //Verificar si el auto ya llego a la meta
            if(autosTerminados[idAuto]) {
                break;
            }

            //Calcular la distancia que avanzo el auto en esta iteracion
            int distanciaAvanzada = std::rand() % 10 + 1;

            //Actualizar la distancia recorrida por el auto
            distanciaRecorrida += distanciaAvanzada;

            //Imprimir la distancia recorrida por el auto
            if (distanciaRecorrida >= distanciaTotal) {
                std::cout << "\033[1;32mAuto" << idAuto << " avanza " << distanciaAvanzada << " metros y termina la carrera en el lugar " << lugarActual << "!\033[0m\n";
                ordenLlegada.push_back({idAuto, lugarActual++});
            } else {
                std::cout << "Auto" << idAuto << " avanza " << distanciaAvanzada << " metros. (Total: " << distanciaRecorrida << " metros)\n";
            }

            //Incrementar el numero de autos movidos
            autosMovidos++;
            if(autosMovidos == numeroAutos) {
                autosMovidos = 0;
                // Inicializar el generador de numeros aleatorios con una semilla diferente
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(ordenTurnos.begin(), ordenTurnos.end(), g);
            }
        }

        //Notificar a todos los hilos que el turno ha cambiado
        cv.notify_all();  

        //Dormir el hilo por un tiempo aleatorio entre 100 y 500 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(velocidades[idAuto]));
    }

    {   //Bloquear el acceso a la variable autosTerminados
        std::lock_guard<std::mutex> lock(mtx);

        // Marcar el auto como terminado
        autosTerminados[idAuto] = true;
    }

    //Notificar a todos los hilos que el auto ha llegado a la meta
    cv.notify_all();
}

// Funcion principal
int main(int argc, char* argv[]) { //argc: numero de argumentos, argv: argumentos

    //Verificar que se haya pasado el numero de autos
    if(argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <Distancia total> <Numero de autos>" << std::endl;
        return 1;
    }   

    //Obtener la distancia total de la carrera y el numero de autos
    int distanciaTotal = std::atoi(argv[1]);
    int numeroAutos = std::atoi(argv[2]);

    // Muestra la distancia total y el numero de autos
    std::cout << "Distancia total carrera: " << distanciaTotal << " metros\n";
    std::cout << "Numero de autos: " << numeroAutos << "\n";
    std::cout << "_______________________________________\n";

    // Inicializar el vector de autos terminados, velocidades y orden de turnos
    autosTerminados.resize(numeroAutos, false);
    velocidades.resize(numeroAutos);
    ordenTurnos.resize(numeroAutos);

    // Inicializar las velocidades de los autos y el orden de los turnos
    for (int i = 0; i < numeroAutos; ++i) {
        velocidades[i] = std::rand() % 401 + 100;
        ordenTurnos[i] = i;
    }

    // Mezclar el orden de los turnos inicialmente
    std::random_device rd;
    // Inicializar el generador de numeros aleatorios con una semilla diferente
    std::mt19937 g(rd());
    // Mezclar el vector de orden de turnos
    std::shuffle(ordenTurnos.begin(), ordenTurnos.end(), g);

    // Crear un vector de hilos para los autos
    std::vector<std::thread> autos;

    for (int i = 0; i < numeroAutos; i++) {
        // Crear hilo para cada auto y guardarlo en el vector "autos"
        autos.push_back(std::thread(moverAuto, i, distanciaTotal, numeroAutos));
    }

    // Esperar a que todos los autos terminen la carrera
    for(auto& autoHilo : autos) {
        autoHilo.join();
    }

    std::cout << "Fin de la Carrera 🏁\n\n";

    //Ordenar el vector de llegada de los autos
    std::cout << "Lugar   Auto\n";
    std::cout << "_____________\n";
    for (const auto& par : ordenLlegada) {
        std::cout << par.second << "       Auto" << par.first << "\n";
    }

    return 0;
}