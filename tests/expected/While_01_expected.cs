using System;

namespace TranspiledProgram {
    public static class TesteWhile {
        public static void Main(string[] args) {
            int contador = 1;
            Console.WriteLine("Iniciando a contagem usando o laco WHILE:");
            while (contador <= 5) {
                Console.Write("O valor atual do contador e: {0}\n", contador);
                contador = contador + 1;
            }
            Console.WriteLine("Contagem finalizada com sucesso!");
        }

    }
}