using System;

namespace TranspiledProgram {
    public static class IfOperadoresRelacionais_01 {
        public static void Main(string[] args) {
            int a = 3;
            int b = 7;
            if ((a == b)) {
                Console.WriteLine("igual");
            }
            if ((a != b)) {
                Console.WriteLine("diferente");
            }
            if ((a < b)) {
                Console.WriteLine("menor");
            }
            if ((a <= b)) {
                Console.WriteLine("menor ou igual");
            }
            if ((a > b)) {
                Console.WriteLine("maior");
            }
            if ((a >= b)) {
                Console.WriteLine("maior ou igual");
            }
        }

    }
}