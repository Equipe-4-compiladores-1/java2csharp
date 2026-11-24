class OperadoresLogicos_01 {
    public static void main(String[] args) {
        boolean ativo = true;
        boolean bloqueado = false;
        boolean administrador = false;
        boolean permitido = ativo && (!bloqueado || administrador);
        if (permitido || bloqueado && !administrador) {
            System.out.println("permitido");
        }
    }
}
