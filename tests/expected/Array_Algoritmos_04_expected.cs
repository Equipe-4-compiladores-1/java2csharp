using System;

namespace TranspiledProgram {
    public static class ArrayAlgoritmos04 {
        public static void Main(string[] args) {
            int n = 5;
            int[] dados = new int[n];
            dados[0] = 14;
            dados[1] = 3;
            dados[2] = 42;
            dados[3] = 7;
            dados[4] = 29;
            int maior = dados[0];
            int i = 1;
            while (i < n) {
                if (dados[i] > maior) {
                    maior = dados[i];
                }
                i++;
            }
            for (int j = 0; j < n / 2; j++) {
                int temp = dados[j];
                dados[j] = dados[n - 1 - j];
                dados[n - 1 - j] = temp;
            }
            Console.WriteLine(maior);
            Console.WriteLine(dados[0]);
        }

    }
}