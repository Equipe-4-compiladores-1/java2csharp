using System;

namespace TranspiledProgram {
    public static class IfAninhado_01 {
        public static void Main(string[] args) {
            int x = 10;
            int y = 5;
            if ((x > 0)) {
                if ((y > 0)) {
                    Console.WriteLine("ambos positivos");
                } else {
                    Console.WriteLine("apenas x positivo");
                }
            } else {
                if ((y > 0)) {
                    Console.WriteLine("apenas y positivo");
                }
            }
        }

    }
}