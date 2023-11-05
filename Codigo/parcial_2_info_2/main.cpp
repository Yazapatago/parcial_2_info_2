#include <iostream>
#include <vector>

class Ficha {
public:
    enum Estado { VACIA, BLANCA, NEGRA };
    Estado estado;

    Ficha() : estado(VACIA) {}

    void cambiarEstado(Estado nuevoEstado) {
        estado = nuevoEstado;
    }

    Estado obtenerEstado() const {
        return estado;
    }
};

class Tablero {
public:
    static const int TAMANO = 8;
    Ficha casillas[TAMANO][TAMANO];

    void inicializarTablero() {
        for (int i = 1; i <= TAMANO; i++) {
            for (int j = 1; j <= TAMANO; j++) {
                casillas[i][j].cambiarEstado(Ficha::VACIA);
            }
        }
        casillas[4][4].cambiarEstado(Ficha::NEGRA);
        casillas[4][5].cambiarEstado(Ficha::BLANCA);
        casillas[5][4].cambiarEstado(Ficha::BLANCA);
        casillas[5][5].cambiarEstado(Ficha::NEGRA);
    }

    void imprimirTablero() const {
        for (int i = 1; i <= TAMANO; i++) {
            for (int j = 1; j <= TAMANO; j++) {
                switch (casillas[i][j].obtenerEstado()) {
                case Ficha::BLANCA:
                    std::cout << "O ";
                    break;
                case Ficha::NEGRA:
                    std::cout << "X ";
                    break;
                case Ficha::VACIA:
                    std::cout << ". ";
                    break;
                }
            }
            std::cout << std::endl;
        }
    }

    bool movimientoVolteaFichas(int fila, int columna, Ficha::Estado colorFicha) {
        if (fila < 1 || fila > TAMANO || columna < 1 || columna > TAMANO || casillas[fila][columna].obtenerEstado() != Ficha::VACIA) {
            return false;
        }

        int direcciones[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

        for (int dir = 0; dir < 8; dir++) {
            int x = fila + direcciones[dir][0];
            int y = columna + direcciones[dir][1];
            bool fichaEnemigaEncontrada = false;

            while (x >= 1 && x <= TAMANO && y >= 1 && y <= TAMANO) {
                if (casillas[x][y].obtenerEstado() == Ficha::VACIA) {
                    break;
                }
                if (casillas[x][y].obtenerEstado() == colorFicha) {
                    if (fichaEnemigaEncontrada) {
                        return true;
                    }
                    break;
                }
                if (casillas[x][y].obtenerEstado() != colorFicha) {
                    fichaEnemigaEncontrada = true;
                }
                x += direcciones[dir][0];
                y += direcciones[dir][1];
            }
        }

        return false;
    }

    bool esJugadaLegal(int fila, int columna, Ficha::Estado colorFicha) {
        return movimientoVolteaFichas(fila, columna, colorFicha);
    }

    bool tableroLleno() {
        for (int i = 1; i <= TAMANO; i++) {
            for (int j = 1; j <= TAMANO; j++) {
                if (casillas[i][j].obtenerEstado() == Ficha::VACIA) {
                    return false; // Hay al menos una casilla vacía
                }
            }
        }
        return true; // No hay casillas vacías, el tablero está lleno
    }

    void realizarJugada(int fila, int columna, Ficha::Estado colorFicha) {
        if (!movimientoVolteaFichas(fila, columna, colorFicha)) {
            std::cout << "Jugada no válida. Inténtalo de nuevo." << std::endl;
                return;
        }

        // Direcciones posibles para verificar fichas enemigas
        int direcciones[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

        for (int dir = 0; dir < 8; dir++) {
            int x = fila + direcciones[dir][0];
            int y = columna + direcciones[dir][1];

            while (x >= 1 && x <= TAMANO && y >= 1 && y <= TAMANO && casillas[x][y].obtenerEstado() != Ficha::VACIA && casillas[x][y].obtenerEstado() != colorFicha) {
                x += direcciones[dir][0];
                y += direcciones[dir][1];
            }

            if (x >= 1 && x <= TAMANO && y >= 1 && y <= TAMANO && casillas[x][y].obtenerEstado() == colorFicha) {
                x = fila + direcciones[dir][0];
                y = columna + direcciones[dir][1];

                while (x >= 1 && x <= TAMANO && y >= 1 && y <= TAMANO && casillas[x][y].obtenerEstado() != colorFicha) {
                    casillas[x][y].cambiarEstado(colorFicha);
                    x += direcciones[dir][0];
                    y += direcciones[dir][1];
                }
            }
        }

        casillas[fila][columna].cambiarEstado(colorFicha);
    }
};

class Jugador {
public:
    std::string nombre;
    Ficha::Estado colorFicha;

    Jugador(const std::string& nombre, Ficha::Estado color) : nombre(nombre), colorFicha(color) {}

    void realizarJugada(Tablero& tablero, int fila, int columna) {
        bool jugadaValida = false;

        while (!jugadaValida) {
            std::cout << nombre << ", ingresa tu jugada (fila y columna): ";
            std::cin >> fila >> columna;

            if (tablero.esJugadaLegal(fila, columna, colorFicha)) {
                tablero.realizarJugada(fila, columna, colorFicha);
                jugadaValida = true;
            } else {
                std::cout << "Jugada no válida. Inténtalo de nuevo." << std::endl;
            }
        }
    }
};

int main() {
    Tablero tablero;
    tablero.inicializarTablero();

    Jugador jugadorBlanco("Jugador 1", Ficha::BLANCA);
    Jugador jugadorNegro("Jugador 2", Ficha::NEGRA);

    int fila, columna;

    Jugador* jugadorActual = &jugadorBlanco;

    while (true) {
        std::cout << "Tablero actual:\n";
        tablero.imprimirTablero();

        std::cout << jugadorActual->nombre << ", es tu turno.\n";

        jugadorActual->realizarJugada(tablero, fila, columna);

        // Verificar si el tablero está lleno
        if (tablero.tableroLleno()) {
            std::cout << "El juego ha terminado. El tablero está lleno." << std::endl;
                break;
        }

        // Cambiar al otro jugador
        jugadorActual = (jugadorActual == &jugadorBlanco) ? &jugadorNegro : &jugadorBlanco;

        // Verificar si el nuevo jugador puede hacer más jugadas
        bool jugadorPuedeJugar = false;
        for (int i = 1; i <= Tablero::TAMANO; i++) {
            for (int j = 1; j <= Tablero::TAMANO; j++) {
                if (tablero.esJugadaLegal(i, j, jugadorActual->colorFicha)) {
                    jugadorPuedeJugar = true;
                    break;
                }
            }
            if (jugadorPuedeJugar) {
                break;
            }
        }

        // Si el nuevo jugador no puede hacer más jugadas, cambiar nuevamente al otro jugador
        if (!jugadorPuedeJugar) {
            jugadorActual = (jugadorActual == &jugadorBlanco) ? &jugadorNegro : &jugadorBlanco;
        }
    }

    return 0;
}
