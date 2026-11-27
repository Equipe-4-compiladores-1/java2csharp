class ConstantFolding_01 {
    public static void main(String[] args) {
        int arithmetic = (2 + 3) * 4 - 6 / 2;
        int wrapped = 2147483647 + 1;
        int remainder = -7 % 3;
        double floating = (2.5 + 1.5) * 2;
        boolean comparison = 2 + 3 * 4 > 10;
        boolean logic = !(2 > 3) && true;
        int divisionByZero = 4 / 0;
        int variable = 2;
        int notConstant = variable + 3;
    }
}
