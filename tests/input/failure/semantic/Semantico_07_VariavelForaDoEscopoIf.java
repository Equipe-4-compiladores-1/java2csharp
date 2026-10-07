class Semantico_07_VariavelForaDoEscopoIf {
    public static void main(String[] args) {
        int x = 10;
        if (x > 5) {
            int y = x + 1;
        }
        System.out.println(y);
    }
}
