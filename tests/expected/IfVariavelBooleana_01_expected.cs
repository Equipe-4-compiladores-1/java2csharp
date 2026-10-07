using System;

namespace TranspiledProgram {
    public static class IfVariavelBooleana_01 {
        public static void Main(string[] args) {
            bool ativo = false;
            if (ativo) {
                Console.WriteLine(1);
            }
            if (true) {
                Console.WriteLine(2);
            } else {
                Console.WriteLine(3);
            }
        }

    }
}