package dao.db;

public interface Parameters {
	
	void add(MySqlParameter parm);

	int getLength();

	Object getParameter(int i);
	
	boolean needBatch();

}
