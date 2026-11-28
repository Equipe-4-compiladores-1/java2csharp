using System;

namespace TranspiledProgram {
    public static class ConstantFoldingCondicoes_01 {
        public static void Main(string[] args) {
            int x = 12;
            int y = 1;
            if (x == 12) {
                Console.WriteLine("com parenteses");
            }
            if (x == 9) {
                Console.WriteLine("sem parenteses");
            }
            if (x == (y + 2) * 4) {
                Console.WriteLine("com variavel");
            }
        }

    }
}