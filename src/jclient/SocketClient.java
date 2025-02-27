import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class SocketClient {

    private Socket sock;
    private OutputStream out;
    private InputStream in;

    public SocketClient() throws Exception {
        sock = new Socket("localhost", 65000);
        out  = sock.getOutputStream();
        in   = sock.getInputStream();
    }

    public void neworder() throws Exception 
    {
    }

    public Msg read() throws Exception {
        byte[] buf = new byte[4];
        in.read(buf);
        Msg msg = Msg.create(ByteBuffer.wrap(buf).getInt());
        buf = msg.tobytes();
        in.read(buf,4,buf.length-4);
        msg.parse(buf);
        return msg;
    }

    //To make it non-blocking, push a login msg and read 
    //everthing until you get an ack for that.
    public List<Msg> readall(List<Msg> msgs) throws Exception {
        msgs.clear();
        logon();
        while (true) {
            Msg msg = read();
            if (msg.type() == 0)
                break;
            else msgs.add(msg);
        }
        return msgs;
    }

    public void logon() throws Exception 
    {
        Msg msg = Msg.newmsg(0).addl(100).adds("GWY01",10).adds("ME01",10).addi(0);
        out.write(msg.tobytes());
    }

    public void close() throws Exception {
        out.close();
        in.close();
        sock.close();
    }

    public static void main(String[] args) throws Exception 
    {
        List<Msg> msgs = new ArrayList<Msg>();
        SocketClient client = new SocketClient();

        client.logon();
        client.read().print();

        client.neworder();
        for (Msg msg:client.readall(msgs))
            msg.print();
        Thread.sleep(1000);
    }
}

