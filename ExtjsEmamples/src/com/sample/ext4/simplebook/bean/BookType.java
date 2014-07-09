package com.sample.ext4.simplebook.bean;

public class BookType {
	private int id;
	private String title;
	private String detail;
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getDetail() {
		return detail;
	}
	public void setDetail(String detail) {
		this.detail = detail;
	}
	/**
	 * Constructs a <code>String</code> with all attributes
	 * in name = value format.
	 *
	 * @return a <code>String</code> representation
	 * of this object.
	 */
	public String toString()
	{
	    final String TAB = "\n";

	    StringBuffer retValue = new StringBuffer();

	    retValue.append("BookType ( ")
	        .append(super.toString()).append(TAB)
	        .append("id = ").append(this.id).append(TAB)
	        .append("title = ").append(this.title).append(TAB)
	        .append("detail = ").append(this.detail).append(TAB)
	        .append(" )");

	    return retValue.toString();
	}

}
