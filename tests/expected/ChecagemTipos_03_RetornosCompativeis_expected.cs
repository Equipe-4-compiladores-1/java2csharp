using System;

namespace TranspiledProgram {
    public static class ChecagemTipos_03_RetornosCompativeis {
        public static void Main(string[] args) {
            int quadrado = calcularQuadrado(4);
            long total = calcularQuadrado(3);
            double area = calcularArea(2.0);
            bool par = ehPositivo(quadrado);
            exibir(quadrado);
        }

        public static int calcularQuadrado(int x) {
            return x * x;
        }

        public static double calcularArea(double raio) {
            return raio * raio * 3.14;
        }

        public static long ampliar(int valor) {
            return valor;
        }

        public static bool ehPositivo(int n) {
            return n > 0;
        }

        public static void exibir(int valor) {
            Console.WriteLine(valor);
        }

    }
}