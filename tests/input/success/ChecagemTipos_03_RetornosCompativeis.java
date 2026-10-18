class ChecagemTipos_03_RetornosCompativeis {
    public static void main(String[] args) {
        int quadrado = calcularQuadrado(4);
        long total = calcularQuadrado(3);
        double area = calcularArea(2.0);
        boolean par = ehPositivo(quadrado);
        exibir(quadrado);
    }

    int calcularQuadrado(int x) {
        return x * x;
    }

    double calcularArea(double raio) {
        return raio * raio * 3.14;
    }

    long ampliar(int valor) {
        return valor;
    }

    boolean ehPositivo(int n) {
        return n > 0;
    }

    void exibir(int valor) {
        System.out.println(valor);
    }
}
