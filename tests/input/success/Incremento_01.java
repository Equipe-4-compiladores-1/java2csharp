class TesteIncremento {
    public static void main(String[] args) {
        int contador = 10;

        System.out.println("Testando operacoes isoladas:");
        contador++;
        System.out.printf("Contador apos ++: %d\n", contador);

        contador--;
        System.out.printf("Contador apos --: %d\n", contador);

        System.out.println("Testando laco FOR com i++:");
        for (int i = 0; i < 3; i++) {
            System.out.printf("Crescente: %d\n", i);
        }

        System.out.println("Testando laco FOR com j--:");
        for (int j = 3; j > 0; j--) {
            System.out.printf("Decrescente: %d\n", j);
        }
    }
}