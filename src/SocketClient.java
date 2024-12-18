import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class SocketClient {

    public static void main(String[] args) 
        throws UnknownHostException, IOException, 
        ClassNotFoundException, InterruptedException 
    {
        Socket sock = new Socket("localhost", 65000);
        OutputStream out = sock.getOutputStream();
        InputStream in = sock.getInputStream();
        String nam = "selvamd";
        final byte[] bo = new byte[38];
        byte[] b = nam.getBytes(StandardCharsets.UTF_8);
        final byte[] bytes = ByteBuffer.allocate(38).putInt(0).
            putInt(38).putLong(100).put(b).putInt(0).put(b).putInt(0).array();
        // Thread th1 = new Thread(new Runnable() { 
        //     public void run() { 
        //         try { 
        //             for (int i=0;i<1E6;i++) out.write(bytes); 
        //         } catch (Exception e) {} }} ); 
        // Thread th2 = new Thread(new Runnable() { 
        //     public void run() { 
        //         try { for (int i=0;i<1E6;i++) in.read(bo); 
        //         } catch (Exception e) {} }}  ); 
        long l0 = System.currentTimeMillis();  
        for (int i=0;i<100;i++) 
        {
            out.write(bytes);
            in.read(bo);
        }
        // th1.start();
        // th2.start();
        // th1.join();
        // th2.join();
        long l1 = System.currentTimeMillis();            
        System.out.println(1E6*1E3/(l1-l0));
        Thread.sleep(1000);
    }
}

