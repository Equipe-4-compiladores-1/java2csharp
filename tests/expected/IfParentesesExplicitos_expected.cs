using System;

namespace TranspiledProgram {
    public static class IfParentesesExplicitos {
        public static void Main(string[] args) {
            int x = 3;
            if (x == 1 + 2) {
                Console.WriteLine("sem agrupamento explicito");
            }
            if (x == (1 + 2)) {
                Console.WriteLine("com agrupamento explicito");
            }
        }

    }
}