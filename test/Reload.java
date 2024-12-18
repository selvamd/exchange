import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

public class Reload 
{
    //Create new business and accounts 
    public static void main(String[] args) throws Exception
	{
        MsgClient client0, client1, client2;

        client0 = new MsgClient("localhost", 65000);
        client1 = new MsgClient("localhost", 65100);

        client0.send("STORE");
        client1.send("STORE");
        System.out.println("Storing....");
        Thread.sleep(5000);
        client0.send("LOAD");
        client1.send("LOAD");
        Thread.sleep(5000);
        System.out.println("Loaded....");
	}
}
