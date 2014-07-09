package com.sample.filter;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;

import org.apache.log4j.Logger;

public class CharacterEncodingFilter implements Filter {
	
	private static final Logger logger = Logger.getLogger(CharacterEncodingFilter.class);

	@Override
	public void destroy() {
		logger.debug("CharacterEncodingFilter destroy.");
	}

	@Override
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		request.setCharacterEncoding("utf-8");
		chain.doFilter(request, response);
	}

	@Override
	public void init(FilterConfig arg0) throws ServletException {
		logger.debug("CharacterEncodingFilter init.");
	}

}
