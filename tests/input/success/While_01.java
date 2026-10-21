class TesteWhile {
    public static void main(String[] args) {
        int contador = 1;
        System.out.println("Iniciando a contagem usando o laco WHILE:");

        while (contador <= 5) {
            System.out.printf("O valor atual do contador e: %d\n", contador);
            // Incremento do contador (usamos + 1 pois ainda não temos o ++)
            contador = contador + 1;
        }
        System.out.println("Contagem finalizada com sucesso!");
    }
}