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

public class Test
{

    //{b=c,d=e,f=[g|h|i],j=[{1=2,3=4}|{5=6,7=8}],k={a=1,b=c},l=[[1|2],[3|4],[5|6]]}
    private static String tojson(String str)
    {
        int lvl = 0;
        String result = "", jsonsubstr = "";
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
        int fld = str.indexOf(":");
        if (fld == -1) return "\"" + str + "\"";
        return ",\"" + str.substring(0,fld) + "\":" + tojson(str.substring(1+fld));
    }


    //Create new business and accounts 
    public static void main(String[] args) throws Exception
	{
        //String ENDL = "\n";
        //ServerSocket srv = new ServerSocket(8000);
        //String json = tojson("{b:c,d:e,f:[g,h,i],j:[{1:2,3:4},{5:6,7:8}],k:{a:1,b:c},l:[[1,2],[3,4],[5,6]]}");
        ProcessBuilder pb;
        String dir = "/Users/selvamdoraisamy/Desktop/personal/accounts/bin";

        pb = new ProcessBuilder("./loginsvc");
        pb.directory(new File(dir));
        pb.start();

        pb = new ProcessBuilder("./gatewaysvc");
        pb.directory(new File(dir));
        pb.start();

        pb = new ProcessBuilder("./accountsvc");
        pb.directory(new File(dir));
        pb.start();
	}

}
