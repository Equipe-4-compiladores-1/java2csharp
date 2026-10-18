using System;

namespace TranspiledProgram {
    public static class IfComMetodo_01 {
        public static void Main(string[] args) {
            int resultado = maximo(4, 9);
            Console.WriteLine(resultado);
        }

        public static int maximo(int a, int b) {
            if (a > b) {
                return a;
            } else {
                return b;
            }
        }

    }
}