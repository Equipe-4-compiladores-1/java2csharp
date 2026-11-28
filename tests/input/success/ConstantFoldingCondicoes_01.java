class ConstantFoldingCondicoes_01 {
    public static void main(String[] args) {
        int x = 12;
        int y = 1;
        if (x == (1 + 2) * 4) {
            System.out.println("com parenteses");
        }
        if (x == 1 + 2 * 4) {
            System.out.println("sem parenteses");
        }
        if (x == (y + 2) * 4) {
            System.out.println("com variavel");
        }
    }
}