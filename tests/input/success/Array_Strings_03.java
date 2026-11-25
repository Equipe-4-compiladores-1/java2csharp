class ArrayStrings03 {
    public static void main(String[] args) {
        String[] nomes = new String[3];
        nomes[0] = "Ana";
        nomes[1] = "Bruno";
        nomes[2] = "Carla";

        char[] conceitos = new char[3];
        conceitos[0] = 'A';
        conceitos[1] = 'B';
        conceitos[2] = 'A';

        for (int i = 0; i < 3; i++) {
            System.out.printf("Aluno: %s | Conceito: %c\n", nomes[i], conceitos[i]);
        }
    }
}