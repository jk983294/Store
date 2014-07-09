<?xml version="1.0" encoding="utf-8"?>
<list>
  <Total>
    <results>${results}</results>
  </Total>
  <#list items as item>
  <#assign recordName=item.class.name.replaceAll(".*\\.","")>
  	<${recordName}>
  	<#assign methods=item.class.declaredMethods>
  	<#list methods as method>
  		<#assign isGetMethod=method.name.startsWith("get")>
  		<#if isGetMethod = true>
  		<#assign methodName=method.name.replaceAll("get","").toLowerCase()>
	  		<${methodName}>
	  			${method.invoke(item,null)}
	  		</${methodName}>
  		</#if>
  	</#list>
  	</${recordName}>
  </#list>
</list>