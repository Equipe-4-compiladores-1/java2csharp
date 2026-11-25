class ArraySemErr04 {
    public static void main(String[] args) {
        int escalar = 10;
        escalar[0] = 5;

        int[] lista = new int[3];
        lista[true] = 1;
    }
}