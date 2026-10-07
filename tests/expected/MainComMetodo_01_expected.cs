using System;

namespace TranspiledProgram {
    public static class MainComMetodo_01 {
        public static void Main(string[] args) {
            Console.WriteLine("Iniciando programa...");
            inicializar();
            int x = calcular(10, 20) + 5;
        }

        public static void inicializar() {
            return ;
        }

        public static int calcular(int a, int b) {
            return (a * b);
        }

    }
}