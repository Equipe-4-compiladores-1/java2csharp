using System;

namespace TranspiledProgram {
    public static class ArrayCopiaRef05 {
        public static void Main(string[] args) {
            int[] original = new int[3];
            original[0] = 100;
            original[1] = 200;
            original[2] = 300;
            int[] copia = original;
            copia[1] = 999;
            if (copia[1] == 999) {
                int[] local = new int[2];
                local[0] = original[1];
                Console.WriteLine(local[0]);
            }
        }

    }
}