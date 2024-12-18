import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

/*
<From_service>
RegisterSvc { name, oldpwd, pwd, Roles[] } returns serviceId
LoginService(name, pass) returns Status
AuthorizeUser { UserId, Roles[] }
GetAllSessions() returns sessionid[]
GetSessionInfo(sessionid) returns {sessionId, userid, Roles[]}
NotifyLogin(sessionId, userid, Roles[])
NotifyLogout(sessionId)
<From_user>
RegisterUser { name, pwd } returns Status
Login(name, pass) returns SessionId
Logout(sessionId) returns Status
*/

public class SockTest
{

    static  String ENDL = "\n";
    public static void client(String host, String port) throws Exception
    {
        Socket s = new Socket(host,Integer.parseInt(port));
        BufferedReader input   = new BufferedReader(new InputStreamReader(s.getInputStream()));
        BufferedWriter output  = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
        String str = "Hello world";

        for (int i=0;i<10;i++)
        {
            System.out.println("wrote msg = " + str);

            output.write(str,0,str.length());
            output.write(ENDL,0,ENDL.length());
            output.flush();
            
            str = input.readLine();
            System.out.println("got it back");
        }
    }

    public static void echoclient(String host, String port) throws Exception
    {
        Socket s = new Socket(host,Integer.parseInt(port));
        BufferedReader input   = new BufferedReader(new InputStreamReader(s.getInputStream()));
        BufferedWriter output  = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));

        while (true)
        {
            String str = input.readLine();
            System.out.println("got msg = " + str);
            output.write(str,0,str.length());
            output.write(ENDL,0,ENDL.length());
            output.flush();
            System.out.println("sent it back");
        }
    }

    public static void server(String port) throws Exception
    {
        ServerSocket srv = new ServerSocket(Integer.parseInt(port));
        while (true)
        {
            Socket s = srv.accept();
            BufferedReader input   = new BufferedReader(new InputStreamReader(s.getInputStream()));
            BufferedWriter output  = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));

            for (int i=0;i<10;i++)
            {
                String str = input.readLine();
                System.out.println("got msg = " + str);
                output.write(str,0,str.length());
                output.write(ENDL,0,ENDL.length());
                output.flush();
                System.out.println("sent it back");
            }
        }
    }

    //Create new business and accounts 
    public static void main(String[] args) throws Exception
	{
        if (args.length == 2)
            client(args[0],args[1]);
        else 
            server(args[0]);
	}

}
