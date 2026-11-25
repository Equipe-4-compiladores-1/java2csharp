using System;

namespace TranspiledProgram {
    public static class ArrayFor01 {
        public static void Main(string[] args) {
            int tamanho = 5;
            int[] numeros;
            numeros = new int[tamanho];
            for (int i = 0; i < tamanho; i++) {
                numeros[i] = i * 10;
            }
            int soma = 0;
            for (int i = 0; i < tamanho; i++) {
                soma += numeros[i];
            }
            Console.WriteLine(soma);
        }

    }
}