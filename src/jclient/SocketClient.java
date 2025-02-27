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
        String nam = "GWY01      ";
        final byte[] bo = new byte[42];
        byte[] b = nam.getBytes(StandardCharsets.UTF_8);
        b[5] = 0;
        final byte[] bytes = ByteBuffer.allocate(42).putInt(0).
            putInt(42).putLong(100).put(b).put(b).putInt(0).array();
        // Thread th1 = new Thread(new Runnable() { 
        //     public void run() { 
        //         try { 
        //             for (int i=0;i<1E6;i++) out.write(bytes); 
        //         } catch (Exception e) {} }} ); 
        // Thread th2 = new Thread(new Runnable() { 
        //     public void run() { 
        //         try { for (int i=0;i<1E6;i++) in.read(bo); 
        //         } catch (Exception e) {} }}  ); 
        //long l0 = System.currentTimeMillis();  
        out.write(bytes);
        in.read(bo);
        ByteBuffer buf = ByteBuffer.wrap(bo);
        System.out.println(buf.getInt());
        System.out.println(buf.getInt());
        System.out.println(buf.getLong());
        buf.get(b);
        System.out.println(new String(b, StandardCharsets.UTF_8));
        buf.get(b);
        System.out.println(new String(b, StandardCharsets.UTF_8));
        System.out.println(buf.getInt());

        // th1.start();
        // th2.start();
        // th1.join();
        // th2.join();
        //long l1 = System.currentTimeMillis();            
        // System.out.println(1E6*1E3/(l1-l0));
        Thread.sleep(1000);
    }
}

