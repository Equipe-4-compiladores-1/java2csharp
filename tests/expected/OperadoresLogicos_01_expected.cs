using System;

namespace TranspiledProgram {
    public static class OperadoresLogicos_01 {
        public static void Main(string[] args) {
            bool ativo = true;
            bool bloqueado = false;
            bool administrador = false;
            bool permitido = ativo && (!bloqueado || administrador);
            if (permitido || bloqueado && !administrador) {
                Console.WriteLine("permitido");
            }
        }

    }
}
