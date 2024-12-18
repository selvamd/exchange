import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

public class MassTrans extends MsgClient
{
    public String business; 
    public String owner; 
    public String client; 

    public MassTrans(String host, int port) throws Exception
    {
        super(host, port);   
    }   

    /*
    public static void test_accountsvc(BizTester cli) throws Exception
    {
        //transactionid,businessid,clientid,type,description,transactionamount
        String transaction = "Event=CRUD_TRANSACTION,operation=CREATE,businessid=" + cli.business + ",session=" + cli.m_session;
        transaction += ",type=DEPOSIT,description=Initial Deposit,transactionamount=50000.00,client=Selvam capital";
        cli.write("{"+ transaction+"}"); 

        transaction = "Event=CRUD_TRANSACTION,operation=CREATE,businessid=" + cli.business + ",session=" + cli.m_session;
        transaction += ",type=COST,description=My first store purchase,transactionamount=1000.00,client=Barathi stores";
        cli.write("{"+ transaction+"}"); 
        cli.transaction = cli.response("transactionid");

        transaction = "Event=SETTLE,businessid=" + cli.business + ",session=" + cli.m_session;
        transaction += ",settleamount=1000.00,transactionid=" + cli.transaction;
        cli.write("{"+ transaction+"}"); 

        transaction = "Event=DELIVER,businessid=" + cli.business + ",session=" + cli.m_session;
        transaction += ",transactionid=" + cli.transaction;
        transaction += ",costcomponent=[Food,Clothing]";
        transaction += ",deliveryqty=[10,5],deliveryamount=[500,500]";
        cli.write("{"+ transaction+"}"); 

        transaction = "Event=ALLOCATE,businessid=" + cli.business + ",session=" + cli.m_session;
        transaction += ",transactionid=" + cli.transaction + ",costcomponent=Food";
        transaction += ",costcenter=[RKPD Maintenance,DKVS Maintenance]";
        transaction += ",allocationqty=[7,3],allocationamount=[300,200]";
        cli.write("{"+ transaction+"}"); 

        transaction = "Event=ALLOCATE,businessid=" + cli.business + ",session=" + cli.m_session;
        transaction += ",transactionid=" + cli.transaction + ",costcomponent=Clothing";
        transaction += ",costcenter=[RKPD Maintenance,DKVS Maintenance]";
        transaction += ",allocationqty=[2,3],allocationamount=[200,300]";
        cli.write("{"+ transaction+"}"); 
    }*/
    static void settle(MassTrans cli, String transactionid, JsonArray transaction, JsonArray notsettled)
            throws Exception
    {
        for (int i=0;i<transaction.size();i++)
        {
            if (transactionid.equals(transaction.getString(i))) 
            {
                String stransaction = "Event=SETTLE,businessid=" + cli.business + ",session=" + cli.m_session;
                stransaction += ",settleamount=" + notsettled.getString(i) + ",transactionid=" + transactionid;
                cli.write("{"+ stransaction+"}"); 
            }
        }


    }

    //Create new business and accounts 
    public static void main(String[] args) throws Exception
	{
        ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

        String host = "localhost";
        BizTester logcli = new BizTester(host, 65000);
        ScheduledFuture logfuture = executor.scheduleWithFixedDelay(logcli, 0, 50, TimeUnit.MILLISECONDS);
        logcli.loginuser("Kumudha","kumudha");
        while (true)
        {
            Thread.sleep(100);
            if (logcli.m_session != null) 
                break;
        }

        MassTrans cli = new MassTrans(host, 65100);
        cli.m_session = logcli.m_session;
        ScheduledFuture future = executor.scheduleWithFixedDelay(cli, 0, 50, TimeUnit.MILLISECONDS);

        //test_accountsvc(cli);
        cli.write("{Event=GET_TRANSACTIONS,filter=OPEN,businessid=149428195245109,session=" + cli.m_session + "}"); 
        if (cli.m_lastresponse == null) return;

        JsonArray transactionid = cli.m_lastresponse.getJsonArray("transactionid"); 
        JsonArray transtype     = cli.m_lastresponse.getJsonArray("type"); 
        JsonArray description   = cli.m_lastresponse.getJsonArray("description"); 
        JsonArray notsettled    = cli.m_lastresponse.getJsonArray("notsettled"); 
        JsonArray notallocated  = cli.m_lastresponse.getJsonArray("notallocated"); 
        JsonArray notdelivered  = cli.m_lastresponse.getJsonArray("notdelivered"); 

        for (int i=0;i<transtype.size();i++)
            System.out.println(transactionid.getString(i) + "\t" + transtype.getString(i) + "\t" + description.getString(i));

        //settle inputs
        System.out.println("Enter transactions to settle: ");
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        String ids = reader.readLine();
        if (ids.length()>0)
        {
            String[] idarr = ids.split(",");
            for (int i=0;i<idarr.length;i++)
                settle(cli,idarr[i],transactionid,notsettled);
        }

        logcli.logoutuser();
        logfuture.cancel(true);
        logcli.close();
        future.cancel(true);
        cli.close();
        executor.shutdown();

        Thread.sleep(2000);
	}
}
