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
 * 服务相关XML文档的解析，数据格式转换工作
 * 
 * @author Jersey
 * @date 2011-4-19
 */
public class ServiceXmlConvertor {
	public enum ParamType {
		TEXT, CHECKBOX, RADIO, SELECT
	}

	/**
	 * 用XSLT的方式转换XML为HTML，待测试
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
	 * 根据值对map和源文件doc生成XML
	 * @param map
	 * @param sourceDoc
	 * @param outputPath
	 * @return
	 */
	public boolean toXml(Map<String, String[]> map, Document sourceDoc,
			String outputPath) {
		for (String key : map.keySet()) {
			if (!key.startsWith("param_"))
				continue;// 排除非param的值
			String names[] = key.split("_");
			String name = names[1];
			String type = names[2];
			String other = names.length > 3 ? names[3] : null;
			String[] values = map.get(key);
			Element param = (Element) sourceDoc
					.selectSingleNode("//param[@name='" + name + "']");
			if (other == null) {
				if (values.length > 1) {// 复选框是多值的！
					String value = "";
					for (String v : values)
						value += v + ",";
					param.addAttribute("value",
							value.substring(0, value.lastIndexOf(",")));
				} else {
					param.addAttribute("value", values[0]);
				}
			} else if ("min".equals(other.toLowerCase())) {// 是否需要添加min或者max属性
				param.addAttribute("min", values[0]);
			} else if ("max".equals(other.toLowerCase())) {
				param.addAttribute("max", values[0]);
			}
		}
		return XmlUtil.writeXML(sourceDoc, outputPath, "UTF-8");

	}

	/**
	 * 根据值对产生xml文档
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
				continue;// 排除非param的值
			String names[] = key.split("_");
			String name = names[1];
			String type = names[2];
			String other = names.length > 3 ? names[3] : null;
			String[] values = map.get(key);
			Element param;
			// 如果没有该name的param则创建Element
			if ((param = (Element) doc.selectSingleNode("//param[@name='"
					+ name + "']")) == null) {
				param = root.addElement("param").addAttribute("name", name)
						.addAttribute("type", type);
			}
			if (other == null) {
				if (values.length > 1) {// 复选框是多值的！
					String value = "";
					for (String v : values)
						value += v + ",";
					param.addAttribute("value",
							value.substring(0, value.lastIndexOf(",")));
				} else {
					param.addAttribute("value", values[0]);
				}
			} else if ("min".equals(other.toLowerCase())) {// 是否需要添加min或者max属性
				param.addAttribute("min", values[0]);
			} else if ("max".equals(other.toLowerCase())) {
				param.addAttribute("max", values[0]);

			}
		}

		return XmlUtil.writeXML(doc, outputPath, "UTF-8");
	}

	/**
	 * 硬编码形式将服务定义XML文档转换为HTML格式
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
	 * type=text的标签转换为HTML
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
				section += "<label>值：</label>"
						+ "<input   name='"
						+ ("param_" + name + "_text") + "' type='text' value='"
						+ (value == null ? "" : value) + "' />";
			}
			if (param.attribute("min") != null) {
				section += "	<label>最小值：</label>"
						+ "<input name='"
						+ ("param_" + name + "_text_min")
						+ "' type='text' value='" + (min == null ? "" : min)
						+ "'/>";
			}
			if (param.attribute("max") != null) {
				section += "	<label>最大值：</label>"
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
	 * type=checkbox的标签转换为HTML
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
	 * type=radio的标签转换为HTML
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
	 * type=select的标签转换为HTML
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
