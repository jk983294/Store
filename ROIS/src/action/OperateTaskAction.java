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
		// 0.����TID��ȡTASK����
		TaskManager tm = new TaskManager();
		try {
			task = tm.queryTaskByTid(tid);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
			return ERROR;
		}
		// 1.����Ƿ����Bid��bidDeadline�� 2.дDB
		Calendar cal = Calendar.getInstance();
		Timestamp now = new Timestamp(cal.getTime().getTime());
		if (task.getBidDeadline().after(now)) {
			String eid = (String) getSession().get("eid");
			TaskEntpBean te = new TaskEntpBean(task.getTid(), eid, now);
			try {
				if (tm.biddingExist(te)) {
					this.addMessage("taskListMsg", "������[" + task.getTid()
							+ "]�Ѿ�����������벻Ҫ�ظ����꣡");
				} else {
					tm.biddingTask(te);
					this.addMessage("taskListMsg", "������[" + task.getTid()
							+ "]�����������룡");
				}
			} catch (SQLException e) {
				System.err.println(e.getMessage());
				return ERROR;
			}

		} else {

			this.addMessage("taskListMsg", "������[" + task.getTid()
					+ "]�����ֹ�����ѹ���");
		}
		return SUCCESS;
	}

	/**
	 * 0.���Ϸ��ԣ�STATE = ACCEPTED && EID=EID��.
	 * 1.�޸ı���DB.
	 * 2.���ú�̨�̱߳����������.
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
			this.addMessage("myTaskListMsg", "������[" + task.getTid()
					+ "]״̬����ʧ�ܣ���������������������ھ����У�");
			return SUCCESS;
		}
		//����DB
		try {
			task.setStartTime(TimeUtil.getNowTimestamp());
			tm.updateTaskState(tid, TaskState.RUNNING);
		} catch (SQLException e) {
			return ERROR;
		}
		//���ú�̨���ͱ���		
		ThreadPoolManager.execute(new ReportOperation(tid,OperationType.START));
		this.addMessage("myTaskListMsg", "������[" + task.getTid()
							+ "]״̬���ĳɹ���");
		return SUCCESS;
	}
	/**
	 * 0.���Ϸ��ԣ�STATE==RUNNING && EID==EID����ʾʱ�Ѿ�����
	 * 1.���±���DB
	 * 2.���ú�̨�̱߳����������
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
			this.addMessage("myTaskListMsg", "������[" + task.getTid()
					+ "]״̬����ʧ�ܣ��������������������û��ʼ��");
			return SUCCESS;
		}
		//����DB
		try {
			tm.updateTaskState(tid, TaskState.FINISHED);
		} catch (SQLException e) {
			return ERROR;
		}
				
		ThreadPoolManager.execute(new ReportOperation(tid,OperationType.FINISH));
		this.addMessage("myTaskListMsg", "������[" + task.getTid()
							+ "]״̬���ĳɹ���");
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
