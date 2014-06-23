package business.managers;

import java.sql.SQLException;
import java.util.List;

import dao.MessageDao;
import dao.db.DBType;
import dao.db.FixedParameters;
import dao.db.MySqlParameter;
import dao.db.Parameters;

import support.message.MsgQueueMapImpt;
import business.convertor.MsgListConvertor;
import business.entityBeans.MessageBean;
/**
 * 针对TMessages表的增删改查的管理类
 * @author Jersey
 * @date 2011-4-19
 */
public class MessageManager extends BaseManager implements MessageDao {
	private static MsgQueueMapImpt msgQ = null;
	public MsgQueueMapImpt getMsgQueue(){
		if(null==msgQ){
			msgQ=new MsgQueueMapImpt();
		}
		return msgQ;
	}
	@Override
	public int addMessage(MessageBean mb) throws SQLException {
		String sql="INSERT INTO TMessages (EID,TYPE,CONTENT,VIEWED) VALUES(?,?,?,?)";
		Parameters parms=new FixedParameters(4);
		parms.add(new MySqlParameter(DBType.Int32,mb.getEid()));
		parms.add(new MySqlParameter(DBType.String,mb.getType()));
		parms.add(new MySqlParameter(DBType.String,mb.getContent()));
		parms.add(new MySqlParameter(DBType.Boolean,mb.isViewed()));
		return getDb().ExecuteNoneQuery(sql, parms);
	}
	@Override
	public List<MessageBean> queryByEid(String eid) throws SQLException {
		String sql=" SELECT * FROM TMESSAGES WHERE EID=? ORDER BY MID DESC";
		Parameters parms=new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.Int32,eid));
		return getDb().ExecuteObject(new MsgListConvertor(), sql, parms);
	}
	@Override
	public int updateMessage(MessageBean mb) throws SQLException {
		// TODO Auto-generated method stub
		return 0;
	}
}
