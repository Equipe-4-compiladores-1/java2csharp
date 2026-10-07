class IfComMetodo_01 {
    public static void main(String[] args) {
        int resultado = maximo(4, 9);
        System.out.println(resultado);
    }

    int maximo(int a, int b) {
        if (a > b) {
            return a;
        } else {
            return b;
        }
    }
}
