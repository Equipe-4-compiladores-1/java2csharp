class IfAninhado_01 {
    public static void main(String[] args) {
        int x = 10;
        int y = 5;
        if (x > 0) {
            if (y > 0) {
                System.out.println("ambos positivos");
            } else {
                System.out.println("apenas x positivo");
            }
        } else {
            if (y > 0) {
                System.out.println("apenas y positivo");
            }
        }
    }
}
