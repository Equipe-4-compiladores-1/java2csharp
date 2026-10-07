using System;

namespace TranspiledProgram {
    public static class ForClassico {
        public static void Main(string[] args) {
            int y = 0;
            for (int i = 0; i < 5; i = i + 1) {
                y = y + i;
                int z = y;
            }
        }

    }
}

