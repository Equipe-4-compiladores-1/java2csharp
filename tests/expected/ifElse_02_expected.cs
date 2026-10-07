using System;

namespace TranspiledProgram {
    public static class IfElse_02 {
        public static void Main(string[] args) {
            int a = 10;
            int b = 20;
            if ((a < b)) {
                int soma = (a + b);
                int produto = (a * b);
                Console.WriteLine(soma);
                Console.WriteLine(produto);
            } else {
                int diferenca = (a - b);
                Console.WriteLine(diferenca);
                Console.WriteLine("a maior ou igual a b");
            }
        }

    }
}