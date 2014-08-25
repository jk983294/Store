package com.victor.finFriday.excelpoi;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.xssf.model.SharedStringsTable;

public class copySheetHandler extends ReadBaseHandler {
	
	private Sheet copysheet;
	private Row sheetrow;
	private Cell sheetcell;
	
	public copySheetHandler(SharedStringsTable sst) {
		super(sst);
		
	}

	@Override
	public void cellValueHandle() {
		if (isNewRow) {
			sheetrow = copysheet.createRow((short) row -1);
		}
		sheetcell = sheetrow.createCell((short) column-1);
		sheetcell.setCellValue(content);
	}

	public Sheet getCopysheet() {
		return copysheet;
	}

	public void setCopysheet(Sheet copysheet) {
		this.copysheet = copysheet;
	}
	
	

}
