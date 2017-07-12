package Arduino;

import java.sql.*;

public class Conexao 
{
    private static final String DRIVER = "com.mysql.jdbc.Driver";
    private static final String BANCO_DADOS = "teste";
    private static final String SERVIDOR = "localhost";
    private static final String SQL_CONEXAO = "jdbc:mysql://"+SERVIDOR+":3306/"+BANCO_DADOS;

    private static final String USUARIO = "root";
    private static final String SENHA = "root";
    
    private static final Connection Conexao=abrirConexao();

    public static Connection getConexao() {
        return Conexao;
    }

    @SuppressWarnings("UseSpecificCatch")
    public static Connection abrirConexao(){
            try{
                    Class.forName(DRIVER);
                    Connection conexao=DriverManager.getConnection(SQL_CONEXAO, USUARIO, SENHA);
                    return conexao;
            }catch(Exception es){
                    return null;
            }
    }
}
