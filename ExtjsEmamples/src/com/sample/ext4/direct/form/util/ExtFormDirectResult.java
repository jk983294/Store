package com.sample.ext4.direct.form.util;

import java.util.HashMap;
import java.util.Map;

public class ExtFormDirectResult {
	private Object data;
	private String errorMessage;
	private boolean success;
	private Map<String, String> errors = new HashMap<String, String>();
	
	public String getErrorMessage() {
		return errorMessage;
	}
	public void setErrorMessage(String errorMessage) {
		this.errorMessage = errorMessage;
	}
	public Object getData() {
		return data;
	}
	public void setData(Object data) {
		this.data = data;
	}
	public boolean isSuccess() {
		return success;
	}
	public void setSuccess(boolean success) {
		this.success = success;
	}
	public void addError(String key, String value){
    	errors.put(key, value);
    }
	public Map<String, String> getErrors() {
		return errors;
	}
}
