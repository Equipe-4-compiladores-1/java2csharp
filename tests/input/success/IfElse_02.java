class IfElse_02 {
    public static void main(String[] args) {
        int a = 10;
        int b = 20;
        if (a < b) {
            int soma = a + b;
            int produto = a * b;
            System.out.println(soma);
            System.out.println(produto);
        } else {
            int diferenca = a - b;
            System.out.println(diferenca);
            System.out.println("a maior ou igual a b");
        }
    }
}
