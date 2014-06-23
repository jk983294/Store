package dao.db;
/**
 * 固定数量的参数，用于填补PreparedStatement中的占位符
 * @author Jersey
 * @date 2011-4-19
 */
public class FixedParameters implements Parameters {
	private MySqlParameter[] parms;
	private int ptr = 0;

	public FixedParameters(int size) {
		parms = new MySqlParameter[size];
	}

	public void add(MySqlParameter parm) {
		parms[ptr] = parm;
		ptr++;
	}

	public MySqlParameter[] getBuffer() {
		return parms;
	}

	public int getLength() {
		return parms.length;
	}

	public MySqlParameter getParameter(int i) {
		return parms[i];
	}
	
	public boolean needBatch(){
		return false;
	}
}
