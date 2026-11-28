using System;

namespace TranspiledProgram {
    public static class MenosUnario_01 {
        public static void Main(string[] args) {
            int positivo = 12;
            int negativo = -12;
            int invertido = -positivo;
            int precedencia = -6;
            int agrupado = -(positivo + 3);
            double fracao = -2.5;
            Console.WriteLine(negativo);
            Console.WriteLine(invertido);
            Console.WriteLine(precedencia);
            Console.WriteLine(agrupado);
            Console.WriteLine(fracao);
        }

    }
}
