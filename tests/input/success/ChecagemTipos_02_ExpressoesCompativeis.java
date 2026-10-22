class ChecagemTipos_02_ExpressoesCompativeis {
    public static void main(String[] args) {
        int a = 10;
        int b = 3;
        int soma = a + b;
        int produto = (a - b) * 2;
        long l = 5;
        long somaLonga = l + a;
        double d = 2.5;
        double media = (a + d) / 2;
        boolean maior = a > b;
        boolean igual = a == b;
        boolean diferente = d != a;
        boolean menorIgual = l <= a;
        System.out.println("Soma: " + soma);
        System.out.println(media);
    }
}
