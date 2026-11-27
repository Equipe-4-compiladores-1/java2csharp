using System;

namespace TranspiledProgram {
    public static class Saltos_01 {
        public static void Main(string[] args) {
            int soma = 0;
            for (int i = 0; i < 10; i++) {
                if (i % 2 == 0) {
                    continue;
                }
                if (i > 7) {
                    break;
                }
                soma = soma + i;
            }
            Console.WriteLine(soma);
            int n = 0;
            while (true) {
                n++;
                if (n == 5) {
                    break;
                }
            }
            Console.WriteLine(n);
            for (int a = 0; a < 3; a++) {
                int b = 0;
                while (b < 3) {
                    b++;
                    if (b == 2) {
                        continue;
                    }
                    Console.Write("{0} {1}\n", a, b);
                }
                if (a == 1) {
                    break;
                }
            }
        }

    }
}
