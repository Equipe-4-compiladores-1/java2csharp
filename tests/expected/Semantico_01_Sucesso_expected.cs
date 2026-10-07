using System;

namespace TranspiledProgram {
    public static class Principal {
        public static void Main(string[] args) {
            int a = 10;
            int b = a + 5;
            Console.WriteLine(b);
        }

        public static int somar(int x, int y) {
            int resultado = x + y;
            return resultado;
        }

    }
}