using System;

namespace TranspiledProgram {
    public static class TiposByteShort {
        public static void Main(string[] args) {
        }

        public static byte identidadeByte(byte valor) {
            byte copia = valor;
            return copia;
        }

        public static short identidadeShort(short valor) {
            short copia = valor;
            return copia;
        }

        public static short ampliarByte(byte valor) {
            return valor;
        }

        public static int ampliarShort(short valor) {
            return valor;
        }

        public static int somar(byte a, short b) {
            return a + b;
        }

    }
}