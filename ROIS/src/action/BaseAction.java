package action;

import java.util.HashMap;
import java.util.Map;

import org.apache.struts2.interceptor.SessionAware;

import com.opensymphony.xwork2.ActionSupport;

public class BaseAction extends ActionSupport implements SessionAware {//获得Session
//ActionSupport已经实现了Action接口，还实现了Validateable接口，提供了数据校验功能
	private static final long serialVersionUID = 1L;
	private Map<String, Object> session;
	private static Map<String,String> messageMap;
	@Override
	public void setSession(Map<String, Object> session) {
		this.session = session;
	}
	public Map<String,Object> getSession(){
		return this.session;
	}
	
	public synchronized void addMessage(String key, String msg) {
		if(messageMap==null)
			messageMap=new HashMap<String,String>();
		messageMap.put(key, msg);
		
	}
	public synchronized String getMessage(String key){
		if(messageMap==null)
			return "";
		return messageMap.remove(key);
	}
	
}
