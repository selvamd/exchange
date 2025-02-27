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
        Msg msg = (new Msg(0)).addl(100).adds("GWY01",10).adds("ME01",10).addi(0);
        byte[] buf = msg.tobytes();
        out.write(buf);
        byte[] bufo = new byte[buf.length];
        in.read(bufo);
        msg.parse(bufo);
        Thread.sleep(1000);
    }
}

