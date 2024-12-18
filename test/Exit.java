import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

public class Exit 
{
    //Create new business and accounts 
    public static void main(String[] args) throws Exception
	{
        //Login first
        BizTester logcli = new BizTester("localhost", 65000);
        ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
        ScheduledFuture logfuture = executor.scheduleWithFixedDelay(logcli, 0, 50, TimeUnit.MILLISECONDS);
        logcli.loginuser("selvam","1selvam");
        logfuture.cancel(true);
        logcli.close();

        //Login first
        int port = 0;
        if ("account".equals(args[0]))
            port = 65100;
        else if ("login".equals(args[0]))
            port = 65000;
        else return;

        MsgClient cli = new MsgClient("localhost", port);
        cli.m_session = logcli.m_session; 
        ScheduledFuture future = executor.scheduleWithFixedDelay(cli, 0, 50, TimeUnit.MILLISECONDS);
        cli.exit();
        future.cancel(true);
        cli.close();
        executor.shutdown();
	}
}
