import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Msg {

    private List<Object> fields = new ArrayList<Object>();
    private int type;
    public Msg(int typ) {
       type = typ;
    }

    public int type() { return type; }

    public static Msg newmsg(int typ) {
        // gen_msg = 0,nbbo_msg = 1,timer_msg = 2,config_msg = 3,
        // new_order = 4,replace_order = 5,cancel = 6,
        // exec_report = 7,cancel_reject = 8
        return new Msg(typ);
    }

    public static Msg create(int typ) {
        Msg msg = new Msg(typ);
        msg = msg.addl(100).adds("GWY01",10).adds("ME01",10);
        if (typ == 0) return msg.addi(0);
        if (typ == 4) {
            String cols = "OnBehalfOfCompId,DeliverToCompId,OnBehalfOfLocationId,DeliverToLocationId,PartyId,PartySubId,Account";
            for (String col:cols.split(",")) msg.adds("",10); //COMP_ID_LEN
            msg.adds("NVDA",40).adds("123",30); //symbol,clordid
            cols = "ExecInst,OrdType,Side,TimeInForce,AutoCancel,MinQty,PossResend";       
            for (String col:cols.split(",")) msg.addi(0);
            msg.addl(0).addl(0).addl(0); //qty,price,inviteid 
            cols = "ClientType,ConditionalEligible,OrderLife,OrderUrgency,ContraCategory,RiskTier,SelfTradeInst,MinQtyInst,OrdTypeExt";
            for (String col:cols.split(",")) msg.addi(0);
        }
        if (typ == 7) {
            String cols = "OnBehalfOfCompId,DeliverToCompId,OnBehalfOfLocationId,DeliverToLocationId,PartyId,PartySubId,Account";
            for (String col:cols.split(",")) msg.adds("",10); //COMP_ID_LEN
            //clord,origclord,sym,secrityid,session,text
            msg.adds("",30).adds("",30).adds("",40).adds("",40).adds("",4).adds("",100);
            cols = "OrderQty,SendingTime,StopPx,MaxFloor,ExpireTime,AvgPx,CumQty,ExecId,ExecRefId,LastPx,LastShares,OrderId,TradeId,TransactTime,Price";
            for (String col:cols.split(",")) msg.addl(0); 
            cols = "ExecInst,OrdType,Side,TimeInForce,PossResend,AutoCancel,ClientType,NoTradingSessions,ExecType,OrdStatus,OpenClose,OrdRejReason";
            for (String col:cols.split(",")) msg.addi(0); 
        }
        return msg;
    }

    public Msg addi(int val) { fields.add(new Integer(val));return this; }   
    public Msg addl(long val) { fields.add(new Long(val));return this; }   
    public Msg adds(String val,int len) {
        int pos = val.length();
        while (val.length() < len) val += " ";
        val += " ";
        byte[] b = val.getBytes(StandardCharsets.UTF_8);
        b[pos] = 0;
        fields.add(b);
        return this;
    }   

    public int size() {
        int len = 0;
        for (Object o : fields) {
            if (o instanceof byte[]) len += ((byte[])o).length;
            if (o instanceof Integer) len += 4;
            if (o instanceof Long) len += 8;
        }
        len += 8; //type + msgsize
        return len;
    }

    public byte[] tobytes() {
        int len = size();
        ByteBuffer bytes = ByteBuffer.allocate(len);
        bytes.putInt(type);
        bytes.putInt(len);
        for (int i = 0;i < fields.size();i++) {
            if (fields.get(i) instanceof byte[]) 
                bytes.put((byte[])fields.get(i));
            else if (fields.get(i) instanceof Long)
                bytes.putLong((Long)fields.get(i));
            else if (fields.get(i) instanceof Integer)
                bytes.putInt((Integer)fields.get(i));
        }
        return bytes.array();
    }

    public void print() {
        System.out.println("Type=" + type);
        System.out.println("Size=" + size());
        for (Object o:fields) {
            if (o instanceof Integer)
                System.out.println("intfield=" + o);
            if (o instanceof Long)
                System.out.println("longfield=" + o);
            if (o instanceof byte[]) {
                byte[] b = (byte[]) o;
                System.out.println("Stringfld="+new String(b, StandardCharsets.UTF_8));        
            }
        }
    }

    public void parse(byte[] bo) {
        List<Object> out = new ArrayList<Object>();
        ByteBuffer buf = ByteBuffer.wrap(bo);
        buf.getInt(); buf.getInt();
        for (Object o:fields) {
            if (o instanceof Integer)
                out.add(new Integer(buf.getInt()));
            if (o instanceof Long)
                out.add(new Long(buf.getLong()));
            if (o instanceof byte[]) {
                byte[] b = (byte[]) o;
                buf.get(b);
                out.add(b);
            }
        }
        fields = out;
    }
}

