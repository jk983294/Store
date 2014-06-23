package business.entityBeans;

public class ParameterBean {
	//private String name;
	private float min;
	private float max;
	private String unit;
	private boolean tendency;//1：正  0：逆 
	private boolean type;//1:数值型  0：枚举型
		
	public ParameterBean(float min,float max,String unit,boolean tendency,boolean type) {
		//this.name = name;
		this.min = min;
		this.max = max;
		this.unit = unit;
		this.tendency = tendency;
		this.type = type;
	}
	
	public float getMin() {
		return min;
	}
	public void setMin(float min) {
		this.min = min;
	}
	public float getMax() {
		return max;
	}
	public void setMax(float max) {
		this.max = max;
	}
	public String getUnit() {
		return unit;
	}
	public void setUnit(String unit) {
		this.unit = unit;
	}
	public boolean isType() {
		return type;
	}
	public void setType(boolean type) {
		this.type = type;
	}
	public boolean isTendency() {
		return tendency;
	}
	public void setTendency(boolean tendency) {
		this.tendency = tendency;
	}

	@Override
	public String toString() {
		return "ParameterBean [min=" + min + ", max=" + max + ", unit=" + unit
				+ ", tendency=" + tendency + ", type=" + type + "]";
	}
	
}
