using System;

namespace TranspiledProgram {
    public static class TesteIncremento {
        public static void Main(string[] args) {
            int contador = 10;
            Console.WriteLine("Testando operacoes isoladas:");
            contador++;
            Console.Write("Contador apos ++: {0}\n", contador);
            contador--;
            Console.Write("Contador apos --: {0}\n", contador);
            Console.WriteLine("Testando laco FOR com i++:");
            for (int i = 0; i < 3; i++) {
                Console.Write("Crescente: {0}\n", i);
            }
            Console.WriteLine("Testando laco FOR com j--:");
            for (int j = 3; j > 0; j--) {
                Console.Write("Decrescente: {0}\n", j);
            }
        }

    }
}