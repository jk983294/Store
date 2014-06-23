package util;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

public class TimeUtil {
	public static Date getNowDate() {
		Calendar cal = Calendar.getInstance();
		return cal.getTime();
	}

	public static Date getDateFromNow(int field, int amount) {
		Calendar cal = Calendar.getInstance();
		cal.add(field, amount);
		return cal.getTime();
	}

	/**
	 * In the format of "yyyyMMddhhmmss"
	 * 
	 * @return
	 */
	public static String getNowString() {
		return new SimpleDateFormat("yyyyMMddhhmmss").format(getNowDate());
	}

	public static String getNowString(String format) {
		Calendar cal = Calendar.getInstance(Locale.CHINESE);
		SimpleDateFormat sdf = new SimpleDateFormat(format);
		return sdf.format(cal.getTime());

	}

	public static Timestamp getNowTimestamp() {
		Timestamp now = new Timestamp(TimeUtil.getNowDate().getTime());
		return now;
	}

	public static Timestamp getTimestamp(int field, int amount) {
		return new Timestamp(getDateFromNow(field, amount).getTime());
	}
}
