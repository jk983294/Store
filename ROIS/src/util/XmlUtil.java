package util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

public class XmlUtil {
	private static Logger log = Logger.getLogger(XmlUtil.class);
	public static final String XML_HEAD_UTF8 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

	public XmlUtil() {
	}

	/**
	 * ����һ���µ�Document
	 * 
	 * @return Document
	 */
	public static Document buildDocument() {
		return DocumentHelper.createDocument();
	}

	/**
	 * ��XML�ַ������� dom4j document ����
	 * 
	 * @param xmlString
	 *            String XML�ַ���
	 * @return Document
	 */
	public static Document buildDocFromXML(String xmlString) {
		try {
			return DocumentHelper.parseText(xmlString);
		} catch (DocumentException ex) {
			log.error("�޷���XML�ִ�����dom4j Document:" + xmlString, ex);
			return null;
		}
	}

	/**
	 * ���ļ�����dom4j document ����
	 * 
	 * @param fileName
	 *            String �ļ���(����·��)
	 * @return Document
	 */
	public static Document buildDocFromFile(String fileName) {
		try {
			SAXReader sb = new SAXReader();
			sb.setEncoding("GBK");
			File f = new File(fileName);
			System.out.println("�ļ�" + fileName + "�Ƿ���ڣ�" + f.exists());
			return sb.read(f);
		} catch (Exception ex) {
			System.err.println("�޷����ļ�[" + fileName + "]����dom4j document" + ex);
			return null;
		}
	}

	/**
	 * ���XML���ļ���
	 * 
	 * @param doc
	 *            Document
	 * @param fileName
	 *            String �ļ���
	 * @param encoding
	 *            String ����
	 * @return boolean
	 */
	public static boolean writeXML(Document doc, String fileName,
			String encoding) {
		try {
			FileOutputStream fos = new FileOutputStream(fileName);
			OutputFormat format = OutputFormat.createPrettyPrint();
			format.setEncoding(encoding);
			format.setLineSeparator("\r\n");
			XMLWriter writer = new XMLWriter(fos, format);
			writer.write(doc);
			writer.close();
			return true;
		} catch (Exception ex) {
			log.error("��������,�޷����XML[" + encoding + "]���ļ�:" + fileName, ex);
			return false;
		}
	}

	private static final int BUFFER_SIZE = 16 * 1024;

	public static boolean delete(File file) {
		if (file.exists()) {
			file.delete();
			return true;
		}
		return false;
	}

	/**
	 * 
	 * @param src
	 * @param dst
	 */
	public static void copy(File src, File dst) {
		try {
			InputStream in = null;
			OutputStream out = null;
			try {
				in = new BufferedInputStream(new FileInputStream(src),
						BUFFER_SIZE);
				out = new BufferedOutputStream(new FileOutputStream(dst),
						BUFFER_SIZE);
				byte[] buffer = new byte[BUFFER_SIZE];
				while (in.read(buffer) > 0) {
					out.write(buffer);
				}
			} finally {
				if (null != in) {
					in.close();
				}
				if (null != out) {
					out.close();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * ��URL������dom4j document����
	 * 
	 * @param String
	 *            urlString
	 * @return Document
	 */
	public static Document buildDocFromURL(String urlString) {
		try {
			URL url = new URL(urlString);
			SAXReader sb = new SAXReader();
			return sb.read(url);
		} catch (Exception ex) {
			log.error("�޷���URL[" + urlString + "]����dom4j document", ex);
			return null;
		}
	}

	public String getDocumentAt(String urlString) {
		StringBuffer document = new StringBuffer();
		try {
			URL url = new URL(urlString);
			URLConnection conn = url.openConnection();
			BufferedReader reader = new BufferedReader(new InputStreamReader(
					conn.getInputStream()));
			String line = null;
			while ((line = reader.readLine()) != null) {
				document.append(line + "n");
			}
			reader.close();
		} catch (MalformedURLException e) {
			System.out.println("Unable to connect to URL: " + urlString);
		} catch (IOException e) {
			System.out.println("IOException when connecting to URL: "
					+ urlString);
		}
		return document.toString();
	}
}
