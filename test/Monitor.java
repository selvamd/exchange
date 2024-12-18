import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

public class Monitor
{

    static boolean isRunning(Process p)
    {
        try 
        {
            if (p == null) return false;
            p.exitValue();
            //Send heartbeat to check looping
        }
        catch (IllegalThreadStateException e)
        {
            return true;
        }
        return false;
    }

    //Create new business and accounts 
    public static void main(String[] args) throws Exception
	{
        ProcessBuilder pb;
        //String dir = "/Users/selvamdoraisamy/Desktop/personal/accounts/bin";
        String dir = "/home/ec2-user/applications/accounts/bin";
        
        Map<String,Process> processes = new HashMap<String,Process>();
        processes.put("accountsvc",null);
        processes.put("loginsvc",null);
        processes.put("gatewaysvc",null);

        for (String e: processes.keySet())
            Runtime.getRuntime().exec("ps -ef | grep " + e +" | awk  '{print $2}' | xargs kill -9 ").waitFor();

        while (true)
        {
            Thread.sleep(3000);            

            //Check if processes are up and running
            for (Map.Entry<String,Process> e: processes.entrySet())
            {
                String  pname = e.getKey();
                if (!isRunning(e.getValue()))
                {
                    pb = new ProcessBuilder("./" + pname);
                    pb.directory(new File(dir));
                    processes.put(pname,pb.start());
                    break;
                }    
            }
        }
	}

}
