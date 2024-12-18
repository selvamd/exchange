import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import javax.json.*;

public class MsgClient implements Runnable
{
    private static String ENDL = "\n";
    private final Socket socket;
    private final BufferedReader input;
    private final BufferedWriter output;
    public String m_session;
    public JsonObject m_lastresponse;
    private boolean m_processing;

    public MsgClient(String host, int port) throws Exception
    {
        socket  = new Socket(host,port);
        socket.setSoTimeout(100);
        input   = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        output  = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
    }

    public void close() throws Exception 
    {
        input.close();
        output.close();
        socket.close();
    }

    public boolean writeObj(JsonObject obj) throws Exception 
    {
        StringWriter stWriter = new StringWriter();
        JsonWriter jsonWriter = Json.createWriter(stWriter);
        jsonWriter.writeObject(obj);
        jsonWriter.close();
        String json = stWriter.toString();
        m_processing = true;
        String error = "";
        output.write(json,0,json.length());
        output.write(ENDL,0,ENDL.length());
        output.flush();
        while (m_processing)
            Thread.sleep(20);
        return ("None".equals(error));
    }

    public boolean write(String json) throws Exception 
    {
        json = tojson(json);        
        m_processing = true;
        output.write(json,0,json.length());
        output.write(ENDL,0,ENDL.length());
        output.flush();
        while (m_processing)
            Thread.sleep(20);
        return ("None".equals(response("error")));
    }

    public void run()
    {
        try { read(); } 
       catch (Exception e) {}
    }

    public String response(String field)
    {
        if (m_lastresponse == null) return "";
        if (field == null) return "";
        if (m_lastresponse.getJsonString(field)==null) return "";
        return m_lastresponse.getString(field);
    }

    public void read() throws Exception
    {
        String str;
        while ((str = input.readLine())!=null)
        {
            JsonObject json = (JsonObject)Json.createReader(new StringReader(str)).read(); 

            if (json.getJsonString("session") != null)
                m_session = json.getString("session");
            System.out.println(str);
            m_processing = false;
            m_lastresponse = json;
        }
     }

    private static String tojson(String str)
    {
        int lvl = 0;
        String result = "", jsonsubstr = "";
        if (str == null || str.length() == 0) return "";
        if (str.charAt(0) == '{')
        {
            for (String s:str.substring(1, str.length()-1).split(","))
            {
                jsonsubstr += (jsonsubstr.length()>0)? ","+s:s;
                for (int i=0;i<s.length();i++) 
                    if (s.charAt(i)=='{'||s.charAt(i)=='[') lvl++;
                    else if (s.charAt(i)=='}'||s.charAt(i)==']') lvl--;
                if (lvl != 0) continue;
                result += tojson(jsonsubstr);
                jsonsubstr = "";
            }
            return "{" + result.substring(1) + "}";
        }

        if (str.charAt(0)=='[')
        {
            for (String s:str.substring(1, str.length()-1).split(","))
            {
                jsonsubstr += (jsonsubstr.length()>0)? ","+s:s;
                for (int i=0;i<s.length();i++) 
                    if (s.charAt(i)=='{'||s.charAt(i)=='[') lvl++;
                    else if (s.charAt(i)=='}'||s.charAt(i)==']') lvl--;
                if (lvl != 0) continue;
                result += "," + tojson(jsonsubstr);
                jsonsubstr = "";
            }
            return "[" + result.substring(1) + "]";
        }
        int fld = str.indexOf("=");
        if (fld == -1) return "\"" + str + "\"";
        return ",\"" + str.substring(0,fld) + "\":" + tojson(str.substring(1+fld));
    }

    //////////////////////////////////////////////////////////  WRITE FUNCTIONS /////////////////////////////////////////

    public boolean loginuser(String usr, String pwd) throws Exception { 
        return write("{Event=USER_LOGIN,name=" + usr + ",pwd=" + pwd + "}");        
    }

    public void registeruser(String usr, String pwd) throws Exception {
        write("{Event=USER_REGISTER,name=" + usr + ",pwd=" + pwd + "}");        
    }

    public void registersvc(String usr, String pwd, String oldpwd, String[] roles) throws Exception {
        String role = null; 
        for (int i=0;i<roles.length;i++) 
            role = (role==null)? roles[i]:role+","+roles[i];
        write("{Event=SVC_REGISTER,roles=[" + role + "],name=" + usr + ",pwd=" + pwd + "}");        
    }

    public void authorize(String usr, String[] roles) throws Exception { 
        String role = null; 
        for (int i=0;i<roles.length;i++)
            role = (role==null)? roles[i]:role+","+roles[i];
        write("{Event=AUTHORIZE,roles=[" + role + "],user=" + usr + "}");        
    }

    public void send(String evt) throws Exception { 
        String json = "{\"Event\":\"" +evt+ "\"}";        
        output.write(json,0,json.length());
        output.write(ENDL,0,ENDL.length());
        output.flush();
    }

    public void stockQuery(String fields, String filter, String sort, int offset, int count) throws Exception { 
        JsonObjectBuilder bldr = Json.createObjectBuilder();
        bldr.add("Event","STOCK_QUERY");
        bldr.add("session",m_session);
        bldr.add("fields",fields);
        bldr.add("filter",filter);
        bldr.add("sort",sort);
        bldr.add("count",""+count);
        bldr.add("offset",""+offset);
        writeObj(bldr.build());
    }

    public void logoutuser() throws Exception { 
        JsonObjectBuilder bldr = Json.createObjectBuilder();
        bldr.add("Event","USER_LOGOUT");
        bldr.add("session",m_session);
        writeObj(bldr.build());
    }

}
