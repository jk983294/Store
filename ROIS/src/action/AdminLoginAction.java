package action;

public class AdminLoginAction extends BaseAction{

	private static final long serialVersionUID = 5588836496177305089L;
	private String username;
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	private String password;
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	
	@Override
	public void validate(){
		if(null==username || "".equals(username.trim()))
			this.addFieldError("username", "Should not be empty username!");
		if(null==password || "".equals(password.trim()))
			this.addFieldError("password", "Should not be empty password!");
		
	}
	@Override
	public String execute(){
		boolean valid=true;
		if(!"admin".equals(username) ){
			valid=false;
			this.addFieldError("username", "Wrong username!");
		}
		if( !"p@ssword".equals(password)){
			valid=false;
			this.addFieldError("password", "Wrong password!");
		}
		if(valid){
			this.getSession().put("admin", username);
		}
		return valid?SUCCESS:INPUT;
	}
}
