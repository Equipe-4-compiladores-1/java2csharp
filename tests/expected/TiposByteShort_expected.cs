using System;

namespace TranspiledProgram {
    public static class TiposByteShort {
        public static void Main(string[] args) {
        }

        public static sbyte identidadeByte(sbyte valor) {
            sbyte copia = valor;
            return copia;
        }

        public static short identidadeShort(short valor) {
            short copia = valor;
            return copia;
        }

        public static short ampliarByte(sbyte valor) {
            return valor;
        }

        public static int ampliarShort(short valor) {
            return valor;
        }

        public static int somar(sbyte a, short b) {
            return a + b;
        }

    }
}
