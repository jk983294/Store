package business.convertor;

import java.io.StringWriter;
import java.util.List;
import java.util.Map;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.dom4j.Document;
import org.dom4j.Element;

import util.XmlUtil;

/**
 * �������XML�ĵ��Ľ��������ݸ�ʽת������
 * 
 * @author Jersey
 * @date 2011-4-19
 */
public class ServiceXmlConvertor {
	public enum ParamType {
		TEXT, CHECKBOX, RADIO, SELECT
	}

	/**
	 * ��XSLT�ķ�ʽת��XMLΪHTML��������
	 * 
	 * @param xslPath
	 * @param xmlPath
	 * @return
	 * @throws TransformerException
	 */
	public String xml2html(String xslPath, String xmlPath)
			throws TransformerException {
		StringWriter sw = new StringWriter();
		TransformerFactory tFactory = TransformerFactory.newInstance();
		Transformer transformer = tFactory.newTransformer(new StreamSource(
				xslPath));
		transformer.transform(new StreamSource(xmlPath), new StreamResult(sw));
		return sw.toString();
	}
	/**
	 * ����ֵ��map��Դ�ļ�doc����XML
	 * @param map
	 * @param sourceDoc
	 * @param outputPath
	 * @return
	 */
	public boolean toXml(Map<String, String[]> map, Document sourceDoc,
			String outputPath) {
		for (String key : map.keySet()) {
			if (!key.startsWith("param_"))
				continue;// �ų���param��ֵ
			String names[] = key.split("_");
			String name = names[1];
			String type = names[2];
			String other = names.length > 3 ? names[3] : null;
			String[] values = map.get(key);
			Element param = (Element) sourceDoc
					.selectSingleNode("//param[@name='" + name + "']");
			if (other == null) {
				if (values.length > 1) {// ��ѡ���Ƕ�ֵ�ģ�
					String value = "";
					for (String v : values)
						value += v + ",";
					param.addAttribute("value",
							value.substring(0, value.lastIndexOf(",")));
				} else {
					param.addAttribute("value", values[0]);
				}
			} else if ("min".equals(other.toLowerCase())) {// �Ƿ���Ҫ���min����max����
				param.addAttribute("min", values[0]);
			} else if ("max".equals(other.toLowerCase())) {
				param.addAttribute("max", values[0]);
			}
		}
		return XmlUtil.writeXML(sourceDoc, outputPath, "UTF-8");

	}

	/**
	 * ����ֵ�Բ���xml�ĵ�
	 * 
	 * @param map
	 * @param outputPath
	 * @return
	 */
	public boolean toXml(Map<String, String[]> map, String rootName,
			String outputPath) {
		Document doc = XmlUtil.buildDocument();
		Element root = doc.addElement("service");
		root.addAttribute("name", rootName);
		for (String key : map.keySet()) {
			if (!key.startsWith("param_"))
				continue;// �ų���param��ֵ
			String names[] = key.split("_");
			String name = names[1];
			String type = names[2];
			String other = names.length > 3 ? names[3] : null;
			String[] values = map.get(key);
			Element param;
			// ���û�и�name��param�򴴽�Element
			if ((param = (Element) doc.selectSingleNode("//param[@name='"
					+ name + "']")) == null) {
				param = root.addElement("param").addAttribute("name", name)
						.addAttribute("type", type);
			}
			if (other == null) {
				if (values.length > 1) {// ��ѡ���Ƕ�ֵ�ģ�
					String value = "";
					for (String v : values)
						value += v + ",";
					param.addAttribute("value",
							value.substring(0, value.lastIndexOf(",")));
				} else {
					param.addAttribute("value", values[0]);
				}
			} else if ("min".equals(other.toLowerCase())) {// �Ƿ���Ҫ���min����max����
				param.addAttribute("min", values[0]);
			} else if ("max".equals(other.toLowerCase())) {
				param.addAttribute("max", values[0]);

			}
		}

		return XmlUtil.writeXML(doc, outputPath, "UTF-8");
	}

	/**
	 * Ӳ������ʽ��������XML�ĵ�ת��ΪHTML��ʽ
	 * 
	 * @param xmlFile
	 * @return
	 */
	public String toHtml(String xmlFile) {
		if (xmlFile == null)
			return null;
		Document doc = XmlUtil.buildDocFromFile(xmlFile.substring(1));
		String html = "<div><dl>";
		Element root = doc.getRootElement();
		List<Element> params = root.elements("param");
		int i = 1;
		for (Element param : params) {
			ParamType type = ParamType.valueOf(param.attributeValue("type")
					.toUpperCase());
			switch (type) {
			case TEXT:
				html += textHtml(param, i++);
				break;
			case CHECKBOX:
				html += checkboxHtml(param, i++);
				break;
			case RADIO:
				html += radioHtml(param, i++);
				break;
			case SELECT:
				html += selectHtml(param, i++);
				break;
			}
		}
		html += "</dl></div>";
		return html;
	}

	/**
	 * type=text�ı�ǩת��ΪHTML
	 * 
	 * @param param
	 * @param count
	 * @return
	 */
	private String textHtml(Element param, int count) {
		String section = "";
		String name = param.attributeValue("name");
		String value = param.attributeValue("value");
		String min = param.attributeValue("min");
		String max = param.attributeValue("max");
		if (name != null) {
			section += "<dt>" + name + "</dt>	<dd>";
			if (param.attribute("value") != null) {
				section += "<label>ֵ��</label>"
						+ "<input   name='"
						+ ("param_" + name + "_text") + "' type='text' value='"
						+ (value == null ? "" : value) + "' />";
			}
			if (param.attribute("min") != null) {
				section += "	<label>��Сֵ��</label>"
						+ "<input name='"
						+ ("param_" + name + "_text_min")
						+ "' type='text' value='" + (min == null ? "" : min)
						+ "'/>";
			}
			if (param.attribute("max") != null) {
				section += "	<label>���ֵ��</label>"
						+ "<input   name='"
						+ ("param_" + name + "_text_max")
						+ "' type='text' value='" + (max == null ? "" : max)
						+ "'/>";
			}
			section += "</dd>";
		}
		return section;
	}

	/**
	 * type=checkbox�ı�ǩת��ΪHTML
	 * 
	 * @param param
	 * @param count
	 * @return
	 */
	private String checkboxHtml(Element param, int count) {
		String section = "";
		String name = param.attributeValue("name");
		String value = param.attributeValue("value");
		List<Element> options = param.elements("option");

		if (name != null) {
			section += "<dt>" + name + "</dt>	<dd>";
			for (Element option : options) {
				String opValue = option.attributeValue("value");
				section += "[<label>" + opValue + "</label>";
				section += "<input type='checkbox' name='"
						+ ("param_" + name + "_checkbox") + "' "
						+ (contains(value,opValue) ? "checked='checked'" : "")
						+ "' value='" + option.attributeValue("value")
						+ "'>]&nbsp;";
			}
			section += "</dd>";
		}
		return section;
	}

	private boolean contains(String value, String opValue) {
		String[] values = value.split(",");
		boolean result = false;
		for (String v : values)
			if (v.equals(opValue)) {
				result = true;
				break;
			}
		return result;
	}

	/**
	 * type=radio�ı�ǩת��ΪHTML
	 * 
	 * @param param
	 * @param count
	 * @return
	 */
	private String radioHtml(Element param, int count) {
		String section = "";
		String name = param.attributeValue("name");
		String value = param.attributeValue("value");
		List<Element> options = param.elements("option");

		if (name != null) {
			section += "<dt>" + name + "</dt>	<dd>";
			for (Element option : options) {
				String opValue = option.attributeValue("value");
				section += "[<label>" + opValue + "</label>";
				section += "<input type='radio' name='"
						+ ("param_" + name + "_radio") + "' "
						+ (opValue.equals(value) ? "checked='checked'" : "")
						+ "' value='" + option.attributeValue("value")
						+ "'>]&nbsp;";
			}
			section += "</dd>";
		}
		return section;
	}

	/**
	 * type=select�ı�ǩת��ΪHTML
	 * 
	 * @param param
	 * @param count
	 * @return
	 */
	private String selectHtml(Element param, int count) {
		String section = "";
		String name = param.attributeValue("name");
		String value = param.attributeValue("value");
		List<Element> options = param.elements("option");

		if (name != null) {
			section += "<dt>" + name + "</dt>	<dd>";
			section += "<select name='" + ("param_" + name + "_select") + "'>";

			for (Element option : options) {
				String opValue = option.attributeValue("value");
				section += "<option value='" + opValue+ "' "+(opValue.equals(value) ? " selected=\"selected\"" : "") + ">" + opValue
						
						+ "</option>";
			}

			section += "</select></dd>";
		}
		return section;
	}

	
	public void test() {
		System.out.println(toHtml("D:/ServiceDefXml/cpu.service.xml"));
	}
}
