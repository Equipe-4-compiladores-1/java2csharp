class VariaveisEArraysInicializacaoPosterior_02 {
    public static void main(String[] args) {
        int tamanho;
        int[] valores;

        tamanho = 3;
        valores = new int[tamanho];
        valores[0] = 10;
        valores[1] = 20;
        valores[2] = valores[0] + valores[1];

        System.out.println(valores[2]);
    }
}