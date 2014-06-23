package business.managers;

import java.sql.SQLException;
import java.util.List;

import support.TaskState;
import business.convertor.TaskEntpListConvertor;
import business.convertor.TaskListConvertor;
import business.entityBeans.TaskBean;
import business.entityBeans.TaskEntpBean;
import dao.TaskDao;
import dao.db.DBType;
import dao.db.FixedParameters;
import dao.db.MySqlParameter;
import dao.db.Parameters;

public class TaskManager extends BaseManager implements TaskDao {
	@Override
	public int biddingTask(TaskEntpBean te) throws SQLException {
		String sql = "INSERT INTO MAP_TASK_ENTP (tid,eid,bidtime) VALUES(?,?,?)";
		Parameters parms = new FixedParameters(3);
		parms.add(new MySqlParameter(DBType.String, te.getTid()));
		parms.add(new MySqlParameter(DBType.Int32, te.getEid()));
		parms.add(new MySqlParameter(DBType.DateTime, te.getBidTime()));
		return getDb().ExecuteNoneQuery(sql, parms);
	}

	public boolean biddingExist(TaskEntpBean te) throws SQLException {
		String sql = "Select * FROM MAP_TASK_ENTP WHERE TID=? AND EID=?";
		Parameters parms = new FixedParameters(2);
		parms.add(new MySqlParameter(DBType.String, te.getTid()));
		parms.add(new MySqlParameter(DBType.Int32, te.getEid()));
		return getDb().ExecuteScalar(sql, parms) != null;

	}

	@Override
	public int DeleteBidding(String tid) throws SQLException {
		String sql = "DELETE FROM MAP_TASK_ENTP WHERE TID=? ";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.String, tid));
		return getDb().ExecuteNoneQuery(sql, parms);
	}

	@Override
	public int addTask(TaskBean tb) throws SQLException {
		String sql = "INSERT INTO TTask "
				+ "(tid,SID,EID,PUBTIME,STARTTIME,ENDTIME,DEADLINETIME,BIDDEADLINE,STATE,PARAMXML)"
				+ " VALUES(?,?,?,?,?,?,?,?,?,?)";
		Parameters params = new FixedParameters(10);
		params.add(new MySqlParameter(DBType.String, tb.getTid()));
		params.add(new MySqlParameter(DBType.Int32, tb.getSid()));
		params.add(new MySqlParameter(DBType.Int32, tb.getEid()));
		params.add(new MySqlParameter(DBType.DateTime, tb.getPubTime()));
		params.add(new MySqlParameter(DBType.DateTime, tb.getStartTime()));
		params.add(new MySqlParameter(DBType.DateTime, tb.getEndTime()));
		params.add(new MySqlParameter(DBType.DateTime, tb.getDeadlineTime()));
		params.add(new MySqlParameter(DBType.DateTime, tb.getBidDeadline()));
		params.add(new MySqlParameter(DBType.String, tb.getState()));
		params.add(new MySqlParameter(DBType.String, tb.getUrl()));
		return getDb().ExecuteNoneQuery(sql, params);
	}

	@Override
	public int deleteTask(TaskBean tb) throws SQLException {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public List<TaskBean> querySubTaskByEid(String eid) throws SQLException {
		String sql = "SELECT T.TID,T.SID,S.SNAME,T.EID,T.PUBTIME,T.STARTTIME,T.ENDTIME,T.DEADLINETIME,T.BIDDEADLINE,T.STATE,T.PARAMXML FROM TTASK T,TSERVICES S WHERE T.SID=S.SID AND T.STATE='INITIATE' AND T.SID IN (SELECT SID FROM MAP_SRV_ENTP WHERE EID=?)";
		Parameters parm = new FixedParameters(1);
		parm.add(new MySqlParameter(DBType.Int32, eid));
		return getDb().ExecuteObject(new TaskListConvertor(), sql, parm);
	}

	@Override
	public List<TaskBean> checkBidDeadline() throws SQLException {
		String sql = "SELECT * FROM TTASK WHERE BIDDEADLINE < NOW() AND STATE='INITIATE'";
		return this.getDb().ExecuteObject(new TaskListConvertor(), sql, null);
	}

	@Override
	public List<TaskEntpBean> queryTaskEntpByEid(String eid)
			throws SQLException {
		String sql = "SELECT * FROM MAP_TASK_ENTP WHERE EID=?";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.Int32, eid));
		return getDb().ExecuteObject(new TaskEntpListConvertor(), sql, parms);
	}

	@Override
	public List<TaskEntpBean> queryTaskEntpByTid(String tid)
			throws SQLException {
		String sql = "SELECT * FROM MAP_TASK_ENTP WHERE TID=?";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.String, tid));
		return getDb().ExecuteObject(new TaskEntpListConvertor(), sql, parms);
	}

	@Override
	public List<TaskBean> queryMyTaskByEid(String eid) throws SQLException {
		String sql = "SELECT * FROM TTASK T LEFT JOIN TSERVICES S ON T.SID=S.SID WHERE EID=? OR TID IN (SELECT TID FROM MAP_TASK_ENTP WHERE EID=?)";
		Parameters parms = new FixedParameters(2);
		parms.add(new MySqlParameter(DBType.Int32, eid));
		parms.add(new MySqlParameter(DBType.Int32, eid));
		return getDb().ExecuteObject(new TaskListConvertor(), sql, parms);
	}

	@Override
	public TaskBean queryTaskByTid(String tid) throws SQLException {
		String sql = "SELECT * FROM TTASK T WHERE TID=?";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.String, tid));
		List<TaskBean> list = getDb().ExecuteObject(new TaskListConvertor(),
				sql, parms);
		return list.size() == 0 ? null : list.get(0);
	}

	@Override
	public int updateTaskState(String tid, TaskState state) throws SQLException {
		String sql = "UPDATE TTask SET STATE = ? WHERE TID=?";
		if (state == TaskState.RUNNING) {
			sql = "UPDATE TTask SET STATE = ? ,STARTTIME=NOW() WHERE TID=?";
		} else if (state == TaskState.FINISHED) {
			sql = "UPDATE TTask SET STATE = ? ,ENDTIME=NOW() WHERE TID=?";
		}
		Parameters parms = new FixedParameters(2);
		parms.add(new MySqlParameter(DBType.String, state));
		parms.add(new MySqlParameter(DBType.String, tid));
		return getDb().ExecuteNoneQuery(sql, parms);

	}

	@Override
	public int updateTaskOnwer(String tid, String eid) throws SQLException {
		String sql = "UPDATE TTASK SET EID=?,STATE='ACCEPTED' WHERE TID=?";
		Parameters parms = new FixedParameters(2);
		parms.add(new MySqlParameter(DBType.String, eid));
		parms.add(new MySqlParameter(DBType.String, tid));
		return getDb().ExecuteNoneQuery(sql, parms);
	}
}
