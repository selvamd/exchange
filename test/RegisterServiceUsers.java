import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

public class RegisterServiceUsers 
{
    //Registers applications and application users to login server
    public static void main(String[] args) throws Exception
    {
        MsgClient cli    = new MsgClient("localhost", 65000);
        MsgClient svc    = new MsgClient("localhost", 65100);

        ScheduledExecutorService executor   = Executors.newSingleThreadScheduledExecutor();
        ScheduledFuture clifuture           = executor.scheduleWithFixedDelay(cli, 0, 100, TimeUnit.MILLISECONDS);
        ScheduledFuture svcfuture           = executor.scheduleWithFixedDelay(svc, 0, 100, TimeUnit.MILLISECONDS);

        cli.registeruser("Dorai","dorai");
        svc.registersvc("impact", "ebrkr", "ebrkr", "admin,user".split(","));
        svc.authorize("Dorai", "user".split(","));

        cli.send("STORE");
        svc.send("STORE");
        System.out.println("Storing....");
        Thread.sleep(5000);
        svc.send("LOAD");
        cli.send("LOAD");
        Thread.sleep(5000);
        System.out.println("Loaded....");

        cli.loginuser("Dorai","dorai");
        svc.write("{Event=CRUD_BUSINESS,operation=CREATE,name=Doraisamy house business,session=" + cli.m_session + "}"); 
        System.out.println(cli.response("businessid"));

        clifuture.cancel(true);
        svcfuture.cancel(true);
        executor.shutdown();
        cli.close();
        svc.close();
    }

}
