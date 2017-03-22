import java.net.ServerSocket;
import java.net.Socket;
import java.io.*;
import java.sql.*;

/**
 * Created by ARTEM on 10.09.2016.
 */

public class Server {



    private static final String url = "jdbc:mysql://localhost:3306/smart_house?serverTimezone=UTC";
    private static final String user = "root";
    private static final String password = "";

    private static Connection connection;
    private static PreparedStatement statementObject;
    private static ResultSet resultGet;
    private static int result;

    public static String data = new String();
    public static String value = new String();
    public static void main(String[] args) throws Throwable {
        ServerSocket ss = new ServerSocket(7887);
//        getDataFromDataBase();
//        writeToDataBase();
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }
        while (true) {
            Socket s = ss.accept();
            System.err.println("Client accepted");
            new Thread(new SocketProcessor(s)).start();
        }
    }

    private static void getDataFromDataBase() {
        String query = "SELECT * FROM 'data'";

        try {
            connection = DriverManager.getConnection(url, user, password);
            statementObject = connection.prepareStatement(query);
            resultGet = statementObject.executeQuery();

            while (resultGet.next()) {
                System.out.println(resultGet.getString(2));
            }
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }



    private static void writeToDataBase() {
        String query = "update`data` set `value` = '"+value+"'  where `data` = '"+data+"'";

        try {

            connection = DriverManager.getConnection(url, user, password);

            statementObject = connection.prepareStatement(query);
            result = statementObject.executeUpdate();

            System.out.println(query);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }

    /*private static void writeToFile() throws IOException {
        BufferedWriter output = new BufferedWriter(new FileWriter("changedata.txt"));
        output.write(data + ":" + value);
        output.close();
    }*/

    private static class SocketProcessor implements Runnable {

        private Socket s;
        private InputStream is;
        private OutputStream os;

        private SocketProcessor(Socket s) throws Throwable {
            this.s = s;
            this.is = s.getInputStream();
            this.os = s.getOutputStream();
        }

        public void run() {
            try {
                readInput();
                writeResponse("");
                writeToDataBase();
//                writeToFile();
            } catch (Throwable t) {
                t.printStackTrace();
            } finally {
                try {
                    s.close();
                } catch (Throwable t) {
                    t.printStackTrace();
                }
            }
            System.err.println("Client processing finished");
        }

        private void writeResponse(String s) throws Throwable {
            String response = "HTTP/1.1 200 OK\r\n" +
                    "Server: 192.168.0.80:7887\r\n" +
                    "Content-Type: text/html\r\n" +
                    "Content-Length: " + s.length() + "\r\n" +
                    "Connection: close\r\n\r\n";
            String result = response + s;
            os.write(result.getBytes());
            os.flush();
        }

        private void readInput() throws Throwable {
            int i = 0;
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            while(true) {
                String s = br.readLine();
                if(i == 2) {
                    value = s;
                    i = 0;
                }
                if(i == 1) {
                    data = s;
                    i = 2;
                }
                if(s.equals("POST")) { i = 1;}
//                System.out.println(s);
                if(s == null || s.trim().length() == 0) {
                    break;
                }
            }
        }
    }
}
