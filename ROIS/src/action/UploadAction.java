package action;

import java.io.File;

import business.managers.ServiceManager;

import support.GlobalParameter;
import util.XmlUtil;

public class UploadAction extends BaseAction {
	private static final long serialVersionUID = -8920466592471253212L;
	private File upload;
	private String contentType;
	private String fileName;
	private String importType;

	// since we are using <s:file name="upload" .../> the file name will be
	// obtained through getter/setter of <file-tag-name>FileName
	public String getUploadFileName() {
		return fileName;
	}

	public void setUploadFileName(String fileName) {
		this.fileName = fileName;
	}

	// since we are using <s:file name="upload" ... /> the content type will be
	// obtained through getter/setter of <file-tag-name>ContentType
	public String getUploadContentType() {
		return contentType;
	}

	public void setUploadContentType(String contentType) {
		this.contentType = contentType;
	}

	// since we are using <s:file name="upload" ... /> the File itself will be
	// obtained through getter/setter of <file-tag-name>
	public File getUpload() {
		return upload;
	}

	public void setUpload(File upload) {
		this.upload = upload;
	}

	@Override
	public String execute() throws Exception {
		String newFileName = fileName;
		if (importType.toLowerCase().equals("serviceindex")) {
			newFileName = "index.service.xml";
		} else {
			newFileName = "index.order.xml";
		}
//		String projectRealPath = ServletActionContext.getServletContext()
//				.getRealPath("/");
		File uploadFile = new File(GlobalParameter.SrvIndexFilePath());
		XmlUtil.copy(upload, uploadFile);
		ServiceManager.importServices(GlobalParameter.SrvIndexFilePath());
		return SUCCESS;
	}

	public void setImportType(String importType) {
		this.importType = importType;
	}

	public String getImportType() {
		return importType;
	}

}
