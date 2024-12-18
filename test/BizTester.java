import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

public class BizTester extends MsgClient
{
    public String business; 
    public String owner; 
    public String client; 
    public String costcomponent1; 
    public String costcomponent2; 
    public String costcenter1; 
    public String costcenter2; 
    public String transaction; 

    public BizTester(String host, int port) throws Exception
    {
        super(host, port);   
    }   

    public static void test_accountsvc(BizTester cli) throws Exception
    {
        cli.write("{Event=CRUD_BUSINESS,operation=CREATE,name=AccPacc investments,session=" + cli.m_session + "}"); 
        cli.business = cli.response("businessid");

        cli.write("{Event=CRUD_CLIENT,operation=CREATE,name=Selvam capital,businessid="+ cli.business +",session=" + cli.m_session + "}"); 
        cli.owner = cli.response("clientid");

        cli.write("{Event=CRUD_CLIENT,operation=CREATE,name=Barathi stores,businessid="+ cli.business +",session=" + cli.m_session + "}"); 
        cli.client = cli.response("clientid");

        cli.write("{Event=CRUD_COSTCOMPONENT,operation=CREATE,name=Food,type=PRODUCT,businessid="+ cli.business +",session=" + cli.m_session + "}"); 
        cli.costcomponent1 = cli.response("costcomponentid");

        cli.write("{Event=CRUD_COSTCOMPONENT,operation=CREATE,name=Clothing,type=PRODUCT,businessid="+ cli.business +",session=" + cli.m_session + "}"); 
        cli.costcomponent2 = cli.response("costcomponentid");

        cli.write("{Event=CRUD_COSTCENTER,operation=CREATE,name=RKPD Maintenance,businessid="+ cli.business +",session=" + cli.m_session + "}"); 
        cli.costcenter1 = cli.response("costcenterid");

        cli.write("{Event=CRUD_COSTCENTER,operation=CREATE,name=DKVS Maintenance,businessid="+ cli.business +",session=" + cli.m_session + "}"); 
        cli.costcenter2 = cli.response("costcenterid");

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
    }

    //Create new business and accounts 
    public static void main(String[] args) throws Exception
	{
        ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

        String host = "localhost";
        BizTester logcli = new BizTester(host, 65000);
        ScheduledFuture logfuture = executor.scheduleWithFixedDelay(logcli, 0, 50, TimeUnit.MILLISECONDS);
        logcli.loginuser("Dorai","dorai");
        while (true)
        {
            Thread.sleep(100);
            if (logcli.m_session != null) 
                break;
        }

        BizTester cli = new BizTester(host, 65100);
        cli.m_session = logcli.m_session;
        ScheduledFuture future = executor.scheduleWithFixedDelay(cli, 0, 50, TimeUnit.MILLISECONDS);

        //test_accountsvc(cli);
        cli.write("{Event=GET_BALANCESHEET,businessid=149428195245109,session=" + cli.m_session + "}"); 
        //cli.write("{Event=HEARTBEAT}"); 

        logcli.logoutuser();
        logfuture.cancel(true);
        logcli.close();
        future.cancel(true);
        cli.close();
        executor.shutdown();

        System.out.println("\n\n>>>>>>>>>>>>> ALL TESTS RAN SUCCESSFULLY <<<<<<<<<<<<<<<\n\n");

        Thread.sleep(2000);
	}
}
