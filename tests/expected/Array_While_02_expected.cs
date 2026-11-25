using System;

namespace TranspiledProgram {
    public static class ArrayWhile02 {
        public static void Main(string[] args) {
            int n = 4;
            double[] notas = new double[n];
            bool[] aprovados = new bool[n];
            int idx = 0;
            while (idx < n) {
                notas[idx] = (idx + 1) * 2.5;
                aprovados[idx] = notas[idx] >= 6.0;
                idx++;
            }
            Console.WriteLine(notas[0]);
            Console.WriteLine(aprovados[3]);
        }

    }
}