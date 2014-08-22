package com.victor.midas.model;

import java.util.Date;

import org.springframework.data.annotation.Id;

import com.victor.midas.worker.TaskStatus;

//task view object
public class Task {
	@Id
	private String id;
	private String desc;					//worker description
	private Date submit;					//in MongoDB it stores no time zone information
	private Date finish;
	private TaskStatus status;
	
	public Task(String desc) {
		this.desc = desc;
		submit = new Date(System.currentTimeMillis());
		status = TaskStatus.Submit;
	}
	
	public String getDesc() {
		return desc;
	}
	public void setDesc(String desc) {
		this.desc = desc;
	}
	public Date getSubmit() {
		return submit;
	}
	public void setSubmit(Date submit) {
		this.submit = submit;
	}
	public Date getFinish() {
		return finish;
	}
	public void setFinish(Date finish) {
		this.finish = finish;
	}
	public TaskStatus getStatus() {
		return status;
	}
	public void setStatus(TaskStatus status) {
		this.status = status;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}

	@Override
	public String toString() {
		return "Task [desc=" + desc + ", submit=" + submit + ", finish="
				+ finish + ", status=" + status + "]";
	}
	
}
