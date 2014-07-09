package com.sample.ext4.direct.form.action;

import java.util.Map;
import org.apache.commons.fileupload.FileItem;
import com.sample.ext4.direct.form.model.Product;
import com.sample.ext4.direct.form.util.ExtFormDirectResult;
import com.softwarementors.extjs.djn.config.annotations.DirectFormPostMethod;
import com.softwarementors.extjs.djn.config.annotations.DirectMethod;

/**
 * 表单动作处理类
 */
public class FormAction {
	
	/**
	 * 表单读取处理方法
	 * @param productId 产品id
	 * @return 结果对象
	 */
	@DirectMethod
	public ExtFormDirectResult loadProductServer(String productId){
		ExtFormDirectResult result = new ExtFormDirectResult();
		if("001".equals(productId)){
			//获取产品信息
			Product product = new Product();
			product.setProductName("笔记本");
			product.setIntroduction("本产品美观实用，售后服务优秀。");
			//将产品信息放入结果对象中
			result.setData(product);
			result.setSuccess(true);
		}else{
			//设置错误信息
			result.setErrorMessage("读取的产品id不存在");
			result.setSuccess(false);
		}
		return result;
	}
	/**
	 * 表单提交处理方法
	 * @param params 表单参数集合
	 * @param files 文件集合
	 * @return 结果对象
	 */
	@DirectFormPostMethod
	public ExtFormDirectResult submitProductServer(Map<String,String> params, Map<String, FileItem> files){
		ExtFormDirectResult result = new ExtFormDirectResult();
		//获取表单参数
		String productName = params.get("productName");
		if("笔记本".equals(productName)){
			result.setSuccess(true);
		}else{
			result.addError("productName", "产品名称必须是：笔记本");
			//设置错误信息
			result.setErrorMessage("提交产品名称错误");
			result.setSuccess(false);
		}
		return result;
	}
}