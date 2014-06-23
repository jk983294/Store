package dao.db;

public class BatchedParameters implements Parameters{
	private FixedParameters[] parms;
	private int size;
	private int ptr = 0;

	public BatchedParameters(int size,int batchTimes) {
		this.size=size;
		parms=new FixedParameters[batchTimes];
	}
	@Override
	public void add(MySqlParameter parm) {
		if(ptr%size==0){//need a new FiexedParameters
			FixedParameters fp=new FixedParameters(size);
			parms[ptr/size] = fp;
		}
		//��ȡ����Ҫ�����FixedParameters
		FixedParameters f=parms[ptr/size];
		f.add(parm);
		ptr++;
	}

	@Override
	public int getLength() {
		return parms.length;
	}
	/**
	 * ������ص���FixedParametersŶ
	 * @param i
	 * @return 
	 */
	@Override
	public FixedParameters getParameter(int i) {
		return parms[i];
	}
	
	@Override
	public boolean needBatch(){
		return true;
	}

}
