using System;

namespace TranspiledProgram {
    public static class VariaveisSemInicializacao_01 {
        public static void Main(string[] args) {
            int x;
            double y;
            bool flag;
            String texto;
            x = 42;
            y = 3.5;
            flag = true;
            texto = "ok";
            if (flag) {
                Console.WriteLine(x);
                Console.WriteLine(y);
                Console.WriteLine(texto);
            }
        }

    }
}