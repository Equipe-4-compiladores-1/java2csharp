class Semantico_08_VariavelForaDoEscopoElse {
    public static void main(String[] args) {
        int x = 10;
        if (x > 5) {
            System.out.println(x);
        } else {
            int z = x - 1;
        }
        int w = z + 1;
    }
}
