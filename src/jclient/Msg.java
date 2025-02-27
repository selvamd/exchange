import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Msg {

    private List<Object> fields = new ArrayList<Object>();
    private int type;
    public Msg(int typ) {
       type = typ;
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

    public byte[] tobytes() {
        int len = 0;
        for (Object o : fields) {
            if (o instanceof byte[]) len += ((byte[])o).length;
            if (o instanceof Integer) len += 4;
            if (o instanceof Long) len += 8;
        }
        len += 8; //type + msgsize
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

    public void parse(byte[] bo) {
        ByteBuffer buf = ByteBuffer.wrap(bo);
        System.out.println("Type=" + buf.getInt());
        System.out.println("Size=" + buf.getInt());
        for (Object o:fields) {
            if (o instanceof Integer)
                System.out.println("intfield=" + buf.getInt());
            if (o instanceof Long)
                System.out.println("longfield=" + buf.getLong());
            if (o instanceof byte[]) {
                byte[] b = (byte[]) o;
                buf.get(b);
                System.out.println("Stringfld="+new String(b, StandardCharsets.UTF_8));        
            }
        }
    }
}

