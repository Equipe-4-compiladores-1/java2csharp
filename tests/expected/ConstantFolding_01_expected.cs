using System;

namespace TranspiledProgram {
    public static class ConstantFolding_01 {
        public static void Main(string[] args) {
            int arithmetic = 17;
            int wrapped = -2147483648;
            int remainder = -1;
            double floating = 8.0;
            bool comparison = true;
            bool logic = true;
            int divisionByZero = 4 / 0;
            int variable = 2;
            int notConstant = variable + 3;
        }

    }
}
