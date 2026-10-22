using System;

namespace TranspiledProgram {
    public static class ChecagemTipos_02_ExpressoesCompativeis {
        public static void Main(string[] args) {
            int a = 10;
            int b = 3;
            int soma = a + b;
            int produto = (a - b) * 2;
            long l = 5;
            long somaLonga = l + a;
            double d = 2.5;
            double media = (a + d) / 2;
            bool maior = a > b;
            bool igual = a == b;
            bool diferente = d != a;
            bool menorIgual = l <= a;
            Console.WriteLine("Soma: " + soma);
            Console.WriteLine(media);
        }

    }
}