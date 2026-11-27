using System;

namespace TranspiledProgram {
    public static class VariaveisEArraysInicializacaoPosterior_02 {
        public static void Main(string[] args) {
            int tamanho;
            int[] valores;
            tamanho = 3;
            valores = new int[tamanho];
            valores[0] = 10;
            valores[1] = 20;
            valores[2] = valores[0] + valores[1];
            Console.WriteLine(valores[2]);
        }

    }
}