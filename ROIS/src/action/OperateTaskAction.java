package action;

import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Calendar;

import support.OperationType;
import support.TaskState;
import support.ThreadPoolManager;
import util.TimeUtil;
import backStage.worker.ReportOperation;
import business.entityBeans.TaskBean;
import business.entityBeans.TaskEntpBean;
import business.managers.TaskManager;

public class OperateTaskAction extends BaseAction {

	private static final long serialVersionUID = 1969529192540035731L;
	private TaskBean task;
	private String tid;

	public String bid() {
		// 0.根据TID获取TASK对象
		TaskManager tm = new TaskManager();
		try {
			task = tm.queryTaskByTid(tid);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
			return ERROR;
		}
		// 1.检查是否可以Bid（bidDeadline） 2.写DB
		Calendar cal = Calendar.getInstance();
		Timestamp now = new Timestamp(cal.getTime().getTime());
		if (task.getBidDeadline().after(now)) {
			String eid = (String) getSession().get("eid");
			TaskEntpBean te = new TaskEntpBean(task.getTid(), eid, now);
			try {
				if (tm.biddingExist(te)) {
					this.addMessage("taskListMsg", "任务项[" + task.getTid()
							+ "]已经申请过啦，请不要重复竞标！");
				} else {
					tm.biddingTask(te);
					this.addMessage("taskListMsg", "任务项[" + task.getTid()
							+ "]发出竞标申请！");
				}
			} catch (SQLException e) {
				System.err.println(e.getMessage());
				return ERROR;
			}

		} else {

			this.addMessage("taskListMsg", "任务项[" + task.getTid()
					+ "]竞标截止日期已过！");
		}
		return SUCCESS;
	}

	/**
	 * 0.检查合法性（STATE = ACCEPTED && EID=EID）.
	 * 1.修改本地DB.
	 * 2.调用后台线程报告给服务器.
	 * @return
	 */
	public String reportStart() {		
		TaskManager tm = new TaskManager();
		String eid=(String) getSession().get("eid");
		try {
			task = tm.queryTaskByTid(tid);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
			return ERROR;
		}
		if(task.getState()!=TaskState.ACCEPTED ||  !task.getEid().equals(eid)){
			this.addMessage("myTaskListMsg", "任务项[" + task.getTid()
					+ "]状态更改失败！不是您的任务或者任务还在竞标中！");
			return SUCCESS;
		}
		//更新DB
		try {
			task.setStartTime(TimeUtil.getNowTimestamp());
			tm.updateTaskState(tid, TaskState.RUNNING);
		} catch (SQLException e) {
			return ERROR;
		}
		//调用后台发送报告		
		ThreadPoolManager.execute(new ReportOperation(tid,OperationType.START));
		this.addMessage("myTaskListMsg", "任务项[" + task.getTid()
							+ "]状态更改成功！");
		return SUCCESS;
	}
	/**
	 * 0.检查合法性（STATE==RUNNING && EID==EID）显示时已经处理。
	 * 1.更新本地DB
	 * 2.调用后台线程报告给服务器
	 * @return
	 */
	public String reportFinish(){
		TaskManager tm=new TaskManager();
		String eid=(String) getSession().get("eid");
		try {
			task = tm.queryTaskByTid(tid);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
			return ERROR;
		}
		if(task.getState()!=TaskState.RUNNING || !task.getEid().equals(eid)){
			this.addMessage("myTaskListMsg", "任务项[" + task.getTid()
					+ "]状态更改失败！不是您的任务或者任务没开始！");
			return SUCCESS;
		}
		//更新DB
		try {
			tm.updateTaskState(tid, TaskState.FINISHED);
		} catch (SQLException e) {
			return ERROR;
		}
				
		ThreadPoolManager.execute(new ReportOperation(tid,OperationType.FINISH));
		this.addMessage("myTaskListMsg", "任务项[" + task.getTid()
							+ "]状态更改成功！");
		return SUCCESS;
	}
	public void setTask(TaskBean task) {
		this.task = task;
	}

	public TaskBean getTask() {
		return task;
	}

	public void setTid(String tid) {
		this.tid = tid;
	}

	public String getTid() {
		return tid;
	}

}
